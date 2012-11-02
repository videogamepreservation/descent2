/*
THE COMPUTER CODE CONTAINED HEREIN IS THE SOLE PROPERTY OF PARALLAX
SOFTWARE CORPORATION ("PARALLAX").  PARALLAX, IN DISTRIBUTING THE CODE TO
END-USERS, AND SUBJECT TO ALL OF THE TERMS AND CONDITIONS HEREIN, GRANTS A
ROYALTY-FREE, PERPETUAL LICENSE TO SUCH END-USERS FOR USE BY SUCH END-USERS
IN USING, DISPLAYING,  AND CREATING DERIVATIVE WORKS THEREOF, SO LONG AS
SUCH USE, DISPLAY OR CREATION IS FOR NON-COMMERCIAL, ROYALTY OR REVENUE
FREE PURPOSES.  IN NO EVENT SHALL THE END-USER USE THE COMPUTER CODE
CONTAINED HEREIN FOR REVENUE-BEARING PURPOSES.  THE END-USER UNDERSTANDS
AND AGREES TO THE TERMS HEREIN AND ACCEPTS THE SAME BY USE OF THIS FILE.  
COPYRIGHT 1993-1999 PARALLAX SOFTWARE CORPORATION.  ALL RIGHTS RESERVED.
*/


#pragma off (unreferenced)
static char rcsid[] = "$Id: digi.c 2.36 1996/06/05 17:02:22 matt Exp $";
#pragma on (unreferenced)

#include<stdlib.h>
#include<stdio.h>
#include<dos.h>
#include<fcntl.h> 
#include<malloc.h> 
#include<bios.h>
#include<io.h>
#include<conio.h> 
#include<string.h>
#include<ctype.h>

#include "fix.h"
#include "mono.h"
#include "timer.h"
#include "joy.h"
#include "digi.h"
#include "sounds.h"
#include "args.h"
#include "key.h"
#include "game.h"
#include "dpmi.h"
#include "error.h"
#include "cfile.h"
#include "piggy.h"
#include "text.h"


#pragma pack (4);						// Use 32-bit packing!
#pragma off (check_stack);			// No stack checking!
#include "sos.h"
#include "sosm.h"
#include "config.h"
//#include "soscomp.h"

#define _DIGI_SAMPLE_FLAGS (_VOLUME | _PANNING | _TRANSLATE8TO16)

#define _DIGI_MAX_VOLUME (16384)	//16384

// patch files
#define  _MELODIC_PATCH       "melodic.bnk"
#define  _DRUM_PATCH          "drum.bnk"
#define  _DIGDRUM_PATCH       "drum32.dig"

 
int			Digi_initialized 		= 0;
static int	digi_atexit_called	= 0;			// Set to 1 if atexit(digi_close) was called

int digi_driver_board				= 0;
int digi_driver_port					= 0;
int digi_driver_irq					= 0;
int digi_driver_dma					= 0;
int digi_midi_type					= 0;			// Midi driver type
int digi_midi_port					= 0;			// Midi driver port
int digi_max_channels				= 8;
int digi_sample_rate					= SAMPLE_RATE_22K;	// rate to use driver at
static int digi_dma_buffersize	= 4096;			// size of the dma buffer to use
int digi_timer_rate					= 9943;			// rate for the timer to go off to handle the driver system
static int digi_volume				= _DIGI_MAX_VOLUME;		// Max volume
static int midi_volume				= 128/2;						// Max volume
static int midi_system_initialized		= 0;
static int digi_system_initialized		= 0;
static int digi_sound_locks[MAX_SOUNDS];
char digi_last_midi_song[16] = "";
char digi_last_melodic_bank[16] = "";
char digi_last_drum_bank[16] = "";
LPSTR digi_driver_path = _NULL;

WORD								hSOSDigiDriver = 0xffff;			// handle for the SOS driver being used 
static WORD     				hSOSMidiDriver = 0xffff;			// handle for the loaded MIDI driver
static WORD						hTimerEventHandle = 0xffff;		// handle for the timer function

static void * lpInstruments = NULL;		// pointer to the instrument file
static int InstrumentSize = 0;
static void * lpDrums = NULL;				// pointer to the drum file
static int DrumSize = 0;

