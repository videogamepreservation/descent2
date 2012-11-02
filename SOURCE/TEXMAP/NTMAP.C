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
static char rcsid[] = "$Id: ntmap.c 1.66 1996/12/04 19:27:55 matt Exp $";
#pragma on (unreferenced)

#define VESA 0
#define NUM_TMAPS 16

#define HEADLIGHT_LIGHTING 0

#define WIREFRAME 0
#define PERSPECTIVE 1

#include <math.h>
#include <limits.h>
#include <stdio.h>
#include <conio.h>
#include <stdlib.h>

#include "pa_enabl.h"                   //$$POLY_ACC
#include "mono.h"
#include "fix.h"
#include "3d.h"
#include "gr.h"
#include "error.h"
#include "key.h"

#include "texmap.h"
#include "texmapl.h"
#include "rle.h"
#include "scanline.h"

#if defined(POLY_ACC)
#include "poly_acc.h"
#endif

#define	EDITOR_TMAP	1		//if in, include extra stuff

#define F15_5 (F1_0*15 + F0_5)

// 1 means enable special sc2000 code, else enable only for Descent
#define	SC2000K	0
int	SC2000 = SC2000K;

// Temporary texture map, interface from Matt's 3d system to Mike's texture mapper.
g3ds_tmap Tmap1;

grs_bitmap Texmap_ptrs[NUM_TMAPS];
grs_bitmap Texmap4_ptrs[NUM_TMAPS];

fix Range_max=0; // debug, kill me

int	Interpolation_method=0;	// 0 = choose best method
int	Lighting_on;				// initialize to no lighting
int	Tmap_flat_flag = 0;		//	1 = render texture maps as flat shaded polygons.
int	Current_seg_depth;		// HACK INTERFACE: how far away the current segment (& thus texture) is
int	Max_perspective_depth;
int	Max_linear_depth;
int	Max_flat_depth;

//variables for clipping the texture-mapper to screen region
int Window_clip_left, Window_clip_bot, Window_clip_right, Window_clip_top;

// These variables are the interface to assembler.  They get set for each texture map, which is a real waste of time.
//	They should be set only when they change, which is generally when the window bounds change.  And, even still, it's
//	a pretty bad interface.
int	bytes_per_row=-1;
int	write_buffer;
int  	window_left;
int	window_right;
int	window_top;
int	window_bottom;
int	window_width;
int	window_height;
uint	dest_row_data;
int	loop_count;

#define	MAX_Y_POINTERS	1024
int	y_pointers[MAX_Y_POINTERS];

#ifdef USE_SELECTORS
short	pixel_data_selector;		// selector for current pixel data for texture mapper
#endif

fix fix_recip[FIX_RECIP_TABLE_SIZE];

int	Fix_recip_table_computed=0;

fix fx_l, fx_u, fx_v, fx_z, fx_du_dx, fx_dv_dx, fx_dz_dx, fx_dl_dx;
int fx_xleft, fx_xright, fx_y, fx_u_right, fx_v_right, fx_z_right;
unsigned char * pixptr;
int Transparency_on = 0;
int dither_intensity_lighting = 0;

ubyte * tmap_flat_cthru_table;
ubyte tmap_flat_color;
ubyte tmap_flat_shade_value;

// F1_0/Z LOOKUP TABLE:
// Sig bits... 10 looks fuzzy, but only uses 4K
//             11 breaks up when close, but quite acceptable, Uses 8K
//             12	looks good,except when very close. Can be used for game.  Uses 16K. Recommended by John.

#define DIVIDE_SIG_BITS		12
#define Z_SHIFTER 			(30-DIVIDE_SIG_BITS)
#define DIVIDE_TABLE_SIZE	(1<<DIVIDE_SIG_BITS)

ubyte divide_table_filled = 0;
fix divide_table[1<<DIVIDE_SIG_BITS];		// Can use 16 bits if we divide by the greator of Z0 or Z1, instead of always Z0.

void fill_divide_table()
{
	int i;

	divide_table_filled = 1;

	divide_table[0]=f1_0;
	divide_table[1]=f1_0;
	for (i=2; i<DIVIDE_TABLE_SIZE; i++ )	{
		// entry[i] = 1.0 / i+0.5;
		divide_table[i] = fixdiv( F1_0*2, 2*i+1 );
	}
}



