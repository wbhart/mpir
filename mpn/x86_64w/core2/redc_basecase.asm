
;  Core2 mpn_redc_basecase
;  Version 1.0.4
;
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
;
;  Calling interface:
;
;  mp_limb_t __gmpn_redc_basecase(
;     mp_ptr dst,               rcx
;     mp_srcptr src,            rdx
;     mp_size_t len,             r8
;     mp_limb_t npr,             r9
;     mp_ptr tp          [rsp + 40]
; )
;

%include "..\yasm_mac.inc"

%define reg_save_list   rbx, rsi, rdi, rbp, r12, r13, r14, r15

    BITS 64

%macro mpn_add 0

    mov     rax, rcx
    and     rax, 3
    shr     rcx, 2
    cmp     rcx, 0
    jnz     %%1
    mov     r11, [rsi]
    add     r11, [rdx]
    mov     [rdi], r11
    dec     rax
    jz      %%2
    mov     r11, [rsi+8]
    adc     r11, [rdx+8]
    mov     [rdi+8], r11
    dec     rax
    jz      %%2
    mov     r11, [rsi+16]
    adc     r11, [rdx+16]
    mov     [rdi+16], r11
    jmp     %%2

    alignb  16, nop
%%1:mov     r11, [rsi]
    mov     r8, [rsi+8]
    lea     rsi, [rsi+32]
    adc     r11, [rdx]
    adc     r8, [rdx+8]
    lea     rdx, [rdx+32]
    mov     [rdi], r11
    mov     [rdi+8], r8
    lea     rdi, [rdi+32]
    mov     r9, [rsi-16]
    mov     r10, [rsi-8]
    adc     r9, [rdx-16]
    adc     r10, [rdx-8]
    mov     [rdi-16], r9
    dec     rcx
    mov     [rdi-8], r10
    jnz     %%1
    inc     rax
    dec     rax
    jz      %%2
    mov     r11, [rsi]
    adc     r11, [rdx]
    mov     [rdi], r11
    dec     rax
    jz      %%2
    mov     r11, [rsi+8]
    adc     r11, [rdx+8]
    mov     [rdi+8], r11
    dec     rax
    jz      %%2
    mov     r11, [rsi+16]
    adc     r11, [rdx+16]
    mov     [rdi+16], r11
%%2:

%endmacro

%macro mpn_sub 0

    mov     rax, rbp
    and     rax, 3
    shr     rbp, 2
    cmp     rbp, 0
    jnz     %%1
    mov     r11, [rsi]
    sub     r11, [rdx]
    mov     [rbx], r11
    dec     rax
    jz      %%2
    mov     r11, [rsi+8]
    sbb     r11, [rdx+8]
    mov     [rbx+8], r11
    dec     rax
    jz      %%2
    mov     r11, [rsi+16]
    sbb     r11, [rdx+16]
    mov     [rbx+16], r11
    jmp     %%2
    alignb  16, nop
%%1:mov     r11, [rsi]
    mov     r8, [rsi+8]
    lea     rsi, [rsi+32]
    sbb     r11, [rdx]
    sbb     r8, [rdx+8]
    lea     rdx, [rdx+32]
    mov     [rbx], r11
    mov     [rbx+8], r8
    lea     rbx, [rbx+32]
    mov     r9, [rsi-16]
    mov     r10, [rsi-8]
    sbb     r9, [rdx-16]
    sbb     r10, [rdx-8]
    mov     [rbx-16], r9
    dec     rbp
    mov     [rbx-8], r10
    jnz     %%1
    inc     rax
    dec     rax
    jz      %%2
    mov     r11, [rsi]
    sbb     r11, [rdx]
    mov     [rbx], r11
    dec     rax
    jz      %%2
    mov     r11, [rsi+8]
    sbb     r11, [rdx+8]
    mov     [rbx+8], r11
    dec     rax
    jz      %%2
    mov     r11, [rsi+16]
    sbb     r11, [rdx+16]
    mov     [rbx+16], r11
%%2:

%endmacro

%macro addmulloop 1

    alignb  16, nop
%%1:mov     r10, 0
    mul     r13
    add     [r8+r11*8], r12
    adc     r9, rax
    db      0x26
    adc     r10, rdx
    mov     rax, [rsi+r11*8+16]
    mul     r13
    add     [r8+r11*8+8], r9
    adc     r10, rax
    mov     ebx, 0
    adc     rbx, rdx
    mov     rax, [rsi+r11*8+24]
    mov     r12, 0
    mov     r9, 0
    mul     r13
    add     [r8+r11*8+16], r10
    db      0x26
    adc     rbx, rax
    db      0x26
    adc     r12, rdx
    mov     rax, [rsi+r11*8+32]
    mul     r13
    add     [r8+r11*8+24], rbx
    db      0x26
    adc     r12, rax
    db      0x26
    adc     r9, rdx
    add     r11, 4
    mov     rax, [rsi+r11*8+8]
    jnc     %%1

%endmacro

%macro addmulpropro0 0

    imul    r13, rcx
    lea     r8, [r8-8]

%endmacro

%macro addmulpro0 0

    mov     r11, r14
    lea     r8, [r8+8]
    mov     rax, [rsi+r14*8]
    mul     r13
    mov     r12, rax
    mov     rax, [rsi+r14*8+8]
    mov     r9, rdx
    cmp     r14, 0

%endmacro

