;  x86_64 mpn_sub_n -- Subtract two limb vectors of the same length > 0 and
;  store difference in a third limb vector.
;
;  Copyright (C) 2006  Jason Worth Martin <jason.worth.martin@gmail.com>
;
;  This program is free software; you can redistribute it and/or modify
;  it under the terms of the GNU Lesser General Public License as published
;  by the Free Software Foundation; either version 2 of the License, or
;  (at your option) any later version.
;
;  This program is distributed in the hope that it will be useful,
;  but WITHOUT ANY WARRANTY; without even the implied warranty of
;  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
;  GNU General Public License for more details.
;
;  You should have received a copy of the GNU Lesser General Public License
;  along with this program; if not, write to the Free Software Foundation,
;  Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
;
; **************************************************************************
;
;
; CREDITS
;
; This code is based largely on Pierrick Gaudry's excellent assembly
; support for the AMD64 architecture.  (Note that Intel64 and AMD64,
; while using the same instruction set, have very different
; microarchitectures.  So, this code performs very poorly on AMD64
; machines even though it is near-optimal on Intel64.)
;
; Roger Golliver works for Intel and provided insightful improvements
; particularly in using the "lea" instruction to perform additions
; and register-to-register moves.
;
; Eric Bainville has a brilliant exposition of optimizing arithmetic for
; AMD64 (http://www.bealto.it).  I adapted many of the ideas he
; describes to Intel64.
;
; Agner Fog is a demigod in the x86 world.  If you are reading assembly
; code files and you haven't heard of Agner Fog, then take a minute to
; look over his software optimization manuals (http://www.agner.org/).
; They are superb.
;
; *********************************************************************

%include 'yasm_mac.inc'

;
; If YASM supports lahf and sahf instructions, then we'll get rid
; of this.
;
%define save_CF_to_reg_a  db	0x9f
%define get_CF_from_reg_a db	0x9e


;         cycles/limb
; Hammer:     2.5 (for 1024 limbs)
; Woodcrest:  2.6 (for 1024 limbs)	

; INPUT PARAMETERS
; rp	rdi
; up	rsi
; vp	rdx
; n	rcx
	
	BITS	64
GLOBAL_FUNC mpn_sub_n
	push	rbp		; Save off callee-save registers
	push	rbx
	push	r12
	push	r13
	push	r14
	push	r15

	xor	r15,r15			; r15 will be our index, so
					; I'll call it i here after
	save_CF_to_reg_a				; Save CF

	mov	r9,rcx
	sub	r9,4			; r9 = n-(i+4)

	align	16			; aligning for loop
L_mpn_sub_n_main_loop:
	; The goal of our main unrolled loop is to keep all the
	; execution units as busy as possible.  Since
	; there are three ALUs, we try to perform three
	; adds at a time.  Of course, we will have the
	; borrow dependency, so there is at least one
	; clock cycle between each sbb.  However, we'll
	; try to keep the other execution units busy
	; with loads and stores at the same time so that
	; our net throughput is close to one sbb per clock
	; cycle.  Hopefully this function will have asymptotic
	; behavior of taking 3*n clock cycles where n is the
	; number of limbs to sub.
	;
	; Note that I'm using FOUR sbbs at a time, this is just
	; because I wanted to use up all available registers since
	; I'm hoping the out-of-order and loop-pipeline logic in
	; the Xeon will help us out.

	; See if we are still looping
	jle	L_mpn_sub_n_loop_done

	get_CF_from_reg_a			; recover CF
	
	; Load inputs into rbx and r8
	; sub with borrow, and put result in r8
	; then store r8 to output.
	mov	rbx,[rdx+r15*8]
	mov	r8,[rsi+r15*8]
	sbb	r8,rbx
	mov	[rdi+r15*8],r8
	
	; Load inputs into r9 and r10
	; sub with borrow, and put result in r10
	; then store r10 to output.
	mov	r9,[8+rdx+r15*8]
	mov	r10,[8+rsi+r15*8]
	sbb	r10,r9
	mov	[8+rdi+r15*8],r10
	
	; Load inputs into r11 and r12
	; sub with borrow, and put result in r12
	; then store r12 to output.
	mov	r11,[16+rdx+r15*8]
	mov	r12,[16+rsi+r15*8]
	sbb	r12,r11
	mov	[16+rdi+r15*8],r12

	; Load inputs into r13 and r14
	; sub with borrow, and put result in r14
	; then store r14 to output.
	mov	r13,[24+rdx+r15*8]
	mov	r14,[24+rsi+r15*8]
	sbb	r14,r13
	mov	[24+rdi+r15*8],r14

	save_CF_to_reg_a			; save CF

	mov	r10,r15
	add	r10,8
	add	r15,4		; increment by 4.
	
	mov	r9,rcx
	sub	r9,r10		; r9 = n-(i+4)
	jmp	L_mpn_sub_n_main_loop

L_mpn_sub_n_loop_done:
	mov	r15,rcx		; 
	sub	r15,r9		; r15 = n-(n-(i+4))=i+4
	sub	r15,4		; r15 = i
	cmp	r15,rcx
L_mpn_sub_n_post_loop:
	je	L_mpn_sub_n_exit
	get_CF_from_reg_a			; recover CF
	
	; Load inputs into rbx and r8
	; sub with borrow, and put result in r8
	; then store r8 to output.
	mov	rbx,[rdx+r15*8]
	mov	r8,[rsi+r15*8]
	sbb	r8,rbx
	mov	[rdi+r15*8],r8
	save_CF_to_reg_a			; save CF
	add	r15,1
	cmp	r15,rcx
	jmp	L_mpn_sub_n_post_loop
	
	
L_mpn_sub_n_exit:
	xor	rcx,rcx
	get_CF_from_reg_a			; recover the CF
	mov	rax,rcx		; Clears rax without affecting carry flag 
	adc	rax,rax		; returns carry status.
	
	pop	r15		; restore callee-save registers
	pop	r14
	pop	r13
	pop	r12
	pop	rbx
	pop	rbp
	ret
