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

char inferno_rcsid[] = "$Id: inferno.c 2.190 1996/10/29 18:02:51 matt Exp $";
char copyright[] = "DESCENT II  COPYRIGHT (C) 1994-1996 PARALLAX SOFTWARE CORPORATION";

#include <dos.h>
#include <conio.h>
#include <stdio.h>
#include <malloc.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <direct.h>
#include <math.h>
#include <unistd.h>
#include <conio.h>
#include <ctype.h>

#include "pa_enabl.h"       //$$POLY_ACC
#include "gr.h"
#include "vga.h"
#include "mono.h"
#include "key.h"
#include "timer.h"
#include "3d.h"
#include "bm.h"
#include "inferno.h"
#include "error.h"
#include "div0.h"
#include "game.h"
#include "segment.h"		//for Side_to_verts
#include "mem.h"
#include "segpoint.h"
#include "screens.h"
#include "texmap.h"
#include "texmerge.h"
#include "menu.h"
#include "wall.h"
#include "polyobj.h"
#include "effects.h"
#include "digi.h"
#include "iff.h"
#include "pcx.h"
#include "palette.h"
#include "args.h"
#include "sounds.h"
#include "titles.h"
#include "player.h"
#include "text.h"
#include "ipx.h"
#include "newdemo.h"
#include "victor.h"
#include "network.h"
#include "modem.h"
#include "gamefont.h"
#include "kconfig.h"
#include "mouse.h"
#include "joy.h"
#include "newmenu.h"
#include "desc_id.h"
#include "config.h"
#include "joydefs.h"
#include "multi.h"
#include "iglasses.h"
#include "songs.h"
#include "cfile.h"
#include "gameseq.h"
#include "gamepal.h"
#include "mission.h"
#include "dpmi.h"
#include "movie.h"
#include "compbit.h"
#include "vfx.h"

#ifdef TACTILE
#include "tactile.h"
#endif

#  include "3dfx_des.h"

#if defined(POLY_ACC)
#include "poly_acc.h"
extern int Current_display_mode;        //$$ there's got to be a better way than hacking this.
#endif

#ifdef ARCADE
#include "arcade.h"
#include "coindev.h"
#endif

#ifdef EDITOR
#include "editor\editor.h"
#include "editor\kdefs.h"
#include "ui.h"
#endif

#include "vers_id.h"

//Current version number

ubyte Version_major = 1;		//FULL VERSION
ubyte Version_minor = 2;

static const char desc_id_checksum_str[] = DESC_ID_CHKSUM_TAG "0000";	//4-byte checksum
char desc_id_exit_num = 0;

int Function_mode=FMODE_MENU;		//game or editor?
int Screen_mode=-1;					//game screen or editor screen?

//--unused-- grs_bitmap Inferno_bitmap_title;

int WVIDEO_running=0;		//debugger can set to 1 if running

#ifdef EDITOR
int Inferno_is_800x600_available = 0;
#endif

//--unused-- int Cyberman_installed=0;			// SWIFT device present
ubyte CybermouseActive=0;

int __far descent_critical_error_handler( unsigned deverr, unsigned errcode, unsigned __far * devhdr );

void check_joystick_calibration(void);


//--------------------------------------------------------------------------

#ifndef NDEBUG
void do_heap_check()
{
	int heap_status;

	heap_status = _heapset( 0xFF );
	switch( heap_status )
	{
	case _HEAPBADBEGIN:
		mprintf((1, "ERROR - heap is damaged\n"));
		Int3();
		break;
	case _HEAPBADNODE:
		mprintf((1, "ERROR - bad node in heap\n" ));
		Int3();
		break;
	default:
		Error("Invalid heap status.");
	}
}
#endif

int registered_copy=0;
char name_copy[DESC_ID_LEN];

void
check_id_checksum_and_date()
{
	const char name[sizeof(DESC_ID_TAG)-1+DESC_ID_LEN] = DESC_ID_TAG;
	char time_str[] = DESC_DEAD_TIME_TAG "00000000";	//second part gets overwritten
	int i, found;
	unsigned long *checksum, test_checksum;
	time_t current_time, saved_time;

	saved_time = (time_t)strtol(time_str + strlen(DESC_DEAD_TIME_TAG), NULL, 16);
	if (saved_time == (time_t)0)
		return;

	strcpy(name_copy,name+strlen(DESC_ID_TAG));
	registered_copy = 1;

	current_time = time(NULL);
	if (current_time >= saved_time)
		desc_id_exit_num = 1;

	test_checksum = 0;
	for (i = 0; i < strlen(name_copy); i++) {
		found = 0;	  
		test_checksum += name_copy[i];
		if (((test_checksum / 2) * 2) != test_checksum)
			found = 1;
		test_checksum = test_checksum >> 1;
		if (found)
			test_checksum |= 0x80000000;
	}
	checksum = (unsigned long *)(desc_id_checksum_str + strlen(DESC_ID_CHKSUM_TAG));
	if (test_checksum != *checksum)
		desc_id_exit_num = 2;

	printf ("%s %s\n", TXT_REGISTRATION, name_copy);
}

int init_graphics()
{
	int result;

	vga_init();

#if defined(POLY_ACC)
    result=vga_check_mode(SM_640x480x15xPA);
#else
	result=vga_check_mode(SM_320x200C);
#endif

#ifdef EDITOR
	if ( result==0 )	
		result=vga_check_mode(SM_800x600V);
#endif

	switch( result )	{
		case  0:		//Mode set OK
#ifdef EDITOR
						Inferno_is_800x600_available = 1;
#endif
						break;
		case  1:		//No VGA adapter installed
						printf("%s\n", TXT_REQUIRES_VGA );
						return 1;
		case 10:		//Error allocating selector for A0000h
						printf( "%s\n",TXT_ERROR_SELECTOR );
						return 1;
		case 11:		//Not a valid mode support by gr.lib
						printf( "%s\n", TXT_ERROR_GRAPHICS );
						return 1;
#ifdef EDITOR
		case  3:		//Monitor doesn't support that VESA mode.
		case  4:		//Video card doesn't support that VESA mode.

						printf( "Your VESA driver or video hardware doesn't support 800x600 256-color mode.\n" );
						break;
		case  5:		//No VESA driver found.
						printf( "No VESA driver detected.\n" );
						break;
		case  2:		//Program doesn't support this VESA granularity
		case  6:		//Bad Status after VESA call/
		case  7:		//Not enough DOS memory to call VESA functions.
		case  8:		//Error using DPMI.
		case  9:		//Error setting logical line width.
		default:
						printf( "Error %d using 800x600 256-color VESA mode.\n", result );
						break;
#endif
	}

	return 0;
}

void check_dos_version()
{
	int major, minor;
	union REGS regs;

	memset(&regs,0,sizeof(regs));
	regs.x.eax = 0x3000;							// Get MS-DOS Version Number
   int386( 0x21, &regs, &regs );

	major = regs.h.al;
	minor = regs.h.ah;
	
	if ( major < 5 )	{
		Error( "%s %d.%d\n%s", TXT_DOS_VERSION_1, major, minor, TXT_DOS_VERSION_2);
	}
	//printf( "\nUsing MS-DOS %d.%d...\n", major, minor );
}