// track mapping structure, this is used to map which track goes
// out which device. this can also be mapped by the name of the 
// midi track. to map by the name of the midi track use the define
// _MIDI_MAP_TRACK for each of the tracks 
static _SOS_MIDI_TRACK_DEVICE   sSOSTrackMap = { 
   _MIDI_MAP_TRACK, _MIDI_MAP_TRACK, _MIDI_MAP_TRACK, _MIDI_MAP_TRACK, 
   _MIDI_MAP_TRACK, _MIDI_MAP_TRACK, _MIDI_MAP_TRACK, _MIDI_MAP_TRACK, 
   _MIDI_MAP_TRACK, _MIDI_MAP_TRACK, _MIDI_MAP_TRACK, _MIDI_MAP_TRACK, 
   _MIDI_MAP_TRACK, _MIDI_MAP_TRACK, _MIDI_MAP_TRACK, _MIDI_MAP_TRACK,
   _MIDI_MAP_TRACK, _MIDI_MAP_TRACK, _MIDI_MAP_TRACK, _MIDI_MAP_TRACK, 
   _MIDI_MAP_TRACK, _MIDI_MAP_TRACK, _MIDI_MAP_TRACK, _MIDI_MAP_TRACK, 
   _MIDI_MAP_TRACK, _MIDI_MAP_TRACK, _MIDI_MAP_TRACK, _MIDI_MAP_TRACK, 
   _MIDI_MAP_TRACK, _MIDI_MAP_TRACK, _MIDI_MAP_TRACK, _MIDI_MAP_TRACK 
};

// handle for the initialized MIDI song
WORD     wSongHandle = 0xffff;         
ubyte		*SongData=NULL;
int		SongSize;

void digi_stop_all_channels(void);
int verify_sound_channel_free(int channel);

void * digi_load_file( char * szFileName, int * length );

VOID _far digi_midi_callback( WORD PassedSongHandle );
VOID sosEndMIDICallback();

typedef struct digi_channel {
	ubyte		used;				// Non-zero if a sound is playing on this channel 
	int		soundnum;		// Which sound effect is on this channel, -1 if none
	WORD		handle;			// What HMI handle this is playing on
	int		soundobj;		// Which soundobject is on this channel
	int 		persistant;		// This can't be pre-empted
	int		volume;			// the volume of this channel
} digi_channel;

#define MAX_CHANNELS 32
digi_channel channels[MAX_CHANNELS];
int next_channel = 0;
int channels_inited = 0;

int digi_total_locks = 0;

void digi_close_midi()
{
	if (digi_midi_type>0)	{
		if (wSongHandle < 0xffff)	{
		   // stop the last MIDI song from playing
			sosMIDIStopSong( wSongHandle );
		   // uninitialize the last MIDI song
			sosMIDIUnInitSong( wSongHandle );
			wSongHandle = 0xffff;
		}
		if (SongData) 	{
			if (!dpmi_unlock_region(SongData, SongSize))	{
				mprintf( (1, "Error unlocking midi file" ));
			}
			free(SongData);
			SongData = NULL;
		}
	   // reset the midi driver and
	   // uninitialize the midi driver and tell it to free the memory allocated
	   // for the driver
		if ( hSOSMidiDriver < 0xffff )	{
		   sosMIDIResetDriver( hSOSMidiDriver );
	   	sosMIDIUnInitDriver( hSOSMidiDriver, _TRUE  );
			hSOSMidiDriver = 0xffff;
		}

		if ( midi_system_initialized )	{
		   // uninitialize the MIDI system
		   sosMIDIUnInitSystem(); 
			midi_system_initialized = 0;
		}
	}
}

void digi_close_digi()
{
	if (digi_driver_board>0)	{
		if ( hTimerEventHandle < 0xffff )	{
		 	sosTIMERRemoveEvent( hTimerEventHandle );
			hTimerEventHandle = 0xffff;
		}
		if ( hSOSDigiDriver < 0xffff )
		   sosDIGIUnInitDriver( hSOSDigiDriver, _TRUE, _TRUE );
	}
}

