
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

%include 'yasm_mac.inc'

; C	(rdi,rdx+r8)=(rsi,rdx)*(rcx,r8)
; C Version 1.0.7


%macro addmul2lp 1
	align   16
%%1:
	mov     rax, [rsi+rbx*8]
	mul     r8
	add     r9, rax
	mov     rax, [rsi+rbx*8+8]
	adc     r10, rdx
	mov     r11, 0
	mul     rcx
	add     [rdi+rbx*8], r12
	adc     r9, rax
	mov     r12, 0
	adc     r10, rdx
	mov     rax, [rsi+rbx*8+8]
	adc     r11, 0
	mul     r8
	add     [rdi+rbx*8+8], r9
	adc     r10, rax
	adc     r11, rdx
	mov     rax, [rsi+rbx*8+16]
	mul     rcx
	add     r10, rax
	mov     rax, [rsi+rbx*8+16]
	adc     r11, rdx
	adc     r12, 0
	mul     r8
	add     [rdi+rbx*8+16], r10
	mov     r9, 0
	adc     r11, rax
	mov     r10, 0
	mov     rax, [rsi+rbx*8+24]
	adc     r12, rdx
	mov     r15, r15
	mul     rcx
	add     r11, rax
	mov     rax, [rsi+rbx*8+24]
	adc     r12, rdx
	adc     r9, 0
	mul     r8
	add     [rdi+rbx*8+24], r11
	adc     r12, rax
	adc     r9, rdx
	mov     rax, [rsi+rbx*8+32]
	mul     rcx
	add     r12, rax
	adc     r9, rdx
	adc     r10, 0
	add     rbx, 4
	jnc     %%1
%endmacro

%macro addmul2pro0 0
	mov     rcx, [r13+r15*8]
	mul     rcx
	mov     r12, rax
	mov     r9, rdx
	mov     r10, 0
	mov     r8, [r13+r15*8+8]
%endmacro

%macro addmul2epi0 0
	mov     rbx, r14
	mov     rax, [rsi+24]
	mul     r8
	add     [rdi+24], r12
	adc     r9, rax
	adc     r10, rdx
	add     r15, 2
	mov     rax, [rsi+r14*8]
	mov     [rdi+32], r9
	lea     rdi, [rdi+16]
	mov     [rdi+24], r10
%endmacro

%macro addmul2pro1 0
	mov     rcx, [r13+r15*8]
	mul     rcx
	mov     r12, rax
	mov     r10, 0
	mov     r9, rdx
	mov     r8, [r13+r15*8+8]
%endmacro

%macro addmul2epi1 0
	mov     rax, [rsi+16]
	lea     rdi, [rdi+16]
	mul     r8
	add     r9, rax
	mov     rax, [rsi+24]
	mov     r11, 0
	adc     r10, rdx
	mul     rcx
	add     [rdi], r12
	adc     r9, rax
	adc     r10, rdx
	adc     r11, 0
	mov     rax, [rsi+24]
	mul     r8
	add     [rdi+8], r9
	adc     r10, rax
	adc     r11, rdx
	add     r15, 2
	mov     rbx, r14
	mov     rax, [rsi+r14*8]
	mov     [rdi+24], r11
	mov     [rdi+16], r10
%endmacro

%macro addmul2pro2 0
	mov     rcx, [r13+r15*8]
	mul     rcx
	mov     r10, 0
	mov     r12, rax
	mov     r9, rdx
	mov     r8, [r13+r15*8+8]
%endmacro

%macro addmul2epi2 0
	mov     rax, [rsi+8]
	lea     rdi, [rdi+16]
	mul     r8
	add     r9, rax
	mov     rax, [rsi+16]
	adc     r10, rdx
	mov     r11, 0
	mul     rcx
	add     [rdi-8], r12
	adc     r9, rax
	mov     r12, 0
	adc     r10, rdx
	mov     rax, [rsi+16]
	adc     r11, 0
	mul     r8
	add     [rdi], r9
	adc     r10, rax
	adc     r11, rdx
	mov     rax, [rsi+24]
	mul     rcx
	add     r10, rax
	mov     rax, [rsi+24]
	adc     r11, rdx
	adc     r12, 0
	mul     r8
	add     [rdi+8], r10
	adc     r11, rax
	adc     r12, rdx
	mov     rax, [rsi+r14*8]
	mov     [rdi+16], r11
	mov     [rdi+24], r12
	add     r15, 2
	mov     rbx, r14
%endmacro

