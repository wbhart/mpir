;  x86-64 mpn_divrem_euclidean_qr_1 -- mpn by limb division.

;  Copyright 2004, 2005, 2007, 2008, 2009 Free Software Foundation, Inc.

;  Copyright 2010 Brian Gladman (Conversion to yasm format)

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

; mp_limb_t
; mpn_divrem_euclidean_qr_1 (mp_ptr qp, mp_size_t fn,
;               mp_srcptr np, mp_size_t nn, mp_limb_t d)

; mp_limb_t
; mpn_preinv_divrem_1 (mp_ptr qp, mp_size_t fn,
;                      mp_srcptr np, mp_size_t nn, mp_limb_t d,
;                      mp_limb_t dinv, int shift)

;		norm	unorm	frac
; k8		13	13	12
; netburst	44.2	44.2	42.3
; core2	25	24.5	19.3
; nehalem	21.5	20.7	18
; atom	       42	52	37

; INPUT PARAMETERS
; qp 	%rdi
; fn 	%rsi
; np	%rdx
; nn 	%rcx
; d    %r8
; dinv %r9		 	only for mpn_preinv_divrem_1
; shift passed on stack	only for mpn_preinv_divrem_1

%include 'yasm_mac.inc'

%define SPECIAL_CODE_FOR_NORMALIZED_DIVISOR

	TEXT
	align  16
GLOBAL_FUNC mpn_preinv_divrem_1

	xor     eax, eax
	push    r13
	push    r12
	push    rbp
	push    rbx

	mov     r12, rsi
	mov     rbx, rcx
	add     rcx, rsi
	mov     rsi, rdx

	lea     rdi, [rdi+rcx*8-8]

	test    r8, r8
	js      L_nent
	mov     cl, [rsp+40]
	shl     r8, cl
	jmp     L_uent

	align  16
GLOBAL_FUNC mpn_divrem_euclidean_qr_1
	xor     eax, eax
	push    r13
	push    r12
	push    rbp
	push    rbx

	mov     r12, rsi
	mov     rbx, rcx
	add     rcx, rsi
	mov     rsi, rdx
	je      L_ret

	lea     rdi, [rdi+rcx*8-8]
	xor     ebp, ebp

%ifdef  SPECIAL_CODE_FOR_NORMALIZED_DIVISOR

	test    r8, r8
	jns     L_unnormalized

L_normalized:
	test    rbx, rbx
	je      L_8
	mov     rbp, [rsi+rbx*8-8]
	dec     rbx
	mov     rax, rbp
	sub     rbp, r8
	cmovb   rbp, rax
	sbb     eax, eax
	inc     eax
	mov     [rdi], rax
	lea     rdi, [rdi-8]
L_8:
	mov     rdx, r8
	mov     rax, -1
	not     rdx
	div     r8
	mov     r9, rax
	mov     rax, rbp
	jmp     L_nent

	align  16
L_nloop:
	mov     r10, [rsi+rbx*8]
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
	jae     L_nfx
L_nok:
	mov	 [rdi], r13
       sub     rdi, 8
L_nent:
	dec	 rbx
	jns     L_nloop

	xor     ecx, ecx
	jmp     L_87

L_nfx:
	sub	 rax, r8
	inc     r13
	jmp     L_nok

%endif

L_unnormalized:
	test    rbx, rbx
	je      L_44
	mov     rax, [rsi+rbx*8-8]
	cmp     rax, r8
	jae     L_44
	mov     [rdi], rbp
	mov     rbp, rax
	lea     rdi, [rdi-8]
	je      L_ret
	dec     rbx
L_44:
	bsr     rcx, r8
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
	je      L_87
L_uent:
	mov     rbp, [rsi+rbx*8-8]
	shr     rax, cl
	shld    rax, rbp, cl
	sub     rbx, 2
	js      L_ulast

	align  16
L_uloop:
	nop
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
	jae     L_ufx
L_uok:
	mov	 [rdi], r13
	sub     rdi, 8
	dec     rbx
	mov     rbp, r10
	jns     L_uloop
L_ulast:
	lea     r11, [rax+1]
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
	jae     L_93
L_69:   
	mov	 [rdi], r13
       sub     rdi, 8
	jmp     L_87

L_ufx:  
	sub	 rax, r8
	inc     r13
	jmp     L_uok

L_93:   
	sub	 rax, r8
	inc     r13
	jmp     L_69

L_87:   
	mov	 rbp, r8
	neg     rbp
	jmp     L_87b

	align  16
L_floop:
	lea     r11, [rax+1]
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
L_87b:
	dec	 r12
	jns     L_floop

	shr     rax, cl
L_ret:
	pop	 rbx
	pop     rbp
	pop     r12
	pop     r13
	ret

	end