// -------------------------------------------------------------------------------------
void init_fix_recip_table(void)
{
	int	i;

	fix_recip[0] = F1_0;

	for (i=1; i<FIX_RECIP_TABLE_SIZE; i++)
		fix_recip[i] = F1_0/i;

	Fix_recip_table_computed = 1;
}

// -------------------------------------------------------------------------------------
//	Initialize interface variables to assembler.
//	These things used to be constants.  This routine is now (10/6/93) getting called for
//	every texture map.  It should get called whenever the window changes, or, preferably,
//	not at all.  I'm pretty sure these variables are only being used for range checking.
void init_interface_vars_to_assembler(void)
{
	grs_bitmap	*bp;

	bp = &grd_curcanv->cv_bitmap;

	Assert(bp!=NULL);
	Assert(bp->bm_data!=NULL);
	Assert(bp->bm_h <= MAX_Y_POINTERS);

	//	If bytes_per_row has changed, create new table of pointers.
	if (bytes_per_row != (int) bp->bm_rowsize) {
		int	y_val, i;

		bytes_per_row = (int) bp->bm_rowsize;

		y_val = 0;
		for (i=0; i<MAX_Y_POINTERS; i++) {
			y_pointers[i] = y_val;
			y_val += bytes_per_row;
		}
	}

	write_buffer = (int) bp->bm_data;

	window_left = 0;
	window_right = (int) bp->bm_w-1;
	window_top = 0;
	window_bottom = (int) bp->bm_h-1;

	Window_clip_left = window_left;
	Window_clip_right = window_right;
	Window_clip_top = window_top;
	Window_clip_bot = window_bottom;

	window_width = bp->bm_w;
	window_height = bp->bm_h;

	if (!Fix_recip_table_computed)
		init_fix_recip_table();

#if defined(POLY_ACC)
    pa_set_3d_offset(bp->bm_x, bp->bm_y);
#endif

	if ( !divide_table_filled ) fill_divide_table();
}


// -------------------------------------------------------------------------------------
//                             VARIABLES
extern g3ds_tmap Tmap1;

// -------------------------------------------------------------------------------------
//	Returns number preceding val modulo modulus.
//	prevmod(3,4) = 2
//	prevmod(0,4) = 3
int prevmod(int val,int modulus)
{
	if (val > 0)
		return val-1;
	else
		return modulus-1;
//	return (val + modulus - 1) % modulus;
}


//	Returns number succeeding val modulo modulus.
//	succmod(3,4) = 0
//	succmod(0,4) = 1
int succmod(int val,int modulus)
{
	if (val < modulus-1)
		return val+1;
	else
		return 0;

//	return (val + 1) % modulus;
}

