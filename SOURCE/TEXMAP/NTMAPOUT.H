//
// This is the outer loop of the texture mapper.
//
// Put inside a function like:
// void ntmap_outerloop( g3ds_tmap *t )
//
// Uses the following #defines
//
// #define LINEAR	1			// 0=Perspective, 1=Linear
// #define LIGHTING 1		// 0=No lighting, 1=Use lighting
// #define INNER_LOOP asm_tmap_per	// Function to be called when ready to draw.
// #define CHECK_WINDOW	1	// Set to 1 to check window clip
// #define USE_UVS	1			// Set to 1 to allow UV interpolation
//
// Optimizations:
// * 1.40% In per/8 method by taking out the scanline function that was between
//         the outer loop and the inner.  Also, I made the lighting, linear, etc
//         be compile-time rather than run-time switches in the outer loop.  This
//         should also speed up the other forms of the texture mapper, but I didn't
//         profile them.   This is the change that created ntmapout.h.
// * 0.15% In perspective outer loop by getting rid of the redundant multiply
//         (xright*zright) that is done in outer loop and compute_du_dy.
// ***** The two above improvements netted a 1.50% increase in test case ******
//
	int	vlt,vrt,vlb,vrb;	// vertex left top, vertex right top, vertex left bottom, vertex right bottom
	int	topy,boty,y, dy;
	fix	dx_dy_left,dx_dy_right;
#if USE_UVS == 1
	fix	du_dy_left,du_dy_right;
	fix	dv_dy_left,dv_dy_right;
	fix	uleft,vleft,uright,vright;
#endif
#if LIGHTING == 1
	fix	dl_dy_left,dl_dy_right;
	fix	lleft,lright;
#endif
#if LINEAR == 0
	fix	zleft,zright;
	fix	dz_dy_left,dz_dy_right;
#endif
	fix	recip_dyl, recip_dyr;
	int	max_y_vertex;
	fix	xleft,xright;
	int	next_break_left, next_break_right;
	int	last_one = 0;
	fix	dx;

	g3ds_vertex *v3d;

#if LINEAR == 0
		// If perspective, then we need to multiply all of
		// the uv's by one over Z.   
		v3d = t->verts;
		for (y=0; y<t->nv; y++, v3d++) {
			v3d->u = fixmul( v3d->u, v3d->z >> Z_SHIFTER );
			v3d->v = fixmul( v3d->v, v3d->z >> Z_SHIFTER );
		}
#endif

	v3d = t->verts;

	// Determine top and bottom y coords.
	compute_y_bounds(t,&vlt,&vlb,&vrt,&vrb,&max_y_vertex);

	// Set top and bottom (of entire texture map) y coordinates.
	topy = f2i(v3d[vlt].y2d);
	boty = f2i(v3d[max_y_vertex].y2d);
	if (topy > Window_clip_bot)
		return;
	if (boty > Window_clip_bot)
		boty = Window_clip_bot;

	// Set amount to change x coordinate for each advance to next scanline.
	dy = f2i(t->verts[vlb].y2d) - f2i(t->verts[vlt].y2d);
	if (dy < FIX_RECIP_TABLE_SIZE)
		recip_dyl = fix_recip[dy];
	else
		recip_dyl = F1_0/dy;

	dx_dy_left = compute_dx_dy(t,vlt,vlb, recip_dyl);

	dy = f2i(t->verts[vrb].y2d) - f2i(t->verts[vrt].y2d);
	if (dy < FIX_RECIP_TABLE_SIZE)
		recip_dyr = fix_recip[dy];
	else
		recip_dyr = F1_0/dy;

	dx_dy_right = compute_dx_dy(t,vrt,vrb, recip_dyr);

 	// Set initial values for x, u, v
	xleft = v3d[vlt].x2d;
	xright = v3d[vrt].x2d;

#if USE_UVS == 1
		uleft = v3d[vlt].u;
		vleft = v3d[vlt].v;
		du_dy_left = fixmul(t->verts[vlb].u - uleft, recip_dyl);
		dv_dy_left = fixmul(t->verts[vlb].v - vleft, recip_dyl);
		uright = v3d[vrt].u;
		vright = v3d[vrt].v;
		du_dy_right = fixmul(t->verts[vrb].u - uright, recip_dyr);
		dv_dy_right = fixmul(t->verts[vrb].v - vright, recip_dyr);
