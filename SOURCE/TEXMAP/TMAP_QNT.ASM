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

rcsid	db	"$Id: tmap_qnt.asm 1.1 1996/02/01 14:52:01 matt Exp $"
	align	4

_DATA	ends


_TEXT	segment	para public USE32 'CODE'

	align	16

public asm_tmap_scanline_qnt_
asm_tmap_scanline_qnt_:

	NBITS = 5		; Divide every 32 pixels

	START_TIMER

	push	eax
	push	ecx
	push	edx
	push	ebx
	push	ebp
	push	esi
	push	edi
	
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

	INVERT_ECX

; compute initial v coordinate
	mov	eax,ebp	; get v
	PDIV	
	mov	V0, eax

; compute initial u coordinate
	mov	eax,ebx	; get u
	PDIV	
	mov	U0, eax

	mov	ecx,_fx_z

; find number of subdivisions
	mov	eax, _loop_count
	inc	eax
	mov	esi, eax
	and	esi, (1 shl NBITS) - 1
	sar	eax, NBITS
	mov	num_left_over, esi
	jz	DoEndPixels 	;there are no 2^NBITS chunks, do divide/pixel for whole scanline
	mov	_loop_count, eax

; Set deltas to NPIXS pixel increments
	mov	eax, _fx_du_dx
	shl	eax, NBITS
	mov	DU1, eax
	mov	eax, _fx_dv_dx
	shl	eax, NBITS
	mov	DV1, eax
	mov	eax, _fx_dz_dx
	shl	eax, NBITS
	mov	DZ1, eax

	align	4
TopOfLoop4:
	add	ebx, DU1
	add	ebp, DV1
	add	ecx, DZ1

; Done with ebx, ebp, ecx until next iteration
	push	ebx
	push	ecx
	push	ebp
	push	edi
	
	INVERT_ECX

; Find fixed U1		
	mov	eax, ebx
	PDIV	
	mov	ebx, eax	; ebx = U1 until pop's

; Find fixed V1		
	mov	eax, ebp
	PDIV	
	mov	ebp, eax	; ebx = V1 until pop's

; Get last correct U,Vs
	mov	ecx, U0	; ecx = U0 until pop's
	mov	edi, V0	; edi = V0 until pop's

; Make ESI =  V0:U0 in 6:10,6:10 format
	mov	eax, edi
	shr	eax, 6
	mov	esi, ecx
	shl	esi, 10
	mov	si, ax
		
; Make EDX = DV:DU in 6:10,6:10 format
	mov	eax, ebp
	sub	eax, edi
	sar	eax, NBITS+6
	mov	edx, ebx
	sub	edx, ecx
	shl	edx, 10-NBITS	; EDX = V1-V0/ 4 in 6:10 int:frac
	mov	dx, ax	; put delta u in low word

; Save the U1 and V1 so we don't have to divide on the next iteration
	mov	U0, ebx
	mov	V0, ebp

	pop	edi	; Restore EDI before using it
		
; LIGHTING CODE
	mov	ecx, _pixptr

	COUNT = 0
	REPT (1 SHL NBITS)
		mov	eax, esi	; get u,v
		shr	ax, 10		; shift out frac(v)
		rol	eax, 6		; shift int(u) into low eax
		and	eax, 0ffffh	; clear upper bits of eax
		add	esi, edx	; inc u,v
		mov 	al, [eax+ecx]	; get pixel from source bitmap
		cmp	al, 255
		je	@f
		mov 	[edi+COUNT],al	; Draw 4 pixels to display
@@:
		COUNT = COUNT + 1
	ENDM
	add	edi,COUNT

	pop	ebp
	pop	ecx
	pop	ebx
	dec	_loop_count
	jnz	TopOfLoop4

EndOfLoop4:

	test	num_left_over, -1
	je	_none_to_do

	cmp	num_left_over, 4
	ja	DoEndPixels

	; If less than 4, then just keep interpolating without
	; calculating a new DU:DV.
	mov	ecx, _pixptr
	jmp	FinishOff

; ----------------------------------------- Start of LeftOver Pixels ------------------------------------------
DoEndPixels:

	push	edi
	mov	ecx, _fx_z_right

	INVERT_ECX

; Find fixed U1		
	mov	eax, _fx_u_right
	PDIV	
	mov	ebx, eax	; ebx = U1 until pop's

; Find fixed V1		
	mov	eax, _fx_v_right
	PDIV	
	mov	ebp, eax	; ebp = V1 until pop's

	mov	ecx, U0	; ecx = U0 until pop's
	mov	edi, V0	; edi = V0 until pop's

; Make EDX = DV:DU in 6:10,6:10 format
	mov	eax, ebx
	sub	eax, ecx
	mov	edx, eax	; These two lines are faster than cdq
	sar	edx, 31		
	idiv	num_left_over	; eax = (v1-v0)/num_left_over
	shl	eax, 16-6	; go from 16.16 to 6.10, and move into high 16 bits
	mov	esi, eax	; esi = dvdx<<16

	mov	eax, ebp
	sub	eax, edi
	mov	edx, eax	; These two lines are faster than cdq
	sar	edx, 31		
	idiv	num_left_over	; eax = (u1-u0)/num_left_over
	sar	eax, 6		; go from 16.16 to 6.10 (ax=dvdx in 6.10)
	mov	si, ax		; esi = dvdx:dudx
	mov	edx, esi

; Make ESI =  V0:U0 in 6:10,6:10 format
	mov	eax, edi
	shr	eax, 6
	mov	esi, ecx
	shl	esi, 10
	mov	si, ax
	
	pop	edi	; Restore EDI before using it
		
; LIGHTING CODE
	mov	ecx, _pixptr

FinishOff:
	COUNT = 0
	REPT (1 SHL NBITS)
		mov	eax, esi	; get u,v
		shr	ax, 10		; shift out frac(v)
		rol	eax, 6		; shift int(u) into low eax
		and	eax, 0ffffh	; clear upper bits of eax
		add	esi, edx	; inc u,v
		mov 	al, [eax+ecx]	; get pixel from source bitmap
		cmp	al, 255
		je	@f
		mov 	[edi+COUNT],al	; Draw 4 pixels to display
@@:		dec	num_left_over
		COUNT = COUNT + 1
		jz	_none_to_do
	ENDM

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