void change_to_dir(char *cmd_line)
{
	char drive[_MAX_DRIVE], dir[_MAX_DIR], curdir[_MAX_DIR];
	unsigned total, cur_drive;

	_splitpath(cmd_line, drive, dir, NULL, NULL);
	dir[strlen(dir) - 1] = '\0';
	if (drive[0] != '\0') {
		_dos_getdrive(&cur_drive);
		if (cur_drive != (drive[0] - 'A' + 1))
			_dos_setdrive(drive[0] - 'A' + 1, &total);
	}
	getcwd(curdir, _MAX_DIR);
	if (stricmp(&(curdir[2]), dir))
		chdir(dir);
}

void dos_check_file_handles(int num_required)
{
	int i, n;
	FILE * fp[16];

	if ( num_required > 16 )
		num_required = 16;

	n = 0;	
	for (i=0; i<16; i++ )
		fp[i] = NULL;
	for (i=0; i<16; i++ )	{
		fp[i] = fopen( "nul", "wb" );
		if ( !fp[i] ) break;
	}
	n = i;
	for (i=0; i<16; i++ )	{
		if (fp[i])
			fclose(fp[i]);
	}
	if ( n < num_required )	{
		Error(	"\n%s\n"
					"------------------------\n"
					"%d/%d %s\n"
					"%s\n" 
					"%s\n",
						TXT_NOT_ENOUGH_HANDLES,
						n, num_required, TXT_HANDLES_1,
						TXT_HANDLES_2,
						TXT_HANDLES_3);
	}
}

#define NEEDED_DOS_MEMORY   			( 300*1024)		// 300 K
#define NEEDED_LINEAR_MEMORY 			(7680*1024)		// 7.5 MB
#define LOW_PHYSICAL_MEMORY_CUTOFF	(5*1024*1024)	// 5.0 MB
#define NEEDED_PHYSICAL_MEMORY		(2000*1024)		// 2000 KB
#define SOUND_22K_CUTOFF				(10*1024*1024)	// 10.0 MB

extern int piggy_low_memory;

void mem_int_to_string( int number, char *dest )
{
	int i,l,c;
	char buffer[20],*p;

	sprintf( buffer, "%d", number );

	l = strlen(buffer);
	if (l<=3) {
		// Don't bother with less than 3 digits
		sprintf( dest, "%d", number );
		return;
	}

	c = 0;
	p=dest;
	for (i=l-1; i>=0; i-- )	{
		if (c==3) {
			*p++=',';
			c = 0;
		}
		c++;
		*p++ = buffer[i];
	}
	*p++ = '\0';
	strrev(dest);
}

void check_memory()
{
	char text[32];

	printf( "\n%s\n", TXT_AVAILABLE_MEMORY);
	printf( "----------------\n" );
	mem_int_to_string( dpmi_dos_memory/1024, text );
	printf( "Conventional: %7s KB\n", text );
	mem_int_to_string( dpmi_physical_memory/1024, text );
	printf( "Extended:     %7s KB\n", text );
	if ( dpmi_available_memory > dpmi_physical_memory )	{
		mem_int_to_string( (dpmi_available_memory-dpmi_physical_memory)/1024, text );
	} else {
		mem_int_to_string( 0, text );
	}
	printf( "Virtual:      %7s KB\n", text );
	printf( "\n" );

	if ( !FindArg( "-nomemcheck" ))
		if ( dpmi_dos_memory < NEEDED_DOS_MEMORY )	{
			Error( "%d %s\n", NEEDED_DOS_MEMORY - dpmi_dos_memory, TXT_MEMORY_CONFIG );
		}

	if ( dpmi_available_memory < NEEDED_LINEAR_MEMORY )	{
		if ( dpmi_virtual_memory )	{
			Error( "%d %s\n", NEEDED_LINEAR_MEMORY - dpmi_available_memory, TXT_RECONFIGURE_VMM );
		} else {
			Error( "%d %s\n%s\n", NEEDED_LINEAR_MEMORY - dpmi_available_memory, TXT_MORE_MEMORY, TXT_MORE_MEMORY_2 );
		}
	}

	if ( dpmi_physical_memory < NEEDED_PHYSICAL_MEMORY )	{
		if ( dpmi_virtual_memory )	{	
			Error( "%d %s\n%s\n", NEEDED_PHYSICAL_MEMORY - dpmi_physical_memory, TXT_PHYSICAL_MEMORY, TXT_PHYSICAL_MEMORY_2 );
		}
		else
			Error( "%d %s\n", NEEDED_PHYSICAL_MEMORY - dpmi_physical_memory, TXT_PHYSICAL_MEMORY );
	}

	if ( dpmi_physical_memory < LOW_PHYSICAL_MEMORY_CUTOFF )	{
		piggy_low_memory = 1;
	}

	if ( ((dpmi_physical_memory > SOUND_22K_CUTOFF) || FindArg("-sound22k")) && !FindArg("-sound11k") && !FindArg("-lowmem") )
		digi_sample_rate = SAMPLE_RATE_22K;
	else
		digi_sample_rate = SAMPLE_RATE_11K;

}


int Inferno_verbose = 0;

//NO_STACK_SIZE_CHECK uint * stack, *stack_ptr;
//NO_STACK_SIZE_CHECK int stack_size, unused_stack_space;
//NO_STACK_SIZE_CHECK int sil;
//NO_STACK_SIZE_CHECK 
//NO_STACK_SIZE_CHECK int main(int argc,char **argv)
//NO_STACK_SIZE_CHECK {
//NO_STACK_SIZE_CHECK 	uint ret_value;
//NO_STACK_SIZE_CHECK 	
//NO_STACK_SIZE_CHECK 	unused_stack_space = 0;
//NO_STACK_SIZE_CHECK 	stack = &ret_value;
//NO_STACK_SIZE_CHECK 	stack_size = stackavail()/4;
//NO_STACK_SIZE_CHECK 
//NO_STACK_SIZE_CHECK 	for ( sil=0; sil<stack_size; sil++ )	{
//NO_STACK_SIZE_CHECK 		stack--;
//NO_STACK_SIZE_CHECK 		*stack = 0xface0123;
//NO_STACK_SIZE_CHECK 	}
//NO_STACK_SIZE_CHECK 
//NO_STACK_SIZE_CHECK 	ret_value = descent_main( argc, argv );		// Rename main to be descent_main
//NO_STACK_SIZE_CHECK 
//NO_STACK_SIZE_CHECK 	for ( sil=0; sil<stack_size; sil++ )	{
//NO_STACK_SIZE_CHECK 		if ( *stack == 0xface0123 )	
//NO_STACK_SIZE_CHECK 			unused_stack_space++;
//NO_STACK_SIZE_CHECK 		stack++;
//NO_STACK_SIZE_CHECK 	}
//NO_STACK_SIZE_CHECK 
//NO_STACK_SIZE_CHECK 	mprintf(( 0, "Program used %d/%d stack space\n", (stack_size - unused_stack_space)*4, stack_size*4 ));
//NO_STACK_SIZE_CHECK 	key_getch();
//NO_STACK_SIZE_CHECK 
//NO_STACK_SIZE_CHECK 	return ret_value;
//NO_STACK_SIZE_CHECK }

extern int digi_timer_rate;

int descent_critical_error = 0;
unsigned descent_critical_deverror = 0;
unsigned descent_critical_errcode = 0;

