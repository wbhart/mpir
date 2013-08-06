; PROLOGUE(mpn_rsh_divrem_hensel_qr_1_2)

;  Copyright 2009 Jason Moxham
;
;  Windows Conversion Copyright 2008 Brian Gladman
;
;  This file is part of the MPIR Library.
;
;  The MPIR Library is free software; you can redistribute it and/or modify
;  it under the terms of the GNU Lesser General Public License as published
;  by the Free Software Foundation; either version 2.1 of the License, or (at
;  your option) any later version.
;
;  The MPIR Library is distributed in the hope that it will be useful, but
;  WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
;  or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public
;  License for more details.
;
;  You should have received a copy of the GNU Lesser General Public License
;  along with the MPIR Library; see the file COPYING.LIB.  If not, write
;  to the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
;  Boston, MA 02110-1301, USA.
;
;  mp_limb_t  mpn_rsh_divrem_hensel_qr_1_2(mp_ptr, mp_ptr, mp_size_t, mp_limb_t, mp_uint, mp_limb_t)
;  rax                                        rdi     rsi        rdx        rcx       r8         r9
;  rax                                        rcx     rdx         r8         r9 [rsp+40]   [rsp+48]

%include "yasm_mac.inc"

    CPU  Athlon64
    BITS 64

%define reg_save_list rsi, rdi, r12, r13, r14

	FRAME_PROC mpn_rsh_divrem_hensel_qr_1_2, 0, reg_save_list
	mov     rax, r8
	lea     rdi, [rcx+rax*8-16]
	lea     rsi, [rdx+rax*8-16]
    mov     rcx, r9
    mov     rdx, r9
	mov     r9, 2
	sub     r9, rax    
    movsxd  r8, dword [rsp+stack_use+40]
    mov     r10, qword [rsp+stack_use+48]

	mov     rax, 64
	sub     rax, r8
	movq    mm0, r8
	movq    mm1, rax

	mov     rax, rdx
	imul    edx, ecx
	mov     r11, 2
	sub     r11, rdx
	imul    r11d, eax

	mov     rax, r11
	imul    r11d, ecx
	mov     rdx, 2
	sub     rdx, r11
	imul    edx, eax

	mov     rax, rdx
	imul    edx, ecx
	mov     r11, 2
	sub     r11, rdx
	imul    r11d, eax

	mov     rax, r11
	imul    r11d, ecx
	mov     rdx, 2
	sub     rdx, r11
	imul    edx, eax

	mov     rax, rdx
	imul    rdx, rcx
	mov     r11, 2
	sub     r11, rdx
	imul    r11, rax

	mov     rax, r11
	mov     r12, r11
	mul     rcx
	neg     rdx
	imul    r12, rdx

; // for the first limb we can not store (as we have to shift) so we need to
; // do first limb separately , we could do it as normal as an extention of
; // the loop , but if we do it as a 1 limb inverse then we can start it
; // eailer , ie interleave it with the calculation of the 2limb inverse

	mov     r13, r11
	mov     r14, r12

	mov     r11, [rsi+r9*8]
	sub     r11, r10
	sbb     r10, r10

	imul    r11, r13
	movq    mm2, r11
	psrlq   mm2, mm0
	mov     rax, rcx
	mul     r11
	mov     r11, [rsi+r9*8+8]
	mov     r12, [rsi+r9*8+16]
	add     r10, r10
	sbb     r11, rdx
	sbb     r12, 0
	sbb     r10, r10

; mov $0,%r10
	add     r9, 2
	jc      .2
	
	xalign  16
.1:	mov     r8, r12
	mov     rax, r13
	mul     r11

	; mov %rax,-16(%rdi,%r9,8)	#// store low quotient
	movq    mm3, rax
	movq    mm4, mm3
	psllq   mm3, mm1
	psrlq   mm4, mm0
	por     mm2, mm3
	movq    [rdi+r9*8-16], mm2

	imul    r11, r14
	imul    r12, r13
	add     rdx, r11
	add     rdx, r12
	mov     r11, [rsi+r9*8+8]
	mov     r12, [rsi+r9*8+16]

	; mov %rdx,-8(%rdi,%r9,8)	#// store high quotient
	movq    mm3, rdx
	movq    mm2, mm3
	psllq   mm3, mm1
	psrlq   mm2, mm0
	por     mm4, mm3
	movq    [rdi+r9*8-8], mm4

	mov     rax, rcx
	mul     rdx
	add     r10, r10
	sbb     r11, 0
	sbb     r12, 0
	sbb     r10, r10
	cmp     r8, rax
	sbb     r11, rdx
	sbb     r12, 0
	sbb     r10, 0
	add     r9, 2
	jnc     .1
.2:	mov     r8, r12
	mov     rax, r13
	mul     r11

; mov %rax,-16(%rdi,%r9,8)	#// store low quotient
	movq    mm3, rax
	movq    mm4, mm3
	psllq   mm3, mm1
	psrlq   mm4, mm0
	por     mm2, mm3
	movq    [rdi+r9*8-16], mm2

	imul    r11, r14
	imul    r12, r13
	add     rdx, r11
	add     rdx, r12
	cmp     r9, 0
	jne     .4
.3:	mov     r11, [rsi+r9*8+8]
	; mov %rdx,-8(%rdi,%r9,8)	#// store high quotient
	movq    mm3, rdx
	movq    mm2, mm3
	psllq   mm3, mm1
	psrlq   mm2, mm0
	por     mm4, mm3
	movq    [rdi+r9*8-8], mm4

	mov     rax, rcx
	mul     rdx
	add     r10, r10
	sbb     r11, 0
	sbb     r10, r10
	cmp     r8, rax
	sbb     r11, rdx
	sbb     r10, 0
	mov     rax, r11
	imul    rax, r13
	; mov %rax,(%rdi,%r9,8)
	movq    mm3, rax
	movq    mm4, mm3
	psllq   mm3, mm1
	psrlq   mm4, mm0
	por     mm2, mm3
	movq    [rdi+r9*8], mm2
	movq    [rdi+r9*8+8], mm4

	mul     rcx
	add     r10, r10
	mov     rax, 0
	adc     rax, rdx
	emms
	EXIT_PROC reg_save_list

	; mov %rdx,-8(%rdi,%r9,8)	#// store high quotient
.4:	movq    mm3, rdx
	movq    mm2, mm3
	psllq   mm3, mm1
	psrlq   mm2, mm0
	por     mm4, mm3
	movq    [rdi+r9*8-8], mm4
	movq    [rdi+r9*8], mm2

	mov     rax, rcx
	mul     rdx
	cmp     r8, rax
	mov     rax, 0
	adc     rax, rdx
	sub     rax, r10
.5:	emms
	END_PROC reg_save_list

    end