%macro addmulnext0 0

    mov     r10d, 0
    mul     r13
    add     [r8+r11*8], r12
    adc     r9, rax
    adc     r10, rdx
    mov     rax, [rsi+r11*8+16]
    mul     r13
    add     [r8+r11*8+8], r9
    adc     r10, rax
    mov     ebx, 0
    adc     rbx, rdx
    mov     rax, [rsi+r11*8+24]
    mov     r12d, 0
    mov     r9d, 0
    mul     r13
    add     [r8+r11*8+16], r10
    adc     rbx, rax
    adc     r12, rdx
    mov     rax, [rsi+r11*8+32]
    mul     r13
    add     [r8+r11*8+24], rbx
    mov     r13, [r8+r14*8+8]
    adc     r12, rax
    adc     r9, rdx
    imul    r13, rcx
    add     [r8+r11*8+32], r12
    adc     r9, 0
    sub     r15, 1          ; ***
    mov     [r8+r14*8], r9

%endmacro

%macro addmulpropro1 0

%endmacro

%macro addmulpro1 0

    imul    r13, rcx
    mov     rax, [rsi+r14*8]
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
    add     [r8+r11*8], r12
    adc     r9, rax
    adc     r10, rdx
    mov     rax, [rsi+r11*8+16]
    mul     r13
    add     [r8+r11*8+8], r9
    adc     r10, rax
    mov     ebx, 0
    adc     rbx, rdx
    mov     rax, [rsi+r11*8+24]
    mov     r12d, 0
    mul     r13
    add     [r8+r11*8+16], r10
    adc     rbx, rax
    adc     r12, rdx
    add     [r8+r11*8+24], rbx
    mov     r13, [r8+r14*8+8]
    adc     r12, 0
    sub     r15, 1          ; ***
    mov     [r8+r14*8], r12
    lea     r8, [r8+8]

%endmacro

%macro addmulpropro2 0

%endmacro

%macro addmulpro2 0

    imul    r13, rcx
    mov     rax, [rsi+r14*8]
    mov     r11, r14
    mul     r13
    mov     r12, rax
    mov     rax, [rsi+r14*8+8]
    mov     r9, rdx
    cmp     r14, 0

%endmacro

%macro addmulnext2 0

    mul     r13
    add     [r8+r11*8], r12
    adc     r9, rax
    mov     r10d, 0
    adc     r10, rdx
    mov     rax, [rsi+r11*8+16]
    mul     r13
    add     [r8+r11*8+8], r9
    adc     r10, rax
    mov     ebx, 0
    adc     rbx, rdx
    mov     r13, [r8+r14*8+8]
    add     [r8+r11*8+16], r10
    adc     rbx, 0
    mov     [r8+r14*8], rbx
    sub     r15, 1          ; ***
    lea     r8, [r8+8]

%endmacro

%macro addmulpropro3 0

%endmacro

%macro addmulpro3 0

    imul    r13, rcx
    mov     rax, [rsi+r14*8]
    mov     r11, r14
    mul     r13
    mov     r12, rax
    mov     rax, [rsi+r14*8+8]
    mov     r9, rdx
    cmp     r14, 0

%endmacro

%macro addmulnext3 0

    mul     r13
    add     [r8+r11*8], r12
    adc     r9, rax
    mov     r10d, 0
    adc     r10, rdx
    add     [r8+r11*8+8], r9
    adc     r10, 0
    mov     r13, [r8+r14*8+8]
    mov     [r8+r14*8], r10
    lea     r8, [r8+8]
    sub     r15, 1          ; ***

%endmacro

%macro mpn_addmul_1_int 1

    addmulpropro%1
    alignb  16, nop
%%1:addmulpro%1
    jge     %%2
    addmulloop %1
%%2:addmulnext%1
    jnz     %%1

%endmacro

    LEAF_PROC mpn_redc_basecase
    cmp     r8d, 1
    je      one
    FRAME_PROC redc_bc, 0, reg_save_list
    mov     rdi, rcx
    mov     rsi, rdx
    movsxd  rdx, r8d
    mov     rcx, r9
    mov     r8, [rsp+stack_use+0x28]

    mov     r14, 5
    sub     r14, rdx

    mov     [rsp+stack_use+0x10], rsi
    mov     r8, [rsp+stack_use+0x28]

    lea     r8, [r8+rdx*8-40]
    lea     rsi, [rsi+rdx*8-40]
    mov     rbp, rdx
    mov     r15, rdx
    mov     rax, r14
    and     rax, 3
    mov     r13, [r8+r14*8]
    je      .10
    jp      .13
    cmp     rax, 1
    je      .11
.12:mpn_addmul_1_int 2
    jmp     .2

    alignb  16, nop
.10:mpn_addmul_1_int 0
    jmp     .2

    alignb  16, nop
.11:mpn_addmul_1_int 1
    jmp     .2

    alignb  16, nop
.13:mpn_addmul_1_int 3

    alignb  16, nop
.2:
    mov     rcx, rbp
    mov     rdx, [rsp+stack_use+0x28]
    lea     rsi, [rdx+rbp*8]
    mov     rbx, rdi
    mpn_add
    mov     rdx, [rsp+stack_use+0x10]
    jnc     .3
    mov     rsi, rbx
    mpn_sub

.3:
    END_PROC reg_save_list

    alignb  16, nop
one:mov     r8,[rsp+0x28]
    mov     r10, [r8]
    mov     r11, [rdx]
    imul    r9, r10
    mov     rax, r9
    mul     r11
    add     rax, r10
    adc     rdx, [r8+8]     ; rax is zero here
    cmovnc  r11, rax
    sub     rdx, r11
    mov     [rcx], rdx
    ret

    end
