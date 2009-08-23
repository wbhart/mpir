
;  AMD64 mpn_submul_1
;  Copyright 2008,2009 Jason Moxham
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

;	(rdi,rdx) = (rdi,rdx)-(rsi,rdx)*rcx
;	rax = carry
	
%include 'yasm_mac.inc'

    BITS    64
       align 16
	GLOBAL_FUNC mpn_declsh_n
	mov eax,1
	shl rax,cl
	mov rcx,rax
	align 16
   GLOBAL_FUNC mpn_submul_1
	mov     rax, [rsi]
	cmp     rdx, 1
	je      one
	mov     r11, 5
	lea     rsi, [rsi+rdx*8-40]
	lea     rdi, [rdi+rdx*8-40]
	sub     r11, rdx
	mul     rcx
	db      0x26
	mov     r8, rax
	db      0x26
	mov     rax, [rsi+r11*8+8]
	db      0x26
	mov     r9, rdx
	db      0x26
	cmp     r11, 0
	db      0x26
	mov     [rsp-8], rbx
	db      0x26
	jge     skiploop
	align   16
loop1:
	mov     r10, 0
	mul     rcx
	sub     [rdi+r11*8], r8
	adc     r9, rax
	db      0x26
	adc     r10, rdx
	mov     rax, [rsi+r11*8+16]
	mul     rcx
	sub     [rdi+r11*8+8], r9
	adc     r10, rax
	mov     ebx, 0
	adc     rbx, rdx
	mov     rax, [rsi+r11*8+24]
	mov     r8, 0
	mov     r9, 0
	mul     rcx
	sub     [rdi+r11*8+16], r10
	db      0x26
	adc     rbx, rax
	db      0x26
	adc     r8, rdx
	mov     rax, [rsi+r11*8+32]
	mul     rcx
	sub     [rdi+r11*8+24], rbx
	db      0x26
	adc     r8, rax
	db      0x26
	adc     r9, rdx
	add     r11, 4
	mov     rax, [rsi+r11*8+8]
	jnc     loop1
	align   16
skiploop:
	mov     r10d, 0
	mul     rcx
	sub     [rdi+r11*8], r8
	adc     r9, rax
	adc     r10, rdx
	cmp     r11, 2
	jz      next2
	ja      next3
	jp      next1
next0:
	mov     rax, [rsi+r11*8+16]
	mul     rcx
	sub     [rdi+r11*8+8], r9
	adc     r10, rax
	mov     ebx, 0
	adc     rbx, rdx
	mov     rax, [rsi+r11*8+24]
	mov     r8d, 0
	mul     rcx
	sub     [rdi+r11*8+16], r10
	adc     rbx, rax
	adc     r8, rdx
	mov     rax, [rsi+r11*8+32]
	mul     rcx
	sub     [rdi+r11*8+24], rbx
	mov     rbx, [rsp-8]
	adc     r8, rax
	adc     rdx, 0
	sub     [rdi+r11*8+32], r8
	adc     rdx, 0
	mov     rax, rdx
	ret
	align   16
next1:
	mov     rax, [rsi+r11*8+16]
	mul     rcx
	sub     [rdi+r11*8+8], r9
	adc     r10, rax
	mov     r8d, 0
	adc     r8, rdx
	mov     rax, [rsi+r11*8+24]
	mul     rcx
	sub     [rdi+r11*8+16], r10
	adc     r8, rax
	adc     rdx, 0
	sub     [rdi+r11*8+24], r8
	mov     rbx, [rsp-8]
	adc     rdx, 0
	mov     rax, rdx
	ret
	align   16
one:
	mul     rcx
	sub     [rdi], rax
	adc     rdx, 0
	mov     rax, rdx
	ret
	align   16
next2:
	mov     rax, [rsi+r11*8+16]
	mul     rcx
	sub     [rdi+r11*8+8], r9
	adc     r10, rax
	mov     ebx, 0
	adc     rbx, rdx
	sub     [rdi+r11*8+16], r10
	adc     rbx, 0
	mov     rax, rbx
	mov     rbx, [rsp-8]
	ret
	align   16
next3:
	mov     rbx, [rsp-8]
	sub     [rdi+r11*8+8], r9
	adc     r10, 0
	mov     rax, r10
	ret
