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
static char rcsid[] = "$Id: pcx.c 1.19 1996/09/18 16:38:07 jeremy Exp $";
#pragma on (unreferenced)

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "pa_enabl.h"                   //$$POLY_ACC
#include "gr.h"
#include "grdef.h"
#include "mem.h"
#include "pcx.h"
#include "cfile.h"
#include "byteswap.h"

#if defined(POLY_ACC)
#include "poly_acc.h"
#endif

/* PCX Header data type */
typedef struct	{
	ubyte		Manufacturer;
	ubyte		Version;
	ubyte		Encoding;
	ubyte		BitsPerPixel;
	short		Xmin;
	short		Ymin;
	short		Xmax;
	short		Ymax;
	short		Hdpi;
	short		Vdpi;
	ubyte		ColorMap[16][3];
	ubyte		Reserved;
	ubyte		Nplanes;
	short		BytesPerLine;
	ubyte		filler[60];
} PCXHeader;


int pcx_get_dimensions( char *filename, int *width, int *height)
{
	CFILE *PCXfile;
	PCXHeader header;


	PCXfile = cfopen(filename, "rb");
	if (!PCXfile) return PCX_ERROR_OPENING;

	if (cfread(&header, sizeof(PCXHeader), 1, PCXfile) != 1) {
		cfclose(PCXfile);
		return PCX_ERROR_NO_HEADER;
	}
	cfclose(PCXfile);

	*width = header.Xmax - header.Xmin+1;
	*height = header.Ymax - header.Ymin+1;

	return PCX_ERROR_NONE;
}

#ifdef MACINTOSH
int pcx_read_bitmap_palette( char *filename, ubyte *palette)
{
	PCXHeader header;
	CFILE * PCXfile;
	ubyte data;
	int i;

	PCXfile = cfopen( filename , "rb" );
	if ( !PCXfile )
		return PCX_ERROR_OPENING;

	// read 128 char PCX header
	if (cfread( &header, sizeof(PCXHeader), 1, PCXfile )!=1)	{
		cfclose( PCXfile );
		return PCX_ERROR_NO_HEADER;
	}
	// Is it a 256 color PCX file?
	if ((header.Manufacturer != 10)||(header.Encoding != 1)||(header.Nplanes != 1)||(header.BitsPerPixel != 8)||(header.Version != 5))	{
		cfclose( PCXfile );
		return PCX_ERROR_WRONG_VERSION;
	}

	// Read the extended palette at the end of PCX file
	// Read in a character which should be 12 to be extended palette file

	if (palette != NULL) {
		cfseek( PCXfile, -768, SEEK_END );
		cfread( palette, 3, 256, PCXfile );
		cfseek( PCXfile, sizeof(PCXHeader), SEEK_SET );
		for (i=0; i<768; i++ )
			palette[i] >>= 2;
#ifdef MACINTOSH
		for (i = 0; i < 3; i++) {
			data = palette[i];
			palette[i] = palette[765+i];
			palette[765+i] = data;
		}
#endif
	}
}
#endif

