
;  AMD64 mpn_sqr_basecase
;  Copyright 2009 Jason Moxham
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

;	(rdi, 2*rdx) = (rsi,rdx)^2
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
	inc     r14
	lea     rdi, [rdi+8]
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
	inc     r14
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
	inc     r14
	lea     rdi, [rdi+8]
%endmacro

%macro mulnext3 0
	mov     [rdi+r11*8+8], r9
	mov     [rdi+r11*8+16], r10
	inc     r14
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
	adc     rdx, 0
	inc     r14
	mov     [rdi+r11*8+40], rdx
	lea     rdi, [rdi+8]
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
	inc     r14
	lea     rdi, [rdi+8]
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
	add     [rdi+r11*8+16], r10
	adc     rbx, 0
	mov     [rdi+r11*8+24], rbx
	inc     r14
	lea     rdi, [rdi+8]
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
	inc     r14
	lea     rdi, [rdi+8]
	cmp     r14, 4
	jnz     theloop
%endmacro
	
   GLOBAL_FUNC mpn_sqr_basecase
	cmp     rdx, 3
	ja      fourormore
	jz      three
	jp      two
one:
	mov     rax, [rsi]
	mul     rax
	mov     [rdi], rax
	mov     [rdi+8], rdx
	ret
	align   16
fourormore:
;	this code can not handle cases 3,2,1
	mov     [rsp-8], r12
	mov     [rsp-16], r13
	mov     [rsp-24], r14
	mov     [rsp-32], rbx
;	save data for later
	mov     [rsp-40], rdi
	mov     [rsp-48], rsi
	mov     [rsp-56], rdx
	mov     r13, [rsi]
	mov     rax, [rsi+8]
	mov     r14d, 6
	sub     r14, rdx
	lea     rdi, [rdi+rdx*8-40]
	lea     rsi, [rsi+rdx*8-40]
	mov     r11, r14
	mul     r13
	mov     r12, rax
	mov     rax, [rsi+r14*8+8]
	mov     r9, rdx
	cmp     r14, 0
	jge     mulskiploop1
	mulloop 1
mulskiploop1:
	mov     r10d, 0
	mul     r13
	mov     [rdi+r11*8], r12
	add     r9, rax
	adc     r10, rdx
;	could save r9 here 
;	could update here ie lea 8(rdi),rdi and inc r14 
	cmp     r11, 2
	je      mcase2
	ja      mcase3
	jp      mcase1
mcase0:
	mulnext0
	jmp     case1
	align   16
mcase1:
	mulnext1
	jmp     case2
	align   16
mcase2:
	mulnext2
	jmp     case3
	align   16
mcase3:
	mulnext3
;	jmp case0 just fall thru 
	align   16
theloop:
case0:
	mov     rax, [rsi+r14*8]
	mov     r13, [rsi+r14*8-8]
	mov     r11, r14
	mul     r13
	mov     r12, rax
	mov     rax, [rsi+r14*8+8]
	mov     r9, rdx
	cmp     r14, 0
	jge     addmulskiploop0
	addmulloop 0
addmulskiploop0:
	addmulnext0
case1:
	mov     rax, [rsi+r14*8]
	mov     r13, [rsi+r14*8-8]
	mov     r11, r14
	mul     r13
	mov     r12, rax
	mov     rax, [rsi+r14*8+8]
	mov     r9, rdx
	cmp     r14, 0
	jge     addmulskiploop1
	addmulloop 1
addmulskiploop1:
	addmulnext1
case2:
	mov     rax, [rsi+r14*8]
	mov     r13, [rsi+r14*8-8]
	mov     r11, r14
	mul     r13
	mov     r12, rax
	mov     rax, [rsi+r14*8+8]
	mov     r9, rdx
	cmp     r14, 0
	jge     addmulskiploop2
	addmulloop 2
addmulskiploop2:
	addmulnext2
case3:
	mov     rax, [rsi+r14*8]
	mov     r13, [rsi+r14*8-8]
	mov     r11, r14
	mul     r13
	mov     r12, rax
	mov     rax, [rsi+r14*8+8]
	mov     r9, rdx
	cmp     r14, 0
	jge     addmulskiploop3
	addmulloop 3
addmulskiploop3:
	addmulnext3
;	only need to add one more line here
	mov     rax, [rsi+r14*8]
	mov     r13, [rsi+r14*8-8]
	mul     r13
	add     [rdi+r14*8], rax
	adc     rdx, 0
	mov     [rdi+r14*8+8], rdx
;	now lsh by 1 and add in the diagonal
	mov     rdi, [rsp-40]
	mov     rsi, [rsp-48]
	mov     rcx, [rsp-56]
	mov     r12, [rsp-8]
	mov     r13, [rsp-16]
	xor     rbx, rbx
	xor     r11, r11
	lea     rsi, [rsi+rcx*8]
	mov     [rdi], r11
	lea     r10, [rdi+rcx*8]
	mov     [r10+rcx*8-8], r11
	neg     rcx
	align   16
dialoop:
	mov     rax, [rsi+rcx*8]
	mul     rax
	mov     r8, [rdi]
	mov     r9, [rdi+8]
	add     rbx, 1
	adc     r8, r8
	adc     r9, r9
	sbb     rbx, rbx
	add     r11, 1
	adc     r8, rax
	adc     r9, rdx
	sbb     r11, r11
	mov     [rdi], r8
	mov     [rdi+8], r9
	inc     rcx
	lea     rdi, [rdi+16]
	jnz     dialoop
	mov     rbx, [rsp-32]
	mov     r14, [rsp-24]
	ret
	align   16
two:
	mov     rax, [rsi]
	mov     r9, [rsi+8]
	mov     r8, rax
	mul     rax
	mov     [rdi], rax
	mov     rax, r9
	mov     [rdi+8], rdx
	mul     rax
	mov     [rdi+16], rax
	mov     rax, r8
	mov     r10, rdx
	mul     r9
	add     rax, rax
	adc     rdx, rdx
	adc     r10, 0
	add     [rdi+8], rax
	adc     [rdi+16], rdx
	adc     r10, 0
	mov     [rdi+24], r10
	ret
	align   16
three:
	mov     r8, [rsi]
	mov     rax, [rsi+8]
	mul     r8
	mov     r11d, 0
	mov     [rdi+8], rax
	mov     rax, [rsi+16]
	mov     r9, rdx
	mul     r8
	mov     r8, [rsi+8]
	add     r9, rax
	mov     rax, [rsi+16]
	adc     r11, rdx
	mul     r8
	mov     [rdi+16], r9
	add     r11, rax
	mov     r9d, 0
	mov     [rdi+24], r11
	adc     r9, rdx
	mov     [rdi+32], r9
	mov     rcx, -3
	xor     r10, r10
	xor     r11, r11
	lea     rsi, [rsi+24]
	mov     [rdi], r11
	mov     [rdi+40], r11
dialoop1:
	mov     rax, [rsi+rcx*8]
	mul     rax
	mov     r8, [rdi]
	mov     r9, [rdi+8]
	add     r10, 1
	adc     r8, r8
	adc     r9, r9
	sbb     r10, r10
	add     r11, 1
	adc     r8, rax
	adc     r9, rdx
	sbb     r11, r11
	mov     [rdi], r8
	mov     [rdi+8], r9
	inc     rcx
	lea     rdi, [rdi+16]
	jnz     dialoop1
	nop
	ret
