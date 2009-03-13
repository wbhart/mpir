
;  Core2 mpn_divexact_byff
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
; mp_limb_t mpn_divexact_byff(
;     mp_ptr dst,       rcx
;     mp_srcptr src,    rdx
;     mp_size_t size,    r8
; )
;
;  This is an SEH leaf function (no unwind support needed)

%include "..\yasm_mac.inc"

    BITS 64

    LEAF_PROC mpn_divexact_byff
    movsxd  r8, r8d

    xor     eax, eax
    mov     r9, r8
    and     r9, 3
    shr     r8, 2
    cmp     r8, 0
    jnz     .2      ; carry flag is clear here
    sbb     rax, [rdx]
    mov     [rcx], rax
    dec     r9
    jz      .1
    sbb     rax, [rdx+8]
    mov     [rcx+8], rax
    dec     r9
    jz      .1
    sbb     rax, [rdx+16]
    mov     [rcx+16], rax
    dec     r9
.1: sbb     rax, 0
    ret

    alignb  16, nop
.2: sbb     rax, [rdx]
    mov     [rcx], rax
    sbb     rax, [rdx+8]
    mov     [rcx+8], rax
    sbb     rax, [rdx+16]
    mov     [rcx+16], rax
    sbb     rax, [rdx+24]
    mov     [rcx+24], rax
    lea     rdx, [rdx+32]
    dec     r8
    lea     rcx, [rcx+32]
    jnz     .2
    inc     r9
    dec     r9
    jz      .3
    sbb     rax, [rdx]
    mov     [rcx], rax
    dec     r9
    jz      .3
    sbb     rax, [rdx+8]
    mov     [rcx+8], rax
    dec     r9
    jz      .3
    sbb     rax, [rdx+16]
    mov     [rcx+16], rax
    dec     r9
.3: sbb     rax, 0
    ret

    end