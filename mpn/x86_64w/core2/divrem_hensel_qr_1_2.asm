; PROLOGUE(mpn_divrem_hensel_qr_1_2)

;  Copyright 2008 Jason Moxham
;
;  This file is part of the MPIR Library.
;
;  Windows Conversion Copyright 2008 Brian Gladman
;
;  The MPIR Library is free software; you can redistribute it and/or modify
;  it under the terms of the GNU Lesser General Public License as published
;  by the Free Software Foundation; either version 2.1 of the License, or (at
;  your option) any later version.
;  The MPIR Library is distributed in the hope that it will be useful, but
;  WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
;  or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public
;  License for more details.
;  You should have received a copy of the GNU Lesser General Public License
;  along with the MPIR Library; see the file COPYING.LIB.  If not, write
;  to the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
;  Boston, MA 02110-1301, USA.
;
;  mp_limb_t mpn_divrem_hensel_qr_1_2(mp_ptr, mp_ptr, mp_size_t, mp_limb_t)
;  rax                                   rdi     rsi        rdx        rcx
;  rax                                   rcx     rdx         r8         r9
	
%include "yasm_mac.inc"

%define reg_save_list rsi, rdi, r12, r13, r14

    CPU  Athlon64
    BITS 64

    FRAME_PROC mpn_divrem_hensel_qr_1_2, 0, reg_save_list
    mov     rax, r8
	lea     rdi, [rcx+rax*8-8]
	lea     rsi, [rdx+rax*8-8]
    mov     rcx, r9
	mov     rdx, r9
	mov     r9, 1
	sub     r9, rax

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

	mov     r13, r11
	mov     r14, r12

	mov     r11, [rsi+r9*8]
	mov     r12, [rsi+r9*8+8]
	mov     r10, 0
	add     r9, 2
	jc      .2

	xalign  16
.1:	mov     r8, r12
	mov     rax, r13
	mul     r11
	mov     [rdi+r9*8-16], rax
	imul    r11, r14
	imul    r12, r13
	add     rdx, r11
	add     rdx, r12
	mov     r11, [rsi+r9*8]
	mov     r12, [rsi+r9*8+8]
	mov     [rdi+r9*8-8], rdx
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
	mov     [rdi+r9*8-16], rax
	imul    r11, r14
	imul    r12, r13
	add     rdx, r11
	add     rdx, r12
	cmp     r9, 0
	jne     .4
.3:	mov     r11, [rsi+r9*8]
	mov     [rdi+r9*8-8], rdx
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
	mov     [rdi+r9*8], rax
	mul     rcx
	add     r10, r10
	mov     rax, 0
	adc     rax, rdx
    EXIT_PROC reg_save_list

.4:	mov     [rdi+r9*8-8], rdx
	mov     rax, rcx
	mul     rdx
	cmp     r8, rax
	mov     rax, 0
	adc     rax, rdx
	sub     rax, r10
.5:	END_PROC reg_save_list
    
    end