// -------------------------------------------------------------------------------------
//	Select topmost vertex (minimum y coordinate) and bottommost (maximum y coordinate) in
//	texture map.  If either is part of a horizontal edge, then select leftmost vertex for
//	top, rightmost vertex for bottom.
//	Important: Vertex is selected with integer precision.  So, if there are vertices at
//	(0.0,0.7) and (0.5,0.3), the first vertex is selected, because they y coordinates are
//	considered the same, so the smaller x is favored.
//	Parameters:
//		nv		number of vertices
//		v3d	pointer to 3d vertices containing u,v,x2d,y2d coordinates
//	Results in:
//		*min_y_ind
//		*max_y_ind
// -------------------------------------------------------------------------------------
void compute_y_bounds(g3ds_tmap *t, int *vlt, int *vlb, int *vrt, int *vrb,int *bottom_y_ind)
{
	int	i;
	int	min_y,max_y;
	int	min_y_ind;
	int	original_vrt;
	fix	min_x;

	// Scan all vertices, set min_y_ind to vertex with smallest y coordinate.
	min_y = f2i(t->verts[0].y2d);
	max_y = min_y;
	min_y_ind = 0;
	min_x = f2i(t->verts[0].x2d);
	*bottom_y_ind = 0;

	for (i=1; i<t->nv; i++) {
		if (f2i(t->verts[i].y2d) < min_y) {
			min_y = f2i(t->verts[i].y2d);
			min_y_ind = i;
			min_x = f2i(t->verts[i].x2d);
		} else if (f2i(t->verts[i].y2d) == min_y) {
			if (f2i(t->verts[i].x2d) < min_x) {
				min_y_ind = i;
				min_x = f2i(t->verts[i].x2d);
			}
		}
		if (f2i(t->verts[i].y2d) > max_y) {
			max_y = f2i(t->verts[i].y2d);
			*bottom_y_ind = i;
		}
	}

//--removed mk, 11/27/94--	//	Check for a non-upright-hourglass polygon and fix, if necessary, by bashing a y coordinate.
//--removed mk, 11/27/94--	//	min_y_ind = index of minimum y coordinate, *bottom_y_ind = index of maximum y coordinate
//--removed mk, 11/27/94--{
//--removed mk, 11/27/94--	int	max_temp, min_temp;
//--removed mk, 11/27/94--
//--removed mk, 11/27/94--	max_temp = *bottom_y_ind;
//--removed mk, 11/27/94--	if (*bottom_y_ind < min_y_ind)
//--removed mk, 11/27/94--		max_temp += t->nv;
//--removed mk, 11/27/94--
//--removed mk, 11/27/94--	for (i=min_y_ind; i<max_temp; i++) {
//--removed mk, 11/27/94--		if (f2i(t->verts[i%t->nv].y2d) > f2i(t->verts[(i+1)%t->nv].y2d)) {
//--removed mk, 11/27/94--			Int3();
//--removed mk, 11/27/94--			t->verts[(i+1)%t->nv].y2d = t->verts[i%t->nv].y2d;
//--removed mk, 11/27/94--		}
//--removed mk, 11/27/94--	}
//--removed mk, 11/27/94--
//--removed mk, 11/27/94--	min_temp = min_y_ind;
//--removed mk, 11/27/94--	if (min_y_ind < *bottom_y_ind)
//--removed mk, 11/27/94--		min_temp += t->nv;
//--removed mk, 11/27/94--
//--removed mk, 11/27/94--	for (i=*bottom_y_ind; i<min_temp; i++) {
//--removed mk, 11/27/94--		if (f2i(t->verts[i%t->nv].y2d) < f2i(t->verts[(i+1)%t->nv].y2d)) {
//--removed mk, 11/27/94--			Int3();
//--removed mk, 11/27/94--			t->verts[(i+1)%t->nv].y2d = t->verts[i%t->nv].y2d;
//--removed mk, 11/27/94--		}
//--removed mk, 11/27/94--	}
//--removed mk, 11/27/94--}

	// Set "vertex left top", etc. based on vertex with topmost y coordinate
	*vlt = min_y_ind;
	*vrt = *vlt;
	*vlb = prevmod(*vlt,t->nv);
	*vrb = succmod(*vrt,t->nv);

	// If right edge is horizontal, then advance along polygon bound until it no longer is or until all
	// vertices have been examined.
	// (Left edge cannot be horizontal, because *vlt is set to leftmost point with highest y coordinate.)

	original_vrt = *vrt;

	while (f2i(t->verts[*vrt].y2d) == f2i(t->verts[*vrb].y2d)) {
		if (succmod(*vrt,t->nv) == original_vrt) {
			break;
		}
		*vrt = succmod(*vrt,t->nv);
		*vrb = succmod(*vrt,t->nv);
	}
}

// -------------------------------------------------------------------------------------
//	Returns dx/dy given two vertices.
//	If dy == 0, returns 0.0
// -------------------------------------------------------------------------------------
//--fix compute_dx_dy_lin(g3ds_tmap *t, int top_vertex,int bottom_vertex)
//--{
//--	int	dy;
//--
//--	// compute delta x with respect to y for any edge
//--	dy = f2i(t->verts[bottom_vertex].y2d - t->verts[top_vertex].y2d) + 1;
//--	if (dy)
//--		return (t->verts[bottom_vertex].x2d - t->verts[top_vertex].x2d) / dy;
//--	else
//--		return 0;
//--
//--}

fix compute_du_dy_lin(g3ds_tmap *t, int top_vertex,int bottom_vertex, fix recip_dy)
{
	return fixmul(t->verts[bottom_vertex].u - t->verts[top_vertex].u, recip_dy);
}


fix compute_dv_dy_lin(g3ds_tmap *t, int top_vertex,int bottom_vertex, fix recip_dy)
{
	return fixmul(t->verts[bottom_vertex].v - t->verts[top_vertex].v, recip_dy);
}

