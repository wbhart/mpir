; PROLOGUE(mpn_divrem_euclidean_qr_1)

;  x86-64 mpn_divrem_euclidean_qr_1 -- mpn by limb division.

;  Copyright 2004, 2005, 2007, 2008, 2009 Free Software Foundation, Inc.

;  Copyright Brian Gladman 2010 (Conversion to yasm format)

;  This file is part of the GNU MP Library.

;  The GNU MP Library is free software; you can redistribute it and/or modify
;  it under the terms of the GNU Lesser General Public License as published
;  by the Free Software Foundation; either version 3 of the License, or (at
;  your option) any later version.

;  The GNU MP Library is distributed in the hope that it will be useful, but
;  WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
;  or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public
;  License for more details.

;  You should have received a copy of the GNU Lesser General Public License

;	mp_limb_t mpn_divrem_euclidean_qr_1(mp_ptr, mp_size_t, mp_ptr, mp_size_t, mp_limb_t)
;   rax                                    rdi        rsi     rdx        rcx         r8
;   rax                                    rcx        rdx      r8         r9   [rsp+40]

;	mp_limb_t mpn_preinv_divrem_1(mp_ptr, mp_size_t, mp_ptr, mp_size_t, mp_limb_t, mp_limb_t,     int)
;   rax                              rdi        rsi     rdx        rcx         r8         r9   8(rsp)
;   rax                              rcx        rdx      r8         r9   [rsp+40]   [rsp+48] [rsp+56]

%include 'yasm_mac.inc'

    BITS 64
	TEXT

%define reg_save_list   rbx, rbp, rsi, rdi, r12, r13

%define SPECIAL_CODE_FOR_NORMALIZED_DIVISOR

	xalign  16
	WIN64_GCC_PROC mpn_divrem_euclidean_qr_1, 5, frame
	xor     eax, eax
	mov     r12, rsi
	mov     rbx, rcx
	add     rcx, rsi
	mov     rsi, rdx
	je      .17

	lea     rdi, [rdi+rcx*8-8]
	xor     ebp, ebp

%ifdef  SPECIAL_CODE_FOR_NORMALIZED_DIVISOR

	test    r8, r8
	jns     .6

	test    rbx, rbx
	je      .1
	mov     rbp, [rsi+rbx*8-8]
	dec     rbx
	mov     rax, rbp
	sub     rbp, r8
	cmovb   rbp, rax
	sbb     eax, eax
	inc     eax
	mov     [rdi], rax
	lea     rdi, [rdi-8]
.1: mov     rdx, r8
	mov     rax, -1
	not     rdx
	div     r8
	mov     r9, rax
	mov     rax, rbp
	jmp     .4

	xalign  16
.2: mov     r10, [rsi+rbx*8]
	lea     rbp, [rax+1]
	mul     r9
	add     rax, r10
	adc     rdx, rbp
	mov     rbp, rax
	mov     r13, rdx
	imul    rdx, r8
	sub     r10, rdx
	mov     rax, r8
	add     rax, r10
	cmp     r10, rbp
	cmovb   rax, r10
	adc     r13, -1
	cmp     rax, r8
	jae     .5
.3: mov     [rdi], r13
	sub     rdi, 8
.4: dec     rbx
	jns     .2
	xor     ecx, ecx
	jmp     .14
.5: sub     rax, r8
	inc     r13
	jmp     .3
	
%endif

.6: test    rbx, rbx
	je      .7
	mov     rax, [rsi+rbx*8-8]
	cmp     rax, r8
	jae     .7
	mov     [rdi], rbp
	mov     rbp, rax
	lea     rdi, [rdi-8]
	je      .17
	dec     rbx
.7: bsr     rcx, r8
	not     ecx
	sal     r8, cl
	sal     rbp, cl
	mov     rdx, r8
	mov     rax, -1
	not     rdx
	div     r8
	test    rbx, rbx
	mov     r9, rax
	mov     rax, rbp
	je      .14
	mov     rbp, [rsi+rbx*8-8]
	shr     rax, cl
	shld    rax, rbp, cl
	sub     rbx, 2
	js      .10

	xalign  16
.8: nop
	mov     r10, [rsi+rbx*8]
	lea     r11, [rax+1]
	shld    rbp, r10, cl
	mul     r9
	add     rax, rbp
	adc     rdx, r11
	mov     r11, rax
	mov     r13, rdx
	imul    rdx, r8
	sub     rbp, rdx
	mov     rax, r8
	add     rax, rbp
	cmp     rbp, r11
	cmovb   rax, rbp
	adc     r13, -1
	cmp     rax, r8
	jae     .12
.9: mov     [rdi], r13
	sub     rdi, 8
	dec     rbx
	mov     rbp, r10
	jns     .8
