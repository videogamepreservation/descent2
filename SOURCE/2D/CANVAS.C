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

#include <stdlib.h>
#include <malloc.h>
#include <stdio.h>

#include "pa_enabl.h"                   //$$POLY_ACC
#include "mem.h"
#include "error.h"

#include "gr.h"
#include "grdef.h"

#if defined(POLY_ACC)
#include "poly_acc.h"
#endif

grs_canvas * grd_curcanv=NULL;    //active canvas
grs_screen * grd_curscreen=NULL;  //active screen

grs_canvas *gr_create_canvas(int w, int h)
{
	unsigned char * data;
	grs_canvas *new;

    new = (grs_canvas *)malloc( sizeof(grs_canvas) );
	data = (unsigned char *)malloc(w*h);

	new->cv_bitmap.bm_x = 0;
	new->cv_bitmap.bm_y = 0;
	new->cv_bitmap.bm_w = w;
	new->cv_bitmap.bm_h = h;
	new->cv_bitmap.bm_flags = 0;
	new->cv_bitmap.bm_type = BM_LINEAR;
	new->cv_bitmap.bm_rowsize = w;
	new->cv_bitmap.bm_data = data;

	new->cv_color = 0;
	new->cv_drawmode = 0;
	new->cv_font = NULL;
	new->cv_font_fg_color = 0;
	new->cv_font_bg_color = 0;
	return new;
}

#if defined(POLY_ACC)
grs_canvas *gr_create_canvas2(int w, int h, int type)
{
	unsigned char * data;
	grs_canvas *new;

	new = (grs_canvas *)malloc( sizeof(grs_canvas) );
    if(type == BM_LINEAR)
        data = (unsigned char *)malloc(w*h);
    else if(type == BM_LINEAR15)
        data = (unsigned char *)malloc(w*h*PA_BPP);

	new->cv_bitmap.bm_x = 0;
	new->cv_bitmap.bm_y = 0;
	new->cv_bitmap.bm_w = w;
	new->cv_bitmap.bm_h = h;
	new->cv_bitmap.bm_flags = 0;
    new->cv_bitmap.bm_type = type;
	
   new->cv_bitmap.bm_rowsize = type==BM_LINEAR?w:w*PA_BPP;
	
	new->cv_bitmap.bm_data = data;

	new->cv_color = 0;
	new->cv_drawmode = 0;
	new->cv_font = NULL;
	new->cv_font_fg_color = 0;
	new->cv_font_bg_color = 0;
	return new;
}
#endif

grs_canvas *gr_create_sub_canvas(grs_canvas *canv, int x, int y, int w, int h)
{
    grs_canvas *new;

	if (x+w > canv->cv_bitmap.bm_w) {Int3(); w=canv->cv_bitmap.bm_w-x;}
	if (y+h > canv->cv_bitmap.bm_h) {Int3(); h=canv->cv_bitmap.bm_h-y;}

    new = (grs_canvas *)malloc( sizeof(grs_canvas) );

	new->cv_bitmap.bm_x = x+canv->cv_bitmap.bm_x;
	new->cv_bitmap.bm_y = y+canv->cv_bitmap.bm_y;
	new->cv_bitmap.bm_w = w;
	new->cv_bitmap.bm_h = h;
	new->cv_bitmap.bm_flags = 0;
	new->cv_bitmap.bm_type = canv->cv_bitmap.bm_type;
	new->cv_bitmap.bm_rowsize = canv->cv_bitmap.bm_rowsize;

	new->cv_bitmap.bm_data = canv->cv_bitmap.bm_data;
	new->cv_bitmap.bm_data += y*canv->cv_bitmap.bm_rowsize;
#if defined(POLY_ACC)
    new->cv_bitmap.bm_data += canv->cv_bitmap.bm_type==BM_LINEAR?x:x*PA_BPP; //$$POLY_ACC
#else
    new->cv_bitmap.bm_data += x;
#endif

	new->cv_color = canv->cv_color;
   new->cv_drawmode = canv->cv_drawmode;
   new->cv_font = canv->cv_font;
	new->cv_font_fg_color = canv->cv_font_fg_color;
	new->cv_font_bg_color = canv->cv_font_bg_color;
   return new;
}

