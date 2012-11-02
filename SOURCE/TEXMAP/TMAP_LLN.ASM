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
rcsid	db	"$Id: tmap_lln.asm 1.2 1996/01/24 18:33:58 matt Exp $"
	align	4
num_big_steps	dd	?
_DATA	ends

_TEXT	segment	para public USE32 'CODE'

	align	16

public asm_tmap_scanline_lln_
asm_tmap_scanline_lln_:

	START_TIMER

	push	eax
	push	ecx
	push	edx
	push	ebx
	push	ebp
	push	esi
	push	edi
	
; set edi = address of first pixel to modify
	mov	edi, _dest_row_data


	mov	eax, _fx_v
	shr	eax, 6
	mov	edx, _fx_u
	shl	edx, 10
	mov	dx, ax		; EDX=U:V in 6.10 format

	mov	eax, _fx_dv_dx
	shr	eax, 6
	mov	esi, _fx_du_dx
	shl	esi, 10
	mov	si, ax		; ESI=DU:DV in 6.10 format

	mov	ebx, _fx_l
	mov	ebp, _fx_dl_dx

	mov	ecx, _pixptr

	mov	eax, _loop_count
	inc	eax
	mov	_loop_count, eax

	shr	eax, 3
	je	DoLeftOverPixels

	mov	num_big_steps, eax
	and	_loop_count, 7

NextPixelBlock:
	COUNT = 0
	REPT 8
		mov	eax, edx	
		shr	ax, 10
		rol	eax, 6
		and	eax, 0ffffh
		add	edx, esi
		mov	al, [ecx+eax]			
		mov	ah, bh	    	
		add	ebx, ebp
		mov	al, _gr_fade_table[eax]
		mov	[edi+COUNT], al
		COUNT = COUNT + 1
	ENDM
	add	edi, COUNT
	dec	num_big_steps
	jne	NextPixelBlock
	

DoLeftOverPixels:
	mov	eax,_loop_count
	test	eax, -1
	jz	_none_to_do
	shr	eax, 1
	je	one_more_pix
	mov	_loop_count, eax
	pushf


NextPixel:
	COUNT = 0
	REPT 2
		mov	eax, edx	
		shr	ax, 10
		rol	eax, 6
		and	eax, 0ffffh
		add	edx, esi
		mov	al, [ecx+eax]			
		mov	ah, bh	    	
		add	ebx, ebp
		mov	al, _gr_fade_table[eax]
		mov	[edi+COUNT], al
		COUNT = COUNT + 1
	ENDM
	add	edi, COUNT
	dec	_loop_count
	jne	NextPixel

	popf
	jnc	_none_to_do

one_more_pix:	
	mov	eax, edx	
	shr	ax, 10
	rol	eax, 6
	and	eax, 0ffffh
	mov	al, [ecx+eax]			
	mov	ah, bh	    	
	mov	al, _gr_fade_table[eax]
	mov	[edi], al

_none_to_do:	
	pop	edi
	pop	esi
	pop	ebp
	pop	ebx
	pop	edx
	pop	ecx
	pop	eax

	STOP_TIMER

	ret


_TEXT	ends

	end

