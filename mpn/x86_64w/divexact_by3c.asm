; PROLOGUE(mpn_divexact_by3c)

; Version 1.0.4.
;
;  Copyright 2008 Jason Moxham and Brian Gladman
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
;  mp_limb_t mpn_divexact_by3c(mp_ptr, mp_ptr, mp_size_t, mp_limb_t)
;  mp_limb_t  mpn_divexact_by3(mp_ptr, mp_ptr, mp_size_t)
;  rax                            rdi     rsi        rdx        rcx
;  rax                            rcx     rdx         r8         r9

%include "yasm_mac.inc"

    BITS 64

%define MLT1 0x5555555555555555

    xalign  16
    LEAF_PROC mpn_divexact_by3c
    mov     rax, r8
    mov     r8, MLT1
    imul    r9, r8
    jmp     entry

    xalign  16
    LEAF_PROC mpn_divexact_by3
    mov     rax, r8
    mov     r8, MLT1
    xor     r9, r9

entry:
    lea     r10, [rdx+rax*8-24]
    lea     r11, [rcx+rax*8-24]
    mov     ecx, 3
    sub     rcx, rax
    jnc     .2

    xalign  16
.1: mov     rax, [r10+rcx*8]
    mul     r8
    sub     r9, rax
    mov     [r11+rcx*8], r9
    sbb     r9, rdx
    mov     rax, [r10+rcx*8+8]
    mul     r8
    sub     r9, rax
    mov     [r11+rcx*8+8], r9
    sbb     r9, rdx
    mov     rax, [r10+rcx*8+16]
    mul     r8
    sub     r9, rax
    mov     [r11+rcx*8+16], r9
    sbb     r9, rdx
    mov     rax, [r10+rcx*8+24]
    mul     r8
    sub     r9, rax
    mov     [r11+rcx*8+24], r9
    sbb     r9, rdx
    add     rcx, 4
    jnc     .1
.2: test    rcx, 2
    jnz     .3
    mov     rax, [r10+rcx*8]
    mul     r8
    sub     r9, rax
    mov     [r11+rcx*8], r9
    sbb     r9, rdx
    mov     rax, [r10+rcx*8+8]
    mul     r8
    sub     r9, rax
    mov     [r11+rcx*8+8], r9
    sbb     r9, rdx
    add     rcx, 2
.3: test    rcx, 1
    jnz     .4
    mov     rax, [r10+rcx*8]
    mul     r8
    sub     r9, rax
    mov     [r11+rcx*8], r9
    sbb     r9, rdx
.4:	lea     rax, [r9+r9*2]
    neg     rax
    ret

    end
