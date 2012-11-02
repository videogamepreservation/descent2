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

	public	asm_tmap_scanline_flat_

	include	tmap_inc.asm

_DATA	SEGMENT DWORD PUBLIC USE32 'DATA'


_DATA	ENDS

DGROUP	GROUP	_DATA

_TEXT   SEGMENT DWORD PUBLIC USE32 'CODE'

	ASSUME	DS:_DATA
	ASSUME	CS:_TEXT

; --------------------------------------------------------------------------------------------------
; Enter:
;	_xleft	fixed point left x coordinate
;	_xright	fixed point right x coordinate
;	_y	fixed point y coordinate
;**;	_pixptr	address of source pixel map

;   for (x = (int) xleft; x <= (int) xright; x++) {
;      _setcolor(read_pixel_from_tmap(srcb,((int) (u/z)) & 63,((int) (v/z)) & 63));
;      _setpixel(x,y);
;
;      z += dz_dx;
;   }

asm_tmap_scanline_flat_:
	pusha

; Setup for loop:	_loop_count  iterations = (int) xright - (int) xleft
;	edi	initial row pointer = y*320+x

; set edi = address of first pixel to modify
	mov	edi, _dest_row_data
	mov	ecx, _loop_count

; edi = destination pixel pointer
	cmp	_tmap_flat_cthru_table, 0
	jne	do_flat_cthru

	mov	al,_tmap_flat_color

; word align
	inc	ecx
	test	edi,1
	je	edi_even
	mov	[edi],al
	inc	edi
	dec	ecx
	je	_none_to_do
edi_even:	shr	ecx,1
	je	_no_full_words
	mov	ah,al
	rep	stosw
_no_full_words:	adc	ecx,ecx	; if cy set, then write one more pixel (ecx == 0)
	rep	stosb	; write 0 or 1 pixel

_none_to_do:	popa
	ret

do_flat_cthru:
	mov	esi, _tmap_flat_cthru_table
	xor	eax, eax
	cmp	ecx, 0
	je	_none_to_do
	; edi = dest, esi = table, ecx = count

flat_cthru_loop:
	mov	al, [edi]	; get already drawn pixel
	mov	al, [eax+esi]	; xlat thru cthru table
	mov	[edi],al	; write it
	inc	edi
	dec	ecx
	jnz	flat_cthru_loop
	popa
	ret

_TEXT	ends

	end