%macro addmul2pro3 0
	mov     rcx, [r13+r15*8]
	mul     rcx
	mov     r12, rax
	mov     r9, rdx
	mov     r8, [r13+r15*8+8]
	mov     r10, 0
%endmacro

%macro addmul2epi3 0
	mov     rax, [rsi]
	lea     rdi, [rdi+16]
	mul     r8
	add     r9, rax
	mov     rax, [rsi+8]
	adc     r10, rdx
	mov     r11, 0
	mul     rcx
	add     [rdi-16], r12
	adc     r9, rax
	mov     r12, 0
	adc     r10, rdx
	mov     rax, [rsi+8]
	adc     r11, 0
	mul     r8
	add     [rdi-8], r9
	adc     r10, rax
	adc     r11, rdx
	mov     rax, [rsi+16]
	mul     rcx
	add     r10, rax
	mov     rax, [rsi+16]
	adc     r11, rdx
	adc     r12, 0
	mul     r8
	add     [rdi], r10
	mov     r9, 0
	adc     r11, rax
	mov     r10, 0
	mov     rax, [rsi+24]
	adc     r12, rdx
	mov     r15, r15
	mul     rcx
	add     r11, rax
	mov     rax, [rsi+24]
	adc     r12, rdx
	adc     r9, 0
	mul     r8
	add     [rdi+8], r11
	adc     r12, rax
	adc     r9, rdx
	mov     rax, [rsi+r14*8]
	mov     [rdi+16], r12
	mov     [rdi+24], r9
	add     r15, 2
	mov     rbx, r14
%endmacro

%macro mul2lp 0
	align   16
%%1:
	mov     rax, [rsi+rbx*8]
	mul     r8
	add     r9, rax
	mov     rax, [rsi+rbx*8+8]
	adc     r10, rdx
	mov     r11, 0
	mul     rcx
	mov     [rdi+rbx*8], r12
	add     r9, rax
	mov     r12, 0
	adc     r10, rdx
	mov     rax, [rsi+rbx*8+8]
	adc     r11, 0
	mul     r8
	mov     [rdi+rbx*8+8], r9
	add     r10, rax
	adc     r11, rdx
	mov     rax, [rsi+rbx*8+16]
	mul     rcx
	add     r10, rax
	mov     rax, [rsi+rbx*8+16]
	adc     r11, rdx
	adc     r12, 0
	mul     r8
	mov     [rdi+rbx*8+16], r10
	mov     r9, 0
	add     r11, rax
	mov     r10, 0
	mov     rax, [rsi+rbx*8+24]
	adc     r12, rdx
	mov     r15, r15
	mul     rcx
	add     r11, rax
	mov     rax, [rsi+rbx*8+24]
	adc     r12, rdx
	adc     r9, 0
	mul     r8
	mov     [rdi+rbx*8+24], r11
	add     r12, rax
	adc     r9, rdx
	mov     rax, [rsi+rbx*8+32]
	mul     rcx
	add     r12, rax
	adc     r9, rdx
	adc     r10, 0
	add     rbx, 4
	jnc     %%1
%endmacro

%macro mul2pro0 0
	mov     rcx, [r13+r15*8]
	mul     rcx
	mov     r12, rax
	mov     r9, rdx
	mov     r10, 0
	mov     r8, [r13+r15*8+8]
%endmacro

%macro mul2epi0 0
	mov     rbx, r14
	mov     rax, [rsi+24]
	mul     r8
	mov     [rdi+24], r12
	add     r9, rax
	adc     r10, rdx
	add     r15, 2
	mov     rax, [rsi+r14*8]
	mov     [rdi+32], r9
	lea     rdi, [rdi+16]
	mov     [rdi+24], r10
%endmacro

%macro mul2pro1 0
	mov     rcx, [r13+r15*8]
	mul     rcx
	mov     r12, rax
	mov     r10, 0
	mov     r9, rdx
	mov     r8, [r13+r15*8+8]
%endmacro

%macro mul2epi1 0
	mov     rax, [rsi+16]
	lea     rdi, [rdi+16]
	mul     r8
	add     r9, rax
	mov     rax, [rsi+24]
	mov     r11, 0
	adc     r10, rdx
	mul     rcx
	mov     [rdi], r12
	add     r9, rax
	adc     r10, rdx
	adc     r11, 0
	mov     rax, [rsi+24]
	mul     r8
	mov     [rdi+8], r9
	add     r10, rax
	adc     r11, rdx
	add     r15, 2
	mov     rbx, r14
	mov     rax, [rsi+r14*8]
	mov     [rdi+24], r11
	mov     [rdi+16], r10
