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

#define DOUBLE_BUFFER 1
#define USE_MOUSE 0

#include <conio.h>

#include "mono.h"
#include "mouse.h"
#include "gr.h"
#include "3d.h"

//#include "segment.h"
#include "out.c"

grs_canvas *off_screen_buffer;

#define N_SEG_VERTS (sizeof(Vertices) / sizeof(*Vertices))

g3s_point segment_points[N_SEG_VERTS];

void do_int3();

#pragma aux do_int3 = "int 3";

#define f1_1 (65536*11/10)

grs_canvas *screen_canv;	//the one on the screen

#define VIEWER_DIST i2f(100)

void segment_test()
{
	int exit_flag=0;
	vms_vector viewer_position={0,0,-VIEWER_DIST};
	vms_angvec viewer_orient={0,0,0};
	vms_matrix viewer_matrix={	f1_0,0,0,
										0,f1_0,0,
										0,0,f1_0};
	fix viewer_dist = VIEWER_DIST;
	int k;
	fix viewer_zoom=f1_0;

	g3_set_points(segment_points,Vertices);

	while (!exit_flag) {
#if USE_MOUSE
		short dx,dy;
		short btns;

		mouse_get_delta(&dx,&dy);
		btns = mouse_get_btns();

		if (btns) {
			viewer_dist += i2f(dy);
		}
		else if (! (dx==0 && dy==0)) {
			vms_matrix rotmat,tempm;

			GetMouseRotation(dx,dy,&rotmat);

			vm_matrix_x_matrix(&tempm,&viewer_matrix,&rotmat);
			viewer_matrix = tempm;

		}
#else

//vm_angvec_make(&viewer_orient,0x1707,0x49b,0x2e0e);
//vm_angvec_make(&viewer_orient,0x6e05,0x1601,0xdc0a);

		mprintf(0,"orient = %04x %04x %04x\n",viewer_orient.p,viewer_orient.b,viewer_orient.h);
//		k=getch(); exit_flag=(k==27); if (k==8) do_int3();
		vm_angles_2_matrix(&viewer_matrix,&viewer_orient);
#endif

		viewer_position.x = -viewer_matrix.m3;
		viewer_position.y = -viewer_matrix.m6;
		viewer_position.z = -viewer_matrix.m9;
		vm_vec_scale(&viewer_position,viewer_dist);


#if DOUBLE_BUFFER
		gr_set_current_canvas(off_screen_buffer);
#endif
	
		gr_clear_canvas(0);

		g3_start_frame();

//		g3_set_view_angles(&viewer_position,&viewer_orient,viewer_zoom);
		g3_set_view_matrix(&viewer_position,&viewer_matrix,viewer_zoom);

		draw_segment(&Seg1);

		g3_end_frame();

#if !USE_MOUSE
		viewer_orient.p += 65535/1500;
		viewer_orient.h += 65536/500;
		viewer_orient.b += 65536/1000;
#endif

#if DOUBLE_BUFFER
		gr_set_current_canvas(screen_canv);
		gr_ubitmap(0,0,&off_screen_buffer->cv_bitmap);
#endif

		if (kbhit()) {
			k=getch();

			exit_flag=(k==27);

			if (k==' ') do_int3();

			if (k=='[') viewer_zoom=fixmul(viewer_zoom,f1_1);
			if (k==']') viewer_zoom=fixdiv(viewer_zoom,f1_1);

		}
	}


}

//#include "draw_seg.c"

// ----------------------------------------------------------------------------
void draw_segment(segment *s)
{
	int f;
//	int v;
	short	*vp;
	int	nv,nf;
	face *face;

	seg_get_vertex_list(s,&nv,&vp);				// set nv = number of vertices, vp = pointer to vertex indices
	seg_get_num_faces(s,&nf);						// set nf = number of faces
	g3_rotate_list(nv,vp);

	for (f=0; f<nf; f++) {
		face = &Faces[f];

		seg_get_face_vertex_list(s,f,&nv,&vp);	// set nv = number of vertices in face, vp = pointer to vertex indices

		gr_setcolor(f+1);

//		for (v=0;v<nv-1;v++)
//			g3_draw_line(vp[v],vp[v+1]);
//		g3_draw_line(vp[v],vp[0]);

		//g3_draw_poly(4,vp,0);
		//g3_check_and_draw_poly(4,vp,0,NULL);

		//if (g3_check_normal_facing(&Vertices[vp[0]],&face->normal))
		//	g3_draw_poly(4,vp,0);

		g3_check_and_draw_poly(4,vp,&face->normal);


	}


}

main()
{
	minit();

	gr_init(SM_320x200C);	//init 2d, set up 320x200 screen

gr_pal_fade_in(&grd_curscreen->pal);

	mouse_init();

	screen_canv = grd_curcanv;

//	screen_canv = gr_create_sub_canvas(grd_curcanv,0,75,320,50);	//short
//	screen_canv = gr_create_sub_canvas(grd_curcanv,145,0,50,200);	//skinny

#if DOUBLE_BUFFER
	off_screen_buffer = gr_create_canvas(screen_canv->cv_bitmap.bm_w,screen_canv->cv_bitmap.bm_h);
#endif

	gr_set_current_canvas(&grd_curscreen->sc_canvas);
	gr_clear_canvas(1);
	gr_set_current_canvas(screen_canv);

	g3_init();					//init 3d

	segment_test();

	g3_close();					//close the 3d

	gr_close();					//goodbye to the 2d



}


