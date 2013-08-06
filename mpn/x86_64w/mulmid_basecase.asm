; PROLOGUE(mpn_mulmid_basecase)
;
;  AMD64 mpn_mulmid_basecase
;
;  Based on mul_basecase.asm from GMP 4.3.1, modifications are copyright
;  (C) 2009, David Harvey. The original mul_basecase.asm was released under
;  LGPLv3+, license terms reproduced below. These modifications are hereby
;  released under the same terms.
;
;  Windows Conversion Copyright 2010 Dr B R Gladman
;
;  Contributed to the GNU project by Torbjorn Granlund and David Harvey.
;
;  Copyright 2008 Free Software Foundation, Inc.
;
;  This file is part of the GNU MP Library.
;
;  The GNU MP Library is free software; you can redistribute it and/or modify
;  it under the terms of the GNU Lesser General Public License as published
;  by the Free Software Foundation; either version 3 of the License, or (at
;  your option) any later version.
;
;  The GNU MP Library is distributed in the hope that it will be useful, but
;  WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
;  or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public
;  License for more details.
;
;  You should have received a copy of the GNU Lesser General Public License
;  //www.gnu.org/licenses/.
;
; void mpn_mulmid_basecase(mp_ptr, mp_ptr, mp_size_t, mp_ptr, mp_size_t)
;  rax                        rdi     rsi        rdx     rcx         r8
;  rax                        rcx     rdx         r8      r9   [rsp+40]

%define reg_save_list   rbx, rsi, rdi, rbp, r12, r13, r14, r15

%include 'yasm_mac.inc'

        BITS 64
	TEXT
	
	xalign  16
	WIN64_GCC_PROC mpn_mulmid_basecase, 5, frame
	mov     r15, rcx

	lea     r13, [rdx+1]
	sub     r13, r8
	lea     rdi, [rdi+r13*8]
	cmp     r13, 4
	jc      .29
	lea     rsi, [rsi+rdx*8]
	test    r8, 1
	jz      .10

.0:	mov     ebx, r13d
	neg     r13
	mov     rax, [rsi+r13*8]
	mov     r12, [r15]
	mul     r12

	and     r13, -4
	mov     r11, r13

	and     ebx, 3
	jz      .2
	cmp     ebx, 2
	jc      .3
	jz      .4

.1:	mov     r10, rax
	mov     rbx, rdx
	lea     r14, [rel .23]
	jmp     .8

	xalign  16
.2:	mov     rbp, rax
	mov     r10, rdx
	lea     r14, [rel .20]
	jmp     .7

	xalign  16
.3:	add     r11, 4
	mov     rcx, rax
	mov     rbp, rdx
	mov     r10d, 0
	mov     rax, [rsi+r11*8]
	lea     r14, [rel .21]
	jmp     .6

	xalign  16
.4:	mov     rbx, rax
	mov     rcx, rdx
	mov     rax, [rsi+r11*8+24]
	mov     ebp, 0
	mov     r10d, 0
	lea     r14, [rel .22]
	jmp     .9

	xalign  16
.5:	mov     [rdi+r11*8-16], rbx
	add     rcx, rax
	mov     rax, [rsi+r11*8]
	adc     rbp, rdx
.6:	mov     ebx, 0
	mul     r12
	mov     [rdi+r11*8-8], rcx
	add     rbp, rax
	adc     r10, rdx
.7:	mov     rax, [rsi+r11*8+8]
	mul     r12
	mov     [rdi+r11*8], rbp
	add     r10, rax
	adc     rbx, rdx
.8:	mov     rax, [rsi+r11*8+16]
	mul     r12
	mov     [rdi+r11*8+8], r10
	mov     ebp, 0
	mov     r10, rbp
	add     rbx, rax
	mov     rax, [rsi+r11*8+24]
	mov     rcx, rbp
	adc     rcx, rdx
