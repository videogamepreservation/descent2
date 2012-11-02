// ----------------------------------------------------------------------------
void draw_segment(segment *s)
{
	int f,v;
	short	*vp;
	int	nv,nf;

	seg_get_vertex_list(s,&nv,&vp);				// set nv = number of vertices, vp = pointer to vertex indices
	seg_get_num_faces(s,&nf);						// set nf = number of faces
	g3_rotate_list(nv,vp);
//	g3_project_list(nv,vp);

	for (f=0; f<nf; f++) {
		seg_get_face_vertex_list(s,f,&nv,&vp);	// set nv = number of vertices in face, vp = pointer to vertex indices

		gr_setcolor(f+1);

		for (v=0;v<nv-1;v++)
			g3_draw_line(vp[v],vp[v+1]);

		g3_draw_line(vp[v],vp[0]);


	}


}
ÿ