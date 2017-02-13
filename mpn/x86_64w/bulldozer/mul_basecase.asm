;  AMD64 mpn_mul_basecase optimised for AMD Bulldozer and Piledriver.

;  Contributed to the GNU project by Torbjörn Granlund.

;  Copyright 2003-2005, 2007, 2008, 2011-2013 Free Software Foundation, Inc.

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

; cycles/limb	mul_1		mul_2		mul_3		addmul_2
; AMD K8,K9
; AMD K10
; AMD bull	~4.8		~4.55		-		~4.3
; AMD pile	~4.6		~4.55		-		~4.55
; AMD bobcat
; AMD jaguar
; Intel P4
; Intel core
; Intel NHM
; Intel SBR
; Intel IBR
; Intel HWL
; Intel BWL
; Intel atom
; VIA nano

; The inner loops of this code are the result of running a code generation and
; optimisation tool suite written by David Harvey and Torbjorn Granlund.

; TODO
;  * Merge bull-specific mul_1, if it is not slower the TOOM22 range.
;    Alternatively, we could tweak the present code (which was loopmixed for a
;    different CPU).
;  * Merge faster mul_2, such as the one in the same directory as this file.
;  * Further micro-optimise.

;  mp_limb_t mpn_mul_basecase(mp_ptr, mp_ptr, mp_size_t, mp_ptr, mp_size_t)
;  rax                           rdi     rsi        rdx     rcx         r8
;  rax                           rcx     rdx         r8      r9   [rsp+40]

%include 'yasm_mac.inc'

%define reg_save_list rsi, rdi, rbx, rbp, r12, r13, r14, r15

	TEXT
	align	16
	FRAME_PROC mpn_mul_basecase, 0, reg_save_list
    mov     rdi, rcx
    mov     rsi, rdx
    mov     rdx, r8
    mov     rcx, r9
    mov     r8, [rsp+stack_use+40]

	mov     rbx, rdx
	neg     rbx
	mov     rax, [rsi]
	lea     rsi, [rsi+rdx*8]
	lea     rdi, [rdi+rdx*8]
	mov     r9, [rcx]
	mul     r9
	test    r8b, 1
	jz      .13
.0: test    bl, 1
	jnz     .4
.1:	mov     r10, rax
	mov     r11, rdx
	mov     rax, [rsi+rbx*8+8]
	test    bl, 2
	jnz     .3
.2:	lea     rbp, [rbx+2]
	jmp     .9
.3:	lea     rbp, [rbx]
	jmp     .11
.4:	mov     r11, rax
	mov     r10, rdx
	test    bl, 2
	jz      .6
.5:	lea     rbp, [rbx+3]
	test    rbp, rbp
	js      .8
	mov     [rdi-8], rax
	mov     [rdi], rdx
    EXIT_PROC reg_save_list

.6:	lea     rbp, [rbx+1]
	mov     rax, [rsi+rbx*8+8]
	jmp     .10
	align	16
.7:	mov     r10, rdx
	add     r11, rax
.8:	mov     rax, [rsi+rbp*8-16]
	adc     r10, 0
	mul     r9
	add     r10, rax
	mov     [rdi+rbp*8-24], r11
	mov     rax, [rsi+rbp*8-8]
	mov     r11, rdx
	adc     r11, 0
.9:	mul     r9
	mov     [rdi+rbp*8-16], r10
	add     r11, rax
	mov     r10, rdx
	mov     rax, [rsi+rbp*8]
	adc     r10, 0
.10:mul     r9
	mov     [rdi+rbp*8-8], r11
	mov     r11, rdx
	add     r10, rax
	mov     rax, [rsi+rbp*8+8]
	adc     r11, 0
.11:mul     r9
	mov     [rdi+rbp*8], r10
	add     rbp, 4
	jnc     .7
.12:add     r11, rax
	adc     rdx, 0
	mov     [rdi-8], r11
	mov     [rdi], rdx
	dec     r8d
	jz      .35
	lea     rcx, [rcx+8]
	lea     rdi, [rdi+8]
	jmp     .20
.13:mov     r14, [rcx+8]
	test    bl, 1
	jnz     .15
.14:lea     rbp, [rbx]
	mov     r12, rax
	mov     rax, [rsi+rbx*8]
	mov     r11, rdx
	mul     r14
	mov     r10, rax
	mov     [rdi+rbx*8], r12
	mov     rax, [rsi+rbx*8+8]
	mov     r12, rdx
	jmp     .18
.15:lea     rbp, [rbx+1]
	mov     r10, rax
	mov     r13, rdx
	mov     rax, [rsi+rbx*8]
	mul     r14
	mov     [rdi+rbx*8], r10
	mov     r10, rdx
	mov     r12, rax
	mov     rax, [rsi+rbx*8+8]
	jmp     .17
	align	32
.16:add     r12, rax
	mov     rax, [rsi+rbp*8]
	adc     r10, 0
