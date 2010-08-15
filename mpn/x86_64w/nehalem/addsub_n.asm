
;  Copyright 2009 Jason Moxham
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
;  mp_limb_t mpn_addsub_n(mp_ptr, mp_ptr, mp_ptr, mp_ptr, mp_size_t)
;  rax                       rdi     rsi     rdx     rcx         r8
;  rax                       rcx     rdx      r8      r9   [rsp+40]

%include "yasm_mac.inc"

%define reg_save_list   rbx, rbp, rsi, rdi

    CPU  Core2
    BITS 64

    FRAME_PROC mpn_addsub_n, 0, reg_save_list
    mov     rbx, qword [rsp+stack_use+40]

    lea     rdx, [rdx+rbx*8]
    lea     r8, [r8+rbx*8]
    lea     rcx, [rcx+rbx*8]
    lea     r9, [r9+rbx*8]
    neg     rbx
    xor     rax, rax
    xor     rsi, rsi
    test    rbx, 3
    jz      .2
.1:	mov     rdi, [r8+rbx*8]
    add     rax, 1
    sbb     rdi, [r9+rbx*8]
    sbb     rax, rax
    add     rsi, 1
    adc     rdi, [rdx+rbx*8]
    sbb     rsi, rsi
    mov     [rcx+rbx*8], rdi
    add     rbx, 1          ; ***
    test    rbx, 3
    jnz     .1
.2: cmp     rbx, 0
    jz      .4

    xalign  16
.3: add     rax, 1
    mov     rdi, [r8+rbx*8]
    mov     rbp, [r8+rbx*8+8]
    mov     r10, [r8+rbx*8+16]
    mov     r11, [r8+rbx*8+24]
    sbb     rdi, [r9+rbx*8]
    sbb     rbp, [r9+rbx*8+8]
    sbb     r10, [r9+rbx*8+16]
    sbb     r11, [r9+rbx*8+24]
    sbb     rax, rax
    add     rsi, 1
    adc     rdi, [rdx+rbx*8]
    adc     rbp, [rdx+rbx*8+8]
    adc     r10, [rdx+rbx*8+16]
    adc     r11, [rdx+rbx*8+24]
    mov     [rcx+rbx*8], rdi
    mov     [rcx+rbx*8+8], rbp
    mov     [rcx+rbx*8+16], r10
    mov     [rcx+rbx*8+24], r11
    sbb     rsi, rsi
    add     rbx, 4
    jnz     .3
.4: sub     rax, rsi
    END_PROC reg_save_list

    end