void digi_close()
{
	if (!Digi_initialized) return;
	Digi_initialized = 0;

	if ( digi_system_initialized )	{
		timer_set_function( NULL );
	}

	if ( digi_driver_board == _GUS_8_ST || digi_driver_board == _GUS_16_ST )	{
		digi_close_digi();
		digi_close_midi();
	} else {
		digi_close_midi();
		digi_close_digi();
	}

	if ( digi_system_initialized )	{
	   // uninitialize the DIGI system
		sosDIGIUnInitSystem();
		sosTIMERUnInitSystem( 0 );
		digi_system_initialized = 0;
	}

}

extern int loadpats( char * filename );

int digi_load_fm_banks( char * melodic_file, char * drum_file )
{	
   WORD     wError;                 // error code returned from functions

	// set the instrument file for the MIDI driver to use, since we are using
	// the FM driver two instrument files are needed, the first is for 
	// all melodic instruments and the second is for events on channel 10
	// which is the drum track.
	// set the drum instruments
	if (lpInstruments)	{
		dpmi_unlock_region(lpInstruments, InstrumentSize);
		free( lpInstruments );
	}
			
	lpInstruments = digi_load_file( melodic_file, &InstrumentSize );
	if ( !lpInstruments )	{
		printf( "%s '%s'\n", TXT_SOUND_ERROR_OPEN, melodic_file );
		return 0;
	}

	if (!dpmi_lock_region(lpInstruments, InstrumentSize))	{
		printf( "%s '%s', ptr=0x%8x, len=%d bytes\n", TXT_SOUND_ERROR_LOCK, melodic_file, lpInstruments, InstrumentSize );
		return 0;
	}
	
	if( ( wError =  sosMIDISetInsData( hSOSMidiDriver, lpInstruments, 0x01  ) ) ) 	{
		printf( "%s %s \n", TXT_SOUND_ERROR_HMI, sosGetErrorString( wError ) );
		return 0;
	}
	
	if (lpDrums)	{
		dpmi_unlock_region(lpDrums, DrumSize);
		free( lpDrums );
	}
			
	lpDrums = digi_load_file( drum_file, &DrumSize );
	if ( !lpDrums )	{
		printf( "%s '%s'\n", TXT_SOUND_ERROR_OPEN, drum_file );
		return 0;
	}

	if (!dpmi_lock_region(lpDrums, DrumSize))	{
		printf( "%s  '%s', ptr=0x%8x, len=%d bytes\n", TXT_SOUND_ERROR_LOCK_DRUMS, drum_file, lpDrums, DrumSize );
		return 0;
	}
	
	 // set the drum instruments
	if( ( wError =  sosMIDISetInsData( hSOSMidiDriver, lpDrums, 0x01  ) ) )	{
		printf( "%s %s\n", TXT_SOUND_ERROR_HMI, sosGetErrorString( wError ) );
		return 0;
	}
	
	return 1;
}

int digi_init_midi()
{
   WORD     wError;                 // error code returned from functions
	_SOS_MIDI_INIT_DRIVER			sSOSMIDIInitDriver;	// structure for the MIDI driver initialization function 
	_SOS_MIDI_HARDWARE				sSOSMIDIHardware; 	// structure for the MIDI driver hardware

	if ( digi_midi_type > 0 )	{
		// Lock the TrackMap array, since HMI references it in an interrupt.
		if (!dpmi_lock_region ( &sSOSTrackMap, sizeof(sSOSTrackMap)) )	{
			printf( "%s\n", TXT_SOUND_ERROR_MIDI);
			digi_close();
			return 1;
		}

		if (!dpmi_lock_region ((void near *)digi_midi_callback, (char *)sosEndMIDICallback - (char near *)digi_midi_callback))	{
			printf( "%s\n", TXT_SOUND_ERROR_MIDI_CALLBACK);
			digi_close();
			return 1;
		}

	   // initialize the MIDI system
	   sosMIDIInitSystem( digi_driver_path, _SOS_DEBUG_NORMAL );
		midi_system_initialized = 1;

	   // set the pointer to the driver memory for the MIDI driver to 
	   // _NULL. this will tell the load driver routine to allocate new
	   // memory for the MIDI driver
	   sSOSMIDIInitDriver.lpDriverMemory  = _NULL;
		sSOSMIDIInitDriver.sDIGIInitInfo = _NULL;
	
		sSOSMIDIHardware.wPort = digi_midi_port;
	
	   // load and initialize the MIDI driver 
	   if( ( wError = sosMIDIInitDriver( digi_midi_type, &sSOSMIDIHardware, &sSOSMIDIInitDriver, &hSOSMidiDriver ) ) )	{
	      printf( "SOUND: (HMI) '%s'\n", sosGetErrorString( wError ) );
			digi_close();
	      return 1;
	   }
		sosMIDIEnableChannelStealing( _FALSE );
	}
	return 0;
}

