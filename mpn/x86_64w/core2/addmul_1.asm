;  x86-64 mpn_addmul_1 and mpn_submul_1, optimized for "Core 2".

;  Copyright 2003, 2004, 2005, 2007, 2008, 2009, 2011, 2012 Free Software
;  Foundation, Inc.

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
;  along with the GNU MP Library.  If not, see http://www.gnu.org/licenses/.

; mp_limb_t mpn_addmul_1 (mp_ptr, mp_srcptr, mp_size_t, mp_limb_t)
;  rax                       rdi        rsi        rdx        rcx
;  rax                       rcx        rdx         r8         r9

%include 'yasm_mac.inc'

%define reg_save_list   rbx, rbp, rsi, rdi

	TEXT

	xalign  16
	WIN64_GCC_PROC mpn_addmul_1c, 4
	lea     rbx, [rdx]
	neg     rbx
	mov     rax, [rsi]
	mov     r10, [rdi]
	lea     rdi, [rdi+rdx*8-16]
	lea     rsi, [rsi+rdx*8]
	mul     rcx
	add     rax, r8
	adc     rdx, 0
	bt      ebx, 0
	jc      .1
	lea     r11, [rax]
	mov     rax, [rsi+rbx*8+8]
	lea     rbp, [rdx]
	mul     rcx
	add     rbx, 2
	jns     .5
	lea     r8, [rax]
	mov     rax, [rsi+rbx*8]
	lea     r9, [rdx]
	jmp     .3
.1:	add     rbx, 1
	jns     .6
	lea     r8, [rax]
	mov     rax, [rsi+rbx*8]
	lea     r9, [rdx]
	mul     rcx
	lea     r11, [rax]
	mov     rax, [rsi+rbx*8+8]
	lea     rbp, [rdx]
	jmp     .4

	xalign  16
.2:	mul     rcx
	add     r10, r8
	lea     r8, [rax]
	mov     rax, [rsi+rbx*8]
	adc     r11, r9
	mov     [rdi+rbx*8-8], r10
	mov     r10, [rdi+rbx*8]
	lea     r9, [rdx]
	adc     rbp, 0
.3:	mul     rcx
	add     r10, r11
	lea     r11, [rax]
	mov     rax, [rsi+rbx*8+8]
	adc     r8, rbp
	mov     [rdi+rbx*8], r10
	mov     r10, [rdi+rbx*8+8]
	lea     rbp, [rdx]
	adc     r9, 0
.4:	add     rbx, 2
	js      .2
	mul     rcx
	add     r10, r8
	adc     r11, r9
	mov     [rdi-8], r10
	adc     rbp, 0
.5:	mov     r10, [rdi]
	add     r10, r11
	adc     rax, rbp
	mov     [rdi], r10
	adc     rdx, 0
.6:	mov     r10, [rdi+8]
	add     r10, rax
	mov     [rdi+8], r10
	mov     eax, ebx
	adc     rax, rdx
	WIN64_GCC_END

	xalign  16
	WIN64_GCC_PROC mpn_addmul_1, 4
	lea     rbx, [rdx]
	neg     rbx
	mov     rax, [rsi]
	mov     r10, [rdi]
	lea     rdi, [rdi+rdx*8-16]
	lea     rsi, [rsi+rdx*8]
	mul     rcx
	bt      ebx, 0
	jc      .1
	lea     r11, [rax]
	mov     rax, [rsi+rbx*8+8]
	lea     rbp, [rdx]
	mul     rcx
	add     rbx, 2
	jns     .5
	lea     r8, [rax]
	mov     rax, [rsi+rbx*8]
	lea     r9, [rdx]
	jmp     .3
.1:	add     rbx, 1
	jns     .6
	lea     r8, [rax]
	mov     rax, [rsi+rbx*8]
	lea     r9, [rdx]
	mul     rcx
	lea     r11, [rax]
	mov     rax, [rsi+rbx*8+8]
	lea     rbp, [rdx]
	jmp     .4

	xalign  16
.2:	mul     rcx
	add     r10, r8
	lea     r8, [rax]
	mov     rax, [rsi+rbx*8]
	adc     r11, r9
	mov     [rdi+rbx*8-8], r10
	mov     r10, [rdi+rbx*8]
	lea     r9, [rdx]
	adc     rbp, 0
.3:	mul     rcx
	add     r10, r11
	lea     r11, [rax]
	mov     rax, [rsi+rbx*8+8]
	adc     r8, rbp
	mov     [rdi+rbx*8], r10
	mov     r10, [rdi+rbx*8+8]
	lea     rbp, [rdx]
	adc     r9, 0
.4:	add     rbx, 2
	js      .2
	mul     rcx
	add     r10, r8
	adc     r11, r9
	mov     [rdi-8], r10
	adc     rbp, 0
.5:	mov     r10, [rdi]
	add     r10, r11
	adc     rax, rbp
	mov     [rdi], r10
	adc     rdx, 0
.6:	mov     r10, [rdi+8]
	add     r10, rax
	mov     [rdi+8], r10
	mov     eax, ebx
	adc     rax, rdx
	WIN64_GCC_END

	end
