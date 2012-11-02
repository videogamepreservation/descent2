; THE COMPUTER CODE CONTAINED HEREIN IS THE SOLE PROPERTY OF PARALLAX
; SOFTWARE CORPORATION ("PARALLAX").  PARALLAX, IN DISTRIBUTING THE CODE TO
; END-USERS, AND SUBJECT TO ALL OF THE TERMS AND CONDITIONS HEREIN, GRANTS A
; ROYALTY-FREE, PERPETUAL LICENSE TO SUCH END-USERS FOR USE BY SUCH END-USERS
; IN USING, DISPLAYING,  AND CREATING DERIVATIVE WORKS THEREOF, SO LONG AS
; SUCH USE, DISPLAY OR CREATION IS FOR NON-COMMERCIAL, ROYALTY OR REVENUE
; FREE PURPOSES.  IN NO EVENT SHALL THE END-USER USE THE COMPUTER CODE
; CONTAINED HEREIN FOR REVENUE-BEARING PURPOSES.  THE END-USER UNDERSTANDS
; AND AGREES TO THE TERMS HEREIN AND ACCEPTS THE SAME BY USE OF THIS FILE.  
; COPYRIGHT 1993-1999 PARALLAX SOFTWARE CORPORATION.  ALL RIGHTS RESERVED.
ifndef _3D_INC
_3D_INC equ 1

MAX_POINTS_IN_POLY = 100

	include	fix.inc
	include	vecmat.inc

;Structures

;Stucture to store clipping codes in a word
g3s_codes	STRUCT 2t
cc_or	ubyte	?	;or is low byte
cc_and	ubyte	?	;and is high byte
g3s_codes	ENDS

;Used to store rotated points for mines.  Has frame count to indictate
;if rotated, and flag to indicate if projected.
g3s_point	struct
	union
p3_vec	 vms_vector <>
	 struct
x	  fix	?
y	  fix	?
z	  fix	?
	 ends
xyz	 fix	3t DUP (?)
	ends	;end of union
p3_u	fix	?	;u & v for this point
p3_v	fix	?
p3_l	fix	?
p3_sx	fix	?	;screen x & y
p3_sy	fix	?
p3_codes	db	?	;clipping codes
p3_flags	db	?	;projected?
p3_pad	dw	?	;keep longword aligned
g3s_point	ends

;Flags

;for g3_flags field of g3s_point
PF_PROJECTED	equ	1	;has been projected
PF_OVERFLOW	equ	2	;can't project
PF_TEMP_POINT	equ	4	;created during clip
PF_UVS	equ	8	;has uv values set
PF_LVS	equ	16	;has lighting values set

;for clipping codes
CC_OFF_LEFT	equ	1
CC_OFF_RIGHT	equ	2
CC_OFF_BOT	equ	4
CC_OFF_TOP	equ	8
CC_BEHIND	equ	80h

_DATA	segment	dword public USE32 'DATA'

;Global variables

	extdef	vms_vector,View_position,Matrix_scale,Window_scale
	extdef	vms_matrix,View_matrix,Unscaled_matrix
	extf	View_zoom

	extd	Vbuf0,Vbuf1,Num_points

	extd	Canvas_width,Canvas_height	;the actual width & height
	extf	Canv_w2,Canv_h2			;fixed-point width,height/2

	extf	Vertex_list

;Variables local to 3d library
	extd	free_point_num

_DATA	ends

_TEXT	segment	dword public USE32 'CODE'

;Routines

;from setup.asm
	extn	g3_init	;eax=point list, edx=vec list
	extn	g3_close	;takes nothing, ret nothing
	extn	g3_start_frame	;takes nothing, ret nothing
	extn	g3_end_frame	;takes nothing, ret nothing

;from matrix.asm
	extn	g3_set_view_angles	;esi=angvec,esi=pos,eax=zoom
	extn	g3_set_view_matrix	;esi=matrix,edi=pos,eax=zoom