int digi_init_digi()
{
   WORD     wError;                 // error code returned from functions
	_SOS_INIT_DRIVER					sSOSInitDriver;			// structure used to initialize the driver
	_SOS_HARDWARE						sSOSHardwareSettings;	// device settings


	//use larger DMA buffers for GUS.  These values were determined by 
	//John through experimentation
	if (digi_driver_board == _GUS_8_ST)
		digi_dma_buffersize = 5120;	// 5K
	else if (digi_driver_board == _GUS_16_ST)
		digi_dma_buffersize = 6144;	// 6K

	Assert(digi_sample_rate == SAMPLE_RATE_11K || digi_sample_rate == SAMPLE_RATE_22K);

	if ( digi_driver_board > 0 )	{
		memset( &sSOSInitDriver, 0, sizeof(_SOS_INIT_DRIVER) );
		sSOSInitDriver.wBufferSize			= digi_dma_buffersize;
		sSOSInitDriver.lpBuffer				= _NULL;
		sSOSInitDriver.wAllocateBuffer	= _TRUE;
		//sSOSInitDriver.wParam 				= 19;
		sSOSInitDriver.wSampleRate			= digi_sample_rate;
		sSOSInitDriver.lpDriverMemory		= _NULL;
		sSOSInitDriver.lpTimerMemory		= _NULL;
	
		sSOSHardwareSettings.wPort = digi_driver_port;
		sSOSHardwareSettings.wIRQ = digi_driver_irq;
		sSOSHardwareSettings.wDMA = digi_driver_dma;

	   if( ( wError = sosDIGIInitDriver(  digi_driver_board, &sSOSHardwareSettings, &sSOSInitDriver, &hSOSDigiDriver ) ) )
	   {
	      // display the error types
	      printf( "SOUND: (HMI) '%s'\n", sosGetErrorString( wError ) );
			digi_close();
	      return 1;
	   }

	   if( ( wError = sosTIMERRegisterEvent( 1193180/digi_timer_rate, sSOSInitDriver.lpFillHandler, &hTimerEventHandle ) ) )	{
	      // display the error types
	      printf( "SOUND: (HMI) '%s'\n", sosGetErrorString( wError ) );
			digi_close();
	      return 1;
		}
	}
	return 0;
}


int digi_init()
{
	int i;

	if ( FindArg( "-nomusic" )) 
		digi_midi_type = 0;

	if ( FindArg( "-nosound" ) )	{
		digi_midi_type = 0;
		digi_driver_board	= 0;
	}

	Digi_initialized = 1;

	if ( !FindArg( "-noloadpats" ) )	{
		if ( digi_midi_type == _MIDI_GUS )	{
			char fname[128];
			static pats_loaded = 0;
			if (!pats_loaded) {
				strcpy( fname, "DESCENTG.INI" );
				loadpats(fname);
				pats_loaded = 1;
			}
		}
	}

	// initialize the DIGI system and lock down all SOS memory
	sosDIGIInitSystem( digi_driver_path, _SOS_DEBUG_NORMAL );

	// Initialize timer, where we will call it from out own interrupt
	// routine, and we will call DOS 18.2Hz ourselves.
	sosTIMERInitSystem( 0, _SOS_DEBUG_NO_TIMER );
	timer_set_function( sosTIMERHandler );		// New way
	
	digi_system_initialized = 1;

	if ( (digi_midi_type<1) && (digi_driver_board<1) )
		return 0;

	if ( digi_driver_board == _RAP10_8_MONO )	{
		if (digi_init_digi()) return 1;
		if (digi_init_midi()) return 1;
	} else {
		if (digi_init_midi()) return 1;
		if (digi_init_digi()) return 1;
	}

	if (!digi_atexit_called)	{
		atexit( digi_close );
		digi_atexit_called = 1;
	}

	next_channel = 0;
	for (i=0; i<MAX_CHANNELS; i++ )	{
		channels[i].used = 0;
		channels[i].soundnum = -1;
	}
	digi_init_sounds();
	digi_set_midi_volume( midi_volume );

	for (i=0; i<MAX_SOUNDS; i++ )
		digi_sound_locks[i] = 0;
	digi_stop_all_channels();

	return 0;
}

