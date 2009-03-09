
;  Copyright 1999, 2000, 2001, 2002 Free Software Foundation, Inc.
;
;  Copyright 2008 Brian Gladman
;
;  This file is part of the MPIR Library.
;
;  The MPIR Library is free software; you can redistribute it and/or
;  modify it under the terms of the GNU Lesser General Public License as
;  published by the Free Software Foundation; either version 2.1 of the
;  License, or (at your option) any later version.
;
;  The MPIR Library is distributed in the hope that it will be useful,
;  but WITHOUT ANY WARRANTY; without even the implied warranty of
;  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
;  Lesser General Public License for more details.
;
;  You should have received a copy of the GNU Lesser General Public
;  License along with the MPIR Library; see the file COPYING.LIB.  If
;  not, write to the Free Software Foundation, Inc., 51 Franklin Street,
;  Fifth Floor, Boston, MA 02110-1301, USA.
;
; AMD64 mpn_divexact_1 -- mpn by limb exact division
;
;  Calling interface:
;
; void mpn_divexact_1(
;     mp_ptr dst,           rcx
;       mp_srcptr src,      rdx
;       mp_size_t size,      r8
;       mp_limb_t divisor    r9
; )
;
; since the inverse takes a while to setup,plain division is used for small
; Multiplying works out faster for size>=3 when the divisor is odd or size>=4
; when the divisor is even.
;
; This is an SEH Frame Function with a leaf prologue

%include "yasm_mac.inc"

%define reg_save_list       rsi, rdi

    BITS 64

    extern  __gmp_modlimb_invert_table
    
    LEAF_PROC mpn_divexact_1
    movsxd  r8, r8d
    mov     r10, rdx
    mov     rax, r9
    and     rax, byte 1
    add     rax, r8
    cmp     rax, byte 4
    jae     .1
    xor     rdx,rdx

.0: mov     rax, [r10+r8*8-8]
    div     r9
    mov     [rcx+r8*8-8], rax
    sub     r8, 1
    jnz     .0
    ret                     ; avoid single byte return
.1:
    FRAME_PROC mul_by_inverse, 0, reg_save_list
    mov     rsi, rdx        ; src pointer
    mov     rdi, rcx        ; dst pointer
    bsf     rcx, r9         ; remove powers of two
    shr     r9, cl
    mov     rax, r9
    shr     rax, 1
    and     rax, 127
    lea     rdx, [rel __gmp_modlimb_invert_table]
    movzx   rax, byte [rdx+rax]

; If f(x) = 0, then x[n+1] = x[n] - f(x) / f'(x) is Newton's iteration for a
; root. With f(x) = 1/x - v we obtain x[n + 1] = 2 * x[n] - v * x[n] * x[n]
; as an iteration for x = 1 / v.  This provides quadratic convergence so
; that the number of bits of precision doubles on each iteration.  The
; iteration starts with 8-bit precision.

    lea     edx, [rax+rax]
    imul    eax, eax
    imul    eax, r9d
    sub     edx, eax            ; inv -> rdx (16-bit approx)

    lea     eax, [rdx+rdx]
    imul    edx, edx
    imul    edx, r9d
    sub     eax, edx            ; inv -> rdx (32-bit approx)

    lea     rdx, [rax+rax]
    imul    rax, rax
    imul    rax, r9
    sub     rdx, rax            ; inv -> rdx (64-bit approx)

    lea     rsi, [rsi+r8*8]
    lea     rdi, [rdi+r8*8]
    neg     r8

    mov     r10, rdx            ; inverse multiplier -> r10
    xor     r11, r11
    mov     rax, [rsi+r8*8]
    or      rcx, rcx
    mov     rdx, [rsi+r8*8+8]
    jz      .3                  ; if divisor is odd
    shrd    rax, rdx, cl
    add     r8, 1
    jmp     .5

    alignb  16, nop
.2: mul     r9                  ; divisor is odd
    mov     rax, [rsi+r8*8]
    sub     rdx, r11
    sub     rax, rdx
    sbb     r11, r11
.3: imul    rax, r10
    mov     [rdi+r8*8], rax
    add     r8, 1
    jnz     .2
    jmp     .6

    alignb  16, nop
.4: mul     r9                  ; divisor is even
    sub     rdx, r11
    mov     rax, [rsi+r8*8-8]
    mov     r11, [rsi+r8*8]
    shrd    rax, r11, cl
    sub     rax, rdx
    sbb     r11, r11
.5: imul    rax, r10
    mov     [rdi+r8*8-8],rax
    add     r8, 1
    jnz     .4

    mul     r9
    mov     rax, [rsi-8]
    sub     rdx, r11
    shr     rax, cl
    sub     rax, rdx
    imul    rax, r10
    mov     [rdi-8], rax

.6: END_PROC reg_save_list

    end
