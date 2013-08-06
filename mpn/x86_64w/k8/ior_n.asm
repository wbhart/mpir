; PROLOGUE(mpn_ior_n)

;  Version 1.0.4
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
;  void mpn_ior_n(mp_ptr, mp_srcptr, mp_srcptr, mp_size_t)
;                    rdi        rsi        rdx        rcx
;                    rcx        rdx         r8         r9

%include "yasm_mac.inc"

    CPU  Athlon64
    BITS 64

%define T3      r10
%define T4      r11

    LEAF_PROC mpn_ior_n
    mov     rax, r9
    and     rax, 3
    shr     r9, 2
    jz      .2

    xalign  8
.1: mov     r10, [rdx]
    mov     r11, [rdx+8]
    lea     rdx, [rdx+32]
    or      r10, [r8]
    or      r11, [r8+8]
    lea     r8, [r8+32]
    mov     [rcx], r10
    mov     [rcx+8], r11
    lea     rcx, [rcx+32]
    mov     T3, [rdx-16]
    mov     T4, [rdx-8]
    or      T3, [r8-16]
    or      T4, [r8-8]
    mov     [rcx-16], T3
    dec     r9
    mov     [rcx-8], T4
    jnz     .1
.2: inc     rax
    dec     rax
    jz      .3
    mov     r10, [rdx]
    or      r10, [r8]
    mov     [rcx], r10
    dec     rax
    jz      .3
    mov     r10, [rdx+8]
    or      r10, [r8+8]
    mov     [rcx+8], r10
    dec     rax
    jz      .3
    mov     r10, [rdx+16]
    or      r10, [r8+16]
    mov     [rcx+16], r10
.3: ret

    end
