
;  AMD64 mpn_sqr_basecase -- square an mpn number.
;  Version 1.0.4.
;
;  Copyright 2008 Jason Moxham
;
;  Windows Conversion Copyright 2008 Brian Gladman
;
;  This file is part of the MPIR Library.
;  The MPIR Library is free software; you can redistribute it and/or modify
;  it under the terms of the GNU Lesser General Public License as published
;  by the Free Software Foundation; either verdxon 2.1 of the License, or (at
;  your option) any later verdxon.
;  The MPIR Library is distributed in the hope that it will be useful, but
;  WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
;  or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public
;  License for more details.
;  You should have received a copy of the GNU Lesser General Public License
;  along with the MPIR Library; see the file COPYING.LIB.  If not, write
;  to the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
;  Boston, MA 02110-1301, USA.
;
;  Calling interface:
;
;  void mpn_sqr_basecase(
;     mp_ptr dst,       rcx
;     mp_srcptr src,    rdx
;     mp_size_t size     r8
;  )
;
; Squuare src[size] and write the result to dst[2 * size]
;
; This is an SEH frame function with a leaf prologue

%macro mpn_mul_1_int 0

	mov     r10d, 4
	sub     r10, rdx
	mov     r11d, 0
	jnc     %%2
	
	alignb  16, nop
%%1:mov     rax, [rsi+r10*8+16]
	mov     r9d, 0
	mul     r13
	add     r11, rax
	mov     [rdi+r10*8+16], r11
	mov     rax, [rsi+r10*8+24]
	adc     r9, rdx
	mul     r13
	mov     r11d, 0
	add     r9, rax
	mov     rax, [rsi+r10*8+32]
	adc     r11, rdx
	mul     r13
	mov     [rdi+r10*8+24], r9
	add     r11, rax
	mov     r9d, 0
	mov     [rdi+r10*8+32], r11
	mov     rax, [rsi+r10*8+40]
	mov     r11d, 0
	adc     r9, rdx
	mul     r13
	add     r9, rax
	mov     [rdi+r10*8+40], r9
	adc     r11, rdx
	add     r10, 4
	jnc     %%1

%%2:test    r10, 2
	jnz     %%3
	mov     rax, [rsi+r10*8+16]
	mov     r9d, 0
	mul     r13
	add     r11, rax
	mov     [rdi+r10*8+16], r11
	mov     rax, [rsi+r10*8+24]
	adc     r9, rdx
	mul     r13
	mov     r11d, 0
	add     r9, rax
	adc     r11, rdx
	add     r10, 2
	mov     [rdi+r10*8+8], r9

%%3:test    r10, 1
	jnz     %%4
	mov     rax, [rsi+r10*8+16]
	mov     r9d, 0
	mul     r13
	add     r11, rax
	adc     r9, rdx
	mov     [rdi+r10*8+24], r9

%%4:mov     [rdi+r10*8+16], r11

%endmacro

; changes from standard mpn_addmul_1 internal loop
; change lables
; change r8 to r12   , rcx to r13

%macro addmulloop 1

	alignb  16, nop
%%1:mov     r10d, 0
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
	mov     r9d, 0
	mul     r13
	add     [rdi+r11*8+16], r10
	adc     rbx, rax
	adc     r12, rdx
	mov     rax, [rsi+r11*8+32]
	mul     r13
	add     [rdi+r11*8+24], rbx
	adc     r12, rax
	adc     r9, rdx
	add     r11, 4
	mov     rax, [rsi+r11*8+8]
	jnc     %%1

%endmacro

%macro addmulnext0 0

; here is 3 loads ie if r11=0
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
	mov     r9d, 0
	mul     r13
	add     [rdi+r11*8+16], r10
	adc     rbx, rax
	adc     r12, rdx
	mov     rax, [rsi+r11*8+32]
	mul     r13
	add     [rdi+r11*8+24], rbx
	adc     r12, rax
	adc     r9, rdx
	add     [rdi+r11*8+32], r12
	adc     r9, 0
	mov     [rdi+r11*8+40], r9

%endmacro

%macro addmulnext1 0

; here is 2 loads ie if r11=1
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
	add     [rdi+r11*8+24], rbx
	adc     r12, 0
	mov     [rdi+r11*8+32], r12

%endmacro

%macro addmulnext2 0

; here is 1 load ie if r11=2
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
	add     [rdi+r11*8+16], r10
	adc     rbx, 0
	mov     [rdi+r11*8+24], rbx

%endmacro

%macro addmulnext3 0

; here is 0 loads ie if r11=3
	mov     r10d, 0
	mul     r13
	add     [rdi+r11*8], r12
	adc     r9, rax
	adc     r10, rdx
	add     [rdi+r11*8+8], r9
	adc     r10, 0
	mov     [rdi+r11*8+16], r10

%endmacro

; changes from standard addmul_1
; change lables
; change r8 to r12
; write top limb ax straight to mem dont return  (NOTE we WRITE NOT ADD)
; remove one limb special case
; remove push/pop , basecase must save rbx
; split into mod4 types and remove rets
; surround with outer loop and pop ret
; todo ----------- can ignore small values
; this addmul MUST have a param whic is 0123 which is our r11