// Toggles sound system on/off
void digi_reset()	
{
	if ( Digi_initialized )	{
		digi_stop_all_channels();
		digi_close();
		mprintf( (0, "Sound system DISABLED.\n" ));
	} else {
		digi_init();
		mprintf( (0, "Sound system ENABLED.\n" ));
	}
}

ubyte * digi_lock_sound_data( int soundnum )
{
	int i;

	if ( !Digi_initialized ) return NULL;
	if ( digi_driver_board <= 0 )	return NULL;

	if ( digi_sound_locks[soundnum] == 0 )	{
		digi_total_locks++;
		//mprintf(( 0, "Total sound locks = %d\n", digi_total_locks ));
		i = dpmi_lock_region( GameSounds[soundnum].data, GameSounds[soundnum].length );
		if ( !i ) Error( "Error locking sound %d\n", soundnum );
	}
	digi_sound_locks[soundnum]++;
	return GameSounds[soundnum].data;
}

void digi_unlock_sound_data( int soundnum )
{
	int i;

	if ( !Digi_initialized ) return;
	if ( digi_driver_board <= 0 )	return;

	Assert( digi_sound_locks[soundnum] > 0 );

	if ( digi_sound_locks[soundnum] == 1 )	{
		digi_total_locks--;
		//mprintf(( 0, "Total sound locks = %d\n", digi_total_locks ));
		i = dpmi_unlock_region( GameSounds[soundnum].data, GameSounds[soundnum].length );
		if ( !i ) Error( "Error unlocking sound %d\n", soundnum );
	}
	digi_sound_locks[soundnum]--;
}

void digi_stop_all_channels()
{
	int i;

	for (i=0; i<MAX_CHANNELS; i++ )
		digi_stop_sound(i);

	for (i=0; i<MAX_SOUNDS; i++ )	{
		Assert( digi_sound_locks[i] == 0 );
	}
}

void digi_set_max_channels(int n)
{
	digi_max_channels	= n;

	if ( digi_max_channels < 1 ) 
		digi_max_channels = 1;
	if ( digi_max_channels > 32 ) 
		digi_max_channels = 32;

	if ( !Digi_initialized ) return;
	if ( digi_driver_board <= 0 )	return;

	digi_stop_all_channels();
}

int digi_get_max_channels()
{
	return digi_max_channels;
}

int digi_is_channel_playing( int c )
{
	if (!Digi_initialized) return 0;
	if (digi_driver_board<1) return 0;

	if ( channels[c].used && (!sosDIGISampleDone( hSOSDigiDriver, channels[c].handle) )  )
		return 1;
	return 0;
}

void digi_set_channel_volume( int c, int volume )
{
	if (!Digi_initialized) return;
	if (digi_driver_board<1) return;

	if ( !channels[c].used ) return;

	sosDIGISetSampleVolume( hSOSDigiDriver, channels[c].handle, fixmuldiv(volume,digi_volume,F1_0)  );
}
	
void digi_set_channel_pan( int c, int pan )
{
	if (!Digi_initialized) return;
	if (digi_driver_board<1) return;

	if ( !channels[c].used ) return;

	sosDIGISetPanLocation( hSOSDigiDriver, channels[c].handle, pan  );
}

void digi_stop_sound( int c )
{
	if (!Digi_initialized) return;
	if (digi_driver_board<1) return;

	if (!channels[c].used) return;

	if ( digi_is_channel_playing(c)  )		{
		sosDIGIStopSample(hSOSDigiDriver, channels[c].handle );
	}
	digi_unlock_sound_data(channels[c].soundnum);
	channels[c].used = 0;

	channels[c].soundobj = -1;
	channels[c].persistant = 0;
}

void digi_end_sound( int c )
{
	if (!Digi_initialized) return;
	if (digi_driver_board<1) return;

	if (!channels[c].used) return;

	channels[c].soundobj = -1;
	channels[c].persistant = 0;
}




extern void digi_end_soundobj(int channel);	
extern int SoundQ_channel;
extern void SoundQ_end();

