
;  Core2 mpn_sqr_baseL5 -- square an mpn number.
;  Version 1.0L5
;
;  Copyright 2008 Jason Moxham
;
;  Windows Conversion Copyright 2008 Brian Gladman
;
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
;
;  Calling interface:
;
;  void mpn_sqr_baseL5(
;     mp_ptr dst,       rcx
;     mp_srcptr src,    rdx
;     mp_size_t size     r8
;  )
;
; Squuare src[size] and write the result to dst[2 * size]
;
; This is an SEH frame function with a leaf prologue

%include "..\yasm_mac.inc"

%define reg_save_list   rbx, rsi, rdi, r12, r13, r14

    BITS 64

%macro mulloop 0

    alignb  16, nop
%%1:mov     r10, 0
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
    add     r14, 1              ; ***
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
    add     r14, 1              ; ***
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
    add     r14, 1              ; ***
    lea     rdi, [rdi+8]

%endmacro

%macro mulnext3 0

    mov     [rdi+r11*8+8], r9
    mov     [rdi+r11*8+16], r10
    add     r14, 1              ; ***
    lea     rdi, [rdi+8]

%endmacro

%macro addmulloop 0

    alignb  16, nop
%%1:mov     r10, 0
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
    add     r14, 1              ; ***
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
    add     r14, 1              ; ***
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
    add     r14, 1              ; ***
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
    add     r14, 1              ; ***
    lea     rdi, [rdi+8]
    cmp     r14, 4

%endmacro

    LEAF_PROC mpn_sqr_basecase
    cmp     r8d, 3
    ja      fourormore
    jz      three
    jp      two
    mov     rax, [rdx]
    mul     rax
    mov     [rcx], rax
    mov     [rcx+8], rdx
    ret

    alignb  16, nop
fourormore:
    FRAME_PROC fourormore, 0, reg_save_list
    mov     rdi, rcx
    mov     rsi, rdx
    movsxd  rdx, r8d

    mov     [rsp+stack_use+8], rdi
    mov     [rsp+stack_use+16], rsi
    mov     [rsp+stack_use+24], rdx

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
    jge     .1
    mulloop

.1: mov     r10d, 0
    mul     r13
    mov     [rdi+r11*8], r12
    add     r9, rax
    adc     r10, rdx
    cmp     r11, 2
    je      L22
    ja      L23
    jp      L21
L20:mulnext0
    jmp     L51

    alignb  16, nop
L21:mulnext1
    jmp     L52

    alignb  16, nop
L22:mulnext2
    jmp     L53

    alignb  16, nop
L23:mulnext3

    alignb  16, nop
L3: mov     rax, [rsi+r14*8]
    mov     r13, [rsi+r14*8-8]
    mov     r11, r14
    mul     r13
    mov     r12, rax
    mov     rax, [rsi+r14*8+8]
    mov     r9, rdx
    cmp     r14, 0
    jge     L40
    addmulloop
L40:addmulnext0

L51:mov     rax, [rsi+r14*8]
    mov     r13, [rsi+r14*8-8]
    mov     r11, r14
    mul     r13
    mov     r12, rax
    mov     rax, [rsi+r14*8+8]
    mov     r9, rdx
    cmp     r14, 0
    jge     L41
    addmulloop
L41:addmulnext1

L52:mov     rax, [rsi+r14*8]
    mov     r13, [rsi+r14*8-8]
    mov     r11, r14
    mul     r13
    mov     r12, rax
    mov     rax, [rsi+r14*8+8]
    mov     r9, rdx
    cmp     r14, 0
    jge     L42
    addmulloop
L42:addmulnext2

L53:mov     rax, [rsi+r14*8]
    mov     r13, [rsi+r14*8-8]
    mov     r11, r14
    mul     r13
    mov     r12, rax
    mov     rax, [rsi+r14*8+8]
    mov     r9, rdx
    cmp     r14, 0
    jge     L43
    addmulloop
L43:addmulnext3
    jnz     L3

    mov     rax, [rsi+r14*8]
    mov     r13, [rsi+r14*8-8]
    mul     r13
    add     [rdi+r14*8], rax
    adc     rdx, 0
    mov     [rdi+r14*8+8], rdx

    mov     rdi, [rsp+stack_use+8]
    mov     rsi, [rsp+stack_use+16]
    mov     rcx, [rsp+stack_use+24]
    xor     rbx, rbx
    xor     r11, r11
    lea     rsi, [rsi+rcx*8]
    mov     [rdi], r11
    lea     r10, [rdi+rcx*8]
    mov     [r10+rcx*8-8], r11
    neg     rcx

    alignb  16, nop
.6: mov     rax, [rsi+rcx*8]
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
    add     rcx, 1          ; ***
    lea     rdi, [rdi+16]
    jnz     .6
    END_PROC reg_save_list

    alignb  16, nop
two:
    mov     rax, [rdx]
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

    align   16
three:
    FRAME_PROC three, 0, rsi, rdi
    mov     rdi, rcx
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
    mov     rdi, -3
    xor     r10, r10
    xor     r11, r11
    lea     rsi, [rsi+24]
    mov     [rcx], r11
    mov     [rcx+40], r11

.1: mov     rax, [rsi+rdi*8]
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
    add     rdi, 1      ; ***
    lea     rcx, [rcx+16]
    jnz     .1
    END_PROC rsi, rdi

    end
