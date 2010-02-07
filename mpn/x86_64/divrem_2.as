
;  x86-64 mpn_divrem_2 -- Divide an mpn number by a normalized 2-limb number.

;  Copyright 2007, 2008 Free Software Foundation, Inc.

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

;		norm	frac
; k8		20	20
; netburst	73	73
; core2	37	37
; nehalem	33	33

; INPUT PARAMETERS
; qp 	rdi
; fn 	rsi
; np	rdx
; nn 	rcx
; dp	r8
; dinv r9

%include 'yasm_mac.inc'
    BITS 64
	TEXT

%define reg_save_list   rbx, rbp, rsi, rdi, r12, r13, r14, r15

    align  16
GLOBAL_FUNC mpn_divrem_2

	push    r15
	lea     rax, [rdx+rcx*8]
	push    r14
	push    r13
	mov     r13, rsi
	push    r12
	lea     r12, [rax-24]
	push    rbp
	mov     rbp, rdi
	push    rbx
	mov     r11, [r8+8]
	mov     r9, [rax-8]
	mov     r8, [r8]
	mov     r10, [rax-16]
	xor     r15d, r15d
	cmp     r11, r9
	ja      L_2
	setb    dl
	cmp     r8, r10
	setbe   al
	or      dl, al
	jne     L_23
L_2:
	lea     rbx, [rcx+r13-3]
	test    rbx, rbx
	js      L_6
	mov     rdx, r11
	mov     rax, -1
	not     rdx
	div     r11
	mov     rdx, r11
	mov     rdi, rax
	imul    rdx, rax
	mov     r14, rdx
	mul     r8
	mov     rcx, rdx
	mov     rdx, -1
	add     r14, r8
	adc     rdx, 0
	add     r14, rcx
	adc     rdx, 0
	js      L_8
L_18:
	dec     rdi
	sub     r14, r11
	sbb     rdx, 0
	jns     L_18
L_8:

%ifdef NEW
	lea     rbp, [rbp+rbx*8]
	mov     rcx, rbx
	mov     rbx, r9
	mov     r9, rdi
	mov     r14, r10
	mov     rsi, r11
	neg     rsi
	align  16
L_loop:
	mov     rax, r9
	mul     rbx
	add     rax, r14
	mov     r10, rax
	adc     rdx, rbx
	mov     rdi, rdx
	imul    rdx, rsi
	mov     rax, r8
	lea     rbx, [rdx+r14]
	mul     rdi
	xor     r14d, r14d
	cmp     r13, rcx
	jg      L_19
	mov     r14, [r12]
	sub     r12, 8
L_19:
	sub	 r14, r8
	sbb     rbx, r11
	sub     r14, rax
	sbb     rbx, rdx
	inc     rdi
	xor     edx, edx
	cmp     rbx, r10
	mov     rax, r8
	adc     rdx, -1
	add     rdi, rdx
	and     rax, rdx
	and     rdx, r11
	add     r14, rax
	adc     rbx, rdx
	cmp     rbx, r11
	jae     L_fix
L_bck:
	mov	 [rbp], rdi 
	sub     rbp, 8
	dec     rcx
	jns     L_loop

	mov     r10, r14
	mov     r9, rbx

%else

	lea     rbp, [rbp+rbx*8]
	mov     rcx, rbx
	mov     rax, r9
	mov     rsi, r10
	align  16
L_loop:
	mov     r14, rax
	mul     rdi
	mov     r9, r11
	add     rax, rsi
	mov     rbx, rax
	adc     rdx, r14
	lea     r10, [rdx+1]
	mov     rax, rdx
	imul    r9, rdx
	sub     rsi, r9
	xor     r9d, r9d
	mul     r8
	cmp     r13, rcx
	jg      L_13
	mov     r9, [r12]
	sub     r12, 8
L_13:
	sub	 r9, r8
	sbb     rsi, r11
	sub     r9, rax
	sbb     rsi, rdx
	cmp     rsi, rbx
	sbb     rax, rax
	not     rax
	add     r10, rax
	mov     rbx, r8
	and     rbx, rax
	and     rax, r11
	add     r9, rbx
	adc     rax, rsi
	cmp     r11, rax
	jbe     L_fix
L_bck:
	mov	 [rbp], r10
	sub     rbp, 8
	mov     rsi, r9
	dec     rcx
	jns     L_loop

	mov     r10, rsi
	mov     r9, rax

%endif

L_6:
	mov     [r12+8], r10
	mov     [r12+16], r9
	pop     rbx
	pop     rbp
	pop     r12
	pop     r13
	pop     r14
	mov     rax, r15
	pop     r15
	ret

L_23:
	inc     r15d
	sub     r10, r8
	sbb     r9, r11
	jmp     L_2

%ifdef NEW

L_fix:
	seta	 dl
	cmp     r14, r8
	setae   al
	orb     al, dl
	je      L_bck
	inc     rdi
	sub     r14, r8
	sbb     rbx, r11
	jmp     L_bck

%else
L_fix:
	jb	 L_88
	cmp     r9, r8
	jb      L_bck
L_88:
	inc	 r10
	sub     r9, r8
	sbb     rax, r11
	jmp     L_bck

%endif
	