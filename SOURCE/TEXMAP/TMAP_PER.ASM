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

	public	asm_tmap_scanline_per_

	include	tmap_inc.asm



_DATA	SEGMENT DWORD PUBLIC USE32 'DATA'
	align	4

_DATA	ENDS

DGROUP	GROUP	_DATA


_TEXT   SEGMENT PARA PUBLIC USE32 'CODE'
	ASSUME	DS:_DATA
	ASSUME	CS:_TEXT

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

	align	16
asm_tmap_scanline_per_:

	START_TIMER

	pusha
	
;---------------------------- setup for loop ---------------------------------
; Setup for loop:	_loop_count  iterations = (int) xright - (int) xleft
;	esi	source pixel pointer = pixptr
;	edi	initial row pointer = y*320+x
; NOTE: fx_xright and fx_xleft changed from fix to int by mk on 12/01/94.

; set esi = pointer to start of texture map data

; set edi = address of first pixel to modify
	mov	edi, _dest_row_data

; set initial values
	mov	ebx,_fx_u
	mov	ebp,_fx_v
	mov	ecx,_fx_z

;================ PERSPECTIVE TEXTURE MAP INNER LOOPS ========================
;
; Usage in loop:	eax	division, pixel value
;	ebx	u
;	ecx	z
;	edx	division
;	ebp	v
;	esi	source pixel pointer
;	edi	destination pixel pointer

;-------------------- NORMAL PERSPECTIVE TEXTURE MAP LOOP -----------------
tmap_loop:
	mov	esi, ebx	; esi becomes u coordinate

	align	4
tmap_loop0:

; compute v coordinate
	push	ecx
	shr	ecx, Z_SHIFTER
	mov	eax, ebp	; get v
	mov	edx, eax
	sar	edx, 31
	idiv	ecx	; eax = (v/z)
	and	eax, 03fh

	mov	ebx,eax

; compute u coordinate
	mov	eax, esi	; get u
	mov	edx, eax
	sar	edx, 31
	idiv	ecx	; eax = (u/z)
	pop	ecx
	and	eax, 03fh

	shl 	eax,32-6
	shld 	ebx,eax,6	; esi = v*64+u

	;and	ebx,03f3fh	; mask with height-1

; read 1 pixel
	xor	eax, eax
	test	_Lighting_on, -1
	add	ebx, _pixptr
	mov	al, [ebx]	; get pixel from source bitmap
	je	NoLight1

; LIGHTING CODE
	mov	ebx, _fx_l	; get temp copy of lighting value
	mov	ah, bh	; get lighting level
	add	ebx, _fx_dl_dx	; update lighting value
	mov	al, _gr_fade_table[eax]	; xlat pixel thru lighting tables
	mov	_fx_l, ebx	; save temp copy of lighting value

; transparency check
NoLight1:	cmp	al,255
	je	skip1

	mov	[edi],al
skip1:	inc	edi
	
; update deltas
	add	ebp,_fx_dv_dx
	add	esi,_fx_du_dx
	add	ecx,_fx_dz_dx
	je	_div_0_abort	; would be dividing by 0, so abort

	dec	_loop_count
	jns	tmap_loop0


_none_to_do:	
	popa
	STOP_TIMER
	ret

; We detected a z=0 condition, which seems pretty bogus, don't you think?
; So, we abort, but maybe we want to know about it.
_div_0_abort:
;**	int	3
	jmp	_none_to_do

_TEXT	ends

	end