%endmacro

%macro mul2pro2 0
	mov     rcx, [r13+r15*8]
	mul     rcx
	mov     r10, 0
	mov     r12, rax
	mov     r9, rdx
	mov     r8, [r13+r15*8+8]
%endmacro

%macro mul2epi2 0
	mov     rax, [rsi+8]
	lea     rdi, [rdi+16]
	mul     r8
	add     r9, rax
	mov     rax, [rsi+16]
	adc     r10, rdx
	mov     r11, 0
	mul     rcx
	mov     [rdi-8], r12
	add     r9, rax
	mov     r12, 0
	adc     r10, rdx
	mov     rax, [rsi+16]
	adc     r11, 0
	mul     r8
	mov     [rdi], r9
	add     r10, rax
	adc     r11, rdx
	mov     rax, [rsi+24]
	mul     rcx
	add     r10, rax
	mov     rax, [rsi+24]
	adc     r11, rdx
	adc     r12, 0
	mul     r8
	mov     [rdi+8], r10
	add     r11, rax
	adc     r12, rdx
	mov     rax, [rsi+r14*8]
	mov     [rdi+16], r11
	mov     [rdi+24], r12
	add     r15, 2
	mov     rbx, r14
%endmacro

%macro mul2pro3 0
	mov     rcx, [r13+r15*8]
	mul     rcx
	mov     r12, rax
	mov     r9, rdx
	mov     r8, [r13+r15*8+8]
	mov     r10, 0
%endmacro

%macro mul2epi3 0
	mov     rax, [rsi]
	lea     rdi, [rdi+16]
	mul     r8
	add     r9, rax
	mov     rax, [rsi+8]
	adc     r10, rdx
	mov     r11, 0
	mul     rcx
	mov     [rdi-16], r12
	add     r9, rax
	mov     r12, 0
	adc     r10, rdx
	mov     rax, [rsi+8]
	adc     r11, 0
	mul     r8
	mov     [rdi-8], r9
	add     r10, rax
	adc     r11, rdx
	mov     rax, [rsi+16]
	mul     rcx
	add     r10, rax
	mov     rax, [rsi+16]
	adc     r11, rdx
	adc     r12, 0
	mul     r8
	mov     [rdi], r10
	mov     r9, 0
	add     r11, rax
	mov     r10, 0
	mov     rax, [rsi+24]
	adc     r12, rdx
	mov     r15, r15
	mul     rcx
	add     r11, rax
	mov     rax, [rsi+24]
	adc     r12, rdx
	adc     r9, 0
	mul     r8
	mov     [rdi+8], r11
	add     r12, rax
	adc     r9, rdx
	mov     rax, [rsi+r14*8]
	mov     [rdi+16], r12
	mov     [rdi+24], r9
	add     r15, 2
	mov     rbx, r14
%endmacro

%macro mul1lp 0
	align   16
%%1:
	mov     r10, 0
	mul     r8
	mov     [rdi+rbx*8-8], r12
	add     r9, rax
	db      0x26
	adc     r10, rdx
	mov     rax, [rsi+rbx*8+8]
	mul     r8
	mov     [rdi+rbx*8], r9
	add     r10, rax
	mov     r11d, 0
	adc     r11, rdx
	mov     rax, [rsi+rbx*8+16]
	mov     r12, 0
	mov     r9, 0
	mul     r8
	mov     [rdi+rbx*8+8], r10
	db      0x26
	add     r11, rax
	db      0x26
	adc     r12, rdx
	mov     rax, [rsi+rbx*8+24]
	mul     r8
	mov     [rdi+rbx*8+16], r11
	db      0x26
	add     r12, rax
	db      0x26
	adc     r9, rdx
	add     rbx, 4
	mov     rax, [rsi+rbx*8]
	jnc     %%1
%endmacro

; rbx is 0
%macro mulnext0 0
	mov     rax, [rsi+8]
	mul     r8
	mov     [rdi], r9
	add     r10, rax
	mov     r11d, 0
	adc     r11, rdx
	mov     rax, [rsi+16]
	mov     r12d, 0
	mul     r8
	mov     [rdi+8], r10
	add     r11, rax
	adc     r12, rdx
	mov     rax, [rsi+24]
	mul     r8
	mov     [rdi+16], r11
	add     r12, rax
	adc     rdx, 0
	mov     [rdi+24], r12
	mov     rax, [rsi+r14*8]
	mov     [rdi+32], rdx
	inc     r15
	lea     rdi, [rdi+8]
	mov     rbx, r14
