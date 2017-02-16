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

	text
	align	32
	LEAF_PROC mpn_mullow_n_basecase
	cmp     r9, 3
	je      asm_sym(?mpn_mullow1)
	ja      asm_sym(?mpn_mullow2)
	mov     rax, [rdx]
	cmp     r9, 2
	jae     .1

	imul    rax, [r8]
	mov     [rcx], rax
	ret

.1: mov     r10, rdx
    mov     r9, [r8]
	mul     r9
	mov     [rcx], rax
	mov     rax, [r10+8]
	imul    rax, r9
	add     rdx, rax
	mov     r9, [r8+8]
	mov     r11, [r10]
	imul    r11, r9
	add     rdx, r11
	mov     [rcx+8], rdx
	ret

%define reg_save_list rsi, rdi

	align	32
	FRAME_PROC ?mpn_mullow1, 0, reg_save_list
    mov     rdi, rcx
    mov     rsi, rdx
    mov     rcx, r9
	mov     rax, [rsi]
    mov     r9, [r8]
	mul     r9
	mov     [rdi], rax
	mov     r10, rdx
	mov     rax, [rsi+8]
	mul     r9
	imul    r9, [rsi+16]
	add     r10, rax
	adc     r9, rdx
	mov     r11, [r8+8]
	mov     rax, [rsi]
	mul     r11
	add     r10, rax
	adc     r9, rdx
	imul    r11, [rsi+8]
	add     r9, r11
	mov     [rdi+8], r10
	mov     r10, [r8+16]
	mov     rax, [rsi]
	imul    r10, rax
	add     r9, r10
	mov     [rdi+16], r9
    END_PROC reg_save_list

%define reg_save_list rsi, rdi, rbx, rbp, r12, r13, r14, r15

	align	32
	FRAME_PROC ?mpn_mullow2, 0, reg_save_list
    mov     rdi, rcx
    mov     rsi, rdx
    mov     rcx, r9
	mov     rax, [rsi]
	mov     r10, [r8]
	lea     rdi, [rdi+rcx*8]
	lea     rsi, [rsi+rcx*8]
	mov     r9d, 0
	sub     r9, rcx
	mul     r10
	mov     r11, [r8+8]
	test    cl, 1
	jnz     .4
.1:	test    cl, 2
	jnz     .3
.2:	lea     r13, [r9]
	mov     [rdi+r9*8], rax
	mov     rcx, rdx
	mov     rax, [rsi+r9*8]
	xor     ebp, ebp
	jmp     .9
.3:	lea     r13, [r9-2]
	mov     rbp, rax
	mov     rax, [rsi+r9*8]
	mov     r12, rdx
	xor     ebx, ebx
	jmp     .11
.4:	test    cl, 2
	jnz     .6
.5:	lea     r13, [r9+1]
	mov     [rdi+r9*8], rax
	mov     rax, [rsi+r9*8]
	mov     rbx, rdx
	xor     ecx, ecx
	jmp     .8
.6:	lea     r13, [r9-1]
	xor     r12d, r12d
	mov     rcx, rax
	mov     rbp, rdx
	mov     rax, [rsi+r9*8]
	jmp     .10

	align	16
.7:	mul     r10
	add     r12, rax
	mov     rax, [rsi+r13*8-8]
	mov     [rdi+r13*8-8], r12
	adc     rbx, rdx
	adc     ecx, 0
.8:	mul     r11
	add     rbx, rax
	adc     rcx, rdx
	mov     ebp, 0
	mov     rax, [rsi+r13*8]
	mul     r10
	add     rbx, rax
	mov     [rdi+r13*8], rbx
	adc     rcx, rdx
	mov     rax, [rsi+r13*8]
	adc     ebp, 0
.9:	mul     r11
	add     rcx, rax
	adc     rbp, rdx
	mov     rax, [rsi+r13*8+8]
	mul     r10
	mov     r12d, 0
	add     rcx, rax
	adc     rbp, rdx
	adc     r12d, 0
	mov     rax, [rsi+r13*8+8]
.10:mul     r11
	add     rbp, rax
	mov     [rdi+r13*8+8], rcx
	adc     r12, rdx
	mov     ebx, 0
	mov     rax, [rsi+r13*8+16]
	mul     r10
	add     rbp, rax
	mov     rax, [rsi+r13*8+16]
	adc     r12, rdx
	adc     ebx, 0
.11:mul     r11
	mov     ecx, 0
	add     r12, rax
	mov     rax, [rsi+r13*8+24]
	mov     [rdi+r13*8+16], rbp
	adc     rbx, rdx
	add     r13, 4
	js      .7
