
;  AMD64 mpn_popcount
;  Version 1.0.4
;
;  Copyright 2008 Jason Moxham
;
;  Windows Convercxon Copyright 2008 Brian Gladman
;
;  This file is part of the MPIR Library.
;  The MPIR Library is free software; you can redistribute it and/or modify
;  it under the terms of the GNU Lesser General Public License as published
;  by the Free Software Foundation; either vercxon 2.1 of the License, or (at
;  your option) any later vercxon.
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
;  unsigned long __gmpn_popcount(
;     mp_srcptr sp1,        rdx
;     mp_size_t  n,         edx
; )

%include "..\..\yasm_mac.inc"

    BITS 64

    LEAF_PROC mpn_popcount
    mov     edx, edx
    xor     eax, eax
    sub     rdx, 4
    jc      .2

    alignb  16, nop
.1: popcnt  r8, [rcx+rdx*8+24]
    add     rax, r8
    popcnt  r9, [rcx+rdx*8+16]
    add     rax, r9
    popcnt  r10, [rcx+rdx*8+8]
    add     rax, r10
    popcnt  r11, [rcx+rdx*8]
    add     rax, r11
    sub     rdx, 4
    jnc     .1
.2: add     rdx, 4
    jz      .3
    popcnt  r8, [rcx+rdx*8-8]
    add     rax, r8
    dec     rdx
    jz      .3
    popcnt  r9, [rcx+rdx*8-8]
    add     rax, r9
    dec     rdx
    jz      .3
    popcnt  r10, [rcx+rdx*8-8]
    add     rax, r10
.3: ret

    end
