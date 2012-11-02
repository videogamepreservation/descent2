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
static char rcsid[] = "$Id: tmapflat.c 1.21 1996/12/04 19:27:54 matt Exp $";
#pragma on (unreferenced)


#include <math.h>
// #include <graph.h>
#include <limits.h>
#include <stdio.h>
#include <conio.h>
#include <stdlib.h>

#include "pa_enabl.h"                   //$$POLY_ACC
// #include "hack3df.h"
#include "fix.h"
#include "mono.h"
#include "gr.h"
#include "grdef.h"
// #include "ui.h"
#include "texmap.h"
#include "texmapl.h"
#include "scanline.h"

//#include "tmapext.h"


extern void texture_map_flat(g3ds_tmap *t);
extern void texture_map_flat_faded(g3ds_tmap *t);

#include "3d.h"
#include "error.h"

#if defined(POLY_ACC)
#include "poly_acc.h"
#endif

typedef struct pnt2d {
	fix x,y;
} pnt2d;

#pragma off (unreferenced)		//bp not referenced

//this takes the same partms as draw_tmap, but draws a flat-shaded polygon
void draw_tmap_flat(grs_bitmap *bp,int nverts,g3s_point **vertbuf)
{
	g3ds_tmap	my_tmap;
	int	i;
	fix	average_light;

	Assert(nverts < MAX_TMAP_VERTS);

	average_light = vertbuf[0]->p3_l;
	for (i=1; i<nverts; i++)
		average_light += vertbuf[i]->p3_l;

	if (nverts == 4)
		average_light = f2i(average_light * NUM_LIGHTING_LEVELS/4);
	else
		average_light = f2i(average_light * NUM_LIGHTING_LEVELS/nverts);

	if (average_light < 0)
		average_light = 0;
	else if (average_light > NUM_LIGHTING_LEVELS-1)
		average_light = NUM_LIGHTING_LEVELS-1;

	tmap_flat_color = gr_fade_table[average_light*256 + bp->avg_color];

	my_tmap.nv = nverts;

	for (i=0; i<nverts; i++) {
		my_tmap.verts[i].x2d = vertbuf[i]->p3_sx;
		my_tmap.verts[i].y2d = vertbuf[i]->p3_sy;
	}

#if defined(POLY_ACC)
    if ( Gr_scanline_darkening_level >= GR_FADE_LEVELS )
        i = 255;
    else
        i = 255.0 * (float)(GR_FADE_LEVELS - Gr_scanline_darkening_level)/(float)GR_FADE_LEVELS;
	pa_draw_flat(&my_tmap, tmap_flat_color, i);
#else
	if ( Gr_scanline_darkening_level >= GR_FADE_LEVELS )
		texture_map_flat( &my_tmap );
	else	{
		tmap_flat_shade_value = Gr_scanline_darkening_level;
		texture_map_flat_faded( &my_tmap );
	}	
#endif

}

//	-----------------------------------------------------------------------------------------
//	This is the gr_upoly-like interface to the texture mapper which uses texture-mapper compatible
//	(ie, avoids cracking) edge/delta computation.
void gr_upoly_tmap(int nverts, int *vert )
{
	g3ds_tmap	my_tmap;
	int	i;

	Assert(nverts < MAX_TMAP_VERTS);

	my_tmap.nv = nverts;

	for (i=0; i<nverts; i++) {
		my_tmap.verts[i].x2d = *vert++;
		my_tmap.verts[i].y2d = *vert++;
	}
	tmap_flat_color = COLOR;

#ifdef _3DFX
   _3dfx_DrawFlatShadedPoly( &my_tmap, _3dfx_PaletteToARGB( COLOR ) );
   if ( _3dfx_skip_ddraw )
      return;
#endif

#if defined(POLY_ACC)
    if ( Gr_scanline_darkening_level >= GR_FADE_LEVELS )
        i = 255;
    else
        i = 255.0 * (float)(GR_FADE_LEVELS - Gr_scanline_darkening_level)/(float)GR_FADE_LEVELS;
    pa_draw_flat(&my_tmap, tmap_flat_color, i);
#else
	if ( Gr_scanline_darkening_level >= GR_FADE_LEVELS )
		texture_map_flat( &my_tmap );
	else	{
		tmap_flat_shade_value = Gr_scanline_darkening_level;
		texture_map_flat_faded( &my_tmap );
	}	
#endif
}

