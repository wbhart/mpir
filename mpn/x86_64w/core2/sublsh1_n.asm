
;  Core2 mpn_sublsh1_n
;  Version 1.0.3
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
;  rcx[r10] = rdx[r10] - 2 * r14[r10]
;
;  void __gmpn_sublsh1_n(
;     mp_ptr rp,          rcx
;     mp_srcptr xp,       rdx
;     mp_srcptr yp,        r8
;     mp_size_t  n,        r9
;  )
;
; This is an SEH Frame Function

%include "..\yasm_mac.inc"

%define reg_save_list rbx, rsi, rdi, r12, r13, r14, r15

    BITS 64

    FRAME_PROC mpn_sublsh1_n, 0, reg_save_list
    movsxd  rax, r9d

    lea     rsi, [rdx+rax*8]
    lea     rdx, [r8+rax*8]
    lea     rdi, [rcx+rax*8]
    mov     rcx, rax

    neg     rcx
    xor     r9, r9
    xor     rax, rax
    test    rcx, 3
    jz      .2

.1: mov     r10, [rsi+rcx*8]
    add     r9, 1
    sbb     r10, [rdx+rcx*8]
    sbb     r9, r9
    add     rax, 1
    sbb     r10, [rdx+rcx*8]
    sbb     rax, rax
    mov     [rdi+rcx*8], r10
    add     rcx, 1              ; ***
    test    rcx, 3
    jnz     .1
.2: cmp     rcx, 0
    jz      .4

    alignb  16, nop
.3: mov     r10, [rsi+rcx*8]
    mov     rbx, [rsi+rcx*8+8]
    mov     r11, [rsi+rcx*8+16]
    mov     r8, [rsi+rcx*8+24]
    mov     r12, [rdx+rcx*8]
    mov     r13, [rdx+rcx*8+8]
    mov     r14, [rdx+rcx*8+16]
    mov     r15, [rdx+rcx*8+24]
    add     r9, 1
    sbb     r10, r12
    sbb     rbx, r13
    sbb     r11, r14
    sbb     r8, r15
    sbb     r9, r9
    add     rax, 1
    sbb     r10, r12
    sbb     rbx, r13
    sbb     r11, r14
    sbb     r8, r15
    sbb     rax, rax
    mov     [rdi+rcx*8], r10
    mov     [rdi+rcx*8+8], rbx
    mov     [rdi+rcx*8+16], r11
    mov     [rdi+rcx*8+24], r8
    add     rcx, 4
    jnz     .3
.4: add     rax, r9
    neg     rax
    END_PROC reg_save_list

    end