#pragma off (check_stack)
int __far descent_critical_error_handler(unsigned deverror, unsigned errcode, unsigned __far * devhdr )
{
	devhdr = devhdr;
	descent_critical_error++;
	descent_critical_deverror = deverror;
	descent_critical_errcode = errcode;
	return _HARDERR_FAIL;
}
void chandler_end (void)  // dummy functions
{
}
#pragma on (check_stack)

extern int Network_allow_socket_changes;

extern void vfx_set_palette_sub(ubyte *);

extern int Game_vfx_flag;
extern int Game_victor_flag;
extern int Game_vio_flag;
extern int Game_3dmax_flag;
extern int VR_low_res;

extern int Config_vr_type;
extern int Config_vr_resolution;
extern int Config_vr_tracking;
extern int grd_fades_disabled;

#define LINE_LEN	100

//read help from a file & print to screen
print_commandline_help()
{
	CFILE *ifile;
	int have_binary=0;
	char line[LINE_LEN];
	int screen_lines,line_count;

	ifile = cfopen("help.tex","rb");
	if (!ifile) {
		ifile = cfopen("help.txb","rb");
		if (!ifile)
			Error("Cannot load help text file.");
		have_binary = 1;
	}

	screen_lines = *((ubyte *) 0x484);

	line_count = 3 + registered_copy;		//count banner lines, & maybe registration line

	while (cfgets(line,LINE_LEN,ifile)) {

		if (have_binary) {
			int i;
			for (i = 0; i < strlen(line) - 1; i++) {
				encode_rotate_left(&(line[i]));
				line[i] = line[i] ^ BITMAP_TBL_XOR;
				encode_rotate_left(&(line[i]));
			}
		}

		if (line[0] == ';')
			continue;		//don't show comments

		printf("%s",line);

		if (isatty(stdout->_handle) && ++line_count == screen_lines-1) {
			char c;
			printf("\n%s",TXT_PRESS_ANY_KEY3);
			c = key_getch();
			printf("\n");
			if (c=='q' || c=='Q' || c==27)
				return;
			line_count=0;
		}
	}

	cfclose(ifile);

}

void do_joystick_init()
{
 
	joy_set_timer_rate( digi_timer_rate );	 	// Tell joystick how fast timer is going

#ifdef TACTILE
	{
   int t;
   if (t=FindArg ("-iforce"))
    {				  
	  TactileStick=TACTILE_IMMERSION;			  
     Tactile_open ((int)atoi(Args[t+1]));
    }
	}
#endif

	if (!FindArg( "-nojoystick" ))	{
		if (Inferno_verbose) printf( "\n%s", TXT_VERBOSE_6);
		joy_init();
		if ( FindArg( "-joyslow" ))	{
			if (Inferno_verbose) printf( "\n%s", TXT_VERBOSE_7);
			joy_set_slow_reading(JOY_SLOW_READINGS);
		}
		if ( FindArg( "-joypolled" ))	{
			if (Inferno_verbose) printf( "\n%s", TXT_VERBOSE_8);
			joy_set_slow_reading(JOY_POLLED_READINGS);
		}
		if ( FindArg( "-joybios" ))	{
			if (Inferno_verbose) printf( "\n%s", TXT_VERBOSE_9);
			joy_set_slow_reading(JOY_BIOS_READINGS);
		}

	//	Added from Descent v1.5 by John.  Adapted by Samir.
		if ( FindArg( "-gameport" ))    {
        	if ( init_gameport() )  {
         	joy_set_slow_reading(JOY_BIOS_READINGS);
        	} else {
                Error( "\nCouldn't initialize the Notebook Gameport.\nMake sure the NG driver is loaded.\n" );
        	}
		}
	} else {
		if (Inferno_verbose) printf( "\n%s", TXT_VERBOSE_10);
	}
}

// These must match the setup code!
#define VRD_NONE					0
#define VRD_VFX1					1
#define VRD_CYBERMAXX			2
#define VRD_CYBERMAXX2			3
#define VRD_IGLASSES				4
#define VRD_3DMAX					5
#define VRD_LCDBIOS				6
#define VRD_CYBERSHADES			7
#define VRD_SIMULEYES			8

//set this to force game to run in low res
int disable_high_res=0;

//initialize stuff for various VR headsets
void do_headset_init()
{
	//int i;
	int is_special=1;		//assume special mode

	switch( Config_vr_type )	{
	case VRD_NONE:
		set_display_mode(0);		//set normal 320x200 mode
		is_special = 0;			//not a special mode
		break;
	case VRD_VFX1: {
		int status;
		status = vfx_init();
		mprintf((0,"VFX statux = %x\n",status));
		if (!status) {
			printf(	"Warning: Cannot find VFX1 driver.\n"
						" Press Esc to abort Descent 2, any other key to continue without VFX1 support.\n"
						" Use SETUP to disable VFX1 support.\n");
			if (key_getch() == KEY_ESC)
				exit(1);
			set_display_mode(0);
			is_special = 0;
		}
		else {
			//make sure we have version 2.0 drivers by trying to go into 
			//line-sequential mode.  If we fail to go into that mode, we
			//must have the old drivers (I guess)
			if (vfx_enable_stereo()) {	//non-zero mean error
				printf(	"Warning: You must have version 2.0 or later drivers to run in VFX-1 mode.\n"
							" Contact Forte at 716-427-8604 or http://www.fortevr.com for updated drivers.\n"
							" Press Esc to abort Descent 2, any other key to continue without VFX1 support.\n"
							" Use SETUP to disable VFX1 support.\n");
				if (key_getch() == KEY_ESC)
					exit(1);
				set_display_mode(0);
				is_special = 0;
			}
			else
				vfx_disable_stereo();

			if (Config_vr_tracking>0)
				vfx_sense_init();
			Game_vfx_flag = 1;
			//game_init_render_buffers(SM_640x480V, 320, 240, VR_AREA_DET, 0 );
			game_init_render_buffers(SM_320x400U, 320, 200, VR_INTERLACED, VRF_USE_PAGING );
			//VR_low_res = 3;
			disable_high_res=1;	//headset can't display 640x480
		}
		
		break;
	}
	case VRD_CYBERMAXX:
		if ( Config_vr_tracking>0 )
			victor_init_tracking( Config_vr_tracking );
		Game_victor_flag = 1;
		game_init_render_buffers(SM_320x200C, 320, 100, VR_INTERLACED, VRF_COMPATIBLE_MENUS );
		VR_low_res = 0;
		break;
	case VRD_CYBERMAXX2:
		if ( Config_vr_tracking>0 )
			victor_init_tracking( Config_vr_tracking );
		game_init_render_buffers(SM_320x400U, 320, 200, VR_INTERLACED, VRF_USE_PAGING );
		VR_low_res = 3;
		break;
	case VRD_IGLASSES:
		if (Config_vr_tracking>0)
			iglasses_init_tracking( Config_vr_tracking );
		Game_vio_flag = 1;
		game_init_render_buffers(SM_320x400U, 320, 200, VR_INTERLACED, VRF_USE_PAGING );
		VR_low_res = 3;
		break;
	case VRD_3DMAX:
		if (!is_3dbios_installed())     {
			printf(	"Warning: Kasan's 3DBIOS needs to be installed.\n"
						" Press Esc to abort Descent 2, any other key to continue without 3DMax support.\n"
						" Use SETUP to disable 3DMax support.\n");
			if ( key_getch() == KEY_ESC )
				exit(1);
			set_display_mode(0);
			is_special = 0;
			break;
		}
		if ( Config_vr_resolution==1 )	{
			Game_3dmax_flag = 2;
			//use special mode handled by 3dmax bios
			game_init_render_buffers(SM_320x400_3DMAX, 320, 200, VR_INTERLACED, 0);
			VR_low_res = 0;
		} else {
			Game_3dmax_flag = 1;
			game_init_render_buffers(SM_320x200C, 320, 100, VR_INTERLACED, 0 );
			VR_low_res = 0;
		}
		break;
	case VRD_LCDBIOS:
	case VRD_CYBERSHADES:
	case VRD_SIMULEYES:
		if ( Config_vr_type == VRD_SIMULEYES )
			VR_use_reg_code = 1;

		if (!is_3dbios_installed()) {
			printf(	"Warning: LCDBIOS needs to be installed.\n"
						" Press Esc to abort D2, any other key to continue without LCDBIOS support.\n"
						" Use SETUP to disable LCDBIOS support.\n");
			if ( key_getch() == KEY_ESC )
				exit(1);
			set_display_mode(0);
			is_special = 0;
			break;
		}
		if (Inferno_verbose) printf( "Enabling LCDBIOS...\n" );
		Game_3dmax_flag = 3;
		switch( Config_vr_resolution )	{
		case 0:	game_init_render_buffers(SM_320x200C, 320, 100, VR_INTERLACED, 0 );	break;
		case 1:	game_init_render_buffers(SM_320x400U, 320, 200, VR_INTERLACED, VRF_USE_PAGING ); break;
		case 2:	game_init_render_buffers(SM_640x400V, 640, 200, VR_INTERLACED, 0 ); break;
		case 3:	game_init_render_buffers(SM_640x480V, 640, 240, VR_INTERLACED, 0 ); break;
		case 4:	game_init_render_buffers(SM_800x600V, 800, 300, VR_INTERLACED, 0 ); break;
		}
		VR_low_res = 0;
		break;
	}

	if (Game_victor_flag) {
		char *vswitch = getenv( "CYBERMAXX" );
		if ( vswitch )	{
			char *p = strstr( vswitch, "/E:R" ); 
			if ( p )	{
				VR_eye_switch = 1;
			} else 
				VR_eye_switch = 0;
		} else {		
			VR_eye_switch = 0;
		}
	}

	if (is_special)
		set_display_mode(-1);	//flag as special non-changable mode
}

