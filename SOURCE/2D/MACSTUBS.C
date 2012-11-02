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

#include "gr.h"
#include "error.h"

void gr_sync_display()
{
}

void gr_modex_uscanline( short x1, short x2, short y, unsigned char color )
{
	x1 = x1;
	x2 = x2;
	y = y;
	color = color;
	Int3();
}

int  gr_modex_setmode(short mode)
{
	mode = mode;
	Int3();
	return 0;
}

void gr_modex_setplane(short plane)
{
	plane = plane;
	Int3();
}

void gr_modex_setstart(short x, short y, int wait_for_retrace)
{
	x = x;
	y = y;
	wait_for_retrace = wait_for_retrace;
	Int3();
}

void gr_modex_line( void )
{
	Int3();
}

int  gr_vesa_setmodea(int mode)
{
	mode = mode;
	Int3();
	return 0;
}

int  gr_vesa_checkmode(int mode)
{
	mode = mode;
	Int3();
	return 0;
}

void gr_vesa_setstart(short x, short y )
{
	x = x;
	y = y;
	Int3();
}

void gr_vesa_setpage(int page)
{
	page = page;
	Int3();
}

void gr_vesa_incpage()
{
	Int3();
}

void gr_vesa_scanline(short x1, short x2, short y, unsigned char color )
{
	x1 = x1;
	x2 = x2;
	y = y;
	color = color;
	Int3();
}

int gr_vesa_setlogical(int pixels_per_scanline)
{
	pixels_per_scanline = pixels_per_scanline;
	Int3();
	return 0;
}

void gr_vesa_bitblt( unsigned char * source_ptr, unsigned int vesa_address, int height, int width )
{
	source_ptr = source_ptr;
	vesa_address = vesa_address;
	height = height;
	width = width;
	Int3();
}

void gr_vesa_pixel( unsigned char color, unsigned int offset )
{
	color = color;
	offset = offset;
	Int3();
}

void gr_vesa_bitmap( grs_bitmap * source, grs_bitmap * dest, int x, int y )
{
	source = source;
	dest = dest;
	x = x;
	y = y;
	Int3();
}