.9:	mul     r12
	add     r11, 4
	js      .5
	mov     [rdi-16], rbx
	add     rcx, rax
	mov     [rdi-8], rcx
	mov     [rdi+8], rbp
	adc     rbp, rdx
	mov     [rdi], rbp
	dec     r8
	jz      .40
	lea     rsi, [rsi-8]
	lea     r15, [r15+8]
	mov     r11, r13
	mov     r12, [r15]
	mov     r9, [r15+8]
	jmp     r14

	xalign  16
.10:mov     ebx, r13d

	neg     r13
	mov     rax, [rsi+r13*8-8]
	mov     r12, [r15]
	mov     r9, [r15+8]
	mul     r9
	and     r13, -4
	mov     r11, r13
	and     ebx, 3
	jz      .12
	cmp     ebx, 2
	jc      .13
	jz      .14
.11:mov     rcx, rax
	mov     rbp, rdx
	lea     r14, [rel .23]
	jmp     .17

	xalign  16
.12:mov     rbx, rax
	mov     rcx, rdx
	lea     r14, [rel .20]
	jmp     .16

	xalign  16
.13:mov     r10, rax
	mov     rbx, rdx
	mov     ecx, 0
	lea     r14, [rel .21]
	jmp     .19

	xalign  16
.14:mov     rbp, rax
	mov     r10, rdx
	mov     ebx, 0
	mov     rax, [rsi+r11*8+16]
	lea     r14, [rel .22]
	jmp     .18

	xalign  16
.15:mov     rax, [rsi+r11*8-8]
	mul     r9
	add     rbx, rax
	adc     rcx, rdx
.16:mov     ebp, 0
	mov     rax, [rsi+r11*8]
	mul     r12
	add     rbx, rax
	mov     rax, [rsi+r11*8]
	adc     rcx, rdx
	adc     ebp, 0
	mul     r9
	add     rcx, rax
	mov     [rdi+r11*8], rbx
	adc     rbp, rdx
.17:mov     rax, [rsi+r11*8+8]
	mul     r12
	mov     r10d, 0
	add     rcx, rax
	adc     rbp, rdx
	mov     ebx, 0
	adc     r10d, 0
	mov     rax, [rsi+r11*8+8]
	mov     [rdi+r11*8+8], rcx
	mul     r9
	add     rbp, rax
	mov     rax, [rsi+r11*8+16]
	adc     r10, rdx
.18:mov     ecx, 0
	mul     r12
	add     rbp, rax
	mov     rax, [rsi+r11*8+16]
	adc     r10, rdx
	adc     ebx, 0
	mul     r9
	add     r10, rax
	mov     [rdi+r11*8+16], rbp
	adc     rbx, rdx
.19:mov     rax, [rsi+r11*8+24]
	mul     r12
	add     r10, rax
	adc     rbx, rdx
	adc     ecx, 0
	add     r11, 4
	mov     [rdi+r11*8-8], r10
	jnz     .15
	mov     [rdi], rbx
	mov     [rdi+8], rcx
	sub     r8, 2
	jz      .40
	lea     r15, [r15+16]
	lea     rsi, [rsi-16]
	mov     r11, r13
	mov     r12, [r15]
	mov     r9, [r15+8]
	jmp     r14

	xalign  16
.20:mov     rax, [rsi+r11*8-8]
	mul     r9
	mov     rcx, rax
	mov     rbp, rdx
	mov     r10d, 0
	jmp     .25

	xalign  16
.21:mov     rax, [rsi+r11*8+16]
	mul     r9
	mov     rbx, rax
	mov     rcx, rdx
	mov     ebp, 0
	mov     rax, [rsi+r11*8+24]
	jmp     .28

	xalign  16
.22:mov     rax, [rsi+r11*8+8]
	mul     r9
	mov     r10, rax
	mov     rbx, rdx
	mov     ecx, 0
	jmp     .27

	xalign  16
.23:mov     rax, [rsi+r11*8]
	mul     r9
	mov     rbp, rax
	mov     r10, rdx
	mov     ebx, 0
	mov     ecx, 0
	jmp     .26
	
	xalign  16