do_register_player(ubyte *title_pal)
{
	Players[Player_num].callsign[0] = '\0';

	if (!Auto_demo) 	{

		key_flush();

		//now, before we bring up the register player menu, we need to 
		//do some stuff to make sure the palette is ok.  First, we need to
		//get our current palette into the 2d's array, so the remapping will
		//work.  Second, we need to remap the fonts.  Third, we need to fill 
		//in part of the fade tables so the darkening of the menu edges works

		memcpy(gr_palette,title_pal,sizeof(gr_palette));
		remap_fonts_and_menus(1);
		RegisterPlayer();		//get player's name
	}

}

#define verbose  if (Inferno_verbose) printf

do_network_init()
{
	if (!FindArg( "-nonetwork" ))	{
		int socket=0, showaddress=0, t;
		int ipx_error;

		verbose( "\n%s ", TXT_INITIALIZING_NETWORK);
		if ((t=FindArg("-socket")))
			socket = atoi( Args[t+1] );
		//@@if ( FindArg("-showaddress") ) showaddress=1;
		if ((ipx_error=ipx_init(IPX_DEFAULT_SOCKET+socket,showaddress))==0)	{
  			verbose( "%s %d.\n", TXT_IPX_CHANNEL, socket );
			Network_active = 1;
		} else {
			switch( ipx_error )	{
			case 3: 	verbose( "%s\n", TXT_NO_NETWORK); break;
			case -2: verbose( "%s 0x%x.\n", TXT_SOCKET_ERROR, IPX_DEFAULT_SOCKET+socket); break;
			case -4: verbose( "%s\n", TXT_MEMORY_IPX ); break;
			default:
				verbose( "%s %d", TXT_ERROR_IPX, ipx_error );
			}
			verbose( "%s\n",TXT_NETWORK_DISABLED);
			Network_active = 0;		// Assume no network
		}
		ipx_read_user_file( "descent.usr" );
		ipx_read_network_file( "descent.net" );
		//@@if ( FindArg( "-dynamicsockets" ))
		//@@	Network_allow_socket_changes = 1;
		//@@else
		//@@	Network_allow_socket_changes = 0;
	} else {
		verbose( "%s\n", TXT_NETWORK_DISABLED);
		Network_active = 0;		// Assume no network
	}

}


//	3d BIOS enhancements from Descent v1.5 by John.
//		Added by Samir.

int is_3dbios_installed()
{
	dpmi_real_regs rregs;

  	memset(&rregs,0,sizeof(dpmi_real_regs));
 	rregs.eax = 0x4ed0;
  	dpmi_real_int386x( 0x10, &rregs );
  	if ( (rregs.edx & 0xFFFF) != 0x3344 ) return 0;
   else return 1;
}


int init_gameport()
{
 	union REGS regs;

  	memset(&regs,0,sizeof(regs));
  	regs.x.eax = 0x8400;
  	regs.x.edx = 0xF0;
 	int386( 0x15, &regs, &regs );
   if ( ( regs.x.eax & 0xFFFF ) == 'SG' )	return 1;
   else return 0;
}

typedef struct {
	char unit;
	ushort dev_offset;
	ushort dev_segment;
} dev_list;

typedef struct _Dev_Hdr {
	unsigned int dev_next;
	unsigned short dev_att;
	ushort dev_stat;
	ushort dev_int;
	char dev_name[8];
	short dev_resv;
	char dev_letr;
	char dev_units;
} dev_header;

char CDROM_dir[30] = "";

