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

;* @ArgI - Macro function returns an argument specified by number
;* from a VARARG list.
;*
;* Shows:   Directives - FOR       LOCAL    EXITM
;*                       TEXTEQU   =
;*          Operator   - EQ
;*
;* Params:  index - one-based number of the argument to be returned
;*          arglist - argument list
@ArgI MACRO index:REQ, arglist:VARARG
    LOCAL count, retstr
    count = 0
    FOR arg, <arglist>
        count = count + 1
        IF count EQ index
            retstr TEXTEQU <arg>
        ENDIF
    ENDM
    EXITM retstr
ENDM

;mulitply 2 matrices, fill in dest.
;takes ebp=dest, esi=src0, edi=scr1

;vm_matrix_x_matrix:

	for	s0,<<m1,m2,m3>,<m4,m5,m6>,<m7,m8,m9>>
	 for	s1,<<m1,m4,m7>,<m2,m5,m8>,<m3,m6,m9>>
	  for	e,<1,2,3>

	   mov	eax,[esi].@ArgI(e,s0)
	   imul	[edi].@ArgI(e,s1)

	   if	e EQ 1
	    mov	ebx,eax
	    mov	ecx,edx
	   else
	    add	ebx,eax
	    adc	ecx,edx
	   endif

	  endm

	  shrd	ebx,ecx,16	;fixup ebx
	  mov	[ebp].(@ArgI(1,s0)+@ArgI(1,s1)),ebx

	 endm
	endm
