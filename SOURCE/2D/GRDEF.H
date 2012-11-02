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

extern int  gr_modex_setmode(short mode);
extern void gr_modex_setplane(short plane);
extern void gr_modex_setstart(short x, short y, int wait_for_retrace);
extern void gr_modex_uscanline( short x1, short x2, short y, unsigned char color );

extern void gr_pal_setblock( int start, int n, unsigned char * palette );
extern void gr_pal_getblock( int start, int n, unsigned char * palette );
extern void gr_pal_setone( int index, unsigned char red, unsigned char green, unsigned char blue );

extern int  gr_vesa_setmodea(int mode);
extern int  gr_vesa_checkmode(int mode);
extern void gr_vesa_setstart(short x, short y );
extern void gr_vesa_setpage(int page);
extern void gr_vesa_incpage();
extern void gr_vesa_scanline(short x1, short x2, short y, unsigned char color );
extern int  gr_vesa_setlogical(int pixels_per_scanline);
extern void gr_vesa_bitblt( unsigned char * source_ptr, unsigned int vesa_address, int height, int width );
extern void gr_vesa_pixel( unsigned char color, unsigned int offset );

void gr_linear_movsb( void * source, void * dest, unsigned short nbytes);
void gr_linear_movsw( void * source, void * dest, unsigned short nbytes);
void gr_linear_stosd( ubyte * source, unsigned char color, unsigned short nbytes);
extern unsigned int gr_var_color;
extern unsigned int gr_var_bwidth;
extern unsigned char * gr_var_bitmap;

void gr_linear_movsd( ubyte * source, ubyte * dest, int nbytes);

#ifndef MACINTOSH
#pragma aux gr_linear_movsd parm [esi] [edi] [ecx] modify exact [ecx esi edi eax ebx] = \
" cld "					\
" mov		ebx, ecx	"	\
" mov		eax, edi"	\
" and		eax, 011b"	\
" jz		d_aligned"	\
" mov		ecx, 4"		\
" sub		ecx, eax"	\
" sub		ebx, ecx"	\
" rep		movsb"		\
" d_aligned: "			\
" mov		ecx, ebx"	\
" shr		ecx, 2"		\
" rep 	movsd"		\
" mov		ecx, ebx"	\
" and 	ecx, 11b"	\
" rep 	movsb";
#endif 		// ifdef MACINTOSH

void gr_linear_line( int x0, int y0, int x1, int y1);

extern unsigned int Table8to32[256];

extern unsigned char * gr_video_memory;

#define WIDTH   grd_curcanv->cv_bitmap.bm_w
#define HEIGHT  grd_curcanv->cv_bitmap.bm_h
#define MINX    0
#define MINY    0
#define MAXX    (WIDTH-1)
#define MAXY    (HEIGHT-1)
#define TYPE    grd_curcanv->cv_bitmap.bm_type
#define DATA    grd_curcanv->cv_bitmap.bm_data
#define XOFFSET grd_curcanv->cv_bitmap.bm_x
#define YOFFSET grd_curcanv->cv_bitmap.bm_y
#define ROWSIZE grd_curcanv->cv_bitmap.bm_rowsize
#define COLOR   grd_curcanv->cv_color


void order( int *x1, int *x2 );

