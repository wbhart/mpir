;  AMD64 mpn_mul_basecase optimised for Intel Haswell.

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

%include 'yasm_mac.inc'

%ifdef USE_WIN64
%else
%define rp       rdi
%define up       rsi
%define un_param rdx
%define vp       rcx

%define un       rbx
%define un8      bl

%define w0       r10
%define w1       r11
%define w2       r12
%define w3       r13
%define n        rbp
%endif

BITS    64

align 16

GLOBAL_FUNC mpn_mul_basecase
	push 	rbx
	push 	rbp
	push 	r12
	push 	r13
	push 	r14
	mov 	un, rdx
	neg 	un

	mov 	n, rdx
	sar 	n, 2

	test 	r8b, 1
	jz 	.Ldo_mul_2
	mov 	rdx, [vp]

.Ldo_mul_1:
	test 	un8, 1
	jnz 	.Lm1x1

.Lm1x0:
	test 	un8, 2
	jnz 	.Lm110

.Lm100:
	mulx 	w2, r14, [up]
	mulx 	w3, w1, [up+8]
	lea 	rp, [rp-24]
	jmp 	.Lm1l0

.Lm110:
	mulx 	r9, w3, [up]
	mulx 	r14, w1, [up+8]
	lea 	rp, [rp-8]
	test 	n, n
	jz 	.Lcj2
	mulx 	w2, w0, [up+16]
	lea 	up, [up+16]
	jmp 	.Lm1l2

.Lm1x1:
	test 	un8, 2
	jz 	.Lm111

.Lm101:
	mulx 	r14, r9, [up]
	lea 	rp, [rp-16]
	test 	n, n
	jz 	.Lcj1
	mulx 	w2, w0, [up+8]
	lea 	up, [up+8]
	jmp 	.Lm1l1

.Lm111:
	mulx 	w3, w2, [up]
	mulx 	r9, w0, [up+8]
	mulx 	r14, w1, [up+16]
	lea 	up, [up+24]
	test 	n, n
	jnz 	.Lgt3
	add 	w3, w0
	jmp 	.Lcj3

.Lgt3:
	add 	w3, w0
	jmp 	.Lm1l3


	align 32
.Lm1tp:
	lea 	rp, [rp+32]
.Lm1l3:
	mov 	[rp], w2
	mulx 	w2, w0, [up]
.Lm1l2:
	mov 	[rp+8], w3
	adc 	r9, w1
.Lm1l1:
	adc 	r14, w0
	mov 	[rp+16], r9
	mulx 	w3, w1, [up+8]
.Lm1l0:
	mov 	[rp+24], r14
	mulx 	r9, w0, [up+16]
	adc 	w2, w1
	mulx 	r14, w1, [up+24]
	adc 	w3, w0
	lea 	up, [up+32]
	dec 	n
	jnz 	.Lm1tp

.Lm1ed:
	lea 	rp, [rp+32]
.Lcj3:
	mov 	[rp], w2
.Lcj2:
	mov 	[rp+8], w3
	adc 	r9, w1
.Lcj1:
	mov 	[rp+16], r9
	adc 	r14, 0
	mov 	[rp+24], r14

	dec 	r8d
	jz 	.Lret5

	lea 	vp, [vp+8]
	lea 	rp, [rp+32]
	jmp 	.Ldo_addmul

.Ldo_mul_2:
	mov 	r9, [vp]
	mov 	r14, [vp+8]
	lea 	n, [un]
	sar 	n, 2
	test 	un8, 1
	jnz 	.Lm2x1

.Lm2x0:
	xor 	w0, w0
	test 	un8, 2
	mov 	rdx, [up]
	mulx 	w1, w2, r9
	jz 	.Lm2l0

.Lm210:
	lea 	rp, [rp-16]
	lea 	up, [up-16]
	jmp 	.Lm2l2

.Lm2x1:
	xor 	w2, w2
	test 	un8, 2
	mov 	rdx, [up]
	mulx 	w3, w0, r9
	jz 	.Lm211

.Lm201:
	lea 	rp, [rp-24]
	lea 	up, [up+8]
	jmp 	.Lm2l1

.Lm211:
	lea 	rp, [rp-8]
	lea 	up, [up-8]
	jmp 	.Lm2l3


	align 16
.Lm2tp:
	mulx 	w0, rax, r14
	add 	w2, rax
	mov 	rdx, [up]
	mulx 	w1, rax, r9
	adc 	w0, 0
	add 	w2, rax
	adc 	w1, 0
	add 	w2, w3
.Lm2l0:
	mov 	[rp], w2
	adc 	w1, 0
	mulx 	w2, rax, r14
	add 	w0, rax
	mov 	rdx, [up+8]
	adc 	w2, 0
	mulx 	w3, rax, r9
	add 	w0, rax
	adc 	w3, 0
	add 	w0, w1
.Lm2l3:
	mov 	[rp+8], w0
	adc 	w3, 0
	mulx 	w0, rax, r14
	add 	w2, rax
	mov 	rdx, [up+16]
	mulx 	w1, rax, r9
	adc 	w0, 0
	add 	w2, rax
	adc 	w1, 0
	add 	w2, w3
