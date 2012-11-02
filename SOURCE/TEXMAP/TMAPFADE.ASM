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

	public	asm_tmap_scanline_shaded_

	include	tmap_inc.asm

_DATA	SEGMENT DWORD PUBLIC USE32 'DATA'
		

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
;**;	_pixptr	address of source pixel map

;   for (x = (int) xleft; x <= (int) xright; x++) {
;      _setcolor(read_pixel_from_tmap(srcb,((int) (u/z)) & 63,((int) (v/z)) & 63));
;      _setpixel(x,y);
;
;      z += dz_dx;
;   }

asm_tmap_scanline_shaded_:
	push	eax
	push	ebx
	push	ecx
	push	edi

	mov	edi, _dest_row_data

	mov	ecx, 0
	mov	ch,_tmap_flat_shade_value
	and	ch, 31

	mov	ebx, _loop_count
NextPixel:
	mov	cl, [edi]		; get pixel
	mov	al, _gr_fade_table[ecx]	; darken pixel
	mov	[edi], al		; write pixel
	inc	edi			; goto next pixel
	dec	ebx
	jns	NextPixel

_none_to_do:	
	pop	edi
	pop	ecx
	pop	ebx
	pop	eax
	ret

_TEXT	ends

	end
