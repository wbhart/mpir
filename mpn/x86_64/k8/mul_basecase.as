
;  AMD64 mpn_mul_basecase
;  Copyright 2008,2009 Jason Moxham
;  This file is part of the MPIR Library.
;  The MPIR Library is free software; you can redistribute it and/or modify
;  it under the terms of the GNU Lesser General Public License as published
;  by the Free Software Foundation; either version 2.1 of the License, or (at
;  your option) any later version.
;  The MPIR Library is distributed in the hope that it will be useful, but
;  WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
;  or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public
;  License for more details.
;  You should have received a copy of the GNU Lesser General Public License
;  along with the MPIR Library; see the file COPYING.LIB.  If not, write
;  to the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
;  Boston, MA 02110-1301, USA.

;	(rdi, rdx + r8) = (rsi, rdx)*(rcx, r8)
;	Version 1.0.5
;	same as the addmul for now
;	changes from standard mul
;	change  r8 to r12   and rcx to r13
;	reemove ret and write last limb

%include 'yasm_mac.inc'

    BITS    64

%macro mulloop 1
	align   16
%%1:
	mov     r10, 0
	mul     r13
	mov     [rdi+r11*8], r12
	add     r9, rax
	db      0x26
	adc     r10, rdx
	mov     rax, [rsi+r11*8+16]
	mul     r13
	mov     [rdi+r11*8+8], r9
	add     r10, rax
	mov     ebx, 0
	adc     rbx, rdx
	mov     rax, [rsi+r11*8+24]
	mov     r12, 0
	mov     r9, 0
	mul     r13
	mov     [rdi+r11*8+16], r10
	db      0x26
	add     rbx, rax
	db      0x26
	adc     r12, rdx
	mov     rax, [rsi+r11*8+32]
	mul     r13
	mov     [rdi+r11*8+24], rbx
	db      0x26
	add     r12, rax
	db      0x26
	adc     r9, rdx
	add     r11, 4
	mov     rax, [rsi+r11*8+8]
	jnc     %%1
%endmacro

%macro mulnext0 0
	mov     rax, [rsi+r11*8+16]
	mul     r13
	mov     [rdi+r11*8+8], r9
	add     r10, rax
	mov     ebx, 0
	adc     rbx, rdx
	mov     rax, [rsi+r11*8+24]
	mov     r12d, 0
	mul     r13
	mov     [rdi+r11*8+16], r10
	add     rbx, rax
	adc     r12, rdx
	mov     rax, [rsi+r11*8+32]
	mul     r13
	mov     [rdi+r11*8+24], rbx
	add     r12, rax
	adc     rdx, 0
	mov     [rdi+r11*8+32], r12
	mov     [rdi+r11*8+40], rdx
	inc     r8
	mov     rax, [rsi+r14*8]
	mov     r11, r14
%endmacro

%macro mulnext1 0
	mov     rax, [rsi+r11*8+16]
	mul     r13
	mov     [rdi+r11*8+8], r9
	add     r10, rax
	mov     r12d, 0
	adc     r12, rdx
	mov     rax, [rsi+r11*8+24]
	mul     r13
	mov     [rdi+r11*8+16], r10
	add     r12, rax
	adc     rdx, 0
	mov     [rdi+r11*8+24], r12
	mov     [rdi+r11*8+32], rdx
	inc     r8
	lea     rdi, [rdi+8]
%endmacro

%macro mulnext2 0
	mov     rax, [rsi+r11*8+16]
	mul     r13
	mov     [rdi+r11*8+8], r9
	add     r10, rax
	mov     ebx, 0
	adc     rbx, rdx
	mov     [rdi+r11*8+16], r10
	mov     [rdi+r11*8+24], rbx
	inc     r8
	mov     rax, [rsi+r14*8]
	mov     r11, r14
%endmacro

