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
    mov     r10, rdx
	mov     rax, [rdx]

	align	16
    cmp     r9, 2
	ja      .1
    imul    rax, [r8]
	mov     [rcx], rax
	ret

.1: mov     r9, [r8]
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
	mov     r9, [r8]
	lea     rdi, [rdi+rcx*8]
	lea     rsi, [rsi+rcx*8]
	neg     rcx
	mul     r9
	mov     rbx, [r8+8]
	test    cl, 1
	jnz     .2
.1:	lea     rbp, [rcx]
	xor     r10, r10
	mov     r12, rax
	mov     r11, rdx
	jmp     .5
.2:	lea     rbp, [rcx+1]
	xor     r11, r11
	xor     r12, r12
	mov     r10, rax
	mov     r13, rdx
	jmp     .4
	align	32
.3:	mul     r9
	add     r10, rax
	mov     r13, rdx
	adc     r13, 0
.4:	mov     rax, [rsi+rbp*8-8]
	mul     rbx
	add     r10, r11
	adc     r13, 0
	add     r12, rax
	mov     [rdi+rbp*8-8], r10
	mov     r10, rdx
	adc     r10, 0
	mov     rax, [rsi+rbp*8]
	mul     r9
	add     r12, rax
	mov     r11, rdx
	adc     r11, 0
	add     r12, r13
.5:	mov     rax, [rsi+rbp*8]
	adc     r11, 0
	mul     rbx
	mov     [rdi+rbp*8], r12
	add     r10, rax
	mov     r12, rdx
	mov     rax, [rsi+rbp*8+8]
	adc     r12, 0
	add     rbp, 2
	jnc     .3
.6:	imul    rax, r9
	add     rax, r10
	add     rax, r11
	mov     [rdi-8], rax
	add     rcx, 2
	lea     r8, [r8+16]
	lea     rsi, [rsi-16]
	cmp     rcx, -2
	jge     .19
.7:	mov     r9, [r8]
	mov     rbx, [r8+8]
	mov     rax, [rsi+rcx*8]
	mul     r9
	test    cl, 1
	jnz     .11
.8:	mov     r15, [rdi+rcx*8]
	xor     r12, r12
	xor     r11, r11
	test    cl, 2
	jnz     .10
.9:	lea     rbp, [rcx+1]
	jmp     .16
.10:lea     rbp, [rcx+3]
	mov     r13, rdx
	add     r15, rax
	mov     rax, [rsi+rcx*8]
	mov     r14, [rdi+rcx*8+8]
	adc     r13, 0
	jmp     .14
.11:mov     r14, [rdi+rcx*8]
	xor     r10, r10
	mov     r11, rdx
	test    cl, 2
	jz      .13
.12:lea     rbp, [rcx+2]
	add     r14, rax
	adc     r11, 0
	mov     rax, [rsi+rcx*8]
	mul     rbx
	mov     r15, [rdi+rcx*8+8]
	jmp     .15
.13:lea     rbp, [rcx]
	xor     r13, r13
	jmp     .17
	
    align	32
.14:mul     rbx
	mov     r10, rdx
	add     r14, rax
	adc     r10, 0
	add     r15, r11
	adc     r13, 0
	add     r14, r12
	adc     r10, 0
	mov     rax, [rsi+rbp*8-16]
	mul     r9
	add     r14, rax
	mov     r11, rdx
	adc     r11, 0
	mov     rax, [rsi+rbp*8-16]
	mul     rbx
	mov     [rdi+rbp*8-24], r15
	mov     r15, [rdi+rbp*8-8]
	add     r14, r13
	adc     r11, 0
.15:mov     r12, rdx
	mov     [rdi+rbp*8-16], r14
	add     r15, rax
	adc     r12, 0
	mov     rax, [rsi+rbp*8-8]
	add     r15, r10
	adc     r12, 0
	mul     r9
.16:add     r15, rax
	mov     r13, rdx
	adc     r13, 0
	mov     rax, [rsi+rbp*8-8]
	mul     rbx
	add     r15, r11
	mov     r14, [rdi+rbp*8]
	adc     r13, 0
	mov     r10, rdx
	add     r14, rax
	adc     r10, 0
	mov     rax, [rsi+rbp*8]
	mul     r9
	add     r14, r12
	mov     [rdi+rbp*8-8], r15
	mov     r11, rdx
	adc     r10, 0
.17:add     r14, rax
	adc     r11, 0
	mov     rax, [rsi+rbp*8]
	add     r14, r13
	adc     r11, 0
	mul     rbx
	mov     r15, [rdi+rbp*8+8]
	add     r15, rax
	mov     r12, rdx
	adc     r12, 0
	mov     rax, [rsi+rbp*8+8]
	mov     [rdi+rbp*8], r14
	mul     r9
	add     r15, r10
	mov     r13, rdx
	adc     r12, 0
	add     r15, rax
	mov     rax, [rsi+rbp*8+8]
	mov     r14, [rdi+rbp*8+16]
	adc     r13, 0
	add     rbp, 4
	jnc     .14
.18:imul    rax, rbx
	add     r14, rax
	add     r15, r11
	adc     r13, 0
	add     r14, r12
	mov     rax, [rsi-8]
	imul    rax, r9
	add     rax, r14
	mov     [rdi-16], r15
	add     rax, r13
	mov     [rdi-8], rax
	add     rcx, 2
	lea     r8, [r8+16]
	lea     rsi, [rsi-16]
	cmp     rcx, -2
	jl      .7
	jnz     .20
.19:mov     r9, [r8]
	mov     rbx, [r8+8]
	mov     rax, [rsi-16]
	mul     r9
	add     rax, [rdi-16]
	adc     rdx, [rdi-8]
	mov     r10, [rsi-8]
	imul    r10, r9
	mov     r11, [rsi-16]
	imul    r11, rbx
	mov     [rdi-16], rax
	add     r11, r10
	add     r11, rdx
	mov     [rdi-8], r11
    EXIT_PROC reg_save_list

.20:mov     r11, [r8]
	imul    r11, [rsi-8]
	add     r11, rax
	mov     [rdi-8], r11
    END_PROC reg_save_list
 