//serach available CDs for the Descent 2 CD
//returns neg number if error, else drive letter (a:==1)
int find_descent_cd()
{
	dpmi_real_regs rregs;
		
	// Get dos memory for call...
	char * buf;
	int num_drives, i;
	unsigned cdrive, cur_drive, cdrom_drive;

	memset(&rregs,0,sizeof(dpmi_real_regs));
	rregs.eax = 0x1500;
	rregs.ebx = 0;
	dpmi_real_int386x( 0x2f, &rregs );
	if ((rregs.ebx & 0xffff) == 0) {
		return -1;			// No cdrom
	}
	num_drives = rregs.ebx;

	buf = (char *)dpmi_get_temp_low_buffer( sizeof(char *) );	
	if (buf==NULL) {
		return -2;			// Error getting memory!
	}

	memset(&rregs,0,sizeof(dpmi_real_regs));
	rregs.es = DPMI_real_segment(buf);
	rregs.ebx = DPMI_real_offset(buf);
	rregs.eax = 0x150d;
	dpmi_real_int386x( 0x2f, &rregs );
	cdrom_drive = -3;
	_dos_getdrive(&cdrive);

//	mprintf ((0,"num_cd_drives=%d\n",num_drives));

	for (i = 0; i < num_drives; i++) {

//		mprintf ((0,"buf[%d]=%c\n",i,'A'+buf[i]));
		_dos_setdrive(buf[i]+1,&cur_drive);
		_dos_getdrive(&cur_drive);

   //   mprintf ((0,"cur_drive=%d\n",cur_drive));

		if (cur_drive == (buf[i]+1)) {
			#ifndef D2_OEM
			char path[] = "d:\\*.*";
			struct find_t ffblk;

			//check Volume name
			path[0] = 'A'+buf[i];
			_dos_findfirst(path,_A_VOLID,&ffblk);
//			mprintf((0,"volume name = <%s>\n",ffblk.name));
			if (ffblk.name[strlen(ffblk.name)-1] == '.')
				ffblk.name[strlen(ffblk.name)-1] = 0;		//kill dot if present
			if (!stricmp(ffblk.name,"DESCENT.II") || !stricmp(ffblk.name,"DESCENT_.II")) {
				char save_dir[256];

				//Found correct volume name.  Now check for data dir

				getcwd(save_dir,sizeof(save_dir));

				if (!chdir("\\d2data")) {
					chdir(save_dir);
					cdrom_drive = buf[i]+1;
					goto FoundIt;
				}
			}				
			#else
			{	//check for OEM disk

				char spec[64];
				FILE *cd_file;

				sprintf(spec, "%c:descent2.cd", cdrive + 'A' - 1);
				cd_file = fopen(spec,"rt");
				if (cd_file) {
					char *t;
					char save_dir[256];

					fgets(CDROM_dir,sizeof(CDROM_dir),cd_file);
					fclose(cd_file);

					if ((t=strchr(CDROM_dir,'\n')) != NULL)
						*t = 0;

					CDROM_dir[0] = 'A'+buf[i];	//set correct drive letter

					getcwd(save_dir,sizeof(save_dir));

					if (!chdir(CDROM_dir)) {
						chdir(save_dir);
						cdrom_drive = buf[i]+1;
						strcat(CDROM_dir, "\\");
						goto FoundIt;
					}

				}
			}
	
			#endif

		}				
	}
   
   FoundIt:

	_dos_setdrive(cdrive,&cur_drive);
	
// mprintf ((0,"cur_drive=%d cdrom_drive=%d\n",cur_drive,cdrom_drive));     
	return cdrom_drive;
}

//look for D2 CD-ROM.  returns 1 if found cd, else 0
init_cdrom()
{
	int i;

	//scan for CD, etc.

	i = find_descent_cd();

	if (i < 0) {			//no CD

		#ifndef RELEASE
		if ((i=FindArg("-cdproxy")) != 0) {

			#ifndef D2_OEM
			strcpy(CDROM_dir,"d:\\d2data\\");	//set directory
			#else
			strcpy(CDROM_dir,"d:\\d2data\\");	//set directory
			#endif

			CDROM_dir[0] = Args[i+1][0];
			return 1;
		}
		#endif

		return 0;
	}
	else {
		#ifndef D2_OEM		//if OEM, already filled in
		strcpy(CDROM_dir,"d:\\d2data\\");	//set directory
		CDROM_dir[0] = 'A'+i-1;					//set correct drive leter
		#endif

		return 1;
	}
}

#ifdef SHAREARE
#define PROGNAME "d2demo"
#else
#define PROGNAME "d2"
#endif

extern char Language[];

//can we do highres menus?
extern int MenuHiresAvailable;

#ifdef D2_OEM
int intro_played = 0;
#endif

int open_movie_file(char *filename,int must_have);

#if defined(POLY_ACC)
#define MENU_HIRES_MODE SM_640x480x15xPA
#else
#define MENU_HIRES_MODE SM_640x480V
#endif

//	DESCENT II by Parallax Software
//		Descent Main for DOS.

//extern ubyte gr_current_pal[];

#ifdef TACTILE
 extern void SerialHardwareInit();
 extern void SerialInit();
 extern void StatusCheck();
 extern char InBuf[];
 extern int SerialPort;
 extern char StatusBytes[];
#endif

#ifdef	EDITOR
int	Auto_exit = 0;
char	Auto_file[128] = "";
#endif

