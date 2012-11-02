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
	include	tmap_inc.asm
	.list

	assume	cs:_TEXT, ds:_DATA

_DATA	segment	dword public USE32 'DATA'

rcsid	db	"$Id: tmap_ed.asm 1.1 1996/01/24 16:35:44 champaign Exp $"
	align	4

_DATA	ends



_TEXT	segment	dword public USE32 'CODE'

; --------------------------------------------------------------------------------------------------
; Enter:
;	_xleft	fixed point left x coordinate
;	_xright	fixed point right x coordinate
;	_y	fixed point y coordinate
;	_pixptr	address of source pixel map
;	_u	fixed point initial u coordinate
;	_v	fixed point initial v coordinate
;	_z	fixed point initial z coordinate
;	_du_dx	fixed point du/dx
;	_dv_dx	fixed point dv/dx
;	_dz_dx	fixed point dz/dx

;   for (x = (int) xleft; x <= (int) xright; x++) {
;      _setcolor(read_pixel_from_tmap(srcb,((int) (u/z)) & 63,((int) (v/z)) & 63));
;      _setpixel(x,y);
;
;      u += du_dx;
;      v += dv_dx;
;      z += dz_dx;
;   }


zonk	equ	1
xonk	equ	01010101h

public asm_tmap_scanline_editor_
asm_tmap_scanline_editor_:
	pusha
	
;---------------------------- setup for loop ---------------------------------
; Setup for loop:	_loop_count  iterations = (int) xright - (int) xleft
;	esi	source pixel pointer = pixptr
;	edi	initial row pointer = y*320+x

; set esi = pointer to start of texture map data
	mov	edi, _dest_row_data
	

; set initial values
	mov	ebx,_fx_u
	mov	ebp,_fx_v
	mov	ecx,_fx_z

;-------------------- NORMAL PERSPECTIVE TEXTURE MAP LOOP -----------------
	align	4
tmap_loop:
	xchg	ebx, esi

; compute v coordinate
	mov	eax,ebp	; get v
	cdq
	idiv	ecx	; eax = (v/z)

	and	eax,3fh	; mask with height-1
	mov	ebx,eax

; compute u coordinate
	mov	eax,esi	; get u
	cdq
	idiv	ecx	; eax = (u/z)

	shl 	eax,26
	shld 	ebx,eax,6	; esi = v*64+u

; read 1  pixel
	add	ebx, _pixptr
	movzx	eax,byte ptr [ebx]	; get pixel from source bitmap

; transparency check
NoLight1:	cmp	al,255
	je	skip1

	mov	byte ptr [edi],zonk
skip1:	inc	edi
	
; update deltas
	add	ebp,_fx_dv_dx
	add	esi,_fx_du_dx
	add	ecx,_fx_dz_dx
	je	_div_0_abort	; would be dividing by 0, so abort

	xchg	esi, ebx

	dec	_loop_count
	jns	tmap_loop

_div_0_abort:
_none_to_do:	
	popa
	ret

_TEXT	ends

	end

