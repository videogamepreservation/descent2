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
ALWAYS_USE_SCALER = 0	;if true, use scaler even when banked


.386
	option	oldstructs

	.nolist
	include	pstypes.inc
	include	psmacros.inc
	include	gr.inc
	include	3d.inc
	.list

	assume	cs:_TEXT, ds:_DATA

	public	_blob_vertices

_DATA	segment	dword public USE32 'DATA'

rcsid	db	"$Id: rod.asm 1.24 1996/01/08 14:59:13 matt Exp $"
	align	4

bot_width	fix	?
top_width	fix	?

rod_points	db	4*size g3s_point dup (?)

delta_x	fix	?
delta_y	fix	?

edge_vec_x	fix	?
edge_vec_y	fix	?

vertbuf	fix	4 dup (?,?)
_blob_vertices	fix	4 dup (?,?)

bitmap_ptr	dd	?

delta_vec	vms_vector <>
rod_norm	vms_vector <>
temp_vec	vms_vector <>
top_pnt	dd	?
bot_pnt	dd	?

rod_point_list	dd	rod_points
	dd	rod_points+(size g3s_point)
	dd	rod_points+(size g3s_point)*2
	dd	rod_points+(size g3s_point)*3

;values here are set to be half a pixel in from the edges so the texture
;mapper doesn't read past the bitmap  
uvl_list	fix	200h,200h,0
	fix	0fe00h,200h,0
	fix	0fe00h,0fe00h,0
	fix	200h,0fe00h,0

;;uvl_list	fix	0,0,0
;;	fix	10000h,0,0
;;	fix	10000h,10000h,0
;;	fix	0,10000h,0

_orientation_save	dd	?

_DATA	ends



_TEXT	segment	dword public USE32 'CODE'

	extn	scale_bitmap_

;draws a polygon that's always facing the viewer
;takes esi,edi=bottom & top points (g3s_point *), eax,edx=3d width at bottom,top
g3_draw_rod_flat:	pushm	eax,ebx,ecx,edx,esi,edi

	call	calc_rod_corners
	or	cl,cl	;check codes
	jnz	no_draw

	mov	ecx,4	;4 corners
	lea	esi,rod_point_list
	call	g3_draw_poly

	popm	eax,ebx,ecx,edx,esi,edi
	ret

;draws bitmap that's always facing the viewer
;takes esi,edi=bottom & top points (g3s_point *), eax,edx=3d width at bottom,top
;ebx = bitmap
;points must be rotated
g3_draw_rod_tmap:
	pushm	eax,ebx,ecx,edx,esi,edi

	mov	bitmap_ptr,ebx

	;save lighting values
	mov	uvl_list+8,ecx
	mov	uvl_list+20,ecx
	mov	uvl_list+32,ecx
	mov	uvl_list+44,ecx

	call	calc_rod_corners
	or	cl,cl	;check codes
	jnz	no_draw

	mov	ecx,4
	lea	esi,rod_point_list
	lea	ebx,uvl_list
	mov	edx,bitmap_ptr
	call	g3_draw_tmap

	jmp	no_draw


;;@@;can use use scaler?
;;@@if not ALWAYS_USE_SCALER
;;@@	mov	eax,vertbuf+4	;point 0 y
;;@@	cmp	eax,vertbuf+12	;point 1 y
;;@@	jne	must_roll
;;@@;use the scaler!
;;@@endif
;;@@	mov	eax,bitmap_ptr
;;@@	lea	edx,vertbuf
;;@@	call	scale_bitmap_
;;@@	jmp	no_draw
;;@@
;;@@;can't call the scaler
;;@@must_roll:	mov	eax,bitmap_ptr
;;@@	lea	edx,vertbuf
;;@@	mov	ebx,15		;light value
;;@@	call	rotate_bitmap_

no_draw:
	popm	eax,ebx,ecx,edx,esi,edi
	ret




;draws a bitmap with the specified 3d width & height 
;takes esi=position, ebx=width, ecx=height, eax=bitmap
; edx=orientation: bits set mean: 0=flip in u, 1=flip in v, 2=exchange u, v
;modifies eax,esi,ecx

; bool g3_draw_bitmap(vms_vector *pos,fix width,fix height,grs_bitmap *bm);

g3_draw_bitmap:	pushm	edx,edi

	mov	_orientation_save, edx

	mov	bitmap_ptr,eax	;save bitmap

	push	ebx	;save width
	lea	edi,rod_points	;get dest point
	call	g3_rotate_point
	or	bl,bl	;off screen?
	pop	ebx	;restore width
	js	blob_off_screen

;we should check if off screen based on rad

	mov	esi,edi
	call	g3_project_point
	test	[esi].p3_flags,PF_OVERFLOW
	jnz	blob_off_screen

