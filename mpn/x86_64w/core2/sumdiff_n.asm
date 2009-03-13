
;  Core2 mpn_sumdiff_n
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
;  Calling interface (GCC):
;
;       rcx[r10] = r8[r10] + r9[r10]
;       rdx[r10] = r8[r10] - r9[r10]
;
;  return 2 * add_carry + sub_borrow
;
;  mp_limb_t __gmpn_sumdiff_n(
;       mp_ptr dps,         rcx
;       mp_ptr dpd,         rdx
;       mp_srcptr src1,      r8
;       mp_size_t src2,      r9
;       mp_size_t n        r10d
; )
;
; This is an SEH frame function

%include "..\yasm_mac.inc"

%define reg_save_list   rsi, rdi, r12, r13, r14, r15, rbx, rbp

    BITS 64

    FRAME_PROC mpn_sumdiff_n, 0, reg_save_list
    movsxd  r10, dword [rsp+stack_use+40]

    lea     rdx, [rdx+r10*8]
    lea     r8, [r8+r10*8]
    lea     rcx, [rcx+r10*8]
    lea     r9, [r9+r10*8]
    neg     r10
    xor     r12, r12
    xor     r11, r11
    test    r10, 3
    jz      .2

.1: mov     rax, [r8+r10*8]
    mov     r13, rax
    add     r12, 1
    adc     rax, [r9+r10*8]
    sbb     r12, r12
    add     r11, 1
    sbb     r13, [r9+r10*8]
    sbb     r11, r11
    mov     [rcx+r10*8], rax
    mov     [rdx+r10*8], r13
    add     r10, 1          ; ***
    test    r10, 3
    jnz     .1

.2: cmp     r10, 0
    jz      .4

    alignb  16, nop
.3: mov     rax, [r8+r10*8]
    mov     rsi, [r8+r10*8+8]
    mov     rdi, [r8+r10*8+16]
    mov     rbp, [r8+r10*8+24]
    mov     r13, rax
    mov     r14, rsi
    mov     r15, rdi
    mov     rbx, rbp
    add     r12, 1
    adc     rax, [r9+r10*8]
    adc     rsi, [r9+r10*8+8]
    adc     rdi, [r9+r10*8+16]
    adc     rbp, [r9+r10*8+24]
    sbb     r12, r12
    add     r11, 1
    sbb     r13, [r9+r10*8]
    sbb     r14, [r9+r10*8+8]
    sbb     r15, [r9+r10*8+16]
    sbb     rbx, [r9+r10*8+24]
    sbb     r11, r11
    mov     [rcx+r10*8], rax
    mov     [rcx+r10*8+8], rsi
    mov     [rcx+r10*8+16], rdi
    mov     [rcx+r10*8+24], rbp
    mov     [rdx+r10*8], r13
    mov     [rdx+r10*8+8], r14
    mov     [rdx+r10*8+16], r15
    mov     [rdx+r10*8+24], rbx
    add     r10, 4
    jnz     .3

.4: lea     rax, [r11+r12*2]
    neg     rax
    END_PROC reg_save_list

    end