fix compute_dl_dy_lin(g3ds_tmap *t, int top_vertex,int bottom_vertex, fix recip_dy)
{
	return fixmul(t->verts[bottom_vertex].l - t->verts[top_vertex].l, recip_dy);

}

fix compute_dx_dy(g3ds_tmap *t, int top_vertex,int bottom_vertex, fix recip_dy)
{
	return fixmul(t->verts[bottom_vertex].x2d - t->verts[top_vertex].x2d, recip_dy);
}

fix compute_du_dy(g3ds_tmap *t, int top_vertex,int bottom_vertex, fix recip_dy)
{
	return fixmul(fixmul(t->verts[bottom_vertex].u,t->verts[bottom_vertex].z) - fixmul(t->verts[top_vertex].u,t->verts[top_vertex].z), recip_dy);
}

fix compute_dv_dy(g3ds_tmap *t, int top_vertex,int bottom_vertex, fix recip_dy)
{
	return fixmul(fixmul(t->verts[bottom_vertex].v,t->verts[bottom_vertex].z) - fixmul(t->verts[top_vertex].v,t->verts[top_vertex].z), recip_dy);

}

fix compute_dz_dy(g3ds_tmap *t, int top_vertex,int bottom_vertex, fix recip_dy)
{
	return fixmul(t->verts[bottom_vertex].z - t->verts[top_vertex].z, recip_dy);

}
int Skip_short_flag=0;

// -------------------------------------------------------------------------------------
//	Texture map current scanline in perspective.
// -------------------------------------------------------------------------------------

int Do_vertical_scan=0;

int	Break_on_flat=0;


// -------------------------------------------------------------------------------------
//	Render a texture map with lighting using perspective interpolation in inner and outer loops.
// -------------------------------------------------------------------------------------

void ntmap_outerloop_correct_lighted( g3ds_tmap *t )
{
	#define LINEAR	0						// 0=Perspective, 1=Linear
	#define LIGHTING 1					// 0=No lighting, 1=Use lighting
#ifdef NASM
	#define INNER_LOOP c_tmap_scanline_per	// Function to be called when ready to draw.
#else
	#define INNER_LOOP asm_tmap_scanline_per	// Function to be called when ready to draw.
#endif
	#define CHECK_WINDOW	1				// Set to 1 to check window clip
	#define USE_UVS 1						// Set to 1 to enable U,V interpolation
	#include "ntmapout.h"
}

void ntmap_outerloop_correct_nolight( g3ds_tmap *t )
{
	#define LINEAR	0						// 0=Perspective, 1=Linear
	#define LIGHTING 0					// 0=No lighting, 1=Use lighting
#ifdef NASM
	#define INNER_LOOP c_tmap_scanline_per_nolight	// Function to be called when ready to draw.
#else
	#define INNER_LOOP asm_tmap_scanline_per	// Function to be called when ready to draw.
#endif
	#define CHECK_WINDOW	1				// Set to 1 to check window clip
	#define USE_UVS 1						// Set to 1 to enable U,V interpolation
	#include "ntmapout.h"
}


extern void asm_tmap_scanline_llt();

void ntmap_outerloop_lin_lighted_trans( g3ds_tmap *t )
{
	#define LINEAR	1						// 0=Perspective, 1=Linear
	#define LIGHTING 1					// 0=No lighting, 1=Use lighting
#ifdef NASM
	#define INNER_LOOP c_tmap_scanline_lin	// Function to be called when ready to draw.
#else
	#define INNER_LOOP asm_tmap_scanline_llt	// Function to be called when ready to draw.
#endif
	#define CHECK_WINDOW	1				// Set to 1 to check window clip
	#define USE_UVS 1						// Set to 1 to enable U,V interpolation
	#include "ntmapout.h"
}

extern void asm_tmap_scanline_lln();

void ntmap_outerloop_lin_lighted_notrans( g3ds_tmap *t )
{
	#define LINEAR	1						// 0=Perspective, 1=Linear
	#define LIGHTING 1					// 0=No lighting, 1=Use lighting
#ifdef NASM
	#define INNER_LOOP c_tmap_scanline_lin	// Function to be called when ready to draw.
#else
	#define INNER_LOOP asm_tmap_scanline_lln	// Function to be called when ready to draw.
#endif
	#define CHECK_WINDOW	1				// Set to 1 to check window clip
	#define USE_UVS 1						// Set to 1 to enable U,V interpolation
	#include "ntmapout.h"
}


