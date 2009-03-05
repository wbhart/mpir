
;  AMD64 mpn_sumdiff_n
;  Copyright 2009 Jason Moxham
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

;	(rdi, r8) = (rdx, r8) + (rcx, r8)  (rsi, r8) = (rdx, r8) - (rcx, r8)
;	return 2*add_carry + sub_borrow
	
%include 'yasm_mac.inc'

    BITS    64

   GLOBAL_FUNC mpn_sumdiff_n
	lea     rsi, [rsi+r8*8]
	lea     rdx, [rdx+r8*8]
	lea     rdi, [rdi+r8*8]
	lea     rcx, [rcx+r8*8]
	neg     r8
	xor     r9, r9
	xor     r10, r10
	test    r8, 3
	jz      next
lp1:
	mov     rax, [rdx+r8*8]
	mov     r11, rax
	add     r9, 1
	adc     rax, [rcx+r8*8]
	sbb     r9, r9
	add     r10, 1
	sbb     r11, [rcx+r8*8]
	sbb     r10, r10
	mov     [rdi+r8*8], rax
	mov     [rsi+r8*8], r11
	inc     r8
	test    r8, 3
	jnz     lp1
next:
	cmp     r8, 0
	jz      skiploop
	mov     [rsp-8], rbx
	mov     [rsp-16], rbp
	mov     [rsp-24], r12
	mov     [rsp-32], r13
	mov     [rsp-40], r14
	mov     [rsp-48], r15
	align   16
loop1:
	mov     rax, [rdx+r8*8]
	mov     rbx, [rdx+r8*8+8]
	mov     rbp, [rdx+r8*8+16]
	mov     r12, [rdx+r8*8+24]
	mov     r11, rax
	mov     r13, rbx
	mov     r14, rbp
	mov     r15, r12
	add     r9, 1
	adc     rax, [rcx+r8*8]
	adc     rbx, [rcx+r8*8+8]
	adc     rbp, [rcx+r8*8+16]
	adc     r12, [rcx+r8*8+24]
	sbb     r9, r9
	add     r10, 1
	sbb     r11, [rcx+r8*8]
	sbb     r13, [rcx+r8*8+8]
	sbb     r14, [rcx+r8*8+16]
	sbb     r15, [rcx+r8*8+24]
	sbb     r10, r10
	mov     [rdi+r8*8], rax
	mov     [rdi+r8*8+8], rbx
	mov     [rdi+r8*8+16], rbp
	mov     [rdi+r8*8+24], r12
	mov     [rsi+r8*8], r11
	mov     [rsi+r8*8+8], r13
	mov     [rsi+r8*8+16], r14
	mov     [rsi+r8*8+24], r15
	add     r8, 4
	jnz     loop1
	mov     rbx, [rsp-8]
	mov     rbp, [rsp-16]
	mov     r12, [rsp-24]
	mov     r13, [rsp-32]
	mov     r14, [rsp-40]
	mov     r15, [rsp-48]
skiploop:
	lea     rax, [r10+r9*2]
	neg     rax
	ret
