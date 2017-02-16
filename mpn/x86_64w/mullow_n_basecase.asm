;  AMD64 mpn_mullow_n_basecase
;
;  Copyright 2015 Free Software Foundation, Inc.
;
;  This file is part of the GNU MP Library.
;
;  The GNU MP Library is free software; you can redistribute it and/or modify
;  it under the terms of either:
;
;    * the GNU Lesser General Public License as published by the Free
;      Software Foundation; either version 3 of the License, or (at your
;      option) any later version.
;
;  or
;
;    * the GNU General Public License as published by the Free Software
;      Foundation; either version 2 of the License, or (at your option) any
;      later version.
;
;  or both in parallel, as here.
;
;  The GNU MP Library is distributed in the hope that it will be useful, but
;  WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
;  or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
;  for more details.
;
;  You should have received copies of the GNU General Public License and the
;  GNU Lesser General Public License along with the GNU MP Library.  If not,
;  see https://www.gnu.org/licenses/.
;
;void mpn_mullow_n_basecase (mp_ptr rp, mp_srcptr up, mp_srcptr vp, mp_size_t n)
;                                  rdi           rsi           rdx          rcx
;                                  rcx           rdx            r8           r9

%include 'yasm_mac.inc'

	TEXT

	LEAF_PROC mpn_mullow_n_basecase
    cmp     r9, 3
    je      asm_sym(?mpn_mullow1)
    jg      asm_sym(?mpn_mullow2)
	mov     rax, [rdx]
    mov     r11, [r8+8]
	mov     r8, [r8]
    cmp     r9, 1
    jg      .3
	
.2:	imul    rax, r8
	mov     [rcx], rax
	ret
	
.3:	mov     r9, [rdx+8]
    imul    r11, rax
	mul     r8
	mov     [rcx], rax
	imul    r8, r9
	lea     rax, [r11+rdx]
	add     rax, r8
	mov     [rcx+8], rax
    ret

%define reg_save_list rsi, rdi
	
	FRAME_PROC ?mpn_mullow1, 0, reg_save_list
    mov     rdi, rcx
    mov     rsi, rdx
    mov     rdx, r8
    mov     rcx, r9
    mov	    rax, [rsi]
	mov	    r8, [rdx]
    mov     r9, [rdx+8]
	mov     r11, [rdx+16]
	mul     r8
	mov     [rdi], rax
	mov     rax, [rsi]
	mov     rcx, rdx
	mul     r9
	imul    r9, [rsi+8]
	mov     r10, [rsi+16]
	imul    r10, r8
	add     rcx, rax
	adc     r9, rdx
	add     r9, r10
	mov     rax, [rsi+8]
	mul     r8
	add     rcx, rax
	adc     r9, rdx
	mov     rax, r11
	imul    rax, [rsi]
	add     r9, rax
	mov     [rdi+8], rcx
	mov     [rdi+16], r9
	END_PROC reg_save_list

%define reg_save_list rsi, rdi, rbx, rbp, r13, r14, r15

	align  16
	FRAME_PROC ?mpn_mullow2, 0, reg_save_list
    mov     rdi, rcx
    mov     rsi, rdx
    mov     rdx, r8
    mov     rcx, r9
.5:	mov     rax, [rsi]
	mov     r13, [rdx]
	mov     r11, rdx
	lea     rdi, [rdi+rcx*8]
	lea     rsi, [rsi+rcx*8]
	neg     rcx
	mul     r13
	test    cl, 1
	jz      .12
.6: lea     rdi, [rdi-8]
	lea     rsi, [rsi-8]
	test    cl, 2
	jnz     .8
.7: lea     r9, [rcx-1]
	lea     r8, [rel .18]
	mov     rbx, rax
	mov     r15, rdx
	xor     ebp, ebp
	xor     r10d, r10d
	mov     rax, [rsi+rcx*8+16]
	jmp     .11

.8: lea     r9, [rcx+1]
	lea     r8, [rel .19]
	mov     rbp, rax
	mov     r10, rdx
	xor     ebx, ebx
	jmp     .10

	align  16
.9: mov     [rdi+r9*8-16], rbx
	add     r15, rax
	mov     rax, [rsi+r9*8]
	adc     rbp, rdx
	xor     ebx, ebx
	mul     r13
	mov     [rdi+r9*8-8], r15
	add     rbp, rax
	adc     r10, rdx
.10:mov     rax, [rsi+r9*8+8]
	mul     r13
	mov     [rdi+r9*8], rbp
	add     r10, rax
	adc     rbx, rdx
	mov     rax, [rsi+r9*8+16]
	mul     r13
	mov     [rdi+r9*8+8], r10
	xor     ebp, ebp
	mov     r10, rbp
	add     rbx, rax
	mov     rax, [rsi+r9*8+24]
	mov     r15, rbp
	adc     r15, rdx
.11:mul     r13
	add     r9, 4
	js      .9
	mov     [rdi-16], rbx
	add     r15, rax
	mov     [rdi-8], r15
	adc     rbp, rdx
	imul    r13, [rsi]
	add     rbp, r13
	mov     [rdi], rbp
	add     rcx, 1
	jz      .23
	mov     r13, [r11+8]
	mov     r14, [r11+16]
	lea     rsi, [rsi+16]
	lea     r11, [r11+8]
	lea     rdi, [rdi+24]
	jmp     r8