.17:mul     r9
	add     r12, rax
	mov     rax, [rsi+rbp*8]
	mov     r11, rdx
	adc     r11, 0
	mul     r14
	add     r12, r13
	adc     r11, 0
	add     r10, rax
	mov     [rdi+rbp*8], r12
	mov     rax, [rsi+rbp*8+8]
	mov     r12, rdx
	adc     r12, 0
.18:mul     r9
	add     r10, rax
	mov     r13, rdx
	adc     r13, 0
	add     r10, r11
	adc     r13, 0
	mov     rax, [rsi+rbp*8+8]
	mul     r14
	add     rbp, 2
	mov     [rdi+rbp*8-8], r10
	mov     r10, rdx
	jnc     .16
.19:add     r12, rax
	adc     rdx, 0
	add     r12, r13
	adc     rdx, 0
	mov     [rdi], r12
	mov     [rdi+8], rdx
	add     r8d, -2
	jz      .34
	lea     rcx, [rcx+16]
	lea     rdi, [rdi+16]
.20:
	mov    [rsp+stack_use+8], r8
.21:mov     r9, [rcx]
	mov     r8, [rcx+8]
	mov     rax, [rsi+rbx*8]
	mul     r9
	test    bl, 1
	jnz     .25
.22:mov     r15, rax
	mov     rax, [rsi+rbx*8]
	mov     r14, rdx
	mul     r8
	test    bl, 2
	jnz     .24
.23:lea     rbp, [rbx]
	mov     r13, [rdi+rbx*8]
	mov     r10, rax
	mov     rax, [rsi+rbx*8+8]
	mov     r11, rdx
	jmp     .31
.24:lea     rbp, [rbx+2]
	mov     r11, [rdi+rbx*8]
	mov     r13, rdx
	mov     r12, rax
	mov     rax, [rsi+rbx*8+8]
	jmp     .29
.25:mov     r14, rax
	mov     rax, [rsi+rbx*8]
	mov     r15, rdx
	mul     r8
	test    bl, 2
	jz      .27
.26:lea     rbp, [rbx+1]
	mov     r12, [rdi+rbx*8]
	mov     r10, rdx
	mov     r13, rax
	jmp     .30
.27:lea     rbp, [rbx-1]
	mov     r10, [rdi+rbx*8]
	mov     r11, rax
	mov     rax, [rsi+rbx*8+8]
	mov     r12, rdx
	jmp     .32
	align	32
.28:
.29:mul     r9
	add     r15, r11
	mov     [rdi+rbp*8-16], r15
	mov     r15, rdx
	adc     r14, rax
	adc     r15, 0
	mov     rax, [rsi+rbp*8-8]
	mul     r8
	mov     r11, [rdi+rbp*8-8]
	mov     r10, rdx
	add     r12, r11
	adc     r13, rax
	adc     r10, 0
.30:mov     rax, [rsi+rbp*8]
	mul     r9
	add     r14, r12
	mov     [rdi+rbp*8-8], r14
	mov     r14, rdx
	adc     r15, rax
	mov     rax, [rsi+rbp*8]
	adc     r14, 0
	mov     r12, [rdi+rbp*8]
	mul     r8
	add     r13, r12
	adc     r10, rax
	mov     rax, [rsi+rbp*8+8]
	mov     r11, rdx
	adc     r11, 0
.31:mul     r9
	add     r15, r13
	mov     [rdi+rbp*8], r15
	adc     r14, rax
	mov     rax, [rsi+rbp*8+8]
	mov     r15, rdx
	adc     r15, 0
	mov     r13, [rdi+rbp*8+8]
	mul     r8
	add     r10, r13
	adc     r11, rax
	mov     rax, [rsi+rbp*8+16]
	mov     r12, rdx
	adc     r12, 0
.32:mul     r9
	add     r14, r10
	mov     [rdi+rbp*8+8], r14
	mov     r14, rdx
	adc     r15, rax
	adc     r14, 0
	mov     rax, [rsi+rbp*8+16]
	mov     r10, [rdi+rbp*8+16]
	mul     r8
	mov     r13, rdx
	add     r11, r10
	adc     r12, rax
	adc     r13, 0
	mov     rax, [rsi+rbp*8+24]
	add     rbp, 4
	jnc     .28
.33:mul     r9
	add     r15, r11
	mov     [rdi-16], r15
	mov     r15, rdx
	adc     r14, rax
	adc     r15, 0
	mov     rax, [rsi-8]
	mul     r8
	mov     r11, [rdi-8]
	add     r12, r11
	adc     r13, rax
	adc     rdx, 0
	add     r14, r12
	adc     r15, 0
	mov     [rdi-8], r14
	add     r15, r13
	mov     [rdi], r15
	adc     rdx, 0
	mov     [rdi+8], rdx
	add     qword [rsp+stack_use+8], -2
	lea     rcx, [rcx+16]
	lea     rdi, [rdi+16]
	jnz     .21
    mov     rax, [rsp+stack_use+8]
.34:
.35:
    END_PROC reg_save_list