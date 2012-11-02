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
	include	3d.inc
	.list

	assume	ds:_DATA

_DATA	segment	dword public USE32 'DATA'

rcsid	db	"$Id: globvars.asm 1.7 1996/01/08 14:59:09 matt Exp $"
	align	4

View_position	vms_vector <>
View_zoom	fix	?

Unscaled_matrix	vms_matrix <>	;before scaling
View_matrix	vms_matrix <>

Window_scale	vms_vector <>	;scaling for window aspect
Matrix_scale	vms_vector <>	;how the matrix is scaled
			;window_scale * zoom
Canvas_width	dd	?	;the actual width
Canvas_height	dd	?	;the actual height

Canv_w2	fix	?	;fixed-point width/2
Canv_h2	fix	?	;fixed-point height/2

;vertex buffers for polygon drawing and clipping
Vbuf0	dd	MAX_POINTS_IN_POLY dup (?)
Vbuf1	dd	MAX_POINTS_IN_POLY dup (?)

;list of 2d coords
Vertex_list	fix	MAX_POINTS_IN_POLY*2 dup (?)

;;;used for determining whether to rotate a frame
;;_Frame_count	dw	-1	;first frame will inc to 0

_DATA	ends

	end