//#ifndef MACINTOSH 
int pcx_read_bitmap( char * filename, grs_bitmap * bmp,int bitmap_type ,ubyte * palette )
{
	PCXHeader header;
	CFILE * PCXfile;
	int i, row, col, count, xsize, ysize;
    ubyte data, *pixdata;
#if defined(POLY_ACC)
    unsigned char local_pal[768];

    pa_flush();
#endif

	PCXfile = cfopen( filename , "rb" );
	if ( !PCXfile )
		return PCX_ERROR_OPENING;

	// read 128 char PCX header
	if (cfread( &header, sizeof(PCXHeader), 1, PCXfile )!=1)	{
		cfclose( PCXfile );
		return PCX_ERROR_NO_HEADER;
	}

	// Is it a 256 color PCX file?
	if ((header.Manufacturer != 10)||(header.Encoding != 1)||(header.Nplanes != 1)||(header.BitsPerPixel != 8)||(header.Version != 5))	{
		cfclose( PCXfile );
		return PCX_ERROR_WRONG_VERSION;
	}
#ifdef MACINTOSH
	header.Xmin = SWAPSHORT(header.Xmin);
	header.Xmax = SWAPSHORT(header.Xmax);
	header.Ymin = SWAPSHORT(header.Ymin);
	header.Ymax = SWAPSHORT(header.Ymax);
#endif

	// Find the size of the image
	xsize = header.Xmax - header.Xmin + 1;
	ysize = header.Ymax - header.Ymin + 1;

#if defined(POLY_ACC)
    // Read the extended palette at the end of PCX file
    if(bitmap_type == BM_LINEAR15)      // need palette for conversion from 8bit pcx to 15bit.
    {
        cfseek( PCXfile, -768, SEEK_END );
        cfread( local_pal, 3, 256, PCXfile );
        cfseek( PCXfile, sizeof(PCXHeader), SEEK_SET );
        for (i=0; i<768; i++ )
            local_pal[i] >>= 2;
        pa_save_clut();
        pa_update_clut(local_pal, 0, 256, 0);
    }
#endif

	if ( bitmap_type == BM_LINEAR )	{
		if ( bmp->bm_data == NULL )	{
			memset( bmp, 0, sizeof( grs_bitmap ) );
			bmp->bm_data = malloc( xsize * ysize );
			if ( bmp->bm_data == NULL )	{
				cfclose( PCXfile );
				return PCX_ERROR_MEMORY;
			}
			bmp->bm_w = bmp->bm_rowsize = xsize;
			bmp->bm_h = ysize;
			bmp->bm_type = bitmap_type;
		}
	}

	if ( bmp->bm_type == BM_LINEAR )	{
		for (row=0; row< ysize ; row++)      {
			pixdata = &bmp->bm_data[bmp->bm_rowsize*row];
			for (col=0; col< xsize ; )      {
				if (cfread( &data, 1, 1, PCXfile )!=1 )	{
					cfclose( PCXfile );
					return PCX_ERROR_READING;
				}
				if ((data & 0xC0) == 0xC0)     {
					count =  data & 0x3F;
					if (cfread( &data, 1, 1, PCXfile )!=1 )	{
						cfclose( PCXfile );
						return PCX_ERROR_READING;
					}
#ifdef MACINTOSH
					if (data == 0)
						data = 255;
					else if (data == 255)
						data = 0;
#endif
					memset( pixdata, data, count );
					pixdata += count;
					col += count;
				} else {
#ifdef MACINTOSH
					if (data == 0)
						data = 255;
					else if (data == 255)
						data = 0;
#endif
					*pixdata++ = data;
					col++;
				}
			}
		}
#if defined(POLY_ACC)
    } else if( bmp->bm_type == BM_LINEAR15 )    {
        ushort *pixdata2, pix15;
        PA_DFX (pa_set_backbuffer_current());
		  PA_DFX (pa_set_write_mode(0));
		for (row=0; row< ysize ; row++)      {
            pixdata2 = (ushort *)&bmp->bm_data[bmp->bm_rowsize*row];
			for (col=0; col< xsize ; )      {
				if (cfread( &data, 1, 1, PCXfile )!=1 )	{
					cfclose( PCXfile );
					return PCX_ERROR_READING;
				}
				if ((data & 0xC0) == 0xC0)     {
					count =  data & 0x3F;
					if (cfread( &data, 1, 1, PCXfile )!=1 )	{
						cfclose( PCXfile );
						return PCX_ERROR_READING;
					}
                    pix15 = pa_clut[data];
                    for(i = 0; i != count; ++i) pixdata2[i] = pix15;
                    pixdata2 += count;
					col += count;
				} else {
                    *pixdata2++ = pa_clut[data];
					col++;
				}
			}
        }
        pa_restore_clut();
		  PA_DFX (pa_swap_buffer());
        PA_DFX (pa_set_frontbuffer_current());

#endif
	} else {
		for (row=0; row< ysize ; row++)      {
			for (col=0; col< xsize ; )      {
				if (cfread( &data, 1, 1, PCXfile )!=1 )	{
					cfclose( PCXfile );
					return PCX_ERROR_READING;
				}
				if ((data & 0xC0) == 0xC0)     {
					count =  data & 0x3F;
					if (cfread( &data, 1, 1, PCXfile )!=1 )	{
						cfclose( PCXfile );
						return PCX_ERROR_READING;
					}
					for (i=0;i<count;i++)
						gr_bm_pixel( bmp, col+i, row, data );
					col += count;
				} else {
					gr_bm_pixel( bmp, col, row, data );
					col++;
				}
			}
		}
	}

	// Read the extended palette at the end of PCX file
	if ( palette != NULL )	{
		// Read in a character which should be 12 to be extended palette file
		if (cfread( &data, 1, 1, PCXfile )==1)	{
			if ( data == 12 )	{
				if (cfread(palette,768, 1, PCXfile)!=1)	{
					cfclose( PCXfile );
					return PCX_ERROR_READING;
				}
				for (i=0; i<768; i++ )
					palette[i] >>= 2;
#ifdef MACINTOSH
				for (i = 0; i < 3; i++) {
					data = palette[i];
					palette[i] = palette[765+i];
					palette[765+i] = data;
				}
#endif
			}
		} else {
			cfclose( PCXfile );
			return PCX_ERROR_NO_PALETTE;
		}
	}
	cfclose(PCXfile);
	return PCX_ERROR_NONE;
}
//#endif

