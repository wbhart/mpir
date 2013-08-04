; PROLOGUE(mpn_divrem_euclidean_qr_2)

;  Copyright 2008 Jason Moxham
;
;  Windows Conversion Copyright 2008 Brian Gladman
;
;  This file is part of the MPIR Library.
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
;  mp_limb_t mpn_divrem_euclidean_qr_2(mp_ptr, mp_ptr, mp_size_t, mp_limb_t)
;  rax                                    rdi     rsi        rdx        rcx
;  rax                                    rcx     rdx         r8         r9

%include "yasm_mac.inc"

%define reg_save_list rbx, rsi, rdi, rbp, r12, r13, r14, r15

    BITS 64

	FRAME_PROC mpn_divrem_euclidean_qr_2, 0, reg_save_list
	mov     rdi, rcx
	mov     rsi, rdx
	mov     rdx, r8
    mov     rcx, r9

	mov     rbx, rdx
	dec     rbx
	mov     r9, 0
	mov     rdx, [rcx+8]
	not     rdx
	mov     rax, -1
	div     qword[rcx+8]
	mov     rbp, rax
	mov     r8, [rsi+rbx*8]
	dec     rbx
	mov     r10, [rsi+rbx*8]
	cmp     r8, [rcx+8]
	jae     .1
	mov     rdx, r8
	mov     r9, r8
	mov     r10, [rsi+rbx*8]
	mov     r8, r10
	mov     r14, 0
	jmp     .3
.1: mov     r14, 1
	mov     rdx, r8
	sub     rdx, [rcx+8]
	sub     r10, [rcx]
	sbb     rdx, 0
	jnc     .2
	dec     r14
	add     r10, [rcx]
	adc     rdx, [rcx+8]
.2: mov     r9, rdx
	mov     r8, r10
.3: dec     rbx
	mov     rax, r9
	js      .8

	xalign  16
.4: cmp     rax, [rcx+8]
	jne     .6
	mov     r10, [rsi+rbx*8]
	mov     r15, -1
	mov     rdx, r8
	add     r10, [rcx]
	adc     rdx, rax
	sbb     rax, rax
	sub     rdx, [rcx]
	adc     rax, 0
	jz      .5
	dec     r15
	add     r10, [rcx]
	adc     rdx, [rcx+8]
.5: mov     rax, rdx
	mov     r8, r10
	jmp     .7
.6:	mov     r9, rax
	bt      r8, 63
	adc     rax, 0
	mov     r15, r8
	mov     r13, 0
	sar     r15, 63
	and     r15, [rcx+8]
	add     r15, r8
	mul     rbp
	add     rax, r15
	mov     rax, [rcx]
	adc     rdx, r9
	mov     r10, rdx
	mul     rdx
	mov     r12, [rcx]
	add     r12, rax
	adc     r13, rdx
	mov     rax, [rcx+8]
	not     r10
	mul     r10
	sub     r9, [rcx+8]
	add     rax, r8
	adc     rdx, r9
	mov     r9, rdx
	and     r9, [rcx+8]
	; swapped r9,rax from here
	add     rax, r9
	mov     r15, rdx
	sub     r15, r10
	and     rdx, [rcx]
	sub     r12, rdx
	sbb     r13, 0
	mov     r8, [rsi+rbx*8]
	sub     r8, r12
	sbb     rax, r13
	mov     r10, [rcx]
	mov     r11, [rcx+8]
	sbb     r9, r9
	and     r10, r9
	and     r11, r9
	add     r15, r9
	add     r8, r10
	adc     rax, r11
	adc     r9, 0
	and     r10, r9
	and     r11, r9
	add     r15, r9
	add     r8, r10
	adc     rax, r11
.7: mov     [rdi+rbx*8], r15
	dec     rbx
	jns     .4
.8: mov     [rsi+8], rax
	mov     [rsi], r8
	mov     rax, r14
    END_PROC reg_save_list
	
	end
