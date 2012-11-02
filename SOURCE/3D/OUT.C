#include "segment.h"
face Faces[] = {
	{
		4,0,1,0,		// num_vertices, tmap_num, face_type, ivert
		0,0,				// iuv, irgb
		{0x00010000, 0x00000000, 0x00000000}			// normal
	},
	{
		4,0,1,4,		// num_vertices, tmap_num, face_type, ivert
		0,0,				// iuv, irgb
		{0x00000000, 0xffff0000, 0x00000000}			// normal
	},
	{
		4,0,1,8,		// num_vertices, tmap_num, face_type, ivert
		0,0,				// iuv, irgb
		{0xffff0000, 0x00000000, 0x00000000}			// normal
	},
	{
		4,0,1,12,		// num_vertices, tmap_num, face_type, ivert
		0,0,				// iuv, irgb
		{0x00000000, 0x00010000, 0x00000000}			// normal
	},

	{
		4,0,1,16,		// num_vertices, tmap_num, face_type, ivert
		0,0,				// iuv, irgb
		{0x00000000, 0x00000000, 0xffff0000}			// normal
	},
	{
		4,0,1,20,		// num_vertices, tmap_num, face_type, ivert
		0,0,				// iuv, irgb
		{0x00000000, 0x00000000, 0x00001000}			// normal
	},
};
vms_vector Vertices[] = {
	{0x000a0000, 0x00190000, 0xffe70000},
	{0x000a0000, 0xfff10000, 0xffe70000},
	{0xfff60000, 0xfff10000, 0xffe70000},
	{0xfff60000, 0x00190000, 0xffe70000},
	{0x000a0000, 0x00190000, 0x00190000},
	{0x000a0000, 0xfff10000, 0x00190000},
	{0xfff60000, 0xfff10000, 0x00190000},
	{0xfff60000, 0x00190000, 0x00190000},
};
uvpair Uvs[] = {
	{0x00000000, 0x00000000},
	{0x00000000, 0x00000000},
	{0x00000000, 0x00000000},
	{0x00000000, 0x00000000},
	{0x00000000, 0x00000000},
	{0x00000000, 0x00000000},
	{0x00000000, 0x00000000},
	{0x00000000, 0x00000000},
};
rgb Rgbs[] = {
	{0x00000000, 0x00000000, 0x00000000},
	{0x00000000, 0x00000000, 0x00000000},
	{0x00000000, 0x00000000, 0x00000000},
	{0x00000000, 0x00000000, 0x00000000},
	{0x00000000, 0x00000000, 0x00000000},
	{0x00000000, 0x00000000, 0x00000000},
	{0x00000000, 0x00000000, 0x00000000},
	{0x00000000, 0x00000000, 0x00000000},
};
short Face_verts[] = {
   2,    3,    7,    6,    
   3,    0,    4,    7, 
   0,    1,    5,    4,    
   1,    2,    6,    5, 

	7,		4,		5,		6,
	0,		3,		2,		1,
};
segment Seg1 = {
	0,			// segnum
	6,			// num_faces
	{0xfff60000, 0xfff10000, 0xffe70000},	// vstart
	{0x000a0000, 0x00190000, 0x00190000},	// vend
	0,				// iface
	{  -1,   -1,   -1,   -1,   -1,   -1},	// children
	{   0,    1,    2,    3},				// fvs
	{   4,    5,    6,    7},				// bvs
	0				// robots
};
ÿ