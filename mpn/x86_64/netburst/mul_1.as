
;  core2 mpn_mul_1
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

%include 'yasm_mac.inc'

;	(rdi, rdx) = (rsi, rdx)*rcx
;	rax = carry
	
    BITS 64
    
   GLOBAL_FUNC mpn_mul_1
;	this is just an addmul , so we can get rid off stack use
;	and simplifiy wind down , and perhaps re-do the OOO order 
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
	align 16
loop1:
	mov     r10, 0
	mul     rcx
	mov     [rdi+r11*8], r8
	add     r9, rax
	db      0x26
	adc     r10, rdx
	mov     rax, [rsi+r11*8+16]
	mul     rcx
	mov     [rdi+r11*8+8], r9
	add     r10, rax
	mov     ebx, 0
	adc     rbx, rdx
	mov     rax, [rsi+r11*8+24]
	mov     r8, 0
	mov     r9, 0
	mul     rcx
	mov     [rdi+r11*8+16], r10
	db      0x26
	add     rbx, rax
	db      0x26
	adc     r8, rdx
	mov     rax, [rsi+r11*8+32]
	mul     rcx
	mov     [rdi+r11*8+24], rbx
	db      0x26
	add     r8, rax
	db      0x26
	adc     r9, rdx
	add     r11, 4
	mov     rax, [rsi+r11*8+8]
	jnc     loop1
	align 16
skiploop:
	mov     r10d, 0
	mul     rcx
	mov     [rdi+r11*8], r8
	add     r9, rax
	adc     r10, rdx
	cmp     r11, 2
	jz      next2
	ja      next3
	jp      next1
next0:
	mov     rax, [rsi+r11*8+16]
	mul     rcx
	mov     [rdi+r11*8+8], r9
	add     r10, rax
	mov     ebx, 0
	adc     rbx, rdx
	mov     rax, [rsi+r11*8+24]
	mov     r8d, 0
	mul     rcx
	mov     [rdi+r11*8+16], r10
	add     rbx, rax
	adc     r8, rdx
	mov     rax, [rsi+r11*8+32]
	mul     rcx
	mov     [rdi+r11*8+24], rbx
	mov     rbx, [rsp-8]
	add     r8, rax
	adc     rdx, 0
	mov     [rdi+r11*8+32], r8
	mov     rax, rdx
	ret
	align 16
next1:
	mov     rax, [rsi+r11*8+16]
	mul     rcx
	mov     [rdi+r11*8+8], r9
	add     r10, rax
	mov     r8d, 0
	adc     r8, rdx
	mov     rax, [rsi+r11*8+24]
	mul     rcx
	mov     [rdi+r11*8+16], r10
	add     r8, rax
	adc     rdx, 0
	mov     [rdi+r11*8+24], r8
	mov     rbx, [rsp-8]
	mov     rax, rdx
	ret
	align 16
one:
	mul     rcx
	mov     [rdi], rax
	mov     rax, rdx
	ret
	align 16
next2:
	mov     rax, [rsi+r11*8+16]
	mul     rcx
	mov     [rdi+r11*8+8], r9
	add     r10, rax
	mov     ebx, 0
	adc     rbx, rdx
	mov     [rdi+r11*8+16], r10
	mov     rax, rbx
	mov     rbx, [rsp-8]
	ret
	align 16
next3:
	mov     rbx, [rsp-8]
	mov     [rdi+r11*8+8], r9
	mov     rax, r10
	ret
