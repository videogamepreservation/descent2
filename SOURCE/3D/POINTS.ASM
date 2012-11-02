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
.386
	option	oldstructs

	.nolist
	include	pstypes.inc
	include	psmacros.inc
	include	gr.inc
	include	3d.inc
	.list

	assume	cs:_TEXT, ds:_DATA

_DATA	segment	dword public USE32 'DATA'

rcsid	db	"$Id: points.asm 1.14 1996/01/08 14:59:12 matt Exp $"
	align	4

tempv	vms_vector <>

tempm	vms_matrix <>

_DATA	ends



_TEXT	segment	dword public USE32 'CODE'

;finds clipping codes for a point. takes eax=point. fills in p3_codes,
;and returns codes in bl.
g3_code_point:
code_point:	push	ecx

	xor	bl,bl	;clear codes
	mov	ecx,[eax].z	;get z
	cmp	[eax].x,ecx	;x>z?
	jle	not_right
	or	bl,CC_OFF_RIGHT
not_right:	cmp	[eax].y,ecx	;y>z?
	jle	not_top
	or	bl,CC_OFF_TOP
not_top:	neg	ecx
	js	not_behind
	or	bl,CC_BEHIND
not_behind:	cmp	[eax].x,ecx	;x<-z?
	jge	not_left
	or	bl,CC_OFF_LEFT
not_left:	cmp	[eax].y,ecx	;y<-z
	jge	not_bot
	or	bl,CC_OFF_BOT
not_bot:	pop	ecx
	mov	[eax].p3_codes,bl
	ret
	

;rotate a point. don't look at rotated flags
;takes esi=dest point, esi=src vector
;returns bl=codes
g3_rotate_point:	pushm	eax,ecx,esi,edi

	push	edi	;save dest

	lea	eax,tempv
	lea	edi,View_position
	call	vm_vec_sub
	mov	esi,eax

	pop	eax
	
	lea	edi,View_matrix
	call	vm_vec_rotate
	mov	[eax].p3_flags,0	;not projected

	;;mov	bx,_Frame_count	;curren frame
	;;mov	[eax].p3_frame,bx

	call	code_point

	popm	eax,ecx,esi,edi
	ret


;projects a point. takes esi=point  
g3_project_point:
	;;ifndef	NDEBUG
	;; push	eax
	;; mov	ax,[esi].p3_frame
	;; cmp	ax,_Frame_count
	;; break_if	ne,'Trying to project unrotated point!'
	;; pop	eax
	;;endif

	test	[esi].p3_flags,PF_PROJECTED
	jnz	no_project
	test	[esi].p3_codes,CC_BEHIND
	jnz	no_project

	pushm	eax,edx

	mov	eax,[esi].x
	imul	Canv_w2
proj_div0:	divcheck	[esi].z,div_overflow_handler
	idiv	[esi].z
	add	eax,Canv_w2
	mov	[esi].p3_sx,eax

	mov	eax,[esi].y
	imul	Canv_h2
proj_div1:	divcheck	[esi].z,div_overflow_handler
	idiv	[esi].z
	neg	eax
	add	eax,Canv_h2
	mov	[esi].p3_sy,eax

	or	[esi].p3_flags,PF_PROJECTED	;projected

	popm	eax,edx

no_project:	ret

div_overflow_handler:
	;int	3
	mov	[esi].p3_flags,PF_OVERFLOW
	popm	eax,edx
	ret