%macro mulnext3 0
	mov     [rdi+r11*8+8], r9
	mov     [rdi+r11*8+16], r10
	inc     r8
	lea     rdi, [rdi+8]
%endmacro

;	changes from standard addmul
;	change  r8 to r12   and rcx to r13
;	reemove ret and write last limb
%macro addmulloop 1
	align   16
%%1:
	mov     r10, 0
	mul     r13
	add     [rdi+r11*8], r12
	adc     r9, rax
	db      0x26
	adc     r10, rdx
	mov     rax, [rsi+r11*8+16]
	mul     r13
	add     [rdi+r11*8+8], r9
	adc     r10, rax
	mov     ebx, 0
	adc     rbx, rdx
	mov     rax, [rsi+r11*8+24]
	mov     r12, 0
	mov     r9, 0
	mul     r13
	add     [rdi+r11*8+16], r10
	db      0x26
	adc     rbx, rax
	db      0x26
	adc     r12, rdx
	mov     rax, [rsi+r11*8+32]
	mul     r13
	add     [rdi+r11*8+24], rbx
	db      0x26
	adc     r12, rax
	db      0x26
	adc     r9, rdx
	add     r11, 4
	mov     rax, [rsi+r11*8+8]
	jnc     %%1
%endmacro

%macro addmulpro0 0
	mov     r13, [rcx+r8*8]
	lea     rdi, [rdi+8]
	mul     r13
	mov     r12, rax
	mov     rax, [rsi+r14*8+8]
	mov     r9, rdx
	cmp     r14, 0
%endmacro

%macro addmulnext0 0
	mov     r10d, 0
	mul     r13
	add     [rdi+r11*8], r12
	adc     r9, rax
	adc     r10, rdx
	mov     rax, [rsi+r11*8+16]
	mul     r13
	add     [rdi+r11*8+8], r9
	adc     r10, rax
	mov     ebx, 0
	adc     rbx, rdx
	mov     rax, [rsi+r11*8+24]
	mov     r12d, 0
	mul     r13
	add     [rdi+r11*8+16], r10
	adc     rbx, rax
	adc     r12, rdx
	mov     rax, [rsi+r11*8+32]
	mul     r13
	add     [rdi+r11*8+24], rbx
	adc     r12, rax
	adc     rdx, 0
	add     [rdi+r11*8+32], r12
	mov     rax, [rsi+r14*8]
	adc     rdx, 0
	inc     r8
	mov     [rdi+r11*8+40], rdx
	mov     r11, r14
%endmacro

%macro addmulpro1 0
	mov     rax, [rsi+r14*8]
	mov     r13, [rcx+r8*8]
	mov     r11, r14
	mul     r13
	mov     r12, rax
	mov     rax, [rsi+r14*8+8]
	mov     r9, rdx
	cmp     r14, 0
%endmacro

%macro addmulnext1 0
	mov     r10d, 0
	mul     r13
	add     [rdi+r11*8], r12
	adc     r9, rax
	adc     r10, rdx
	mov     rax, [rsi+r11*8+16]
	mul     r13
	add     [rdi+r11*8+8], r9
	adc     r10, rax
	mov     r12d, 0
	adc     r12, rdx
	mov     rax, [rsi+r11*8+24]
	mul     r13
	add     [rdi+r11*8+16], r10
	adc     r12, rax
	adc     rdx, 0
	add     [rdi+r11*8+24], r12
	adc     rdx, 0
	mov     [rdi+r11*8+32], rdx
	inc     r8
	lea     rdi, [rdi+8]
%endmacro

%macro addmulpro2 0
	mov     r13, [rcx+r8*8]
	lea     rdi, [rdi+8]
	mul     r13
	mov     r12, rax
	mov     rax, [rsi+r14*8+8]
	mov     r9, rdx
	cmp     r14, 0
%endmacro