.12:imul    rax, r10
	add     rax, r12
	mov     [rdi-8], rax
	add     r9, 2
	lea     r8, [r8+16]
	lea     rsi, [rsi-16]
	cmp     r9, -2
	jge     .26
.13:mov     r10, [r8]
	mov     r11, [r8+8]
	mov     rax, [rsi+r9*8]
	mul     r10
	test    r9b, 1
	jnz     .17
.14:mov     r15, rax
	mov     r14, rdx
	mov     rax, [rsi+r9*8]
	mul     r11
	test    r9b, 2
	jnz     .16
.15:lea     r13, [r9]
	mov     r12, [rdi+r9*8]
	mov     rbx, rax
	lea     rcx, [rdx]
	jmp     .23
.16:lea     r13, [r9+2]
	mov     rcx, [rdi+r9*8]
	mov     rbp, rax
	mov     rax, [rsi+r9*8+8]
	lea     r12, [rdx]
	jmp     .21
.17:mov     r14, rax
	mov     r15, rdx
	mov     rax, [rsi+r9*8]
	mul     r11
	test    r9b, 2
	jz      .19
.18:lea     r13, [r9+1]
	lea     rbx, [rdx]
	mov     rbp, [rdi+r9*8]
	mov     r12, rax
	jmp     .22
.19:lea     r13, [r9-1]
	lea     rbp, [rdx]
	mov     rcx, rax
	mov     rbx, [rdi+r9*8]
	mov     rax, [rsi+r9*8+8]
	jmp     .24
	
    align	16
.20:mul     r11
	add     rcx, rbx
	adc     rbp, rax
	mov     rax, [rsi+r13*8-8]
	lea     r12, [rdx]
	adc     r12, 0
.21:mul     r10
	add     r15, rcx
	mov     [rdi+r13*8-16], r15
	adc     r14, rax
	mov     r15, rdx
	adc     r15, 0
	mov     rax, [rsi+r13*8-8]
	mul     r11
	lea     rbx, [rdx]
	mov     rcx, [rdi+r13*8-8]
	add     rbp, rcx
	adc     r12, rax
	adc     rbx, 0
.22:mov     rax, [rsi+r13*8]
	mul     r10
	add     r14, rbp
	adc     r15, rax
	mov     [rdi+r13*8-8], r14
	mov     r14, rdx
	adc     r14, 0
	mov     rax, [rsi+r13*8]
	mov     rbp, [rdi+r13*8]
	mul     r11
	add     r12, rbp
	adc     rbx, rax
	lea     rcx, [rdx]
	adc     rcx, 0
.23:mov     rax, [rsi+r13*8+8]
	mul     r10
	add     r15, r12
	mov     [rdi+r13*8], r15
	adc     r14, rax
	mov     r15, rdx
	mov     r12, [rdi+r13*8+8]
	adc     r15, 0
	mov     rax, [rsi+r13*8+8]
	mul     r11
	add     rbx, r12
	lea     rbp, [rdx]
	adc     rcx, rax
	mov     rax, [rsi+r13*8+16]
	adc     rbp, 0
.24:mul     r10
	add     r14, rbx
	mov     [rdi+r13*8+8], r14
	mov     r14, rdx
	adc     r15, rax
	mov     rax, [rsi+r13*8+16]
	mov     rbx, [rdi+r13*8+16]
	adc     r14, 0
	add     r13, 4
	jnc     .20
.25:imul    rax, r11
	add     rcx, rbx
	adc     rbp, rax
	mov     rax, [rsi-8]
	imul    rax, r10
	add     r15, rcx
	mov     [rdi-16], r15
	adc     rax, r14
	mov     rcx, [rdi-8]
	add     rbp, rcx
	add     rax, rbp
	mov     [rdi-8], rax
	add     r9, 2
	lea     r8, [r8+16]
	lea     rsi, [rsi-16]
	cmp     r9, -2
	jl      .13
	jnz     .27
.26:mov     r10, [r8]
	mov     r11, [r8+8]
	mov     rax, [rsi-16]
	mul     r10
	add     rax, [rdi-16]
	adc     rdx, [rdi-8]
	mov     rbx, [rsi-8]
	imul    rbx, r10
	mov     rcx, [rsi-16]
	imul    rcx, r11
	mov     [rdi-16], rax
	add     rcx, rbx
	add     rcx, rdx
	mov     [rdi-8], rcx
    EXIT_PROC reg_save_list

.27:mov     r11, [r8]
	imul    r11, [rsi-8]
	add     r11, rax
	mov     [rdi-8], r11
    END_PROC reg_save_list
