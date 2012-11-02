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
	.list

	assume	cs:_TEXT, ds:_DATA

_DATA	segment	dword public USE32 'DATA'

rcsid	db	"$Id: selector.asm 1.5 1995/12/21 14:30:01 matt Exp $"
	align	4

	new_selector	dd 	?
	new_base	dd	?
	new_size	dd	?

_DATA	ends



_TEXT	segment	dword public USE32 'CODE'

; int get_selector( void * address, int size, unsigned short * selector );

PUBLIC 	get_selector_

	; EAX = 32-bit base address
	; EDX = Size in bytes
	; EBX = &selector

	; Returns:  EAX = 0 if ok, otherwise, EAX = non-zero (1)

get_selector_:

	pusha

	mov	new_base, eax
	mov	new_size, edx
	mov	new_selector, ebx
	
	mov	eax, 0
	mov	ecx, 1
	int	31h
	jc	failed
	mov	ecx, new_selector
	mov	[ecx], ax

	; Set the selector's base address using DPMI
	mov	ecx, new_selector
	mov	bx, [ecx]
	mov	eax, 7
	mov	ecx, new_base
	mov	edx, ecx
	shr	ecx, 16
	int	31h
	jc 	failed

	; Set the selector's size using DPMI
	mov	ecx, new_selector
	mov	bx, [ecx]
	mov	ax, 08h
	mov	ecx, new_size
	mov	edx, ecx
	shr	ecx, 16
	int	31h
	jc	failed

	popa
	mov	eax, 0
	ret
failed:
	popa
	mov	eax, 1
	ret

_TEXT	ends

	end