%endmacro

; rbx is 1
%macro mulnext1 0
	mov     rax, [rsi+16]
	mul     r8
	mov     [rdi+8], r9
	add     r10, rax
	mov     r12d, 0
	adc     r12, rdx
	mov     rax, [rsi+24]
	mul     r8
	mov     [rdi+16], r10
	add     r12, rax
	adc     rdx, 0
	mov     [rdi+24], r12
	mov     [rdi+32], rdx
	inc     r15
	lea     rdi, [rdi+8]
	mov     rbx, r14
	mov     rax, [rsi+r14*8]
%endmacro

; rbx is 2
%macro mulnext2 0
	mov     rax, [rsi+24]
	mul     r8
	mov     [rdi+16], r9
	add     r10, rax
	mov     r11d, 0
	adc     r11, rdx
	mov     [rdi+24], r10
	mov     [rdi+32], r11
	inc     r15
	lea     rdi, [rdi+8]
	mov     rax, [rsi+r14*8]
	mov     rbx, r14
%endmacro

; rbx is 3
%macro mulnext3 0
	mov     [rdi+24], r9
	mov     [rdi+32], r10
	inc     r15
	lea     rdi, [rdi+8]
	mov     rax, [rsi+r14*8]
	mov     rbx, r14
%endmacro

%macro mpn_addmul_2_int 1
	jz      %%2
	align   16
%%1:
	addmul2pro%1
	addmul2lp %1
	addmul2epi%1
	jnz     %%1
%%2:
	mov     r13, [rsp-8]
	mov     r14, [rsp-16]
	mov     rbx, [rsp-24]
	mov     r12, [rsp-32]
	mov     r15, [rsp-40]
	ret
%endmacro

%macro oldmulnext0 0
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
	mov     rax, [rsi+r14*8]
	mov     [rdi+r11*8+40], rdx
	inc     r8
	mov     r11, r14
%endmacro

%macro oldmulnext1 0
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
	mov     r11, r14
	mov     rax, [rsi+r14*8]
%endmacro

%macro oldmulnext2 0
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

%macro oldmulnext3 0
	mov     [rdi+r11*8+8], r9
	mov     [rdi+r11*8+16], r10
	inc     r8
	mov     rax, [rsi+r14*8]
	mov     r11, r14
%endmacro

%macro oldaddmulpro0 0
	mov     r13, [rcx+r8*8]
	db      0x26
	mul     r13
	db      0x26
	mov     r12, rax
	mov     rax, [rsi+r14*8+8]
	db      0x26
	mov     r9, rdx
	lea     rdi, [rdi+8]
%endmacro

%macro oldaddmulnext0 0
	mov     r10d, 0
	mul     r13
	add     [rdi], r12
	adc     r9, rax
	adc     r10, rdx
	mov     rax, [rsi+16]
	mul     r13
	add     [rdi+8], r9
	adc     r10, rax
	mov     ebx, 0
	adc     rbx, rdx
	mov     rax, [rsi+24]
	mov     r12d, 0
	mov     r11, r14
	mul     r13
	add     [rdi+16], r10
	adc     rbx, rax
	adc     r12, rdx
	mov     rax, [rsi+32]
	mul     r13
	add     [rdi+24], rbx
	adc     r12, rax
	adc     rdx, 0
	add     [rdi+32], r12
	mov     rax, [rsi+r14*8]
	adc     rdx, 0
	inc     r8
	mov     [rdi+40], rdx
%endmacro

%macro oldaddmulpro1 0
	mov     r13, [rcx+r8*8]
	mul     r13
	mov     r12, rax
	mov     rax, [rsi+r14*8+8]
	mov     r9, rdx
%endmacro

%macro oldaddmulnext1 0
	mov     r10d, 0
	mul     r13
	add     [rdi+8], r12
	adc     r9, rax
	adc     r10, rdx
	mov     rax, [rsi+24]
	mul     r13
	lea     rdi, [rdi+8]
	add     [rdi+8], r9
	adc     r10, rax
	mov     r12d, 0
	mov     rax, [rsi+32]
	adc     r12, rdx
	mov     r11, r14
	mul     r13
	add     [rdi+16], r10
	adc     r12, rax
	adc     rdx, 0
	add     [rdi+24], r12
	adc     rdx, 0
	mov     [rdi+32], rdx
	inc     r8
	mov     rax, [rsi+r14*8]