.24:mov     r10d, 0
	add     rbx, rax
	mov     rax, [rsi+r11*8-8]
	adc     rcx, rdx
	adc     ebp, 0
	mul     r9
	add     [rdi+r11*8-8], rbx
	adc     rcx, rax
	adc     rbp, rdx
.25:mov     rax, [rsi+r11*8]
	mul     r12
	add     rcx, rax
	mov     rax, [rsi+r11*8]
	adc     rbp, rdx
	adc     r10d, 0
	mul     r9
	add     [rdi+r11*8], rcx
	mov     ecx, 0
	adc     rbp, rax
	mov     ebx, 0
	adc     r10, rdx
.26:mov     rax, [rsi+r11*8+8]
	mul     r12
	add     rbp, rax
	mov     rax, [rsi+r11*8+8]
	adc     r10, rdx
	adc     ebx, 0
	mul     r9
	add     [rdi+r11*8+8], rbp
	adc     r10, rax
	adc     rbx, rdx
.27:mov     rax, [rsi+r11*8+16]
	mul     r12
	add     r10, rax
	mov     rax, [rsi+r11*8+16]
	adc     rbx, rdx
	adc     ecx, 0
	mul     r9
	add     [rdi+r11*8+16], r10
	nop ; < not translated >
	adc     rbx, rax
	mov     ebp, 0
	mov     rax, [rsi+r11*8+24]
	adc     rcx, rdx
.28:mul     r12
	add     r11, 4
	jnz     .24
	add     rbx, rax
	adc     rcx, rdx
	adc     ebp, 0
	add     [rdi-8], rbx
	adc     [rdi], rcx
	adc     [rdi+8], rbp
	sub     r8, 2
	jz      .40
	lea     r15, [r15+16]
	lea     rsi, [rsi-16]
	mov     r11, r13
	mov     r12, [r15]
	mov     r9, [r15+8]
	jmp     r14

	xalign  16
.29:xor     ebx, ebx
	xor     ecx, ecx
	xor     ebp, ebp
	neg     r13
	mov     eax, r8d
	and     eax, 3
	jz      .31
	cmp     eax, 2
	jc      .32
	jz      .33
.30:lea     r15, [r15-8]
	mov     r10, r15
	add     r8, 1
	mov     r11, r8
	lea     r14, [rel .36]
	jmp     .36
.31:mov     r10, r15
	mov     r11, r8
	lea     r14, [rip+0]
	mov     rax, [rsi+r11*8-8]
	jmp     .35
.32:lea     r15, [r15+8]
	mov     r10, r15
	add     r8, 3
	mov     r11, r8
	lea     r14, [rip+0]
	mov     rax, [r10-8]
	jmp     .38
.33:lea     r15, [r15-16]
	mov     r10, r15
	add     r8, 2
	mov     r11, r8
	lea     r14, [rip+0]
	mov     rax, [r10+16]
	jmp     .37

	xalign  16
.34:add     rbx, rax
	adc     rcx, rdx
	mov     rax, [rsi+r11*8-8]
	adc     rbp, 0
.35:mul     qword [r10]
	add     rbx, rax
	adc     rcx, rdx
	adc     rbp, 0
.36:mov     rax, [rsi+r11*8-16]
	mul     qword [r10+8]
	add     rbx, rax
	mov     rax, [r10+16]
	adc     rcx, rdx
	adc     rbp, 0
.37:mul     qword [rsi+r11*8-24]
	add     rbx, rax
	mov     rax, [r10+24]
	adc     rcx, rdx
	lea     r10, [r10+32]
	adc     rbp, 0
.38:mul     qword [rsi+r11*8-32]
	sub     r11, 4
	jnz     .34
	add     rbx, rax
	adc     rcx, rdx
	adc     rbp, 0
	mov     [rdi+r13*8], rbx
	inc     r13
	jz      .39
	mov     r11, r8
	mov     r10, r15
	lea     rsi, [rsi+8]
	mov     rbx, rcx
	mov     rcx, rbp
	xor     ebp, ebp
	jmp     r14
.39:mov     [rdi], rcx
	mov     [rdi+8], rbp
.40:WIN64_GCC_END

	end
