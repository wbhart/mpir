; PROLOGUE(mpn_mul_1)
;
;  Copyright 2008 Jason Moxham
;
;  Windows Conversion Copyright 2008 Brian Gladman
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

    CPU  Core2
    BITS 64

    LEAF_PROC mpn_mul_1c
    mov     r11, [rsp+0x28]
    jmp     start
    LEAF_PROC mpn_mul_1
    xor     r11, r11

start:
    mov     rax, r8
	lea     r10, [rdx+rax*8-8]
	lea     rcx, [rcx+rax*8-8]
	mov     r8d, 1
	sub     r8, rax
	mov     rax, [r10+r8*8]
	jz      .1

	xalign  16
.0:	mul     r9
	add     rax, r11
	mov     [rcx+r8*8], rax
	mov     rax, [r10+r8*8+8]
	mov     r11d, 0
	adc     r11, rdx
	add     r8, 1
	jnc     .0
.1: mul     r9
	add     rax, r11
	mov     [rcx+r8*8], rax
	mov     eax, 0
	adc     rax, rdx
	ret

    end
