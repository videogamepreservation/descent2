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
#include "pa_enabl.h"                   //$$POLY_ACC
#include "mem.h"


#include "gr.h"
#include "grdef.h"

#if defined(POLY_ACC)
#include "poly_acc.h"
#include "error.h"
#endif

unsigned char gr_ugpixel( grs_bitmap * bitmap, int x, int y )
{
	switch(bitmap->bm_type)
	{
	case BM_LINEAR:
		return bitmap->bm_data[ bitmap->bm_rowsize*y + x ];
	case BM_MODEX:
		x += bitmap->bm_x;
		y += bitmap->bm_y;
		gr_modex_setplane( x & 3 );
		return gr_video_memory[(bitmap->bm_rowsize * y) + (x/4)];
	case BM_SVGA:
		{
		unsigned int offset;
		offset = (unsigned int)bitmap->bm_data + (unsigned int)bitmap->bm_rowsize * y + x;
		gr_vesa_setpage( offset >> 16 );
		return gr_video_memory[offset & 0xFFFF];
		}
#if defined(POLY_ACC)
    case BM_LINEAR15:
    {
        unsigned short p;
        while(!pa_idle());
        p = *(unsigned short *)(bitmap->bm_data + y * bitmap->bm_rowsize + x * PA_BPP);
        return gr_find_closest_color(((p >> 9) & 0x3e), ((p >> 4) & 0x3e), ((p << 1) & 0x3e));
    }
    default:
        Int3();
#endif
    }
    return 0;
}

unsigned char gr_gpixel( grs_bitmap * bitmap, int x, int y )
{
	if ((x<0) || (y<0) || (x>=bitmap->bm_w) || (y>=bitmap->bm_h)) return 0;

	switch(bitmap->bm_type)
	{
	case BM_LINEAR:
		return bitmap->bm_data[ bitmap->bm_rowsize*y + x ];
	case BM_MODEX:
		x += bitmap->bm_x;
		y += bitmap->bm_y;
		gr_modex_setplane( x & 3 );
		return gr_video_memory[(bitmap->bm_rowsize * y) + (x/4)];
	case BM_SVGA:
		{
		unsigned int offset;
		offset = (unsigned int)bitmap->bm_data + (unsigned int)bitmap->bm_rowsize * y + x;
		gr_vesa_setpage( offset >> 16 );
		return gr_video_memory[offset & 0xFFFF];
		}
#if defined(POLY_ACC)
    case BM_LINEAR15:
    {
        unsigned short p;
        while(!pa_idle());
        p = *(unsigned short *)(bitmap->bm_data + y * bitmap->bm_rowsize + x * PA_BPP);
        return gr_find_closest_color(((p >> 9) & 0x3e), ((p >> 4) & 0x3e), ((p << 1) & 0x3e));
    }
    default:
        Int3();
#endif
    }
	return 0;
}
