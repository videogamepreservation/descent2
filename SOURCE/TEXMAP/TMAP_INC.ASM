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
	include	psmacros.inc

	extb	_gr_fade_table
	extd	_write_buffer
	extd	_window_left,_window_right,_window_top,_window_bottom,_window_width,_window_height
	extd	_bytes_per_row,_y_pointers
	extd	_Lighting_on,_transparency_on,_per2_flag
	extd	_tmap_flat_cthru_table
	extb	_tmap_flat_color,_tmap_flat_shade_value
	extd	_dither_intensity_lighting
	extw	_pixel_data_selector
	extd	_fx_y,_fx_u,_fx_v,_fx_z,_fx_l
	extd	_fx_du_dx,_fx_dv_dx,_fx_dz_dx,_fx_dl_dx
	extd	_fx_xleft,_fx_xright
	extd	_fx_u_right, _fx_v_right, _fx_z_right
	extd	_pixptr
	extd	_loop_count
	extd	_dest_row_data
	extd	_tmap_pln_tmp, _tmap_pln_timer, _tmap_pln_counter;
	extd	U0, U1, V0, V1, DU1, DV1, DZ1, num_left_over

_lighting_tables	equ	_gr_fade_table
write_buffer	equ	_write_buffer

max_window_width	equ	320
num_iters	=	max_window_width

  if num_iters and 1
num_iters = num_iters + 1
  endif

TIMER_CODE = 0

IF TIMER_CODE

START_TIMER MACRO
; Returns EDX:EAX as system clocks
	push	edx
	push	eax
	cli
	db	0fh 
	db	31h	; RDTSC
	mov	_tmap_pln_tmp, eax
;	mov	eax, _loop_count
;	add	_tmap_pln_counter, eax
	pop	eax
	pop	edx
ENDM

STOP_TIMER MACRO
; Returns EDX:EAX as system clocks
	push	edx
	push	eax
	db	0fh 
	db	31h	; RDTSC
	sti
	sub	eax, _tmap_pln_tmp
	js	@f
	inc	_tmap_pln_counter
	add	_tmap_pln_timer, eax
@@:
	pop	eax
	pop	edx
ENDM
ELSE
START_TIMER MACRO
ENDM

STOP_TIMER MACRO
ENDM

ENDIF


NBITS = 4	; 2^NBITS pixels plotted per divide

DIVIDE_SIG_BITS	  	= 12
Z_SHIFTER 		= (30-DIVIDE_SIG_BITS)
DIVIDE_TABLE_SIZE 	= (1 SHL DIVIDE_SIG_BITS)

extrn	_divide_table:dword

PDIV MACRO
	imul	ecx 	; eax = (v*z)
	shrd	eax,edx,16
;USE_DIV	mov	edx,eax
;USE_DIV	sar	edx,16
;USE_DIV	shl	eax,16
;USE_DIV	idiv	ecx	; eax = (v/z)
ENDM

INVERT_ECX	MACRO

;	shr	ecx, Z_SHIFTER		; ECX = 18 bits precision
;	mov	eax, 65536
;	mov	edx, eax
;	sar	edx,16
;	shl	eax,16
;	idiv	ecx
;	mov	ecx, eax

	shr	ecx, Z_SHIFTER
;	cmp	ecx, DIVIDE_TABLE_SIZE-1
;	jl	@f
;	mov	ecx, DIVIDE_TABLE_SIZE-1
	and	ecx, DIVIDE_TABLE_SIZE-1
	mov	ecx, dword ptr _divide_table[ecx*4];
ENDM