.12:mov     r14, [r11+8]
	test    cl, 2
	jz      .14

	align  16
.13:lea     r9, [rcx+0]
	mov     r10, rax
	mov     rbx, rdx
	xor     r15d, r15d
	mov     rax, [rsi+rcx*8]
	lea     r8, [rel .19]
	jmp     .17
	
	align  16
.14:lea     r9, [rcx+2]
	mov     r10d, 0
	mov     r15, rax
	mov     rax, [rsi+rcx*8]
	mov     rbp, rdx
	lea     r8, [rel .18]
	jmp     .16
	
	align  16
.15:mov     rax, [rsi+r9*8-32]
	mul     r14
	add     rbx, rax
	adc     r15, rdx
	mov     rax, [rsi+r9*8-24]
	xor     ebp, ebp
	mul     r13
	add     rbx, rax
	mov     rax, [rsi+r9*8-24]
	adc     r15, rdx
	adc     ebp, 0
	mul     r14
	add     r15, rax
	mov     [rdi+r9*8-24], rbx
	adc     rbp, rdx
	mov     rax, [rsi+r9*8-16]
	mul     r13
	mov     r10d, 0
	add     r15, rax
	adc     rbp, rdx
	mov     rax, [rsi+r9*8-16]
	adc     r10d, 0
.16:mov     ebx, 0
	mov     [rdi+r9*8-16], r15
	mul     r14
	add     rbp, rax
	mov     rax, [rsi+r9*8-8]
	adc     r10, rdx
	mov     r15d, 0
	mul     r13
	add     rbp, rax
	mov     rax, [rsi+r9*8-8]
	adc     r10, rdx
	adc     ebx, r15d
	mul     r14
	add     r10, rax
	mov     [rdi+r9*8-8], rbp
	adc     rbx, rdx
	mov     rax, [rsi+r9*8]
	mul     r13
	add     r10, rax
	adc     rbx, rdx
	adc     r15d, 0
.17:add     r9, 4
	mov     [rdi+r9*8-32], r10
	js      .15
	imul    r14, [rsi-16]
	add     rbx, r14
	imul    r13, [rsi-8]
	add     rbx, r13
	mov     [rdi-8], rbx
	add     rcx, 2
	jz      .23
	mov     r13, [r11+16]
	mov     r14, [r11+24]
	lea     r11, [r11+16]
	lea     rdi, [rdi+16]
	jmp     r8
.18:lea     r9, [rcx-2]
	mov     rax, [rsi+rcx*8-16]
	mul     r13
	mov     r10, rax
	mov     rax, [rsi+rcx*8-16]
	mov     rbx, rdx
	xor     r15d, r15d
	lea     r8, [rel .19]
	jmp     .22
.19:lea     r9, [rcx+0]
	mov     rax, [rsi+rcx*8-16]
	xor     r10d, r10d
	mul     r13
	mov     r15, rax
	mov     rax, [rsi+rcx*8-16]
	mov     rbp, rdx
	lea     r8, [rel .18]
	jmp     .21
	
	align  16
.20:add     [rdi+r9*8-32], r10
	adc     rbx, rax
	mov     rax, [rsi+r9*8-24]
	adc     r15, rdx
	xor     ebp, ebp
	mul     r13
	add     rbx, rax
	mov     rax, [rsi+r9*8-24]
	adc     r15, rdx
	adc     ebp, ebp
	mul     r14
	xor     r10d, r10d
	add     [rdi+r9*8-24], rbx
	adc     r15, rax
	mov     rax, [rsi+r9*8-16]
	adc     rbp, rdx
	mul     r13
	add     r15, rax
	mov     rax, [rsi+r9*8-16]
	adc     rbp, rdx
	adc     r10d, 0
.21:mul     r14
	add     [rdi+r9*8-16], r15
	adc     rbp, rax
	mov     rax, [rsi+r9*8-8]
	adc     r10, rdx
	mul     r13
	xor     ebx, ebx
	add     rbp, rax
	adc     r10, rdx
	mov     r15d, 0
	mov     rax, [rsi+r9*8-8]
	adc     ebx, r15d
	mul     r14
	add     [rdi+r9*8-8], rbp
	adc     r10, rax
	adc     rbx, rdx
	mov     rax, [rsi+r9*8]
	mul     r13
	add     r10, rax
	mov     rax, [rsi+r9*8]
	adc     rbx, rdx
	adc     r15d, 0
.22:mul     r14
	add     r9, 4
	js      .20
	add     [rdi-32], r10
	adc     rbx, rax
	imul    r13, [rsi-24]
	add     rbx, r13
	add     [rdi-24], rbx
	add     rcx, 2
	jns     .23
	lea     r11, [r11+16]
	mov     r13, [r11]
	mov     r14, [r11+8]
	lea     rsi, [rsi-16]
	jmp     r8
.23:
    END_PROC reg_save_list