extern void asm_tmap_scanline_plt();

void ntmap_outerloop_per_lighted_trans( g3ds_tmap *t )
{
	#define LINEAR	0						// 0=Perspective, 1=Linear
	#define LIGHTING 1					// 0=No lighting, 1=Use lighting
#ifdef NASM
	#define INNER_LOOP c_tmap_scanline_per	// Function to be called when ready to draw.
#else
	#define INNER_LOOP asm_tmap_scanline_plt	// Function to be called when ready to draw.
#endif
	#define CHECK_WINDOW	1				// Set to 1 to check window clip
	#define USE_UVS 1						// Set to 1 to enable U,V interpolation
	#include "ntmapout.h"
}

extern void asm_tmap_scanline_pln();

void ntmap_outerloop_per_lighted_notrans( g3ds_tmap *t )
{
	#define LINEAR	0						// 0=Perspective, 1=Linear
	#define LIGHTING 1					// 0=No lighting, 1=Use lighting
#ifdef NASM
	#define INNER_LOOP c_tmap_scanline_per	// Function to be called when ready to draw.
#else
	#define INNER_LOOP asm_tmap_scanline_pln	// Function to be called when ready to draw.
#endif
	#define CHECK_WINDOW	1				// Set to 1 to check window clip
	#define USE_UVS 1						// Set to 1 to enable U,V interpolation
	#include "ntmapout.h"
}

extern void asm_tmap_scanline_lnt();

void ntmap_outerloop_lin_nolight_trans( g3ds_tmap *t )
{
	#define LINEAR	1						// 0=Perspective, 1=Linear
	#define LIGHTING 0					// 0=No lighting, 1=Use lighting
#ifdef NASM
	#define INNER_LOOP c_tmap_scanline_lin_nolight	// Function to be called when ready to draw.
#else
	#define INNER_LOOP asm_tmap_scanline_lnt	// Function to be called when ready to draw.
#endif
	#define CHECK_WINDOW	1				// Set to 1 to check window clip
	#define USE_UVS 1						// Set to 1 to enable U,V interpolation
	#include "ntmapout.h"
}

extern void asm_tmap_scanline_lnn();

void ntmap_outerloop_lin_nolight_notrans( g3ds_tmap *t )
{
	#define LINEAR	1						// 0=Perspective, 1=Linear
	#define LIGHTING 0					// 0=No lighting, 1=Use lighting
#ifdef NASM
	#define INNER_LOOP c_tmap_scanline_lin_nolight	// Function to be called when ready to draw.
#else
	#define INNER_LOOP asm_tmap_scanline_lnn	// Function to be called when ready to draw.
#endif
	#define CHECK_WINDOW	1				// Set to 1 to check window clip
	#define USE_UVS 1						// Set to 1 to enable U,V interpolation
	#include "ntmapout.h"
}


extern void asm_tmap_scanline_pnt();

void ntmap_outerloop_per_nolight_trans( g3ds_tmap *t )
{
	#define LINEAR	0						// 0=Perspective, 1=Linear
	#define LIGHTING 0					// 0=No lighting, 1=Use lighting
#ifdef NASM
	#define INNER_LOOP c_tmap_scanline_per_nolight	// Function to be called when ready to draw.
#else
	#define INNER_LOOP asm_tmap_scanline_pnt	// Function to be called when ready to draw.
#endif
	#define CHECK_WINDOW	1				// Set to 1 to check window clip
	#define USE_UVS 1						// Set to 1 to enable U,V interpolation
	#include "ntmapout.h"
}


extern void asm_tmap_scanline_pnn();

void ntmap_outerloop_per_nolight_notrans( g3ds_tmap *t )
{
	#define LINEAR	0						// 0=Perspective, 1=Linear
	#define LIGHTING 0					// 0=No lighting, 1=Use lighting
#ifdef NASM
	#define INNER_LOOP c_tmap_scanline_per_nolight	// Function to be called when ready to draw.
#else
	#define INNER_LOOP asm_tmap_scanline_pnn	// Function to be called when ready to draw.
#endif
	#define CHECK_WINDOW	1				// Set to 1 to check window clip
	#define USE_UVS 1						// Set to 1 to enable U,V interpolation
	#include "ntmapout.h"
}