#endif

#if LINEAR == 0
		zleft = v3d[vlt].z;
		dz_dy_left = fixmul(t->verts[vlb].z - zleft, recip_dyl);
		zright = v3d[vrt].z;
		dz_dy_right = fixmul(t->verts[vrb].z - zright, recip_dyr);
#endif

#if LIGHTING == 1
		lleft = v3d[vlt].l;
		dl_dy_left = fixmul(t->verts[vlb].l - lleft, recip_dyl);
		lright = v3d[vrt].l;
		dl_dy_right = fixmul(t->verts[vrb].l - lright, recip_dyr);
#endif

	// scan all rows in texture map from top through first break.
	next_break_left = f2i(v3d[vlb].y2d);
	next_break_right = f2i(v3d[vrb].y2d);

	for (y = topy; y < boty; y++) {

		// See if we have reached the end of the current left edge, and if so, set
		// new values for dx_dy and x,u,v
		if (y == next_break_left) {
			fix	recip_dy;

			// Handle problem of double points.  Search until y coord is different.  Cannot get
			// hung in an infinite loop because we know there is a vertex with a lower y coordinate
			// because in the for loop, we don't scan all spanlines.
			while (y == f2i(v3d[vlb].y2d)) {
				vlt = vlb;
				vlb = prevmod(vlb,t->nv);
			}
			next_break_left = f2i(v3d[vlb].y2d);

			dy = f2i(t->verts[vlb].y2d) - f2i(t->verts[vlt].y2d);
			if (dy < FIX_RECIP_TABLE_SIZE)
				recip_dy = fix_recip[dy];
			else
				recip_dy = F1_0/dy;

			dx_dy_left = compute_dx_dy(t,vlt,vlb, recip_dy);

			xleft = v3d[vlt].x2d;

#if USE_UVS == 1
				uleft = v3d[vlt].u;
				vleft = v3d[vlt].v;
				du_dy_left = fixmul(t->verts[vlb].u - uleft, recip_dy);
				dv_dy_left = fixmul(t->verts[vlb].v - vleft, recip_dy);
#endif

#if LINEAR == 0
				zleft = v3d[vlt].z;
				dz_dy_left = fixmul(t->verts[vlb].z - zleft, recip_dy);
#endif

#if LIGHTING == 1
				lleft = v3d[vlt].l;
				dl_dy_left = fixmul(t->verts[vlb].l - lleft, recip_dy);
#endif
		}

		// See if we have reached the end of the current left edge, and if so, set
		// new values for dx_dy and x.  Not necessary to set new values for u,v.
		if (y == next_break_right) {
			fix	recip_dy;

			while (y == f2i(v3d[vrb].y2d)) {
				vrt = vrb;
				vrb = succmod(vrb,t->nv);
			}

			next_break_right = f2i(v3d[vrb].y2d);

			dy = f2i(t->verts[vrb].y2d) - f2i(t->verts[vrt].y2d);
			if (dy < FIX_RECIP_TABLE_SIZE)
				recip_dy = fix_recip[dy];
			else
				recip_dy = F1_0/dy;

			dx_dy_right = compute_dx_dy(t,vrt,vrb, recip_dy);

			xright = v3d[vrt].x2d;


#if USE_UVS == 1
				uright = v3d[vrt].u;
				vright = v3d[vrt].v;
				du_dy_right = fixmul(t->verts[vrb].u - uright, recip_dy);
				dv_dy_right = fixmul(t->verts[vrb].v - vright, recip_dy);
#endif

#if LINEAR == 0
				zright = v3d[vrt].z;
				dz_dy_right = fixmul(t->verts[vrb].z - zright, recip_dy);
#endif

#if LIGHTING == 1
				lright = v3d[vrt].l;
				dl_dy_right = fixmul(t->verts[vrb].l - lright, recip_dy);
#endif
		}

draw_scanline:
		fx_xright = f2i(xright);
		fx_xleft = f2i(xleft);
		dx = fx_xright - fx_xleft;

		if ( (y >= Window_clip_top) && (dx>=0) && (xright>=0) && (xleft<=xright) )	{
			fix	recip_dx;
		
			fx_y = y;

			// setup to call assembler scanline renderer
			if (dx < FIX_RECIP_TABLE_SIZE)
				recip_dx = fix_recip[dx];
			else
				recip_dx = F1_0/dx;

#if USE_UVS == 1
				fx_u = uleft;
				fx_du_dx = fixmul(uright - uleft,recip_dx);
				fx_v = vleft;
				fx_dv_dx = fixmul(vright - vleft,recip_dx);
				fx_u_right = uright;
				fx_v_right = vright;
#endif

#if LINEAR == 0
				fx_z = zleft;
				fx_dz_dx = fixmul(zright - zleft,recip_dx);
				fx_z_right = zright;
#endif


#if CHECK_WINDOW == 1
			if (fx_xleft > Window_clip_right)
				goto dont_draw_scanline;
			if (fx_xright < Window_clip_left)
				goto dont_draw_scanline;
			if (fx_xright > Window_clip_right)	{
#if LINEAR == 0
					// For perspective, we need to do proper clipping
					// on fx_z_right, fx_u_right, fx_v_right
					int delta = fx_xright - Window_clip_right;
					fx_u_right -= fx_du_dx*delta;
					fx_v_right -= fx_dv_dx*delta;
					fx_z_right -= fx_dz_dx*delta;
					fx_xright = Window_clip_right;
#else
					fx_xright = Window_clip_right;
#endif
			}
#endif

#if LIGHTING == 1
			{
				fix	mul_thing;
				
				if (lleft < 0) lleft = 0;
				if (lright < 0) lright = 0;
				if (lleft > (NUM_LIGHTING_LEVELS*F1_0-F1_0/2)) lleft = (NUM_LIGHTING_LEVELS*F1_0-F1_0/2);
				if (lright > (NUM_LIGHTING_LEVELS*F1_0-F1_0/2)) lright = (NUM_LIGHTING_LEVELS*F1_0-F1_0/2);
				
				fx_l = lleft;
				fx_dl_dx = fixmul(lright - lleft,recip_dx);
				
				//	This is a pretty ugly hack to prevent lighting overflows.
				mul_thing = dx * fx_dl_dx;
				if (lleft + mul_thing < 0)
					fx_dl_dx += 12;
				else if (lleft + mul_thing > (NUM_LIGHTING_LEVELS*F1_0-F1_0/2))
					fx_dl_dx -= 12;
// lighting values are passed in fixed point, but need to be in 8 bit integer, 8 bit fraction so we can easily
// get the integer by reading %bh
				fx_l >>= 8;
				fx_dl_dx >>= 8;
				if ( fx_dl_dx < 0 )
					fx_dl_dx++;		// round towards 0 for negative deltas
			}
#endif

			if ( fx_xleft < 0 ) fx_xleft = 0;
			dest_row_data = write_buffer + y_pointers[fx_y] + fx_xleft;
			loop_count = fx_xright - fx_xleft;
			if ( loop_count >= 0 )	{
				INNER_LOOP();
			}
		}

#if CHECK_WINDOW == 1
dont_draw_scanline:
#endif
		if ( last_one )
			return;

#if LIGHTING == 1
			lleft += dl_dy_left;
			lright += dl_dy_right;
#endif

#if USE_UVS == 1
			uleft += du_dy_left;
			vleft += dv_dy_left;
	
			uright += du_dy_right;
			vright += dv_dy_right;
#endif

		xleft += dx_dy_left;
		xright += dx_dy_right;

#if LINEAR == 0
			zleft += dz_dy_left;
			zright += dz_dy_right;
#endif
	}

	last_one = 1;
	goto draw_scanline;

#undef LINEAR
#undef LIGHTING
#undef INNER_LOOP
#undef CHECK_WINDOW
#undef USE_UVS

ÿ