%endmacro

%macro oldaddmulpro2 0
	mov     r13, [rcx+r8*8]
	lea     rdi, [rdi+8]
	mul     r13
	mov     r12, rax
	mov     rax, [rsi+r14*8+8]
	mov     r9, rdx
%endmacro

%macro oldaddmulnext2 0
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

%macro oldaddmulpro3 0
	mov     r13, [rcx+r8*8]
	db      0x26
	mul     r13
	db      0x26
	mov     r12, rax
	db      0x26
	lea     rdi, [rdi+8]
	db      0x26
	mov     r9, rdx
	mov     rax, [rsi+r14*8+8]
%endmacro

%macro oldaddmulnext3 0
	mov     r11, r14
	mul     r13
	add     [rdi+24], r12
	adc     r9, rax
	adc     rdx, 0
	add     [rdi+32], r9
	mov     rax, [rsi+r14*8]
	adc     rdx, 0
	inc     r8
	mov     [rdi+40], rdx
%endmacro

%macro oldmpn_muladdmul_1_int 1
	oldmulnext%1
	jz      %%2
	align   16
%%1:
	oldaddmulpro%1
	oldaddmulnext%1
	jnz     %%1
%%2:
	mov     r13, [rsp-8]
	mov     r14, [rsp-16]
	mov     rbx, [rsp-24]
	mov     r12, [rsp-32]
	ret
%endmacro

	ASM_START
	GLOBAL_FUNC mpn_mul_basecase
; the current mul does not handle case one 
	cmp     rdx, 4
	jg      L_fiveormore
	cmp     rdx, 1
	je      L_one
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
	mov     r10d, 0
	mul     r13
	mov     [rdi+r11*8], r12
	add     r9, rax
	adc     r10, rdx
	cmp     r11, 2
	ja      L_oldcase3
	jz      L_oldcase2
	jp      L_oldcase1
L_oldcase0:
	oldmpn_muladdmul_1_int 0
L_oldcase1:
	oldmpn_muladdmul_1_int 1
L_oldcase2:
	oldmpn_muladdmul_1_int 2
L_oldcase3:
	oldmpn_muladdmul_1_int 3
	align   16
L_fiveormore:
; rdx >= 5  as we dont have an inner jump
; (rdi,rdx+r8)=(rsi,rdx)*(rcx,r8)
	mov     [rsp-8], r13
	mov     [rsp-16], r14
	mov     [rsp-24], rbx
	mov     [rsp-32], r12
	mov     [rsp-40], r15
	mov     r14, 4
	sub     r14, rdx
	lea     rdi, [rdi+rdx*8-32]
	lea     rsi, [rsi+rdx*8-32]
	mov     r13, rcx
	mov     r15, r8
	lea     r13, [r13+r15*8]
	neg     r15
	mov     rbx, r14
	mov     rax, [rsi+r14*8]
	bt      r15, 0
	jnc     L_even
L_odd:
	inc     rbx
	mov     r8, [r13+r15*8]
	mul     r8
	mov     r12, rax
	mov     rax, [rsi+r14*8+8]
	mov     r9, rdx
	cmp     rbx, 0
	jge     L_mulskiploop
	mul1lp 
L_mulskiploop:
	mov     r10d, 0
	mul     r8
	mov     [rdi+rbx*8-8], r12
	add     r9, rax
	adc     r10, rdx
	cmp     rbx, 2
	ja      L_mul1case3
	jz      L_mul1case2
	jp      L_mul1case1
L_mul1case0:
	mulnext0
	jmp     L_case0
L_mul1case1:
	mulnext1
	jmp     L_case3
L_mul1case2:
	mulnext2
	jmp     L_case2
L_mul1case3:
	mulnext3
	jmp     L_case1
L_even:
	; as all the mul2pro? are the same
	mul2pro0
	mul2lp 
	cmp     rbx, 2
	ja      L_mul2case0
	jz      L_mul2case1
	jp      L_mul2case2
L_mul2case3:
	mul2epi3
L_case3:
	mpn_addmul_2_int 3
L_mul2case2:
	mul2epi2
L_case2:
	mpn_addmul_2_int 2
L_mul2case1:
	mul2epi1
L_case1:
	mpn_addmul_2_int 1
L_mul2case0:
	mul2epi0
L_case0:
	mpn_addmul_2_int 0
	align   16
L_one:
	mov     rax, [rsi]
	mul	qword [rcx]
	mov     [rdi], rax
	mov     [rdi+8], rdx
	ret
	end

