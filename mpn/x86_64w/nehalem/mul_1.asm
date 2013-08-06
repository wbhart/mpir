; PROLOGUE(mpn_mul_1)
;  Copyright 2010 Jason Moxham
;
;  Windows Conversion Copyright 2010 Brian Gladman
;
;  This file is part of the MPIR Library.
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
;  mp_limb_t  mpn_mul_1(mp_ptr, mp_ptr, mp_size_t, mp_limb_t)
;  mp_limb_t mpn_mul_1c(mp_ptr, mp_ptr, mp_size_t, mp_limb_t, mp_limb_t)
;  rax                     rdi     rsi        rdx        rcx         r8
;  rax                     rcx     rdx         r8         r9   [rsp+40]

%include "yasm_mac.inc"

    CPU  nehalem
    BITS 64

%define reg_save_list   rsi, rdi

    LEAF_PROC mpn_mul_1c
    mov     r11, [rsp+0x28]
    jmp     start

    LEAF_PROC mpn_mul_1
    xor     r11, r11

    xalign  16
start:
    FRAME_PROC ?mpn_nehalem_mul, 0, reg_save_list
    mov     rax, r8
	mov     r8, 3
	lea     rdi, [rcx+rax*8-24]
	lea     rsi, [rdx+rax*8-24]
    mov     rcx, r9
	sub     r8, rax
	mov     r9d, 0
	mov     rax, [rsi+r8*8+24-24]
	jnc     .1

	xalign  16
.0:	mul     rcx
	add     r11, rax
	mov     [rdi+r8*8], r11
	mov     r10d, 0
	mov     rax, [rsi+r8*8+8]
	adc     r10, rdx
	mov     r11d, 0
	mul     rcx
	add     r10, rax
	mov     r9d, 0
	mov     rax, [rsi+r8*8+16]
	mov     [rdi+r8*8+8], r10
	adc     r9, rdx
	mul     rcx
	add     r9, rax
	mov     rax, [rsi+r8*8+24]
	mov     [rdi+r8*8+16], r9
	adc     r11, rdx
	add     r8, 3
	jnc     .0
.1:
	cmp     r8, 1
	ja      .4
	je      .3
.2:	mul     rcx
	add     r11, rax
	mov     [rdi], r11
	mov     r10d, 0
	mov     rax, [rsi+8]
	adc     r10, rdx
	mov     r11d, 0
	mul     rcx
	add     r10, rax
	mov     r9d, 0
	mov     rax, [rsi+16]
	mov     [rdi+8], r10
	adc     r9, rdx
	mul     rcx
	add     r9, rax
	mov     [rdi+16], r9
	adc     r11, rdx
	mov     rax, r11
    EXIT_PROC reg_save_list

.3:	mul     rcx
	add     r11, rax
	mov     [rdi+8], r11
	mov     r10d, 0
	mov     rax, [rsi+16]
	adc     r10, rdx
	mov     r11d, 0
	mul     rcx
	add     r10, rax
	mov     r9d, 0
	mov     [rdi+16], r10
	adc     r9, rdx
	mov     rax, r9
	EXIT_PROC reg_save_list

.4:	mul     rcx
	add     r11, rax
	mov     [rdi+16], r11
	mov     r10d, 0
	adc     r10, rdx
	mov     rax, r10
    END_PROC reg_save_list

    end
