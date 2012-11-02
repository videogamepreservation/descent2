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
static char rcsid[] = "$Id: $";
#pragma on (unreferenced)

#include "segment.h"
#include "3d.h"
#include "gr.h"

extern	face		Faces[];
extern	vector	Vertices[];
extern	uvpair	Uvs[];
extern	rgb		Rgbs[];

extern	int		Num_faces,Num_vertices,Num_uvs,Num_rgbs,Num_face_verts;

extern	short		Face_verts[];
extern	int		Face_verts_free;


// ---------------------------------------------------------------------------------------------
//           ---------- Segment interrogation functions ----------
// ----------------------------------------------------------------------------
//	Return a pointer to the list of vertex indices for the current segment in vp and
//	the number of vertices in *nv.
void seg_get_vertex_list(segment *s,int *nv,short **vp)
{
	*vp = s->fvs;
	*nv = 8;
}

// ----------------------------------------------------------------------------
//	Return a pointer to the list of vertex indices for face facenum in vp and
//	the number of vertices in *nv.
void seg_get_face_vertex_list(segment *s,int facenum,int *nv,short **vp)
{
	face	*fp = &Faces[s->iface + facenum];

	*vp = &Face_verts[fp->ivert];
	*nv = fp->num_vertices;
}

// ----------------------------------------------------------------------------
//	Set *nf = number of faces in segment s.
void seg_get_num_faces(segment *s,int *nf)
{
	*nf = s->num_faces;
}