// Volume 0-F1_0
int digi_start_sound(short soundnum, fix volume, int pan, int looping, int loop_start, int loop_end, int soundobj )
{
	int i, starting_channel;
	WORD sHandle;
	_SOS_START_SAMPLE sSOSSampleData;

	if ( !Digi_initialized ) return -1;
	if ( digi_driver_board <= 0 )	return -1;

	memset( &sSOSSampleData, 0, sizeof(_SOS_START_SAMPLE));

	Assert(GameSounds[soundnum].data != -1);
		
	sSOSSampleData.wChannel 				= _CENTER_CHANNEL;
	sSOSSampleData.wSampleID				= soundnum;
	sSOSSampleData.dwSampleSize 			= ( LONG )GameSounds[soundnum].length;
	sSOSSampleData.lpSamplePtr				= GameSounds[soundnum].data;
	sSOSSampleData.lpCallback				= _NULL;		//sosDIGISampleCallback;
	sSOSSampleData.wSamplePanLocation	= pan;			// 0 - 0xFFFF
	sSOSSampleData.wSamplePanSpeed 		= 0;
	sSOSSampleData.wVolume					= fixmuldiv(volume,digi_volume,F1_0);
	sSOSSampleData.wSampleFlags			= _DIGI_SAMPLE_FLAGS;
	if (looping )	{
		sSOSSampleData.wLoopCount			= -1;				// loop forever.
		sSOSSampleData.wSampleFlags 		|= _LOOPING; 		// Mark it as a looper.
	}
	if (loop_start != -1 ) {
		Assert( loop_end != -1);
		sSOSSampleData.wSampleFlags		|= _STAGE_LOOP;
		sSOSSampleData.dwSampleLoopPoint	= loop_start;
		sSOSSampleData.dwSampleLoopLength = loop_end - loop_start;
	}

	starting_channel = next_channel;

	while(1)	{
		if ( !channels[next_channel].used ) break;

		if ( sosDIGISampleDone( hSOSDigiDriver, channels[next_channel].handle)  ) break;

		if ( !channels[next_channel].persistant )	{
			sosDIGIStopSample(hSOSDigiDriver, channels[next_channel].handle );
			break;	// use this channel!	
		}
		next_channel++;
		if ( next_channel >= digi_max_channels )
			next_channel = 0;
		if ( next_channel == starting_channel ) {
			mprintf(( 1, "OUT OF SOUND CHANNELS!!!\n" ));
			return -1;
		}
	}
	if ( channels[next_channel].used )	{
		digi_unlock_sound_data(channels[next_channel].soundnum);
		channels[next_channel].used = 0;
		if ( channels[next_channel].soundobj > -1 )	{
			digi_end_soundobj(channels[next_channel].soundobj);	
		}
		if (SoundQ_channel==next_channel)
			SoundQ_end();
	}

	digi_lock_sound_data(soundnum);
	sHandle = sosDIGIStartSample( hSOSDigiDriver, &sSOSSampleData );
	if ( sHandle == _ERR_NO_SLOTS )	{
		mprintf(( 1, "NOT ENOUGH SOUND SLOTS!!!\n" ));
		digi_unlock_sound_data(soundnum);
		return -1;
	}

	#ifndef NDEBUG
	verify_sound_channel_free(next_channel);
	#endif

	//free up any sound objects that were using this handle
	for (i=0; i<digi_max_channels; i++ )	{
		if ( channels[i].used && (channels[i].handle == sHandle)  )	{
			digi_unlock_sound_data(channels[i].soundnum);
			channels[i].used = 0;
			if ( channels[i].soundobj > -1 )	{
				digi_end_soundobj(channels[i].soundobj);	
			}
			if (SoundQ_channel==i)
				SoundQ_end();
		}
	}

	channels[next_channel].used = 1;
	channels[next_channel].soundnum = soundnum;
	channels[next_channel].soundobj = soundobj;
	channels[next_channel].handle = sHandle;
	channels[next_channel].volume = volume;
	channels[next_channel].persistant = 0;
	if ( (soundobj > -1) || (looping) || (volume>F1_0) )
		channels[next_channel].persistant = 1;

	i = next_channel;
	next_channel++;
	if ( next_channel >= digi_max_channels )
		next_channel = 0;

	return i;
}