extern void asm_tmap_scanline_editor();
extern void c_tmap_scanline_editor();

void ntmap_outerloop_editor( g3ds_tmap *t )
{
	#define LINEAR	0						// 0=Perspective, 1=Linear
	#define LIGHTING 0					// 0=No lighting, 1=Use lighting
#ifdef NASM
	#define INNER_LOOP c_tmap_scanline_editor	// Function to be called when ready to draw.
#else
	#define INNER_LOOP asm_tmap_scanline_editor	// Function to be called when ready to draw.
#endif
	#define CHECK_WINDOW	0				// Set to 1 to check window clip
	#define USE_UVS 1						// Set to 1 to enable U,V interpolation
	#include "ntmapout.h"
}

// -------------------------------------------------------------------------------------
//	Render a texture map.
// Linear in outer loop, linear in inner loop.
// -------------------------------------------------------------------------------------
void texture_map_flat(g3ds_tmap *t)
{
	#define LINEAR	1						// 0=Perspective, 1=Linear
	#define LIGHTING 0					// 0=No lighting, 1=Use lighting
#ifdef NASM
	#define INNER_LOOP c_tmap_scanline_flat	// Function to be called when ready to draw.
#else
	#define INNER_LOOP asm_tmap_scanline_flat	// Function to be called when ready to draw.
#endif
	#define CHECK_WINDOW	1				// Set to 1 to check window clip
	#define USE_UVS 0						// Set to 1 to enable U,V interpolation
	#include "ntmapout.h"
}

extern void asm_tmap_scanline_shaded();	// In tmapfade.asm

void texture_map_flat_faded(g3ds_tmap *t)
{
	#define LINEAR	1						// 0=Perspective, 1=Linear
	#define LIGHTING 0					// 0=No lighting, 1=Use lighting
#ifdef NASM
	#define INNER_LOOP c_tmap_scanline_shaded	// Function to be called when ready to draw.
#else
	#define INNER_LOOP asm_tmap_scanline_shaded	// Function to be called when ready to draw.
#endif
	#define CHECK_WINDOW	1				// Set to 1 to check window clip
	#define USE_UVS 0						// Set to 1 to enable U,V interpolation
	#include "ntmapout.h"
}


// fix	DivNum = F1_0*12;

extern void draw_tmap_flat(grs_bitmap *bp,int nverts,g3s_point **vertbuf);

