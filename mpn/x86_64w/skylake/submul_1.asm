;  AMD64 mpn_submul_1 optimised for Intel Haswell.

;  Contributed to the GNU project by Torbjörn Granlund.
;  Converted to MPIR by Alexander Kruppa.

;  Copyright 2013 Free Software Foundation, Inc.

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
;  mp_limb_t mpn_submul_1(mp_ptr, mp_ptr, mp_size_t, mp_limb_t)
;  rax                       rdi     rsi        rdx        rcx
;  rax                       rcx     rdx         r8        r9d

%include 'yasm_mac.inc'

BITS 64

%define reg_save_list rbx, rbp, rsi, rdi, r12, r13
%define RP      rdi
%define S1P     rsi
%define Size    rbp
%define Sizeb   bpl
%define Limb    rcx

%define Tmp0    r12
%define Tmp1    r13
%define Tmp2    rax
%define Tmp3    rbx
%define Tmp4    r8
%define Tmp5    r9
%define Tmp6    r10
%define Tmp7    r11
%define Tmp8    rcx

%define ADDSUB sub
%define ADCSBB sbb

align 16

FRAME_PROC mpn_submul_1, 0, reg_save_list
    mov     rdi, rcx 
    mov     rsi, rdx
	mov 	rbp, r8 ; mulx requires one input in rdx
	mov 	rdx, r9

	test 	Sizeb, 1
	jnz 	.Lbx1

.Lbx0:  shr 	Size, 2
	jc 	.Lb10 ;ajs:notshortform

.Lb00:	mulx 	Tmp0, Tmp1, [S1P]
	mulx 	Tmp2, Tmp3, [S1P+8]
	add 	Tmp3, Tmp0
	adc 	Tmp2, 0
	mov 	Tmp0, [RP]
	mov 	Tmp8, [RP+8]
	mulx 	Tmp4, Tmp5, [S1P+16]
	lea 	RP, [RP-16]
	lea 	S1P, [S1P+16]
	ADDSUB 	Tmp0, Tmp1
	jmp 	.Llo0 ;ajs:notshortform

.Lbx1:	shr 	Size, 2
	jc 	.Lb11

.Lb01:	mulx 	Tmp6, Tmp7, [S1P]
	jnz 	.Lgt1
.Ln1:	ADDSUB 	[RP], Tmp7
	mov 	eax, 0
	adc 	Tmp2, Tmp6
	jmp 	.Lret ;ajs:notshortform

.Lgt1:	mulx 	Tmp0, Tmp1, [S1P+8]
	mulx 	Tmp2, Tmp3, [S1P+16]
	lea 	S1P, [S1P+24]
	add 	Tmp1, Tmp6
	adc 	Tmp3, Tmp0
	adc 	Tmp2, 0
	mov 	Tmp6, [RP]
	mov 	Tmp0, [RP+8]
	mov 	Tmp8, [RP+16]
	lea 	RP, [RP-8]
	ADDSUB 	Tmp6, Tmp7
	jmp 	.Llo1

.Lb11:	mulx 	Tmp2, Tmp3, [S1P]
	mov 	Tmp8, [RP]
	mulx 	Tmp4, Tmp5, [S1P+8]
	lea 	S1P, [S1P+8]
	lea 	RP, [RP-24]
	inc 	Size	
	ADDSUB 	Tmp8, Tmp3
	jmp 	.Llo3

.Lb10:	mulx 	Tmp4, Tmp5, [S1P]
	mulx 	Tmp6, Tmp7, [S1P+8]
	lea 	RP, [RP-32]
	mov 	eax, 0
	clc
	jz 	.Lend ;ajs:notshortform	

	align 16
.Ltop:	adc 	Tmp5, Tmp2
	lea 	RP, [RP+32]
	adc 	Tmp7, Tmp4
	mulx 	Tmp0, Tmp1, [S1P+16]
	mov 	Tmp4, [RP]
	mulx 	Tmp2, Tmp3, [S1P+24]
	lea 	S1P, [S1P+32]
	adc 	Tmp1, Tmp6
	adc 	Tmp3, Tmp0
	adc 	Tmp2, 0
	mov 	Tmp6, [RP+8]
	mov 	Tmp0, [RP+16]
	ADDSUB 	Tmp4, Tmp5
	mov 	Tmp8, [RP+24]
	mov 	[RP], Tmp4
	ADCSBB 	Tmp6, Tmp7
.Llo1:	mulx 	Tmp4, Tmp5, [S1P]
	mov 	[RP+8], Tmp6
	ADCSBB 	Tmp0, Tmp1
.Llo0:	mov 	[RP+16], Tmp0
	ADCSBB 	Tmp8, Tmp3
.Llo3:	mulx 	Tmp6, Tmp7, [S1P+8]
	mov 	[RP+24], Tmp8
	dec 	Size
	jnz 	.Ltop

.Lend:	adc 	Tmp5, Tmp2
	adc 	Tmp7, Tmp4
	mov 	Tmp4, [RP+32]
	mov 	Tmp2, Tmp6
	adc 	Tmp2, 0
	mov 	Tmp6, [RP+40]
	ADDSUB 	Tmp4, Tmp5
	mov 	[RP+32], Tmp4
	ADCSBB 	Tmp6, Tmp7
	mov 	[RP+40], Tmp6
	adc 	Tmp2, 0

.Lret:
    END_PROC reg_save_list
