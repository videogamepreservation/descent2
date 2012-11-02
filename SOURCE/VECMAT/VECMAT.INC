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
ifndef _VECMAT_INC
_VECMAT_INC equ 1

	include	fix.inc

;Structures

vms_vector	struct
	union
	 struct
x	  fix	?
y	  fix	?
z	  fix	?
	 ends
xyz	 fix	3 dup (?)
	ends
vms_vector	ends


vms_svec	struct
	union
	 struct
sv_x	  dw	?
sv_y	  dw	?
sv_z	  dw	?
	 ends
sv_xyz	 dw	3 dup (?)
	ends
vms_svec	ends

vms_angvec	struct
	union
	struct
pitch	 fixang	?
bank	 fixang	?
head	 fixang	?
	ends
	struct
;p	 fixang	?
;b	 fixang	?
;h	 fixang	?
	ends
	ends
vms_angvec	ends

vms_matrix	struct
	union
	 struct
m1	  fix	?
m4	  fix	?
m7	  fix	?
m2	  fix	?
m5	  fix	?
m8	  fix	?
m3	  fix	?
m6	  fix	?
m9	  fix	?
	 ends
	 struct
rvec	  fix	?,?,?
uvec	  fix	?,?,?
fvec	  fix	?,?,?
	 ends
;;mm	 fix	9 dup (?)
	ends
vms_matrix	ends


;Macros

;copies one vector to another, using the register specified.  If none
;specified, uses eax
vm_copy	macro	dest,src,reg:=<eax>
	for	ofs,<x,y,z>
	 mov	reg,[src].ofs
	 mov	[dest].ofs,reg
	endm
	endm

;copies one angvec to another, using the register specified.  If none
;specified, uses eax (and ax). Note the trick to get the word part of
;the register without knowing what the register is.
vm_acopy	macro	dest,src,reg:=<eax>
	mov	reg,fix ptr [src].p	;copy two at once
	mov	fix ptr [dest].p,reg
	db	66h	;size override, use short
	mov	reg,fix ptr [src].h ;copy last angle
	db	66h	;size override, use short
	mov	fix ptr [dest].h,reg
	endm

;Global contants
	extdef	vms_vector,_vmd_zero_vector
	extdef	vms_matrix,_vmd_identity_matrix

;Routines

;register usage appears here, but see VECMAT.H for other info

	extn	vm_vec_add	;eax=dest, esi,edi=srcs
	extn	vm_vec_sub	;eax=dest, esi,edi=srcs
	extn	vm_vec_add2	;edi=dest, esi=source
	extn	vm_vec_sub2	;edi=dest, esi=source
	extn	vm_vec_avg	;eax=dest, esi,edi=srcs
	extn	vm_vec_avg4	;eax=dest, esi,edi,ecx,edx=srcs
	extn	vm_vec_scale	;ebx=vec, ecx=scale
	extn	vm_vec_copy_scale	;edi=dest, ebx=src, ecx=scale
	extn	vm_vec_scale2	;edi=vec, ebx=n,edx=d
	extn	vm_vec_mag	;esi=vec, returns eax=mag
	extn	vm_vec_dist	;esi,edi=vecs, returns eax=dist
	extn	vm_vec_mag_quick	;esi=vec, returns eax=approx dist
	extn	vm_vec_dist_quick	;esi,edi=vecs, returns eax=approx dist
	extn	vm_vec_normalize	;esi=vec, returns ecx=mag
	extn	vm_vec_normalize_quick	;esi=vec, returns ecx=mag
	extn	vm_vec_copy_normalize	;edi=dest, esi=src
	extn	vm_vec_copy_normalize_quick	;edi=dest, esi=src
	extn	vm_vec_normalized_dir	;edi=dest, esi=endpoint, ebx=startpoint
	extn	vm_vec_normalized_dir_quick	;edi=dest, esi=endpoint, ebx=startpoint
	extn	vm_vec_dotprod	;esi,edi=vecs, ret eax=dotprod
	extn	vm_vec_crossprod	;eax=dest, esi,edi=srcs
	extn	vm_vec_normal	;ebx=dest, eax,esi,edi=srcs
	extn	vm_vec_perp	;ebx=dest, eax,esi,edi=srcs
	extn	vm_vec_delta_ang	;esi,edi=vecs, eax=(optional) fvec, ret ax=angle
	extn	vm_vec_delta_ang_norm ;esi,edi=vec, ret ax=angle
	extn	vm_angles_2_matrix	;edi=dest, esi=angvec
	extn	vm_vector_2_matrix	;edi=dest, esi=fwdvec, eax=upvec, ebx=rightvec
	extn	vm_vector_2_matrix_norm	;edi=dest, esi=fwdvec, eax=upvec, ebx=rightvec
	extn	vm_vec_rotate	;eax=dest, esi=src, edi=matrix
	extn	vm_transpose_matrix	;edi=matrix (transpose in place)
	extn	vm_copy_transpose_matrix ;edi=dest, esi=src
	extn	vm_matrix_x_matrix	;eax=dest, esi,edi=srcs
	extn	vm_vec_ang_2_matrix	;esi=vector, eax=angle, edi=matrix
	extn	vm_dist_to_plane	;ebx=norm, edi=plane pnt, esi=check pnt, ret eax=dist
	extn	vm_extract_angles_matrix	;edi=angles, esi=matrix
	extn	vm_vec_scale_add	;edi=dest, ebx=src1, esi=src2, ecx=scale
	extn	vm_vec_scale_add2	;edi=dest, esi=src, ecx=scale
	extn	vm_extract_angles_vector	;edi=angvec, esi=vec TRASHES ESI
	extn	vm_extract_angles_vector_normalized	;edi=angvec, esi=vec

endif

