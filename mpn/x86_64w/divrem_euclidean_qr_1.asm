
;  Copyright 2008 Jason Moxham
;
;  Windows Conversion Copyright 2008 Brian Gladman
;
;  This file is part of the MPIR Library.
;
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
;  mp_limb_t mpn_divrem_euclidean_qr_1(mp_ptr, mp_ptr, mp_size_t, mp_limb_t)
;  rax                                    rdi     rsi        rdx        rcx
;  rax                                    rcx     rdx        r8d         r9

%include "yasm_mac.inc"

%define reg_save_list rsi, rdi, rbp, r12, r13, r14, r15

    BITS 64

    FRAME_PROC mpn_divrem_euclidean_qr_1, 0, reg_save_list
    mov     rdi, rcx
    mov     rsi, rdx
    movsxd  r14, r8d

    xor     r15, r15
    bsr     rcx, r9
    xor     rcx, 63
    shl     r9, cl
    xor     r11, r11

    mov     rdx, r9
    not     rdx
    xor     rax, rax
    not     rax
    div     r9
    mov     r8, rax
    xor     rax, rax
    xor     rbp, rbp

    xalign   16
.1: 
	mov     r13, [rsi+r14*8-8]
    mov     r12, r13
    neg     rcx
    cmovnc  r13, r15
    shr     r13, cl
    neg     rcx
    shl     r12, cl
    mov     r10, r12
    sar     r10, 63
    add     rax, r13
    sub     rax, r10
    add     rax, r11
    add     r11, r13
    add     r11, rbp
    mul     r8
    mov     r13, r11
    and     r10, r9
    add     r10, r12
    add     rax, r10
    mov     r10, r11
    adc     r10, rdx
    not     r10
    mov     rax, r9
    mul     r10
    sub     r13, r9
    add     rax, r12
    mov     r11, r9
    adc     rdx, r13
    and     r11, rdx
    mov     rbp, rax
    sub     rdx, r10
    mov     [rdi+r14*8-8], rdx
    dec     r14
    jnz     .1
    add     r11, rax
    shr     r11, cl
    mov     rax, r11
    END_PROC reg_save_list

    end