int main(int argc,char **argv)
{
	int i,t;		//note: don't change these without changing stack lockdown code below
	ubyte title_pal[768];

	error_init(NULL, NULL);

	setbuf(stdout, NULL);	// unbuffered output via printf

	InitArgs( argc,argv );

	if ( FindArg( "-verbose" ) )
		Inferno_verbose = 1;

	// Initialize DPMI before anything else!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	// (To check memory size and availbabitliy and allocate some low DOS memory)
	verbose( "%s... ", TXT_INITIALIZING_DPMI);
	dpmi_init(Inferno_verbose);		// Before anything
	verbose( "\n" );

	if (!dpmi_lock_region((void near *)descent_critical_error_handler,(char *)chandler_end - (char near *)descent_critical_error_handler))	{
		Error( "Unable to lock critial error handler" );
	}
	if (!dpmi_lock_region(&descent_critical_error,sizeof(int)))	{
		Error( "Unable to lock critial error handler" );
	}
	if (!dpmi_lock_region(&descent_critical_deverror,sizeof(unsigned)))	{
		Error( "Unable to lock critial error handler" );
	}
	if (!dpmi_lock_region(&descent_critical_errcode,sizeof(unsigned)))	{
		Error( "Unable to lock critial error handler" );
	}
//lock	dpmi_lock_region(&gr_current_pal,4096);
//nolock	dpmi_lock_region(&gr_current_pal,1024*1024);
//nolock	dpmi_lock_region(&gr_current_pal,1024*1024/2);
//nolock	dpmi_lock_region(&gr_current_pal,1024*1024/4);
//lock	dpmi_lock_region(&gr_current_pal,1024*1024/8);
//nolock	dpmi_lock_region(((ubyte *) &gr_current_pal)+1024*1024/8,1024*1024/8);
//nolock	dpmi_lock_region(((ubyte *) &gr_current_pal)+1024*1024/8,1024*1024/16);
//lock	dpmi_lock_region(((ubyte *) &gr_current_pal)+1024*1024/8,1024*1024/32);
//nolock	dpmi_lock_region(((ubyte *) &gr_current_pal)+1024*1024/8+1024*1024/32,1024*1024/32);
//nolock, same as above:	dpmi_lock_region(((ubyte *) &gr_current_pal)+1024*160,1024*32);
//nolock	dpmi_lock_region(((ubyte *) &gr_current_pal)+1024*160,1024*16);
//nolock	dpmi_lock_region(((ubyte *) &gr_current_pal)+1024*160,1024*8);
//nolock	dpmi_lock_region(((ubyte *) &gr_current_pal)+1024*160,1024*4);
//lock	dpmi_lock_region(((ubyte *) &gr_current_pal)+1024*160,1024*2);
//nolock	dpmi_lock_region(((ubyte *) &gr_current_pal)+1024*162,1024*2);
//nolock	dpmi_lock_region(((ubyte *) &gr_current_pal)+1024*162,1024*1);
//lock	dpmi_lock_region(((ubyte *) &gr_current_pal)+1024*162,1);
//nolock	dpmi_lock_region(((ubyte *) &gr_current_pal)+1024*162+512,1);

	//lock down the entire stack, in the hopes of fixing the crazy hang
	//bug that's been plaguing us these last few days
	//NOTE: this code assumes that the varible i is the first thing 
	//on the stack in this routine, and that the stack is 50K
	dpmi_lock_region(((ubyte *) &i)-1024*50,1024*50);

   #ifdef TACTILE
	 //these adresses and lengths are based on perusing the map file.
	 //they could always change later with a different compiler or
	 //different version of the compiler.
	 dpmi_lock_region ((void near *)SerialHardwareInit,4096);
	 dpmi_lock_region ((void near *)SerialInit,4096);
	 dpmi_lock_region ((void near *)StatusCheck,4096);
	 dpmi_lock_region ((ubyte *)&SerialPort,4096);
	 dpmi_lock_region ((ubyte *)InBuf,4096);
	 dpmi_lock_region ((ubyte *)StatusBytes,4096);
	#endif

	verbose( "\n%s...", TXT_INITIALIZING_CRIT);
	_harderr((void *)descent_critical_error_handler );

	//tell cfile about our counter
	cfile_set_critical_error_counter_ptr(&descent_critical_error);

	#ifdef SHAREWARE
		cfile_init("d2demo.hog");			//specify name of hogfile
	#else
	#define HOGNAME "descent2.hog"
	if (! cfile_init(HOGNAME)) {		//didn't find HOG.  Check on CD
		#ifdef RELEASE
			Error("Could not find required file <%s>",HOGNAME);
		#endif
	}
	#endif
	
	load_text();

	//print out the banner title
	printf("\nDESCENT 2 %s v%d.%d",VERSION_TYPE,Version_major,Version_minor);
	#ifdef VERSION_NAME
	printf("  %s", VERSION_NAME);
	#endif
	#ifdef SANTA
	printf("  Vertigo Enhanced");
	#endif
	printf("  %s %s\n", __DATE__,__TIME__);
	printf("%s\n%s\n",TXT_COPYRIGHT,TXT_TRADEMARK);	

	check_id_checksum_and_date();

	if (FindArg( "-?" ) || FindArg( "-help" ) || FindArg( "?" ) ) {
		print_commandline_help();
		set_exit_message("");
		return(0);
	}

	printf("\n");
	printf(TXT_HELP, PROGNAME);		//help message has %s for program name
	printf("\n");

	verbose( "\n%s...", "Checking for Descent 2 CD-ROM");
	if (!init_cdrom()) {		//look for Cd, etc.
		#if defined(RELEASE) && !defined(D2_OEM)
			Error("Sorry, the Descent 2 CD must be present to run.");
		#endif
	}
	else {		//check to make sure not running off CD
		if (toupper(argv[0][0]) == toupper(CDROM_dir[0]))
			Error("You cannot run Descent II directly from your CD-ROM drive.\n"
					"       To run Descent II, CD to the directory you installed to, then type D2.\n"
					"       To install Descent II, type %c:\\INSTALL.",toupper(CDROM_dir[0]));
	}

	#ifdef PASSWORD
	if ((t = FindArg("-pswd")) != 0) {
		int	n;
		byte	*pp = Side_to_verts;
		int ch;
		for (n=0; n<6; n++)
			for (ch=0; ch<strlen(Args[t+1]); ch++)
				*pp++ ^= Args[t+1][ch];
	}
	else 
		Error("Invalid processor");		//missing password
	#endif

	if ( FindArg( "-autodemo" ))
		Auto_demo = 1;

	#ifndef RELEASE
	if ( FindArg( "-noscreens" ) )
		Skip_briefing_screens = 1;
	#endif

	Lighting_on = 1;

	if ( !FindArg( "-nodoscheck" ))
		check_dos_version();
	
	if ( !FindArg( "-nofilecheck" ))
		dos_check_file_handles(5);

	check_memory();

	if (init_graphics()) return 1;

	#ifdef EDITOR
	if (!Inferno_is_800x600_available)	{
		printf( "The editor will not be available, press any key to start game...\n" );
		Function_mode = FMODE_MENU;
		getch();
	}
	#endif

	#ifndef NDEBUG
		minit();
		mopen( 0, 9, 1, 78, 15, "Debug Spew");
		mopen( 1, 2, 1, 78,  5, "Errors & Serious Warnings");
	#endif

	if (!WVIDEO_running)
		mprintf((0,"WVIDEO_running = %d\n",WVIDEO_running));

	verbose ("%s", TXT_VERBOSE_1);
	ReadConfigFile();

#if defined(POLY_ACC)   //$$ maybe optional but be safe.
    mprintf((0, "Silently, forcing movies to high res for 3D cards.\n"));
    MovieHires = 1;
#endif

	verbose( "\n%s", TXT_VERBOSE_2);
	timer_init();
	timer_set_rate( digi_timer_rate );			// Tell our timer how fast to go (120 Hz)

	verbose( "\n%s", TXT_VERBOSE_3);
	key_init();

	if (!FindArg( "-nomouse" ))	{
		verbose( "\n%s", TXT_VERBOSE_4);
	
      if (FindArg ("-cybermouse"))
		 {
		 // CybermouseActive=InitCyberMouse();
		  if (CybermouseActive)
			goto Here;
		 }
		if (FindArg( "-nocyberman" ))
			mouse_init(0);
		else
			mouse_init(1);

	} 
  else
		verbose( "\n%s", TXT_VERBOSE_5);

   Here:
  
	do_joystick_init();

	verbose( "\n%s", TXT_VERBOSE_11);
	div0_init(DM_ERROR);

	//------------ Init sound ---------------
	if (!FindArg( "-disablesound" ))	{
		if (digi_init())	{
			int key;
			printf( "\n%s\n", "Press ESC to exit Descent II, or any other key to continue.");
			key = key_getch();
			if (key == KEY_ESC)
				exit(0);
		}
	} else {
		verbose( "\n%s",TXT_SOUND_DISABLED );
	}

#ifdef NETWORK
	do_network_init();
#endif

	if (!FindArg("-noserial")) 
		serial_active = 1;
	else 
		serial_active = 0;

#if defined(POLY_ACC)
    Current_display_mode = -1;
    game_init_render_buffers(SM_640x480x15xPA, 640, 480, VR_NONE, VRF_COMPATIBLE_MENUS+VRF_ALLOW_COCKPIT );
#else
	do_headset_init();

	if (!VR_offscreen_buffer)	//if hasn't been initialied (by headset init)
		set_display_mode(0);		//..then set default display mode
#endif

	i = FindArg( "-xcontrol" );
	if ( i > 0 )	{
		kconfig_init_external_controls( strtol(Args[i+1], NULL, 0), strtol(Args[i+2], NULL, 0) );
	}

	verbose( "\n%s\n\n", TXT_INITIALIZING_GRAPHICS);
	if (FindArg("-nofade"))
		grd_fades_disabled=1;
	
	if ((t=gr_init())!=0)				//doesn't do much
		Error(TXT_CANT_INIT_GFX,t);

   #ifdef _3DFX
   _3dfx_Init();
   #endif

	// Load the palette stuff. Returns non-zero if error.
	mprintf( (0, "\nInitializing palette system..." ));
   gr_use_palette_table(DEFAULT_PALETTE );

	mprintf( (0, "\nInitializing font system..." ));
	gamefont_init();	// must load after palette data loaded.

	//determine whether we're using high-res menus & movies
#if !defined(POLY_ACC)
	if (FindArg("-nohires") || FindArg("-nohighres") || (vga_check_mode(MENU_HIRES_MODE) != 0) || disable_high_res)
		MovieHires = MenuHires = MenuHiresAvailable = 0;
	else
#endif
		//NOTE LINK TO ABOVE!
		MenuHires = MenuHiresAvailable = 1;

	mprintf( (0, "\nInitializing movie libraries..." ));
	init_movies();		//init movie libraries

	mprintf( (0, "\nGoing into graphics mode..." ));
	verbose( "\nSetting graphics mode...");
#if defined(POLY_ACC)
	vga_set_mode(SM_640x480x15xPA);
#else
	vga_set_mode(MovieHires?SM_640x480V:SM_320x200C);
#endif

	#ifndef RELEASE
	if ( FindArg( "-notitles" ) ) 
		songs_play_song( SONG_TITLE, 1);
	else
	#endif
	{	//NOTE LINK TO ABOVE!
		int played=MOVIE_NOT_PLAYED;	//default is not played
		int song_playing = 0;

		#ifdef D2_OEM
		#define MOVIE_REQUIRED 0
		#else
		#define MOVIE_REQUIRED 1
		#endif

#ifdef D2_OEM   //$$POLY_ACC, jay.
		{	//show bundler screens
			FILE *tfile;
			char filename[FILENAME_LEN];

			played=MOVIE_NOT_PLAYED;	//default is not played

            played = PlayMovie("pre_i.mve",0);

			if (!played) {
                strcpy(filename,MenuHires?"pre_i1b.pcx":"pre_i1.pcx");

				while ((tfile=fopen(filename,"rb")) != NULL) {
					fclose(tfile);
					show_title_screen( filename, 1, 0 );
                    filename[5]++;
				}
			}
		}
#endif

		#ifndef SHAREWARE
			init_subtitles("intro.tex");
			played = PlayMovie("intro.mve",MOVIE_REQUIRED);
			close_subtitles();
		#endif

		#ifdef D2_OEM
		if (played != MOVIE_NOT_PLAYED)
			intro_played = 1;
		else {						//didn't get intro movie, try titles

			played = PlayMovie("titles.mve",MOVIE_REQUIRED);

			if (played == MOVIE_NOT_PLAYED) {
#if defined(POLY_ACC)
            vga_set_mode(SM_640x480x15xPA);
#else
				vga_set_mode(MenuHires?SM_640x480V:SM_320x200C);
#endif
				mprintf( (0, "\nPlaying title song..." ));
				songs_play_song( SONG_TITLE, 1);
				song_playing = 1;
				mprintf( (0, "\nShowing logo screens..." ));
				show_title_screen( MenuHires?"iplogo1b.pcx":"iplogo1.pcx", 1, 1 );
				show_title_screen( MenuHires?"logob.pcx":"logo.pcx", 1, 1 );
			}
		}

		{	//show bundler movie or screens

			FILE *tfile;
			char filename[FILENAME_LEN];
			int movie_handle;

			played=MOVIE_NOT_PLAYED;	//default is not played

			//check if OEM movie exists, so we don't stop the music if it doesn't
			movie_handle = open_movie_file("oem.mve",0);
			if (movie_handle != -1) {
				close(movie_handle);
				played = PlayMovie("oem.mve",0);
				song_playing = 0;		//movie will kill sound
			}

			if (!played) {
				strcpy(filename,MenuHires?"oem1b.pcx":"oem1.pcx");

				while ((tfile=fopen(filename,"rb")) != NULL) {
					fclose(tfile);
					show_title_screen( filename, 1, 0 );
					filename[3]++;
				}
			}
		}
		#endif

		if (!song_playing)
			songs_play_song( SONG_TITLE, 1);
			
	}

   PA_DFX (pa_splash());

	mprintf( (0, "\nShowing loading screen..." ));
	{
		//grs_bitmap title_bm;
		int pcx_error;
		char filename[14];

		#ifdef SHAREWARE
		strcpy(filename, "descentd.pcx");
		#else
		#ifdef D2_OEM
		strcpy(filename, MenuHires?"descntob.pcx":"descento.pcx");
		#else
		strcpy(filename, MenuHires?"descentb.pcx":"descent.pcx");
		#endif
		#endif

#if defined(POLY_ACC)
		vga_set_mode(SM_640x480x15xPA);
#else
		vga_set_mode(MenuHires?SM_640x480V:SM_320x200C);
#endif

		FontHires = MenuHires;

		if ((pcx_error=pcx_read_bitmap( filename, &grd_curcanv->cv_bitmap, grd_curcanv->cv_bitmap.bm_type, title_pal ))==PCX_ERROR_NONE)	{
			//vfx_set_palette_sub( title_pal );
			gr_palette_clear();
			gr_palette_fade_in( title_pal, 32, 0 );
		} else
			Error( "Couldn't load pcx file '%s', PCX load error: %s\n",filename, pcx_errormsg(pcx_error));
	}

	mprintf( (0, "\nDoing bm_init..." ));
	#ifdef EDITOR
		bm_init_use_tbl();
	#else
		bm_init();
	#endif

	#ifdef EDITOR
	if (FindArg("-hoarddata") != 0) {
		#define MAX_BITMAPS_PER_BRUSH 30
		grs_bitmap * bm[MAX_BITMAPS_PER_BRUSH];
		grs_bitmap icon;
		int nframes;
		short nframes_short;
		ubyte palette[256*3];
		FILE *ofile;
		int iff_error,i;
		char *sounds[] = {"selforb.raw","selforb.r22",		//SOUND_YOU_GOT_ORB			
								"teamorb.raw","teamorb.r22",		//SOUND_FRIEND_GOT_ORB			
								"enemyorb.raw","enemyorb.r22",	//SOUND_OPPONENT_GOT_ORB	
								"OPSCORE1.raw","OPSCORE1.r22"};	//SOUND_OPPONENT_HAS_SCORED

		ofile = fopen("hoard.ham","wb");

	   iff_error = iff_read_animbrush("orb.abm",bm,MAX_BITMAPS_PER_BRUSH,&nframes,palette);
		Assert(iff_error == IFF_NO_ERROR);
		nframes_short = nframes;
		fwrite(&nframes_short,sizeof(nframes_short),1,ofile);
		fwrite(&bm[0]->bm_w,sizeof(short),1,ofile);
		fwrite(&bm[0]->bm_h,sizeof(short),1,ofile);
		fwrite(palette,3,256,ofile);
		for (i=0;i<nframes;i++)
			fwrite(bm[i]->bm_data,1,bm[i]->bm_w*bm[i]->bm_h,ofile);

	   iff_error = iff_read_animbrush("orbgoal.abm",bm,MAX_BITMAPS_PER_BRUSH,&nframes,palette);
		Assert(iff_error == IFF_NO_ERROR);
		Assert(bm[0]->bm_w == 64 && bm[0]->bm_h == 64);
		nframes_short = nframes;
		fwrite(&nframes_short,sizeof(nframes_short),1,ofile);
		fwrite(palette,3,256,ofile);
		for (i=0;i<nframes;i++)
			fwrite(bm[i]->bm_data,1,bm[i]->bm_w*bm[i]->bm_h,ofile);

		for (i=0;i<2;i++) {
			iff_error = iff_read_bitmap(i?"orbb.bbm":"orb.bbm",&icon,BM_LINEAR,palette);
			Assert(iff_error == IFF_NO_ERROR);
			fwrite(&icon.bm_w,sizeof(short),1,ofile);
			fwrite(&icon.bm_h,sizeof(short),1,ofile);
			fwrite(palette,3,256,ofile);
			fwrite(icon.bm_data,1,icon.bm_w*icon.bm_h,ofile);
		}

		for (i=0;i<sizeof(sounds)/sizeof(*sounds);i++) {
			FILE *ifile;
			int size;
			ubyte *buf;
			ifile = fopen(sounds[i],"rb");
			Assert(ifile != NULL);
			size = filelength(ifile->_handle);
			buf = malloc(size);
			fread(buf,1,size,ifile);
			fwrite(&size,sizeof(size),1,ofile);
			fwrite(buf,1,size,ofile);
			free(buf);
			fclose(ifile);
		}

		fclose(ofile);

		exit(1);
	}
	#endif

	//the bitmap loading code changes gr_palette, so restore it
	memcpy(gr_palette,title_pal,sizeof(gr_palette));

	if ( FindArg( "-norun" ) )
		return(0);

	mprintf( (0, "\nInitializing 3d system..." ));
	g3_init();

	mprintf( (0, "\nInitializing texture caching system..." ));
	texmerge_init( 10 );		// 10 cache bitmaps

	mprintf( (0, "\nRunning game...\n" ));
	set_screen_mode(SCREEN_MENU);

	init_game();

	//	If built with editor, option to auto-load a level and quit game
	//	to write certain data.
	#ifdef	EDITOR
	{	int t;
	if ( t = FindArg( "-autoload" ) ) {
		Auto_exit = 1;
		strcpy(Auto_file, Args[t+1]);
	}
		
	}

	if (Auto_exit) {
		strcpy(Players[0].callsign, "dummy");
	} else
	#endif
		do_register_player(title_pal);

	gr_palette_fade_out( title_pal, 32, 0 );

	//check for special stamped version
	if (registered_copy) {
		nm_messagebox("EVALUATION COPY",1,"Continue",
			"This special evaluation copy\n"
			"of DESCENT 2 has been issued to:\n\n"
			"%s\n"
			"\n\n    NOT FOR DISTRIBUTION",
			name_copy);

		gr_palette_fade_out( gr_palette, 32, 0 );
	}

	Game_mode = GM_GAME_OVER;

	if (Auto_demo)	{
		newdemo_start_playback("DESCENT.DEM");		
		if (Newdemo_state == ND_STATE_PLAYBACK )
			Function_mode = FMODE_GAME;
	}

	//do this here because the demo code can do a longjmp when trying to
	//autostart a demo from the main menu, never having gone into the game
	setjmp(LeaveGame);

	while (Function_mode != FMODE_EXIT)
	{
		switch( Function_mode )	{
		case FMODE_MENU:
			set_screen_mode(SCREEN_MENU);
			if ( Auto_demo )	{
				newdemo_start_playback(NULL);		// Randomly pick a file
				if (Newdemo_state != ND_STATE_PLAYBACK)	
					Error("No demo files were found for autodemo mode!");
			} else {
				#ifdef EDITOR
				if (Auto_exit) {
					strcpy(&Level_names[0], Auto_file);
					LoadLevel(1, 1);
					Function_mode = FMODE_EXIT;
					break;
				}
				#endif

				check_joystick_calibration();
				gr_palette_clear();		//I'm not sure why we need this, but we do
				DoMenu();									 	
				#ifdef EDITOR
				if ( Function_mode == FMODE_EDITOR )	{
					create_new_mine();
					SetPlayerFromCurseg();
					load_palette(NULL,1,0);
				}
				#endif
			}
			break;
		case FMODE_GAME:
			#ifdef EDITOR
				keyd_editor_mode = 0;
			#endif
			game();
			if ( Function_mode == FMODE_MENU )
				songs_play_song( SONG_TITLE, 1 );
			break;
		#ifdef EDITOR
		case FMODE_EDITOR:
			keyd_editor_mode = 1;
			editor();
			_harderr( (void*)descent_critical_error_handler );		// Reinstall game error handler
			if ( Function_mode == FMODE_GAME ) {
				Game_mode = GM_EDITOR;
				editor_reset_stuff_on_level();
				N_players = 1;
			}
			break;
		#endif
		default:
			Error("Invalid function mode %d",Function_mode);
		}
	}

	WriteConfigFile();

	#ifndef RELEASE
	if (!FindArg( "-notitles" ))
	#endif

	show_order_form();

	#ifndef NDEBUG
	if ( FindArg( "-showmeminfo" ) )
		show_mem_info = 1;		// Make memory statistics show
	#endif

	return(0);		//presumably successful exit
}