// Returns the channel a sound number is playing on, or
// -1 if none.
int digi_find_channel(int soundno)
{
	int i, is_playing;

	if (!Digi_initialized) return -1;
	if (digi_driver_board<1) return -1;

	if (soundno < 0 ) return -1;
	if (GameSounds[soundno].data==NULL) {
		Int3();
		return -1;
	}

	is_playing = 0;
	for (i=0; i<digi_max_channels; i++ )	{
		if ( channels[i].used && (channels[i].soundnum==soundno) )
			if ( digi_is_channel_playing(i) )
				return i;
	}	
	return -1;
}


extern int Redbook_playing;

void digi_set_midi_volume( int mvolume )
{

	int old_volume = midi_volume;

	if ( mvolume > 127 )
		midi_volume = 127;
	else if ( mvolume < 0 )
		midi_volume = 0;
	else
		midi_volume = mvolume;

	if ( (digi_midi_type > 0) && (hSOSMidiDriver < 0xffff) )		{
		if (!Redbook_playing && (old_volume < 1) && ( midi_volume > 1 ) )	{
			if (wSongHandle == 0xffff )
				digi_play_midi_song( digi_last_midi_song, digi_last_melodic_bank, digi_last_drum_bank, 1 );
		}
		_disable();
		sosMIDISetMasterVolume(midi_volume);
		_enable();
	}

}

void digi_set_digi_volume( int dvolume )
{
	dvolume = fixmuldiv( dvolume, _DIGI_MAX_VOLUME, 0x7fff);
	if ( dvolume > _DIGI_MAX_VOLUME )
		digi_volume = _DIGI_MAX_VOLUME;
	else if ( dvolume < 0 )
		digi_volume = 0;
	else
		digi_volume = dvolume;

	if ( !Digi_initialized ) return;
	if ( digi_driver_board <= 0 )	return;

	//digi_sync_sounds();
}


// 0-0x7FFF 
void digi_set_volume( int dvolume, int mvolume )	
{
	digi_set_midi_volume( mvolume );
	digi_set_digi_volume( dvolume );
//	mprintf(( 1, "Volume: 0x%x and 0x%x\n", digi_volume, midi_volume ));
}

// allocate memory for file, load file, create far pointer
// with DS in selector.
void * digi_load_file( char * szFileName, int * length )
{
   PSTR  pDataPtr;
	CFILE * fp;
	
   // open file
   fp  =  cfopen( szFileName, "rb" );
	if ( !fp ) return NULL;

   *length  =  cfilelength(fp);

   pDataPtr =  malloc( *length );
	if ( !pDataPtr ) return NULL;

   // read in driver
   cfread( pDataPtr, *length, 1, fp);

   // close driver file
   cfclose( fp );

   // return 
   return( pDataPtr );
}


// ALL VARIABLES IN HERE MUST BE LOCKED DOWN!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
VOID _far digi_midi_callback( WORD PassedSongHandle )
{
	sosMIDIStartSong(PassedSongHandle);
} 

VOID sosEndMIDICallback()		// Used to mark the end of digi_midi_callback
{
}

void digi_stop_current_song()
{
	if (!Digi_initialized) return;

	if ( digi_midi_type > 0 )	{
		// Stop last song...
		if (wSongHandle < 0xffff )	{
		   // stop the last MIDI song from playing
			sosMIDIStopSong( wSongHandle );
		   // uninitialize the last MIDI song
			sosMIDIUnInitSong( wSongHandle );
			wSongHandle = 0xffff;
		}
		if (SongData) 	{
			if (!dpmi_unlock_region(SongData, SongSize))	{
				mprintf( (1, "Error unlocking midi file" ));
			}
			free(SongData);
			SongData = NULL;
		}
	}
}


