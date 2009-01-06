;
; AMD64 mpn_mul_basecase -- multiply two mpn numbers.
; Version 1.0.3.
;
;  Copyright 2008 Jason Moxham 
;  Windows conversion by Brian Gladman

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

;  Calling interface:
;
;  l1 = r8d , l2 = dword [rsp+0x28] ; rcx[l1 + l2] = rdx[l1] * r9[l2]
;
;  void __gmpn_mul_basecase(
;     mp_ptr rp,          rcx
;     mp_srcptr xp,       rdx
;     mp_size_t xn,        r8
;     mp_srcptr yp,        r9
;     mp_size_t yn [rsp+0x28]    as a *** 32-bit *** word
; )
;
; Multiply xp[xn] by yp[yn] and write the result to rp[un+vn] with xn >= yn on
; entry.
;
; This is an SEH frame function with a leaf prologue

%include "..\x86_64_asm.inc"

%define reg_save_list   rsi, rdi, r12, r13, r14, r15

%macro  mpn_mul_1_int 0

    mov     rsi, 3
    sub     rsi, rdx
    mov     r13d, 0
    jnc     %%2
    
    align   16
%%1:mov     rax, [r10+rsi*8+16]
	mov     r12d, 0
	mul     r11
	add     r13, rax
	mov     [rcx+rsi*8+16], r13
	mov     rax, [r10+rsi*8+24]
	adc     r12, rdx
	mul     r11
	mov     r13d, 0
	add     r12, rax
	mov     rax, [r10+rsi*8+32]
	adc     r13, rdx
	mul     r11
	mov     [rcx+rsi*8+24], r12
	add     r13, rax
	mov     r12d, 0
	mov     [rcx+rsi*8+32], r13
	mov     rax, [r10+rsi*8+40]
	mov     r13d, 0
	adc     r12, rdx
	mul     r11
	add     r12, rax
	mov     [rcx+rsi*8+40], r12
	adc     r13, rdx
	add     rsi, 4
	jnc     %%1

%%2:test    rsi, 2
    jnz     %%3
	mov     rax, [r10+rsi*8+16]
	mov     r12d, 0
	mul     r11
	add     r13, rax
	mov     [rcx+rsi*8+16], r13
	mov     rax, [r10+rsi*8+24]
	adc     r12, rdx
	mul     r11
	mov     r13d, 0
	add     r12, rax
	adc     r13, rdx
	add     rsi, 2
	mov     [rcx+rsi*8+8], r12

%%3:test    rsi, 1
    jnz     %%4
	mov     rax, [r10+rsi*8+16]
	mov     r12d, 0
	mul     r11
	add     r13, rax
	adc     r12, rdx
	mov     [rcx+rsi*8+24], r12

%%4:mov     [rcx+rsi*8+16], r13

%endmacro

%macro  add_mul_loop 0

    align   16
%%1:mov     r13d, 0
	mul     r11
	add     [rcx+rsi*8], r15
	adc     r12, rax
	adc     r13, rdx
	mov     rax, [r10+rsi*8+16]
	mul     r11
	add     [rcx+rsi*8+8], r12
	adc     r13, rax
	mov     r14d, 0
	adc     r14, rdx
	mov     rax, [r10+rsi*8+24]
	mov     r15d, 0
	mov     r12d, 0
	mul     r11
	add     [rcx+rsi*8+16], r13
	adc     r14, rax
	adc     r15, rdx
	mov     rax, [r10+rsi*8+32]
 	mul     r11
	add     [rcx+rsi*8+24], r14
	adc     r15, rax
	adc     r12, rdx
	add     rsi, 4
	mov     rax, [r10+rsi*8+8]
	jnc     %%1
	
%endmacro

%macro  add_mul_next_0 0

	mov     r13d, 0
	mul     r11
	add     [rcx+rsi*8], r15
	adc     r12, rax
	adc     r13, rdx
	mov     rax, [r10+rsi*8+16]
	mul     r11
	add     [rcx+rsi*8+8], r12
	adc     r13, rax
	mov     r14d, 0
	adc     r14, rdx
	mov     rax, [r10+rsi*8+24]
	mov     r15d, 0
	mov     r12d, 0
	mul     r11
	add     [rcx+rsi*8+16], r13
	adc     r14, rax
	adc     r15, rdx
	mov     rax, [r10+rsi*8+32]
	mul     r11
	add     [rcx+rsi*8+24], r14
	adc     r15, rax
	adc     r12, rdx
	add     [rcx+rsi*8+32], r15
	adc     r12, 0
	mov     [rcx+rsi*8+40], r12
	
