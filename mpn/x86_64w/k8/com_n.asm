; PROLOGUE(mpn_com_n)

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
;  void mpn_com_n(mp_ptr, mp_ptr, mp_size_t)
;                     rdi     rsi       rdx
;                     rcx     rdx        r8

%include "yasm_mac.inc"

    CPU  Athlon64
    BITS 64
    LEAF_PROC mpn_com_n
    mov     rax, r8
    sub     rax, 4
    jc      .2

    xalign  8
.1: mov     r8, [rdx+rax*8+24]
    mov     r9, [rdx+rax*8+16]
    not     r8
    not     r9
    mov     [rcx+rax*8+24], r8
    mov     [rcx+rax*8+16], r9
    mov     r8, [rdx+rax*8+8]
    mov     r9, [rdx+rax*8]
    not     r8
    not     r9
    mov     [rcx+rax*8+8], r8
    mov     [rcx+rax*8], r9
    sub     rax, 4
    jae     .1
.2: add     rax, 4
    jz      .3

; Could still have potential cache-bank conflicts in this tail part

    mov     r8, [rdx+rax*8-8]
    not     r8
    mov     [rcx+rax*8-8], r8
    dec     rax
    jz      .3
    mov     r8, [rdx+rax*8-8]
    not     r8
    mov     [rcx+rax*8-8], r8
    dec     rax
    jz      .3
    mov     r8, [rdx+rax*8-8]
    not     r8
    mov     [rcx+rax*8-8], r8
.3: ret

    end