void digi_play_midi_song( char * filename, char * melodic_bank, char * drum_bank, int loop )
{
	int i;
	char fname[128];
   WORD     wError;                 // error code returned from functions
	CFILE		*fp;

	// structure to pass sosMIDIInitSong
	_SOS_MIDI_INIT_SONG     sSOSInitSong;

	if (!Digi_initialized) return;
	if ( digi_midi_type <= 0 )	return;

	digi_stop_current_song();

	if ( filename == NULL )	return;

	strcpy( digi_last_midi_song, filename );
	strcpy( digi_last_melodic_bank, melodic_bank );
	strcpy( digi_last_drum_bank, drum_bank );

	fp = NULL;

	if ( (digi_midi_type==_MIDI_FM)||(digi_midi_type==_MIDI_OPL3) )	{	
		int sl;
		sl = strlen( filename );
		strcpy( fname, filename );	
		fname[sl-1] = 'q';
		fp = cfopen( fname, "rb" );
	}

	if ( !fp  )	{
		fp = cfopen( filename, "rb" );
		if (!fp) {
	 		mprintf( (1, "Error opening midi file, '%s'", filename ));
	 		return;
		}
	}
	if ( midi_volume < 1 )		{
		cfclose(fp);
		return;				// Don't play song if volume == 0;
	}
	SongSize = cfilelength( fp );
	SongData	= malloc( SongSize );
	if (SongData==NULL)	{
		cfclose(fp);
		mprintf( (1, "Error mallocing %d bytes for '%s'", SongSize, filename ));
		return;
	}
	if ( cfread (  SongData, SongSize, 1, fp )!=1)	{
		mprintf( (1, "Error reading midi file, '%s'", filename ));
		cfclose(fp);
		free(SongData);
		SongData=NULL;
		return;
	}
	cfclose(fp);

	if ( (digi_midi_type==_MIDI_FM)||(digi_midi_type==_MIDI_OPL3) )	{	
		if ( !digi_load_fm_banks(melodic_bank, drum_bank) )	{
			return;
		}
	}
		
	if (!dpmi_lock_region(SongData, SongSize))	{
		mprintf( (1, "Error locking midi file, '%s'", filename ));
		free(SongData);
		SongData=NULL;
		return;
	}

	// setup the song initialization structure
	sSOSInitSong.lpSongData = SongData;
	if ( loop )
		sSOSInitSong.lpSongCallback = digi_midi_callback;
	else
		sSOSInitSong.lpSongCallback = _NULL;

	for ( i=0; i<32; i++ )
		sSOSTrackMap.wTrackDevice[i] = _MIDI_MAP_TRACK;
			
	for ( i=0; i<_SOS_MIDI_MAX_TRACKS; i++ )
		_lpSOSMIDITrack[0][i] = _NULL;

	//initialize the song
	if( ( wError = sosMIDIInitSong( &sSOSInitSong, &sSOSTrackMap, &wSongHandle ) ) )	{
		mprintf( (1, "\nHMI Error : %s", sosGetErrorString( wError ) ));
		free(SongData);
		SongData=NULL;
		return;
	}

	Assert( wSongHandle == 0 );
	 
  // start the song playing
   if( ( wError = sosMIDIStartSong( wSongHandle ) ) ) {
		mprintf( (1, "\nHMI Error : %s", sosGetErrorString( wError ) ));
	   // uninitialize the last MIDI song
		sosMIDIUnInitSong( wSongHandle );
		free(SongData);
		SongData=NULL;
		return;
   }

}


int sound_paused = 0;

void digi_pause_midi()
{
	if (!Digi_initialized) return;

	if (sound_paused==0)	{
		if ( digi_midi_type > 0 )	{
			// pause here
			_disable();
			sosMIDISetMasterVolume(0);
			_enable();
		}
	}
	sound_paused++;
}



void digi_resume_midi()
{
	if (!Digi_initialized) return;

	Assert( sound_paused > 0 );

	if (sound_paused==1)	{
		// resume sound here
		if ( digi_midi_type > 0 )	{
			_disable();
			sosMIDISetMasterVolume(midi_volume);
			_enable();
		}
	}
	sound_paused--;
}


#ifndef NDEBUG
void digi_debug()
{
	int i;
	int n_voices=0;

	if (!Digi_initialized) return;
	if ( digi_driver_board <= 0 ) return;

	for (i=0; i<digi_max_channels; i++ )	{
		if ( digi_is_channel_playing(i) )
			n_voices++;
	}

	mprintf_at(( 0, 2, 0, "DIGI: Active Sound Channels: %d/%d (HMI says %d/32)      ", n_voices, digi_max_channels, sosDIGISamplesPlaying(hSOSDigiDriver) ));
	mprintf_at(( 0, 3, 0, "DIGI: Number locked sounds:  %d                          ", digi_total_locks ));
}
#endif