.10:lea     r11, [rax+1]
	sal     rbp, cl
	mul     r9
	add     rax, rbp
	adc     rdx, r11
	mov     r11, rax
	mov     r13, rdx
	imul    rdx, r8
	sub     rbp, rdx
	mov     rax, r8
	add     rax, rbp
	cmp     rbp, r11
	cmovb   rax, rbp
	adc     r13, -1
	cmp     rax, r8
	jae     .13
.11:mov     [rdi], r13
	sub     rdi, 8
	jmp     .14
.12:sub     rax, r8
	inc     r13
	jmp     .9
.13:sub     rax, r8
	inc     r13
	jmp     .11
.14:mov     rbp, r8
	neg     rbp
	jmp     .16

	xalign  16
.15:lea     r11, [rax+1]
	mul     r9
	add     rdx, r11
	mov     r11, rax
	mov     r13, rdx
	imul    rdx, rbp
	mov     rax, r8
	add     rax, rdx
	cmp     rdx, r11
	cmovb   rax, rdx
	adc     r13, -1
	mov     [rdi], r13
	sub     rdi, 8
.16:dec     r12
	jns     .15
	shr     rax, cl
.17:WIN64_GCC_END

%ifndef EXCLUDE_PREINV

	xalign  16
	WIN64_GCC_PROC mpn_preinv_divrem_1, 7, frame
	xor     eax, eax
	mov     r12, rsi
	mov     rbx, rcx
	add     rcx, rsi
	mov     rsi, rdx
	lea     rdi, [rdi+rcx*8-8]

	test    r8, r8
	js      .3
	mov     cl, [rsp+stack_use+0x38]
	shl     r8, cl
	jmp     .7
	
%ifdef  SPECIAL_CODE_FOR_NORMALIZED_DIVISOR

	xalign  16
.1: mov     r10, [rsi+rbx*8]
	lea     rbp, [rax+1]
	mul     r9
	add     rax, r10
	adc     rdx, rbp
	mov     rbp, rax
	mov     r13, rdx
	imul    rdx, r8
	sub     r10, rdx
	mov     rax, r8
	add     rax, r10
	cmp     r10, rbp
	cmovb   rax, r10
	adc     r13, -1
	cmp     rax, r8
	jae     .4
.2: mov     [rdi], r13
	sub     rdi, 8
.3: dec     rbx
	jns     .1
	xor     ecx, ecx
	jmp     .14
.4: sub     rax, r8
	inc     r13
	jmp     .2
	
%endif

.5: test    rbx, rbx
	je      .6
	mov     rax, [rsi+rbx*8-8]
	cmp     rax, r8
	jae     .6
	mov     [rdi], rbp
	mov     rbp, rax
	lea     rdi, [rdi-8]
	je      .17
	dec     rbx
.6: bsr     rcx, r8
	not     ecx
	sal     r8, cl
	sal     rbp, cl
	mov     rdx, r8
	mov     rax, -1
	not     rdx
	div     r8
	test    rbx, rbx
	mov     r9, rax
	mov     rax, rbp
	je      .14
.7: mov     rbp, [rsi+rbx*8-8]
	shr     rax, cl
	shld    rax, rbp, cl
	sub     rbx, 2
	js      .10

	xalign  16
.8: nop
	mov     r10, [rsi+rbx*8]
	lea     r11, [rax+1]
	shld    rbp, r10, cl
	mul     r9
	add     rax, rbp
	adc     rdx, r11
	mov     r11, rax
	mov     r13, rdx
	imul    rdx, r8
	sub     rbp, rdx
	mov     rax, r8
	add     rax, rbp
	cmp     rbp, r11
	cmovb   rax, rbp
	adc     r13, -1
	cmp     rax, r8
	jae     .12
.9: mov     [rdi], r13
	sub     rdi, 8
	dec     rbx
	mov     rbp, r10
	jns     .8
.10:lea     r11, [rax+1]
	sal     rbp, cl
	mul     r9
	add     rax, rbp
	adc     rdx, r11
	mov     r11, rax
	mov     r13, rdx
	imul    rdx, r8
	sub     rbp, rdx
	mov     rax, r8
	add     rax, rbp
	cmp     rbp, r11
	cmovb   rax, rbp
	adc     r13, -1
	cmp     rax, r8
	jae     .13
.11:mov     [rdi], r13
	sub     rdi, 8
	jmp     .14
.12:sub     rax, r8
	inc     r13
	jmp     .9
.13:sub     rax, r8
	inc     r13
	jmp     .11
.14:mov     rbp, r8
	neg     rbp
	jmp     .16

	xalign  16
.15:lea     r11, [rax+1]
	mul     r9
	add     rdx, r11
	mov     r11, rax
	mov     r13, rdx
	imul    rdx, rbp
	mov     rax, r8
	add     rax, rdx
	cmp     rdx, r11
	cmovb   rax, rdx
	adc     r13, -1
	mov     [rdi], r13
	sub     rdi, 8
.16:dec     r12
	jns     .15
	shr     rax, cl
.17:WIN64_GCC_END

%endif

    end