%endmacro

%macro  add_mul_next_1 0

	mov     r13d, 0
	mul     r11
	add     [rcx+rsi*8], r15
	adc     r12, rax
	adc     r13, rdx
	mov     rax, [r10+rsi*8+16]
	mul     r11
	add     [rcx+rsi*8+8], r12
	adc     r13, rax
	mov     r14d, 0
	adc     r14, rdx
	mov     rax, [r10+rsi*8+24]
	mov     r15d, 0
	mul     r11
	add     [rcx+rsi*8+16], r13
	adc     r14, rax
	adc     r15, rdx
	add     [rcx+rsi*8+24], r14
	adc     r15, 0
	mov     [rcx+rsi*8+32], r15

%endmacro

%macro  add_mul_next_2 0

	mov     r13d, 0
	mul     r11
	add     [rcx+rsi*8], r15
	adc     r12, rax
	adc     r13, rdx
	mov     rax, [r10+rsi*8+16]
	mul     r11
	add     [rcx+rsi*8+8], r12
	adc     r13, rax
	mov     r14d, 0
	adc     r14, rdx
	add     [rcx+rsi*8+16], r13
	adc     r14, 0
	mov     [rcx+rsi*8+24], r14

%endmacro

%macro  add_mul_next_3 0

	mov     r13d, 0
	mul     r11
	add     [rcx+rsi*8], r15
	adc     r12, rax
	adc     r13, rdx
	add     [rcx+rsi*8+8], r12
	adc     r13, 0
	mov     [rcx+rsi*8+16], r13

%endmacro

%macro  mpn_addmul_1_int 1

%%1:mov     rax, [r10+rdi*8]
    mov     r11, [r9]
    mov     rsi, rdi	
    mul     r11
    mov     r15, rax
    mov     rax, [r10+rdi*8+8]
    mov     r12, rdx
    cmp     rdi, 0
    jge     %%2
    add_mul_loop
%%2:add_mul_next_%1
    lea     r9, [r9+8]
    dec     r8
    lea     rcx, [rcx+8]
    jnz     %%1

%endmacro

   bits 64
   section .text

   global   __gmpn_mul_basecase

%ifdef DLL
   export   __gmpn_mul_basecase
%endif

__gmpn_mul_basecase:
    mov     r10, rdx
    movsxd  rdx, r8d
    cmp     rdx, 2
    ja      .3
    jz      .1
	mov     rax, [r10]
	mul     qword [r9]
	mov     [rcx], rax
	mov     [rcx+8], rdx
	ret

.1: mov     r11, [r9]
	mov     rax, [r10]
	mul     r11
	mov     [rcx], rax
	mov     rax, [r10+8]
	mov     r8, rdx
	mul     r11
	add     r8, rax
	adc     rdx, 0
	mov     [rcx+8], r8
	mov     [rcx+16], rdx
	cmp     dword [rsp+0x28], 2
	jz      .2
	ret

.2: mov     r8d, 0
	mov     r11, [r9+8]
	mov     rax, [r10]
	mul     r11
	add     [rcx+8], rax
	mov     rax, [r10+8]
	adc     r8, rdx
	mul     r11
	add     r8, rax
	adc     rdx, 0
	add     [rcx+16], r8
	adc     rdx, 0
	mov     [rcx+24], rdx
	ret	
	
.3: mov     r8d, dword [rsp+0x28]
    prologue mul_m_by_n, 0, reg_save_list
    mov     r11, [r9]
    mov     rdi, 5
    sub     rdi, rdx
    lea     rcx, [rcx+rdx*8-40]
    lea     r10, [r10+rdx*8-40]
    mpn_mul_1_int
    dec     r8
    lea     rcx, [rcx+8]
    lea     r9, [r9+8]
    jz      .9
.4: mov     rax, rdi
    and     rax, 3
    je      .6
    jp      .8
    cmp     rax, 1
    je      .7
    
    align   16
.5: mpn_addmul_1_int 2
    jmp     .9

    align   16
.6: mpn_addmul_1_int 0
    jmp     .9

    align   16
.7: mpn_addmul_1_int 1
    jmp     .9

    align   16
.8: mpn_addmul_1_int 3

.9: epilogue reg_save_list

    end