%macro mpn_addmul_1_int 1

%%1:mov     rax, [rsi+r14*8]
	mov     r13, [rsi+r14*8-8]
	mov     r11, r14
	mul     r13
	mov     r12, rax
	mov     rax, [rsi+r14*8+8]
	mov     r9, rdx
	cmp     r14, 0
	jge     %%2
	addmulloop %1
%%2:addmulnext%1
	inc     r14
	lea     rdi, [rdi+8]
	cmp     r14, 4
	jz      .7

%endmacro

%include "..\x86_64_asm.inc"

%define reg_save_list   rbx, rsi, rdi, r12, r13, r14

   bits     64
   section  .text

   global   __gmpn_sqr_basecase

%ifdef DLL
   export   __gmpn_sqr_basecase
%endif

__gmpn_sqr_basecase:
    cmp     r8d, 3
	jnb     .3
	jp      .2

.1: mov     rax, [rdx]
	mul     rax
	mov     [rcx], rax
	mov     [rcx+8], rdx
	ret

.2: mov     rax, [rdx]
	mov     r9, [rdx+8]
	mov     r8, rax
	mul     rax
	mov     [rcx], rax
	mov     rax, r9
	mov     [rcx+8], rdx
	mul     rax
	mov     [rcx+16], rax
	mov     rax, r8
	mov     r10, rdx
	mul     r9
	add     rax, rax
	adc     rdx, rdx
	adc     r10, 0
	add     [rcx+8], rax
	adc     [rcx+16], rdx
	adc     r10, 0
	mov     [rcx+24], r10
	ret

.3  ja      sqr_main
    prologue sqr_3, 0, rsi, rdi
    mov     rsi, rdx
    mov     r8, [rsi]
	mov     rax, [rsi+8]
	mul     r8
	mov     r11d, 0
	mov     [rcx+8], rax
	mov     rax, [rsi+16]
	mov     r9, rdx
	mul     r8
	mov     r8, [rsi+8]
	add     r9, rax
	mov     rax, [rsi+16]
	adc     r11, rdx
	mul     r8
	mov     [rcx+16], r9
	add     r11, rax
	mov     r9d, 0
	mov     [rcx+24], r11
	adc     r9, rdx
	mov     [rcx+32], r9
	mov     edi, 3
	xor     r10, r10
	xor     r11, r11
	lea     rsi, [rsi+24]
	mov     [rcx], r11
	mov     [rcx+40], r11
	neg     rdi

.4: mov     rax, [rsi+rdi*8]
	mul     rax
	mov     r8, [rcx]
	mov     r9, [rcx+8]
	add     r10, 1
	adc     r8, r8
	adc     r9, r9
	sbb     r10, r10
	add     r11, 1
	adc     r8, rax
	adc     r9, rdx
	sbb     r11, r11
	mov     [rcx], r8
	mov     [rcx+8], r9
	inc     rdi
	lea     rcx, [rcx+16]
	jnz     .4
	epilogue rsi, rdi

; this code can not handle cases 3,2,1
    prologue sqr_main, 0, reg_save_list
    mov     rdi, rcx
    mov     rsi, rdx
    mov     edx, r8d

    mov     [rsp+stack_use+24], rdi     ; use shadow area
	mov     [rsp+stack_use+16], rsi
	mov     [rsp+stack_use+ 8], rdx

	mov     r13, [rsi]
	mov     r14d, 7
	sub     r14, rdx
	lea     rdi, [rdi+rdx*8-40]
	lea     rsi, [rsi+rdx*8-40]
	mpn_mul_1_int
	lea     rdi, [rdi+8]
	mov     rax, r14
	and     rax, 3
	je      .60
	jp      .63
	cmp     rax, 1
	je      .61
	alignb  16, nop
.6:
.62:mpn_addmul_1_int 2
.63:mpn_addmul_1_int 3
.60:mpn_addmul_1_int 0
.61:mpn_addmul_1_int 1
	jmp     .6

; only need to add .1 more line here

.7:	mov     rax, [rsi+r14*8]
	mov     r13, [rsi+r14*8-8]
	mul     r13
	add     [rdi+r14*8], rax
	adc     rdx, 0
	mov     [rdi+r14*8+8], rdx

; now lsh by 1 and add in the diagonal

	mov     rcx, [rsp + stack_use +  8]     ; use shadow area
	mov     rsi, [rsp + stack_use + 16]
	mov     rdi, [rsp + stack_use + 24]

.8: xor     rbx, rbx
	xor     r14, r14
	lea     rsi, [rsi+rcx*8]
	mov     [rdi], r14
	lea     r10, [rdi+rcx*8]
	mov     [r10+rcx*8-8], r14
	neg     rcx

.9: mov     rax, [rsi+rcx*8]
	mul     rax
	mov     r8, [rdi]
	mov     r9, [rdi+8]
	add     rbx, 1
	adc     r8, r8
	adc     r9, r9
	sbb     rbx, rbx
	add     r14, 1
	adc     r8, rax
	adc     r9, rdx
	sbb     r14, r14
	mov     [rdi], r8
	mov     [rdi+8], r9
	inc     rcx
	lea     rdi, [rdi+16]
	jnz     .9

.10:epilogue reg_save_list

	end