;get 2d width & height

	mov	eax,ebx	;get width
	imul	Canv_w2
blob_div0:	divcheck	[esi].z,blob_overflow_handler
	idiv	[esi].z	;width3d*canv_w/z
	fixmul	Matrix_scale.x	;*scale
	mov	edi,eax	;save width

	mov	eax,ecx	;get height
	imul	Canv_h2
blob_div1:	divcheck	[esi].z,blob_overflow_handler
	idiv	[esi].z	;height3d*canv_h/z
	fixmul	Matrix_scale.y	;*scale
	mov	ecx,eax	;save height

;copy 2d points into buffer

	lea	edx,_blob_vertices

	mov	eax,[esi].p3_sx
	sub	eax,edi	;- width
	mov	[edx],eax	;p0.x
	lea	eax,[eax+edi*2]	;+ 2*width
	mov	8[edx],eax	;p1.x
	mov	16[edx],eax	;p2.x

	mov	eax,[esi].p3_sy
	sub	eax,ecx	;- height
	mov	4[edx],eax	;p0.y
	mov	12[edx],eax	;p1.y
	lea	eax,[eax+ecx*2]	;+ 2*height
	mov	20[edx],eax	;p2.y

;now draw
	mov	eax,bitmap_ptr	;get bitmap ptr
	push	ebx
	mov	ebx, _orientation_save	; use normal orientation, no rotation, flip.
	call	scale_bitmap_	;vertbuf in edx
	pop	ebx

blob_off_screen:	popm	edx,edi
	ret

blob_overflow_handler:
	;debug_brk	"blob overflow - get Matt"
	jmp	blob_off_screen




;compute the corners of a rod.  fills in vertbuf. ret codes in al.
;points in esi,edi, widths in eax,edx  
;trashes piles of registers
calc_rod_corners:

	mov	bot_width,eax
	mov	top_width,edx

	mov	bot_pnt,esi
	mov	top_pnt,edi

;compute vector from one point to other, do cross product with vector
;from eye to get perpendiclar

	lea	eax,delta_vec
	call	vm_vec_sub

;unscale for aspect
	mov	eax,delta_vec.x
	fixdiv	matrix_scale.x
	mov	delta_vec.x,eax
	mov	eax,delta_vec.y
	fixdiv	matrix_scale.y
	mov	delta_vec.y,eax


;calc perp vector

;do lots of normalizing to prevent overflowing.  When this code works,
;it should be optimized

	lea	esi,delta_vec
	call	vm_vec_normalize

	vm_copy	temp_vec,edi
	mov	edi,esi
	lea	esi,temp_vec
	call	vm_vec_normalize

	xchg	esi,edi	
	lea	eax,rod_norm
	call	vm_vec_crossprod

	mov	esi,eax
	call	vm_vec_normalize

;scale for aspect

	mov	eax,rod_norm.x
	fixmul	matrix_scale.x
	mov	rod_norm.x,eax
	mov	eax,rod_norm.y
	fixmul	matrix_scale.y
	mov	rod_norm.y,eax


;now we have the usable edge.  generate four points

	mov	cl,0ffh	;codes and

	push	ebp
	lea	ebp,rod_points

;top points
	lea	edi,temp_vec
	lea	ebx,rod_norm
	mov	ecx,top_width
	call	vm_vec_copy_scale
 mov temp_vec.z,0

	mov	eax,ebp
	mov	esi,top_pnt	;temp vec in edi
	call	vm_vec_add

	add	eax,size g3s_point
	call	vm_vec_sub

	lea	ebp,size g3s_point[eax]

;bot points
	lea	edi,temp_vec
	lea	ebx,rod_norm
	mov	ecx,bot_width
	call	vm_vec_copy_scale
 mov temp_vec.z,0

	mov	eax,ebp
	mov	esi,bot_pnt	;temp vec in edi
	call	vm_vec_sub

	add	eax,size g3s_point
	call	vm_vec_add

	pop	ebp

;now code the four points

	mov	cl,0ffh	;codes and

	lea	eax,rod_points
	call	code_point
	and	cl,bl
	add	eax,size g3s_point
	call	code_point
	and	cl,bl
	add	eax,size g3s_point
	call	code_point
	and	cl,bl
	add	eax,size g3s_point
	call	code_point
	and	cl,bl
	jnz	new_new_off_screen

	push	ecx	;save codes

;clear flags for new points (not projected)

	lea	esi,rod_points
	mov	[esi].p3_flags,0
	mov	[esi+size g3s_point].p3_flags,0
	mov	[esi+(size g3s_point)*2].p3_flags,0
	mov	[esi+(size g3s_point)*3].p3_flags,0

	pop	ecx	;restore codes

	ret

new_new_off_screen:
	ret


_TEXT	ends

	end

