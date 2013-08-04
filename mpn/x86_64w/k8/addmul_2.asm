; PROLOGUE(mpn_addmul_2)

;  Copyright 2008 Jason Moxham
;
;  Windows Conversion Copyright 2008 Brian Gladman
;
;  This file is part of the MPIR Library.
;  The MPIR Library is free software; you can redistribute it and/or modify
;  it under the terms of the GNU Lesser General Public License as published
;  by the Free Software Foundation; either version 2.2 of the License, or (at
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
;  mp_limb_t mpn_addmul_2(mp_ptr, mp_ptr, mp_size_t, mp_ptr)
;  rax                       rdi     rsi        rdx     rcx
;  rax                       rcx     rdx         r8      r9

%include "yasm_mac.inc"

    CPU  Athlon64
    BITS 64

%define reg_save_list rbx, rsi, rdi, r12

	FRAME_PROC mpn_addmul_2, 0, reg_save_list
	mov     rdi, rcx
	mov     rsi, rdx
	mov     rax, r8
	
	mov     rcx, [r9]
	mov     r8, [r9+8]
	mov     rbx, 4
	sub     rbx, rax
	lea     rsi, [rsi+rax*8-32]
	lea     rdi, [rdi+rax*8-32]
	mov     r10, 0
	mov     rax, [rsi+rbx*8]
	mul     rcx
	mov     r12, rax
	mov     r9, rdx
	cmp     rbx, 0
	jge     .2
	
	xalign  16
.1:	mov     rax, [rsi+rbx*8]
	mul     r8
	add     r9, rax
	mov     rax, [rsi+rbx*8+8]
	adc     r10, rdx
	mov     r11, 0
	mul     rcx
	add     [rdi+rbx*8], r12
	adc     r9, rax
	mov     r12, 0
	adc     r10, rdx
	mov     rax, [rsi+rbx*8+8]
	adc     r11, 0
	mul     r8
	add     [rdi+rbx*8+8], r9
	adc     r10, rax
	adc     r11, rdx
	mov     rax, [rsi+rbx*8+16]
	mul     rcx
	add     r10, rax
	mov     rax, [rsi+rbx*8+16]
	adc     r11, rdx
	adc     r12, 0
	mul     r8
	add     [rdi+rbx*8+16], r10
	mov     r9, 0
	adc     r11, rax
	mov     r10, 0
	mov     rax, [rsi+rbx*8+24]
	adc     r12, rdx
	mov     r15, r15
	mul     rcx
	add     r11, rax
	mov     rax, [rsi+rbx*8+24]
	adc     r12, rdx
	adc     r9, 0
	mul     r8
	add     [rdi+rbx*8+24], r11
	adc     r12, rax
	adc     r9, rdx
	mov     rax, [rsi+rbx*8+32]
	mul     rcx
	add     r12, rax
	adc     r9, rdx
	adc     r10, 0
	add     rbx, 4
	jnc     .1
.2:	mov     rax, [rsi+rbx*8]
	mul     r8
	cmp     rbx, 2
	ja      .6
	jz      .5
	jp      .4
.3: add     r9, rax
	mov     rax, [rsi+rbx*8+8]
	adc     r10, rdx
	mov     r11, 0
	mul     rcx
	add     [rdi+rbx*8], r12
	adc     r9, rax
	mov     r12, 0
	adc     r10, rdx
	mov     rax, [rsi+rbx*8+8]
	adc     r11, 0
	mul     r8
	add     [rdi+rbx*8+8], r9
	adc     r10, rax
	adc     r11, rdx
	mov     rax, [rsi+rbx*8+16]
	mul     rcx
	add     r10, rax
	mov     rax, [rsi+rbx*8+16]
	adc     r11, rdx
	adc     r12, 0
	mul     r8
	add     [rdi+rbx*8+16], r10
	mov     r9, 0
	adc     r11, rax
	; padding
	mov     r10, 0
	mov     rax, [rsi+rbx*8+24]
	adc     r12, rdx
	; padding
	mov     r15, r15
	mul     rcx
	add     r11, rax
	mov     rax, [rsi+rbx*8+24]
	adc     r12, rdx
	adc     r9, 0
	mul     r8
	add     [rdi+rbx*8+24], r11
	adc     r12, rax
	adc     r9, rdx
	mov     [rdi+rbx*8+32], r12
	mov     rax, r9
    EXIT_PROC reg_save_list

	xalign  16
.4: add     r9, rax
	mov     rax, [rsi+rbx*8+8]
	adc     r10, rdx
	mov     r11, 0
	mul     rcx
	add     [rdi+rbx*8], r12
	adc     r9, rax
	mov     r12, 0
	adc     r10, rdx
	mov     rax, [rsi+rbx*8+8]
	adc     r11, 0
	mul     r8
	add     [rdi+rbx*8+8], r9
	adc     r10, rax
	adc     r11, rdx
	mov     rax, [rsi+rbx*8+16]
	mul     rcx
	add     r10, rax
	mov     rax, [rsi+rbx*8+16]
	adc     r11, rdx
	adc     r12, 0
	mul     r8
	add     [rdi+rbx*8+16], r10
	adc     r11, rax
	adc     r12, rdx
	mov     [rdi+rbx*8+24], r11
	mov     rax, r12
    EXIT_PROC reg_save_list
	
	xalign  16
.5: add     r9, rax
	mov     rax, [rsi+rbx*8+8]
	adc     r10, rdx
	mov     r11, 0
	mul     rcx
	add     [rdi+rbx*8], r12
	adc     r9, rax
	mov     r12, 0
	adc     r10, rdx
	mov     rax, [rsi+rbx*8+8]
	adc     r11, 0
	mul     r8
	add     [rdi+rbx*8+8], r9
	adc     r10, rax
	adc     r11, rdx
	mov     [rdi+rbx*8+16], r10
	mov     rax, r11
    EXIT_PROC reg_save_list

	xalign  16
.6: add     [rdi+rbx*8], r12
	adc     r9, rax
	adc     r10, rdx
	mov     [rdi+rbx*8+8], r9
	mov     rax, r10
.7:	END_PROC reg_save_list

    end