void gr_init_canvas(grs_canvas *canv, unsigned char * pixdata, int pixtype, int w, int h)
{
    canv->cv_color = 0;
    canv->cv_drawmode = 0;
    canv->cv_font = NULL;
	canv->cv_font_fg_color = 0;
	canv->cv_font_bg_color = 0;

	canv->cv_bitmap.bm_x = 0;
	canv->cv_bitmap.bm_y = 0;
	if (pixtype==BM_MODEX)
		canv->cv_bitmap.bm_rowsize = w / 4;
#if defined(POLY_ACC)
    else if(pixtype==BM_LINEAR15)
#ifdef PA_3DFX_VOODOO
		canv->cv_bitmap.bm_rowsize = 2048;
#else
		canv->cv_bitmap.bm_rowsize = w*PA_BPP;
#endif
#endif
    else
		canv->cv_bitmap.bm_rowsize = w;

    canv->cv_bitmap.bm_w = w;
	canv->cv_bitmap.bm_h = h;
	canv->cv_bitmap.bm_flags = 0;
	canv->cv_bitmap.bm_type = pixtype;
	canv->cv_bitmap.bm_data = pixdata;
}

void gr_init_sub_canvas(grs_canvas *new, grs_canvas *src, int x, int y, int w, int h)
{
	new->cv_color = src->cv_color;
	new->cv_drawmode = src->cv_drawmode;
	new->cv_font = src->cv_font;
	new->cv_font_fg_color = src->cv_font_fg_color;
	new->cv_font_bg_color = src->cv_font_bg_color;

	new->cv_bitmap.bm_x = src->cv_bitmap.bm_x+x;
	new->cv_bitmap.bm_y = src->cv_bitmap.bm_y+y;
	new->cv_bitmap.bm_w = w;
	new->cv_bitmap.bm_h = h;
	new->cv_bitmap.bm_flags = 0;
	new->cv_bitmap.bm_type = src->cv_bitmap.bm_type;
	new->cv_bitmap.bm_rowsize = src->cv_bitmap.bm_rowsize;


	new->cv_bitmap.bm_data = src->cv_bitmap.bm_data;
	new->cv_bitmap.bm_data += y*src->cv_bitmap.bm_rowsize;
#if defined(POLY_ACC)
    new->cv_bitmap.bm_data += src->cv_bitmap.bm_type==BM_LINEAR?x:x*PA_BPP;
#else
    new->cv_bitmap.bm_data += x;
#endif
}

void gr_free_canvas(grs_canvas *canv)
{
	if (canv == grd_curcanv) {		//bad!! freeing current canvas!
		Int3();
		gr_set_current_canvas(NULL);
	}

#if defined(POLY_ACC) && !defined(MACINTOSH)
    //
    // prevents freeing the VIRGE framebuffer. This is only a concern in two places in game.c
    //
    if(canv->cv_bitmap.bm_data != pa_get_buffer_address(0) &&
       canv->cv_bitmap.bm_data != pa_get_buffer_address(1) )
    {
        free(canv->cv_bitmap.bm_data );
    }
#else
	free(canv->cv_bitmap.bm_data );
#endif

	free(canv);

}

void gr_free_sub_canvas(grs_canvas *canv)
{
    free(canv);
}

int gr_wait_for_retrace = 1;

void gr_show_canvas( grs_canvas *canv )
{
	if (canv->cv_bitmap.bm_type == BM_MODEX )
		gr_modex_setstart( canv->cv_bitmap.bm_x, canv->cv_bitmap.bm_y, gr_wait_for_retrace );

	else if (canv->cv_bitmap.bm_type == BM_SVGA )
		gr_vesa_setstart( canv->cv_bitmap.bm_x, canv->cv_bitmap.bm_y );

		//	else if (canv->cv_bitmap.bm_type == BM_LINEAR )
		// Int3();		// Get JOHN!
		//gr_linear_movsd( canv->cv_bitmap.bm_data, (void *)0xA0000, 320*200);
#if defined(POLY_ACC)
    else if (canv->cv_bitmap.bm_type == BM_LINEAR15)
        Int3();     // hurray, this got called, now write some code to support it.
#endif
}

void gr_set_current_canvas( grs_canvas *canv )
{
	if (canv==NULL)
		grd_curcanv = &(grd_curscreen->sc_canvas);
	else
		grd_curcanv = canv;

#ifndef MACINTOSH			// these variables are undefined on mac -- not used.

	if ( (grd_curcanv->cv_color >= 0) && (grd_curcanv->cv_color <= 255) )	{
		gr_var_color = grd_curcanv->cv_color;
	} else
		gr_var_color  = 0;
	gr_var_bitmap = grd_curcanv->cv_bitmap.bm_data;
	gr_var_bwidth = grd_curcanv->cv_bitmap.bm_rowsize;

#endif
}

void gr_clear_canvas(int color)
{
	gr_setcolor(color);
	gr_rect(0,0,WIDTH-1,HEIGHT-1);
}

void gr_setcolor(int color)
{
	grd_curcanv->cv_color=color;

#ifndef MACINTOSH
	gr_var_color = color;
#endif
}
