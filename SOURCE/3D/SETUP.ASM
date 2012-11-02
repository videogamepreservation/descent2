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
	;;include	div0.inc
	.list

	assume	cs:_TEXT, ds:_DATA

_DATA	segment	dword public USE32 'DATA'

rcsid	db	"$Id: setup.asm 1.18 1996/02/14 09:59:56 matt Exp $"
	align	4

 extd free_point_num

_DATA	ends



_TEXT	segment	dword public USE32 'CODE'

	extn	init_interface_vars_to_assembler_
	extn	atexit_	;C library function

;I don't what these are supposed to do, but here they are.

;sets up divide overflow handler, and sets the global points variables. 
;takes eax=points, edx=vecs, ecx=num points
;trashes eax,edx
g3_init:

;;##	mov	eax,DM_ERROR
;;##	call	div0_init_
;;##	or	eax,eax	;check error
;;##	jnz	dz_ok
;;##	int	3
;;##dz_ok:

;;@@;handlers for point projection
;;@@	lea	eax,cs:proj_div0
;;@@	lea	edx,cs:div_overflow_handler
;;@@	call	div0_set_handler_
;;@@
;;@@	lea	eax,cs:proj_div1
;;@@	lea	edx,cs:div_overflow_handler
;;@@	call	div0_set_handler_
;;@@
;;@@;handlers for blob draw
;;@@	lea	eax,cs:blob_div0
;;@@	lea	edx,cs:blob_overflow_handler
;;@@	call	div0_set_handler_
;;@@
;;@@	lea	eax,cs:blob_div1
;;@@	lea	edx,cs:blob_overflow_handler
;;@@	call	div0_set_handler_
;;@@
;;@@;handler for sphere projection
;;@@	lea	eax,cs:sphere_proj_div
;;@@	lea	edx,cs:sphere_div_overflow_handler
;;@@	call	div0_set_handler_
;;@@

	lea	eax,cs:g3_close
	call	atexit_

	ret


g3_close:	ret


;cv_bitmap equ 0
;bm_w equ 4
;bm_h equ 6

;start rendering a frame. sets up window vars
g3_start_frame:	pushm	eax,edx,esi

	mov	esi,grd_curcanv

	movzx	eax,w [esi].cv_bitmap.bm_w
	mov	Canvas_width,eax	;store width
	sal	eax,15	;get fixed-point width/2
	mov	Canv_w2,eax

	movzx	eax,w [esi].cv_bitmap.bm_h
	mov	Canvas_height,eax	;store height
	sal	eax,15	;get fixed-point height/2
	mov	Canv_h2,eax

;compute aspect ratio for this canvas
	mov	esi,grd_curscreen
	mov	eax,[esi].sc_aspect	;get pixel ratio
	imul	Canvas_height	;figure in window size
	idiv	Canvas_width

	cmp	eax,f1_0	; > 1?
	jg	scale_y
	mov	Window_scale.x,eax
	mov	Window_scale.y,f1_0
	jmp	got_win_scale
scale_y:	mov	esi,eax
	mov	eax,f1_0
	fixdiv	esi
	mov	Window_scale.y,eax
	mov	Window_scale.x,f1_0
got_win_scale:	mov	Window_scale.z,f1_0	;always 1

	call	init_interface_vars_to_assembler_

	popm	eax,edx,esi
	ret

;check for temp points that haven't been freed, and reset if necessary
g3_end_frame:
	check_free_points
	ret


;@@ public g3_get_view_pos_ptr_
;@@g3_get_view_pos_ptr_:
;@@	lea	eax,View_position
;@@	ret


_TEXT	ends

	end