#if 0		// used to be #ifdef MACINTOSH -- using poly_acc code above now.
//special fast version for Mac - doesn't support SVGA, poly acc, etc.
int pcx_read_bitmap( char * filename, grs_bitmap * bmp,int bitmap_type ,ubyte * palette )
{
	PCXHeader header;
	CFILE * PCXfile;
	int i, row, col, count, xsize, ysize;
	ubyte data, c;
	ubyte row_data[640], *row_ptr, cdata;
	int buffer_size, buffer_pos;
	ubyte buffer[1024];
	ushort colors[256];

	PCXfile = cfopen( filename , "rb" );
	if ( !PCXfile )
		return PCX_ERROR_OPENING;

	// read 128 char PCX header
	if (cfread( &header, sizeof(PCXHeader), 1, PCXfile )!=1)	{
		cfclose( PCXfile );
		return PCX_ERROR_NO_HEADER;
	}

	// Is it a 256 color PCX file?
	if ((header.Manufacturer != 10)||(header.Encoding != 1)||(header.Nplanes != 1)||(header.BitsPerPixel != 8)||(header.Version != 5))	{
		cfclose( PCXfile );
		return PCX_ERROR_WRONG_VERSION;
	}

#if defined(MACINTOSH)
	header.Xmin= swapshort(header.Xmin);
	header.Xmax = swapshort(header.Xmax);
	header.Ymin = swapshort(header.Ymin);
	header.Ymax = swapshort(header.Ymax);
#endif

	// Find the size of the image
	xsize = header.Xmax - header.Xmin + 1;
	ysize = header.Ymax - header.Ymin + 1;

	if ( bmp->bm_data == NULL )	{
		memset( bmp, 0, sizeof( grs_bitmap ) );
		bmp->bm_data = malloc( xsize * ysize );
		if ( bmp->bm_data == NULL )	{
			cfclose( PCXfile );
			return PCX_ERROR_MEMORY;
		}
		bmp->bm_w = bmp->bm_rowsize = xsize;
		bmp->bm_h = ysize;
		bmp->bm_type = bitmap_type;
	}

	// Read the extended palette at the end of PCX file
	// Read in a character which should be 12 to be extended palette file

	if (palette != NULL) {
		cfseek( PCXfile, -768, SEEK_END );
		cfread( palette, 3, 256, PCXfile );
		cfseek( PCXfile, sizeof(PCXHeader), SEEK_SET );
		for (i=0; i<768; i++ )
			palette[i] >>= 2;
#ifdef MACINTOSH
		for (i = 0; i < 3; i++) {
			data = palette[i];
			palette[i] = palette[765+i];
			palette[765+i] = data;
		}
#endif
	}

	buffer_size = 1024;
	buffer_pos = 0;
	
	buffer_size = cfread( buffer, 1, buffer_size, PCXfile );

	for (row=0; row< ysize ; row++)      {
		row_ptr = row_data;
		for (col=0; col< xsize ; )      {
			data = buffer[buffer_pos++];
			if ( buffer_pos == buffer_size )	{
				buffer_size = cfread( buffer, 1, buffer_size, PCXfile );
				buffer_pos = 0;
			}
			//cfread( &data, 1, 1, PCXfile )!=1 )	{
			if ((data & 0xC0) == 0xC0)     {
				count =  data & 0x3F;
				//cfread( &data, 1, 1, PCXfile );
				data = buffer[buffer_pos++];
#ifdef MACINTOSH
				if (data == 0)
					data = 255;
				else if (data == 255)
					data = 0;
#endif
				if ( buffer_pos == buffer_size )	{
					buffer_size = cfread( buffer, 1, buffer_size, PCXfile );
					buffer_pos = 0;
				}
				for (i = 0; i<count;i++)
					*row_ptr++ = data;
				col += count;
			} else {
#ifdef MACINTOSH
				if (data == 0)
					data = 255;
				else if (data == 255)
					data = 0;
#endif
				*row_ptr++ = data;
				col++;
			}
		}
		gr_linear_movsd(row_data, &(bmp->bm_data[row * bmp->bm_rowsize]), bmp->bm_w );
	}

	cfclose(PCXfile);
	return PCX_ERROR_NONE;
}
#endif