;from a 2d point on the screen, compute the vector in 3-space through that point
;takes eax,ebx = 2d point, esi=vector
;the 2d point is relative to the center of the canvas
g3_point_2_vec:	pushm	ecx,edx,esi,edi

	push	esi

	lea	esi,tempv

	;;mov	edx,eax
	;;xor	eax,eax
	;;idiv	Canv_w2
	sal	eax,16
	sub	eax,Canv_w2
	fixdiv	Canv_w2
	imul	Matrix_scale.z
	idiv	Matrix_scale.x
	mov	[esi].x,eax

	;;mov	edx,ebx
	;;xor	eax,eax
	;;sub	Canv_h2
	;;idiv	Canv_h2
	mov	eax,ebx
	sal	eax,16
	sub	eax,Canv_h2
	fixdiv	Canv_h2
	imul	Matrix_scale.z
	idiv	Matrix_scale.y
	neg	eax
	mov	[esi].y,eax

	mov	[esi].z,f1_0

	call	vm_vec_normalize	;get normalized rotated vec

	lea	edi,tempm
	lea	esi,Unscaled_matrix
	call	vm_copy_transpose_matrix

	pop	eax	;get dest
	lea	esi,tempv	;edi=matrix
	call	vm_vec_rotate

	popm	ecx,edx,esi,edi

	ret

;rotate a delta y vector. takes edi=dest vec, ebx=delta y
g3_rotate_delta_y:	pushm	eax,edx
	mov	eax,View_matrix.m4
	fixmul	ebx
	mov	[edi].x,eax
	mov	eax,View_matrix.m5
	fixmul	ebx
	mov	[edi].y,eax
	mov	eax,View_matrix.m6
	fixmul	ebx
	mov	[edi].z,eax
	popm	eax,edx
	ret


;rotate a delta x vector. takes edi=dest vec, ebx=delta x
g3_rotate_delta_x:	pushm	eax,edx
	mov	eax,View_matrix.m1
	fixmul	ebx
	mov	[edi].x,eax
	mov	eax,View_matrix.m2
	fixmul	ebx
	mov	[edi].y,eax
	mov	eax,View_matrix.m3
	fixmul	ebx
	mov	[edi].z,eax
	popm	eax,edx
	ret


;rotate a delta x vector. takes edi=dest vec, ebx=delta z
g3_rotate_delta_z:	pushm	eax,edx
	mov	eax,View_matrix.m7
	fixmul	ebx
	mov	[edi].x,eax
	mov	eax,View_matrix.m8
	fixmul	ebx
	mov	[edi].y,eax
	mov	eax,View_matrix.m9
	fixmul	ebx
	mov	[edi].z,eax
	popm	eax,edx
	ret


;rotate a delta vector. takes edi=dest vec, esi=src vec
g3_rotate_delta_vec:
	pushm	eax,edi
	mov	eax,edi
	lea	edi,View_matrix
	call	vm_vec_rotate
	popm	eax,edi
	ret

;adds a delta vector to a point. takes eax=dest point, esi=src pnt, edi=delta vec
;returns bl=codes.
g3_add_delta_vec:	;;ifndef	NDEBUG
	;; push	eax
	;; mov	ax,[esi].p3_frame
	;; cmp	ax,_Frame_count
	;; break_if	ne,'Trying to add delta to unrotated point!'
	;; pop	eax
	;;endif

	call	vm_vec_add

	;;mov	bx,_Frame_count
	;;mov	[eax].p3_frame,bx
	mov	[eax].p3_flags,0	;not projected

	call	g3_code_point

	ret

;calculate the depth of a point - returns the z coord of the rotated point
;takes esi=vec, returns eax=depth
g3_calc_point_depth:
	pushm	edx,ebx,ecx

	mov	eax,[esi].x
	sub	eax,View_position.x
	imul	View_matrix.fvec.x
	mov	ebx,eax
	mov	ecx,edx

	mov	eax,[esi].y
	sub	eax,View_position.y
	imul	View_matrix.fvec.y
	add	ebx,eax
	adc	ecx,edx

	mov	eax,[esi].z
	sub	eax,View_position.z
	imul	View_matrix.fvec.z
	add	eax,ebx
	adc	edx,ecx

	shrd	eax,edx,16

	popm	edx,ebx,ecx

	ret
    	


_TEXT	ends

	end