// -------------------------------------------------------------------------------------
// Interface from Matt's data structures to Mike's texture mapper.
// -------------------------------------------------------------------------------------
void draw_tmap(grs_bitmap *bp,int nverts,g3s_point **vertbuf)
{
	int	i;
	int	lighting_mode = Lighting_on;
	int	render_method;
	fix 	min_z;
   #ifdef _3DFX
   int   bm_index = bp->bm_handle;
   #endif

	Assert(nverts <= MAX_TMAP_VERTS);
	Assert(bp->bm_w == 64);
	Assert(Lighting_on < 3);

	//	If no transparency and seg depth is large, render as flat shaded.
	if ((Current_seg_depth > Max_linear_depth) && ((bp->bm_flags & 3) == 0)) {
		draw_tmap_flat(bp, nverts, vertbuf);
		return;
	}

	if ( bp->bm_flags & BM_FLAG_RLE )
   {
		bp = rle_expand_texture( bp );		// Expand if rle'd
      #ifdef _3DFX
      bp->bm_handle = bm_index;
      #endif
   }

	Transparency_on = bp->bm_flags & BM_FLAG_TRANSPARENT;
	if (bp->bm_flags & BM_FLAG_NO_LIGHTING)
		lighting_mode = 0;

	pixptr = bp->bm_data;

	render_method = Interpolation_method;
	if (render_method==0) {
		if (Current_seg_depth > Max_perspective_depth)
			render_method = 1;		// use linear
		else
			render_method = 2;		// use perspective
	}

	// Setup texture map in Tmap1
	Tmap1.nv = nverts;						// Initialize number of vertices

	for (i=0; i<nverts; i++) {
		g3ds_vertex	*tvp = &Tmap1.verts[i];
		g3s_point	*vp = vertbuf[i];

		tvp->x2d = vp->p3_sx;
		tvp->y2d = vp->p3_sy;
 		tvp->u = vp->p3_u << 6; //* bp->bm_w;
		tvp->v = vp->p3_v << 6; //* bp->bm_h;

		//	Perspective stuff:
		if ( render_method > 1 )
      {
			//Check for overflow on fixdiv.  Will overflow on vp->z <= something small.  Allow only as low as 256.
			if (vp->p3_z < 256) {
				vp->p3_z = 256;
				// Int3();		// we would overflow if we divided!
			}

			tvp->z = vp->p3_z;	//fixdiv(F1_0*12, vp->p3_z);
			if ( (i==0) || (tvp->z < min_z) )
				min_z = tvp->z;
		}

      #ifdef _3DFX
      tvp->z = vp->p3_z;
      #endif

		#if defined(POLY_ACC)
		 #ifdef PA_3DFX_VOODOO
	      tvp->z = vp->p3_z;
		 #endif
		#endif

		// Lighting stuff:
		if (lighting_mode)
			tvp->l = vp->p3_l * NUM_LIGHTING_LEVELS;
	}

#if defined(POLY_ACC)
 #ifdef PA_3DFX_VOODOO
    switch (render_method)
    {
        case 1:                             // linear interpolation
            pa_draw_tmap(bp, &Tmap1, Transparency_on, lighting_mode, 0, min_z);  // linear
            break;
        case 2:
        case 3:                             // perspective every pixel interpolation
            pa_draw_tmap(bp, &Tmap1, Transparency_on, lighting_mode, 1, min_z);  // persp.
            break;
        default:
            Assert(0);              // Illegal value for Interpolation_method, must be 0,1,2,3
			
    }
	return;
 #endif
#endif

#ifdef _3DFX

   if ( _3dfx_available )
   {
      if ( _3dfx_rendering_poly_obj )
      {
         _3dfx_DownloadAndUseTexture( bp->bm_handle );
      }
      if ( Lighting_on )
      {
         guColorCombineFunction( GR_COLORCOMBINE_TEXTURE_TIMES_ITRGB );

         _3dfxDrawTmap( bp, &Tmap1 );
      }
      else
      {
         guColorCombineFunction( GR_COLORCOMBINE_DECAL_TEXTURE );

         _3dfxDrawTmap( bp, &Tmap1 );
      }
   }
   _3dfx_triangles_rendered_post_clip += Tmap1.nv - 2;

   if ( _3dfx_skip_ddraw )
      goto done;
#endif

	// Calculate 1/Z for perspective
	if ( render_method > 1 )	{
		g3ds_vertex	*tvp = Tmap1.verts;
		for (i=0; i<nverts; i++, tvp++) {
			tvp->z = fixdiv( min_z, tvp->z );
			// Z is now in the range 0-F1_0 (17 bits)
			// So, in the next line, we need to 
			// scale Z down so that it fits in the available
			// number of significant bits (12 bits now).  But, at the
			// same time, we're scaling it up by Z_SHIFTER amount so 
			// that it use 30 bits for Z interpolation, which keeps the	
			// dz_dx values from becoming so small that they go to zero.
			tvp->z <<= Z_SHIFTER-(17-DIVIDE_SIG_BITS);					
		}
	}

	// render_method:
	//   1 = linear, 2 = subdivided perspective, 3 = full perspective
	// lighting_mode:
	//	  0 = no lighting, 1=lighting, 2=editor

#if defined(POLY_ACC)
    switch (render_method)
    {
/*  obsolete 3/5/96.
        case 0:                             // choose best interpolation
            if (Current_seg_depth > Max_perspective_depth)
                pa_draw_tmap(bp, &Tmap1, Transparency_on, lighting_mode, 0);  // linear
            else
                pa_draw_tmap(bp, &Tmap1, Transparency_on, lighting_mode, 1);  // persp.
            break;
*/
        case 1:                             // linear interpolation
            pa_draw_tmap(bp, &Tmap1, Transparency_on, lighting_mode, 0, min_z);  // linear
            break;
        case 2:
        case 3:                             // perspective every pixel interpolation
            pa_draw_tmap(bp, &Tmap1, Transparency_on, lighting_mode, 1, min_z);  // persp.
            break;
        default:
            Assert(0);              // Illegal value for Interpolation_method, must be 0,1,2,3
    }

#else
	switch( lighting_mode )	{
	case 0:
		switch (render_method) {	
		case 1:								// linear interpolation
			if (Transparency_on)
				ntmap_outerloop_lin_nolight_trans(&Tmap1);
			else
				ntmap_outerloop_lin_nolight_notrans(&Tmap1);
			break;
		case 2:								// subdivided perspective
			if (Transparency_on)
				ntmap_outerloop_per_nolight_trans( &Tmap1 );
			else
				ntmap_outerloop_per_nolight_notrans( &Tmap1 );
			break;
		case 3:								// perspective every pixel interpolation
			ntmap_outerloop_correct_nolight( &Tmap1 );
			break;
		default:
			Assert(0);				// Illegal value for Interpolation_method, must be 0,1,2,3
		}
		break;
	case 1:
		switch (render_method) {	
		case 1:								// linear interpolation
			if (Transparency_on)
				ntmap_outerloop_lin_lighted_trans(&Tmap1);
			else
				ntmap_outerloop_lin_lighted_notrans(&Tmap1);
			break;
		case 2:								// subdivided perspective
			if (Transparency_on)
				ntmap_outerloop_per_lighted_trans( &Tmap1 );
			else
				ntmap_outerloop_per_lighted_notrans( &Tmap1 );
			break;
		case 3:								// perspective every pixel interpolation
			ntmap_outerloop_correct_lighted( &Tmap1 );
			break;
		default:
			Assert(0);				// Illegal value for Interpolation_method, must be 0,1,2,3
		}
		break;
#ifdef EDITOR_TMAP
	case 2:
		ntmap_outerloop_editor( &Tmap1 );
		break;
#endif
	}
#endif  //POLY_ACC

#ifdef _3DFX
done:
   return;
#endif

}