int pcx_write_bitmap( char * filename, grs_bitmap * bmp, ubyte * palette )
{
	int retval;
	int i;
	ubyte data;
	PCXHeader header;
	FILE * PCXfile;

	memset( &header, 0, sizeof( PCXHeader ) );

	header.Manufacturer = 10;
	header.Encoding = 1;
	header.Nplanes = 1;
	header.BitsPerPixel = 8;
	header.Version = 5;
	header.Xmax = bmp->bm_w-1;
	header.Ymax = bmp->bm_h-1;
	header.BytesPerLine = bmp->bm_w;

	PCXfile = fopen( filename , "wb" );
	if ( !PCXfile )
		return PCX_ERROR_OPENING;

	if ( fwrite( &header, sizeof( PCXHeader ), 1, PCXfile ) != 1 )	{
		fclose( PCXfile );
		return PCX_ERROR_WRITING;
	}

	for (i=0; i<bmp->bm_h; i++ )	{
		if (!pcx_encode_line( &bmp->bm_data[bmp->bm_rowsize*i], bmp->bm_w, PCXfile ))	{
			fclose( PCXfile );
			return PCX_ERROR_WRITING;
		}
	}

	// Mark an extended palette
	data = 12;
	if (fwrite( &data, 1, 1, PCXfile )!=1)	{
		fclose( PCXfile );
		return PCX_ERROR_WRITING;
	}

	// Write the extended palette
	for (i=0; i<768; i++ )
		palette[i] <<= 2;

	retval = fwrite( palette, 768, 1, PCXfile );

	for (i=0; i<768; i++ )
		palette[i] >>= 2;

	if (retval !=1)	{
		fclose( PCXfile );
		return PCX_ERROR_WRITING;
	}

	fclose( PCXfile );
	return PCX_ERROR_NONE;

}

// subroutine for writing an encoded byte pair
// returns count of bytes written, 0 if error
int pcx_encode_byte(ubyte byt, ubyte cnt, FILE * fid)
{
	if (cnt) {
		if ( (cnt==1) && (0xc0 != (0xc0 & byt)) )	{
			if(EOF == putc((int)byt, fid))
				return 0; 	// disk write error (probably full)
			return 1;
		} else {
			if(EOF == putc((int)0xC0 | cnt, fid))
				return 0; 	// disk write error
			if(EOF == putc((int)byt, fid))
				return 0; 	// disk write error
			return 2;
		}
	}
	return 0;
}

// returns number of bytes written into outBuff, 0 if failed
int pcx_encode_line(ubyte *inBuff, int inLen, FILE * fp)
{
	ubyte this, last;
	int srcIndex, i;
	register int total;
	register ubyte runCount; 	// max single runlength is 63
	total = 0;
	last = *(inBuff);
	runCount = 1;

	for (srcIndex = 1; srcIndex < inLen; srcIndex++) {
		this = *(++inBuff);
		if (this == last)	{
			runCount++;			// it encodes
			if (runCount == 63)	{
				if (!(i=pcx_encode_byte(last, runCount, fp)))
					return(0);
				total += i;
				runCount = 0;
			}
		} else {   	// this != last
			if (runCount)	{
				if (!(i=pcx_encode_byte(last, runCount, fp)))
					return(0);
				total += i;
			}
			last = this;
			runCount = 1;
		}
	}

	if (runCount)	{		// finish up
		if (!(i=pcx_encode_byte(last, runCount, fp)))
			return 0;
		return total + i;
	}
	return total;
}

//text for error messges
char pcx_error_messages[] = {
	"No error.\0"
	"Error opening file.\0"
	"Couldn't read PCX header.\0"
	"Unsupported PCX version.\0"
	"Error reading data.\0"
	"Couldn't find palette information.\0"
	"Error writing data.\0"
};


//function to return pointer to error message
char *pcx_errormsg(int error_number)
{
	char *p = pcx_error_messages;

	while (error_number--) {

		if (!p) return NULL;

		p += strlen(p)+1;

	}

	return p;

}

