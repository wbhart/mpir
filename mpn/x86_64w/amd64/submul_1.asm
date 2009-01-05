
;  AMD64 mpn_submul_1
;  Copyright 2008 Jason Moxham
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
include(`../config.m4') ; < not translated >
;	(rdi,rdx)=(rdi,rdx)-(rsi,rdx)*rcx
;	rax=borrow
	ASM_START
PROLOGUE(mpn_submul_1) ; < not translated >
; Version 1.0.3
	mov     rax, [rsi]
; this is neccessary as submul will not handle 1 limb
	cmp     rdx, 1
	jnz     notone
	mul     rcx
	sub     [rdi], rax
	adc     rdx, 0
	mov     rax, rdx
	ret
notone:
	mov     r11, 5
	lea     rsi, [rsi+rdx*8-40]
	lea     rdi, [rdi+rdx*8-40]
	sub     r11, rdx
	mul     rcx
	mov     r8, rax
	mov     rax, [rsi+r11*8+8]
	mov     r9, rdx
	cmp     r11, 0
; for windows can just move this push to the prologue
	push    rbx
	jge     skiploop
	align   16
loop:
	mov     r10d, 0
	mul     rcx
	sub     [rdi+r11*8], r8
	adc     r9, rax
	adc     r10, rdx
	mov     rax, [rsi+r11*8+16]
	mul     rcx
	sub     [rdi+r11*8+8], r9
	adc     r10, rax
	mov     ebx, 0
	adc     rbx, rdx
	mov     rax, [rsi+r11*8+24]
	mov     r8d, 0
	mov     r9d, 0
	mul     rcx
	sub     [rdi+r11*8+16], r10
	adc     rbx, rax
	adc     r8, rdx
	mov     rax, [rsi+r11*8+32]
	mul     rcx
	sub     [rdi+r11*8+24], rbx
	adc     r8, rax
	adc     r9, rdx
	add     r11, 4
	mov     rax, [rsi+r11*8+8]
	jnc     loop
; ALIGN(16)
skiploop:
	jz      next0
	jp      next3
	cmp     r11, 1
	je      next1
; ALIGN(16)
next2:
; here is 1 load ie if r11=2
	mov     r10d, 0
	mul     rcx
	sub     [rdi+r11*8], r8
	adc     r9, rax
	adc     r10, rdx
	mov     rax, [rsi+r11*8+16]
	mul     rcx
	sub     [rdi+r11*8+8], r9
	adc     r10, rax
	mov     ebx, 0
	adc     rbx, rdx
	sub     [rdi+r11*8+16], r10
	adc     rbx, 0
	mov     rax, rbx
	pop     rbx
	ret
	align   16
next3:
; here is 0 loads ie if r11=3
	mov     r10d, 0
	mul     rcx
	sub     [rdi+r11*8], r8
	adc     r9, rax
	adc     r10, rdx
	sub     [rdi+r11*8+8], r9
	adc     r10, 0
	mov     rax, r10
	pop     rbx
	ret
	align   16
next1:
; here is 2 loads ie if r11=1
	mov     r10d, 0
	mul     rcx
	sub     [rdi+r11*8], r8
	adc     r9, rax
	adc     r10, rdx
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
	sub     [rdi+r11*8+24], rbx
	adc     r8, 0
	mov     rax, r8
	pop     rbx
	ret
	align   16
next0:
; here is 3 loads ie if r11=0
	mov     r10d, 0
	mul     rcx
	sub     [rdi+r11*8], r8
	adc     r9, rax
	adc     r10, rdx
	mov     rax, [rsi+r11*8+16]
	mul     rcx
	sub     [rdi+r11*8+8], r9
	adc     r10, rax
	mov     ebx, 0
	adc     rbx, rdx
	mov     rax, [rsi+r11*8+24]
	mov     r8d, 0
	mov     r9d, 0
	mul     rcx
	sub     [rdi+r11*8+16], r10
	adc     rbx, rax
	adc     r8, rdx
	mov     rax, [rsi+r11*8+32]
	mul     rcx
	sub     [rdi+r11*8+24], rbx
	adc     r8, rax
	adc     r9, rdx
	sub     [rdi+r11*8+32], r8
	adc     r9, 0
	mov     rax, r9
	pop     rbx
	ret
	EPILOGUE