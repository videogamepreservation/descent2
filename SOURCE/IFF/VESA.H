extern int gr_vesa_setmode(int vesa_mode);
extern int gr_vesa_setpage(int page_num );
extern int gr_vesa_incpage();
extern void gr_vesa_setstart(int x, int y);
extern int gr_vesa_setlogical(int desired_width);
extern void gr_vesa_pixel8( int x, int y, int color);
extern void gr_vesa_pixel15( int x, int y, int color);
extern void gr_vesa_copy_canvas( void * buffer );
ÿ