;from points.asm
	;;extn	g3_set_points	;eax=point list, edx=vec list
	extn	g3_rotate_point	;edi=dest point, esi=src vec
	extn	g3_project_point	;esi=point
	extn	g3_point_2_vec	;esi=vec, eax,ebx = 2d point (shorts)
	extn	g3_code_point	;eax=point, fill in code, ret bl=code

	extn	g3_rotate_delta_x	;edi=dest vec, ebx=delta
	extn	g3_rotate_delta_y	;edi=dest vec, ebx=delta
	extn	g3_rotate_delta_z	;edi=dest vec, ebx=delta
	extn	g3_rotate_delta_vec	;edi=dest vec, esi=delta vec
	extn	g3_add_delta_vec	;eax=dest pnt, esi=src pnt, edi=delta vec. ret bl=codes

	extn	g3_calc_point_depth	;esi=point, ret eax=depth

	extn	proj_div0,proj_div1,div_overflow_handler,code_point
	extn	blob_div0,blob_div1,blob_overflow_handler
	extn	sphere_proj_div,sphere_div_overflow_handler

;from draw.asm
	extn	g3_draw_line	;esi,edi=points
	extn	g3_draw_poly	;ecx=cnt, esi=point nums, al=ret: 1 off screen, 0 drew
	extn	g3_check_and_draw_poly	;ecx=cnt, esi=point nums, edi=normal, al=ret: -1 not facing, 1 off screen, 0 drew
	extn	g3_draw_tmap	;ecx=cnt, esi=point nums, ebx=uvs, edx=bitmap, al=ret: 1 off screen, 0 drew
	extn	g3_check_and_draw_tmap	;ecx=cnt, esi=point nums, ebx=uvs, edx=bitmap, edi=normal, al=ret: -1 not facing, 1 off screen, 0 drew
	extn	g3_check_normal_facing	;esi=vec, edi=norm, ret al
	;;extn	g3_compute_lighting_value	;ecx=surface normal scale, esi=rotated point, ret ecx
	extn	g3_draw_sphere	;esi=pos (rotated), ecx=radius
	extn	g3_set_special_render ;eax=tmap draw, edx=flat drawer, ebx=line drawer

;from clipper.asm
	extn	clip_line	;esi,edi=points (ptrs)
	extn	reset_temp_points	;(takes nothing)
	extn	free_temp_point	;esi=point (ptr)
	extn	clip_polygon	;esi=src list,edi=dest list,ecx=n, al=codes_or

;from objects.asm
	extn	g3_draw_object	;esi=pos,edi=mat,eax=size,ebx=obj_id


;from horizon.asm
	extn	g3_draw_horizon	;eax=sky_color, edx=ground_color
	extn	g3_compute_sky_polygon	;ebx=2d points, ecx=vecs

;from instance.asm
	extn	g3_start_instance_matrix	;esi=pos,edi=matrix. trashes esi,edi
	extn	g3_start_instance_angles	;esi=pos,edi=angvec. trashes esi,edi

	extn	g3_done_instance	;no parms

;from rod.asm
	extn	g3_draw_rod_tmap	;esi,edi=bot,top points,eax,edx=bot,top width, ebx=bitmap, ecx=lightval
	extn	g3_draw_rod_flat	;esi,edi=bot,top points,eax,edx=bot,top width
	extn	g3_draw_bitmap	;esi=pos,ebx=width3d,ecx=height3d,eax=bm, trashes eax,ecx,esi

;from interp.asm

	extn	g3_set_interp_points	;eax=ptr to list
	extn	g3_draw_polygon_model	;esi=model ptr, edi=bitmap list ptr, eax=anim angles, edx=light, ebx=ptr to glow values (or NULL)
	extn	g3_init_polygon_model	;esi=model ptr
	extn	g3_uninit_polygon_model	;esi=model ptr
	extn	g3_draw_morphing_model	;esi=model ptr, edi=bitmap list ptr, eax=anim angles, edx=light, ebx=new points
	extn	g3_remap_interp_colors	;(void)

_TEXT	ends



;check for overflow before doing divide.
;assume numerator is in edx:eax.  Denominator and label to jump to if
;overflow are passed.
divcheck	macro	den,lbl
	local	not_neg1,not_neg2
	push	ecx
	mov	ecx,edx
	or	ecx,ecx
	jns	not_neg1
	neg	ecx
not_neg1:	shld	ecx,eax,1	;num * 2
	cmp	ecx,den
	pop	ecx
	jae	lbl
	endm

;see if any free points left unfreed, and if so, reset the free point list
check_free_points	macro
	local	free_points_ok
	test	free_point_num,-1
	jz	free_points_ok
	debug_brk	"Error: free_point_num != 0 after draw"
	call	reset_temp_points
free_points_ok:
	endm


endif

