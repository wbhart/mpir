; PROLOGUE(mpn_mul_basecase)

;  Copyright 2009 Jason Moxham
;
;  Windows Conversion Copyright 2008 Brian Gladman
;
;  This file is part of the MPIR Library.
;
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
;
;  mp_limb_t mpn_mul_basecase(mp_ptr, mp_ptr, mp_size_t, mp_ptr, mp_size_t)
;  rax                           rdi     rsi        rdx     rcx         r8
;  rax                           rcx     rdx         r8      r9   [rsp+40]

%include "yasm_mac.inc"

%macro addmul2lp 0
    xalign  16
%%1:mov     rax, [rsi+rbx*8]
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
    xalign  16
%%1:mov     rax, [rsi+rbx*8]
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
    xalign  16
%%1:mov     r10, 0
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
    xalign  16
%%1:addmul2pro%1
    addmul2lp
    addmul2epi%1
    jnz     %%1
%%2:
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
    xalign  16
%%1:
    oldaddmulpro%1
    oldaddmulnext%1
    jnz     %%1
%%2:
%endmacro

    CPU  Core2
    BITS 64

%define reg_save_list   rbx, rsi, rdi, r12, r13, r14

    LEAF_PROC mpn_mul_basecase
    ; the current mul does not handle case one
    cmp     r8d, 4
    jg      __gmpn_core2_mbc2
    cmp     r8d, 1
    je      one

    WIN64_GCC_PROC mpn_core2_mbc1, 5, frame
    movsxd  rdx, edx
    movsxd  r8, r8d

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
    ja      .4
    jz      .3
    jp      .2
.1:	oldmpn_muladdmul_1_int 0
    WIN64_GCC_EXIT frame

    xalign  16
.2:	oldmpn_muladdmul_1_int 1
    WIN64_GCC_EXIT frame

    xalign  16
.3:	oldmpn_muladdmul_1_int 2
    WIN64_GCC_EXIT frame

    xalign  16
.4:	oldmpn_muladdmul_1_int 3

    xalign  16
.5:	WIN64_GCC_END frame

; rdx >= 5  as we dont have an inner jump
; (rdi,rdx+r8)=(rsi,rdx)*(rcx,r8)

%undef  reg_save_list
%define reg_save_list   rbx, rsi, rdi, r12, r13, r14, r15

    xalign  16
.6:	WIN64_GCC_PROC mpn_core2_mbc2, 5, frame

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
    jnc     .13
.7:	inc     rbx
    mov     r8, [r13+r15*8]
    mul     r8
    mov     r12, rax
    mov     rax, [rsi+r14*8+8]
    mov     r9, rdx
    cmp     rbx, 0
    jge     .8
    mul1lp
.8:	mov     r10d, 0
    mul     r8
    mov     [rdi+rbx*8-8], r12
    add     r9, rax
    adc     r10, rdx
    cmp     rbx, 2
    ja      .12
    jz      .11
    jp      .10
.9:	mulnext0
    jmp     .21
.10:mulnext1
    jmp     .15
.11:mulnext2
    jmp     .17
.12:mulnext3
    jmp     .19
    ; as all the mul2pro? are the same
.13:mul2pro0
    mul2lp
    cmp     rbx, 2
    ja      .20
    jz      .18
    jp      .16
.14:mul2epi3
.15:mpn_addmul_2_int 3
    jmp     .22
.16:mul2epi2
.17:mpn_addmul_2_int 2
    jmp     .22
.18:mul2epi1
.19:mpn_addmul_2_int 1
    jmp     .22
.20:mul2epi0
.21:mpn_addmul_2_int 0

    xalign  16
.22:WIN64_GCC_END frame

    xalign  16
one:mov     rax, [rdx]
    mul     qword [r9]
    mov     [rcx], rax
    mov     [rcx+8], rdx
    ret
    
	end