#ifdef _3DFX

#define SNAP( a ) ( ( fix ) ( (a) & ( 0xFFFFF000L ) ) )

void _3dfxDrawTmap( grs_bitmap *bp, g3ds_tmap *Tmap1 )
{
   GrVertex a, b, c;
   int      i;

   if ( _3dfx_no_texture )
      guColorCombineFunction( GR_COLORCOMBINE_ITRGB );

   a.r = a.g = a.b = Tmap1->verts[0].l * fix_to_rgb;

   a.tmuvtx[0].oow = a.oow = 1.0 / ( Tmap1->verts[0].z * fix_to_float );
   a.tmuvtx[0].sow = Tmap1->verts[0].u * fix_to_st * a.oow;
   a.tmuvtx[0].tow = Tmap1->verts[0].v * fix_to_st * a.oow;

   a.x = SNAP( Tmap1->verts[0].x2d ) * fix_to_float;
   a.y = SNAP( Tmap1->verts[0].y2d ) * fix_to_float;

   for ( i = 1; i < Tmap1->nv - 1; i++ )
   {
      b.r = b.g = b.b = Tmap1->verts[i].l * fix_to_rgb;
      b.tmuvtx[0].oow = b.oow = 1.0 / ( Tmap1->verts[i].z * fix_to_float );
      b.tmuvtx[0].sow = Tmap1->verts[i].u * fix_to_st * b.oow;
      b.tmuvtx[0].tow = Tmap1->verts[i].v * fix_to_st * b.oow;
      b.x   = SNAP( Tmap1->verts[i].x2d ) * fix_to_float;
      b.y   = SNAP( Tmap1->verts[i].y2d ) * fix_to_float;

      c.r = c.g = c.b = Tmap1->verts[i+1].l * fix_to_rgb;
      c.tmuvtx[0].oow = c.oow = 1.0 / ( Tmap1->verts[i+1].z * fix_to_float );
      c.tmuvtx[0].sow = Tmap1->verts[i+1].u * fix_to_st * c.oow;
      c.tmuvtx[0].tow = Tmap1->verts[i+1].v * fix_to_st * c.oow;
      c.x   = SNAP( Tmap1->verts[i+1].x2d ) * fix_to_float;
      c.y   = SNAP( Tmap1->verts[i+1].y2d ) * fix_to_float;

      grDrawTriangle( &a, &b, &c );
   }
}

#endif