%macro addmulnext2 0
	mov     r10d, 0
	mul     r13
	add     [rdi+r11*8], r12
	adc     r9, rax
	adc     r10, rdx
	mov     rax, [rsi+r11*8+16]
	mul     r13
	mov     ebx, 0
	add     [rdi+r11*8+8], r9
	adc     r10, rax
	adc     rbx, rdx
	mov     rax, [rsi+r14*8]
	add     [rdi+r11*8+16], r10
	adc     rbx, 0
	mov     [rdi+r11*8+24], rbx
	inc     r8
	mov     r11, r14
%endmacro

%macro addmulpro3 0
	mov     rax, [rsi+r14*8]
	mov     r13, [rcx+r8*8]
	mov     r11, r14
	mul     r13
	mov     r12, rax
	mov     rax, [rsi+r14*8+8]
	mov     r9, rdx
	cmp     r14, 0
%endmacro

%macro addmulnext3 0
	mul     r13
	add     [rdi+r11*8], r12
	adc     r9, rax
	mov     r10d, 0
	adc     r10, rdx
	add     [rdi+r11*8+8], r9
	adc     r10, 0
	mov     [rdi+r11*8+16], r10
	inc     r8
	lea     rdi, [rdi+8]
%endmacro

%macro mpn_muladdmul_1_int 1
	mulnext%1
	jz      %%3
	align   16
%%1:
	addmulpro%1
	jge     %%2
	addmulloop %1
%%2:
	addmulnext%1
	jnz     %%1
%%3:
	mov     r13, [rsp-8]
	mov     r14, [rsp-16]
	mov     rbx, [rsp-24]
	mov     r12, [rsp-32]
	ret
%endmacro
	
   GLOBAL_FUNC mpn_mul_basecase
;	the current mul does not handle case one 
	cmp     rdx, 2
	jz      two
	jb      one
	mov     [rsp-8], r13
	mov     [rsp-16], r14
	mov     [rsp-24], rbx
	mov     [rsp-32], r12
	mov     r14, 5
	sub     r14, rdx
	lea     rdi, [rdi+rdx*8-40]
	lea     rcx, [rcx+r8*8]
	neg     r8
	lea     rsi, [rsi+rdx*8-40]
	mov     rax, [rsi+r14*8]
	mov     r13, [rcx+r8*8]
	mov     r11, r14
	mul     r13
	mov     r12, rax
	mov     rax, [rsi+r14*8+8]
	mov     r9, rdx
	cmp     r14, 0
	jge     mulskiploop
	mulloop 1
mulskiploop:
	mov     r10d, 0
	mul     r13
	mov     [rdi+r11*8], r12
	add     r9, rax
	adc     r10, rdx
	cmp     r11, 2
	ja      case3
	jz      case2
	jp      case1
case0:
	mpn_muladdmul_1_int 0
	align   16
case1:
	mpn_muladdmul_1_int 1
	align   16
case2:
	mpn_muladdmul_1_int 2
	align   16
case3:
	mpn_muladdmul_1_int 3
	align   16
one:
	mov     rax, [rsi]
	mul     qword [rcx]
	mov     [rdi], rax
	mov     [rdi+8], rdx
	ret
	align   16
two:
	mov     r10, [rcx]
	mov     rax, [rsi]
	mul     r10
	mov     [rdi], rax
	mov     rax, [rsi+8]
	mov     r9, rdx
	mul     r10
	add     r9, rax
	adc     rdx, 0
	mov     [rdi+8], r9
	mov     [rdi+16], rdx
	cmp     r8, 2
	jz      twobytwo
	ret
twobytwo:
	mov     r9d, 0
	mov     r10, [rcx+8]
	mov     rax, [rsi]
	mul     r10
	add     [rdi+8], rax
	mov     rax, [rsi+8]
	adc     r9, rdx
	mul     r10
	add     r9, rax
	adc     rdx, 0
	add     [rdi+16], r9
	adc     rdx, 0
	mov     [rdi+24], rdx
	ret