.Lm2l2:
	mov 	[rp+16], w2
	adc 	w1, 0
	mulx 	w2, rax, r14
	add 	w0, rax
	mov 	rdx, [up+24]
	adc 	w2, 0
	mulx 	w3, rax, r9
	add 	w0, rax
	adc 	w3, 0
	add 	w0, w1
	lea 	up, [up+32]
.Lm2l1:
	mov 	[rp+24], w0
	adc 	w3, 0
	inc 	n
	lea 	rp, [rp+32]
	jnz 	.Lm2tp

.Lm2ed:
	mulx 	rax, rdx, r14
	add 	w2, rdx
	adc 	rax, 0
	add 	w2, w3
	mov 	[rp], w2
	adc 	rax, 0
	mov 	[rp+8], rax
	add 	r8d, -2
	jz 	.Lret5
	lea 	vp, [vp+16]
	lea 	rp, [rp+16]

.Ldo_addmul:
	push 	r15
	push 	r8
	lea 	rp, [un*8+rp]
	lea 	up, [un*8+up]

.Louter:
	mov 	r9, [vp]
	mov 	r8, [vp+8]
	lea 	n, [un+2]
	sar 	n, 2
	mov 	rdx, [up]
	test 	un8, 1
	jnz 	.Lbx1

.Lbx0:
	mov 	r14, [rp]
	mov 	r15, [rp+8]
	mulx 	w1, rax, r9
	add 	r14, rax
	mulx 	w2, rax, r8
	adc 	w1, 0
	mov 	[rp], r14
	add 	r15, rax
	adc 	w2, 0
	mov 	rdx, [up+8]
	test 	un8, 2
	jnz 	.Lb10

.Lb00:
	lea 	up, [up+16]
	lea 	rp, [rp+16]
	jmp 	.Llo0

.Lb10:
	mov 	r14, [rp+16]
	lea 	up, [up+32]
	mulx 	w3, rax, r9
	jmp 	.Llo2

.Lbx1:
	mov 	r15, [rp]
	mov 	r14, [rp+8]
	mulx 	w3, rax, r9
	add 	r15, rax
	adc 	w3, 0
	mulx 	w0, rax, r8
	add 	r14, rax
	adc 	w0, 0
	mov 	rdx, [up+8]
	mov 	[rp], r15
	mulx 	w1, rax, r9
	test 	un8, 2
	jz 	.Lb11

.Lb01:
	mov 	r15, [rp+16]
	lea 	rp, [rp+24]
	lea 	up, [up+24]
	jmp 	.Llo1

.Lb11:
	lea 	rp, [rp+8]
	lea 	up, [up+8]
	jmp 	.Llo3


	align 16
.Ltop:
	mulx 	w3, rax, r9
	add 	r15, w0
	adc 	w2, 0
.Llo2:
	add 	r15, rax
	adc 	w3, 0
	mulx 	w0, rax, r8
	add 	r14, rax
	adc 	w0, 0
	lea 	rp, [rp+32]
	add 	r15, w1
	mov 	rdx, [up-16]
	mov 	[rp-24], r15
	adc 	w3, 0
	add 	r14, w2
	mov 	r15, [rp-8]
	mulx 	w1, rax, r9
	adc 	w0, 0
.Llo1:
	add 	r14, rax
	mulx 	w2, rax, r8
	adc 	w1, 0
	add 	r14, w3
	mov 	[rp-16], r14
	adc 	w1, 0
	add 	r15, rax
	adc 	w2, 0
	add 	r15, w0
	mov 	rdx, [up-8]
	adc 	w2, 0
.Llo0:
	mulx 	w3, rax, r9
	add 	r15, rax
	adc 	w3, 0
	mov 	r14, [rp]
	mulx 	w0, rax, r8
	add 	r14, rax
	adc 	w0, 0
	add 	r15, w1
	mov 	[rp-8], r15
	adc 	w3, 0
	mov 	rdx, [up]
	add 	r14, w2
	mulx 	w1, rax, r9
	adc 	w0, 0
.Llo3:
	add 	r14, rax
	adc 	w1, 0
	mulx 	w2, rax, r8
	add 	r14, w3
	mov 	r15, [rp+8]
	mov 	[rp], r14
	mov 	r14, [rp+16]
	adc 	w1, 0
	add 	r15, rax
	adc 	w2, 0
	mov 	rdx, [up+8]
	lea 	up, [up+32]
	inc 	n
	jnz 	.Ltop

.Lend:
	mulx 	w3, rax, r9
	add 	r15, w0
	adc 	w2, 0
	add 	r15, rax
	adc 	w3, 0
	mulx 	rax, rdx, r8
	add 	r15, w1
	mov 	[rp+8], r15
	adc 	w3, 0
	add 	rdx, w2
	adc 	rax, 0
	add 	rdx, w3
	mov 	[rp+16], rdx
	adc 	rax, 0
	mov 	[rp+24], rax

	add 	DWORD [rsp], -2
	lea 	vp, [vp+16]
	lea 	up, [up+un*8-16]
	lea 	rp, [rp+un*8+32]
	jnz 	.Louter

	pop 	rax
	pop 	r15
.Lret5:
	pop 	r14
.Lret4:
	pop 	r13
.Lret3:
	pop 	r12
.Lret2:
	pop 	rbp
	pop 	rbx

	ret