void check_joystick_calibration()	{
	int x1, y1, x2, y2, c;
	fix t1;

	if ( (Config_control_type!=CONTROL_JOYSTICK) &&
		  (Config_control_type!=CONTROL_FLIGHTSTICK_PRO) &&
		  (Config_control_type!=CONTROL_THRUSTMASTER_FCS) &&
		  (Config_control_type!=CONTROL_GRAVIS_GAMEPAD)
		) return;

	joy_get_pos( &x1, &y1 );

	t1 = timer_get_fixed_seconds();
	while( timer_get_fixed_seconds() < t1 + F1_0/100 )
		;

	joy_get_pos( &x2, &y2 );

	// If joystick hasn't moved...
	if ( (abs(x2-x1)<30) &&  (abs(y2-y1)<30) )	{
		if ( (abs(x1)>30) || (abs(x2)>30) ||  (abs(y1)>30) || (abs(y2)>30) )	{
			c = nm_messagebox( NULL, 2, TXT_CALIBRATE, TXT_SKIP, TXT_JOYSTICK_NOT_CEN );
			if ( c==0 )	{
				joydefs_calibrate();
			}
		}
	}

}

void show_order_form()
{
#if !defined(EDITOR) && (defined(SHAREWARE) || defined(D2_OEM))

	int pcx_error;
	char title_pal[768];
	char	exit_screen[16];

	gr_set_current_canvas( NULL );
	gr_palette_clear();

	key_flush();		

	#ifdef D2_OEM
		strcpy(exit_screen, MenuHires?"ordrd2ob.pcx":"ordrd2o.pcx");
	#else
	#if defined(SHAREWARE)
		strcpy(exit_screen, "orderd2.pcx");
	#else
		strcpy(exit_screen, MenuHires?"warningb.pcx":"warning.pcx");
	#endif
	#endif

	if ((pcx_error=pcx_read_bitmap( exit_screen, &grd_curcanv->cv_bitmap, grd_curcanv->cv_bitmap.bm_type, title_pal ))==PCX_ERROR_NONE) {
		//vfx_set_palette_sub( title_pal );
		gr_palette_fade_in( title_pal, 32, 0 );
		key_getch();
		gr_palette_fade_out( title_pal, 32, 0 );		
	}
	else
		Int3();		//can't load order screen

	key_flush();		

#endif
}


