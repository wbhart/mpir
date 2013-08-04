; PROLOGUE(mpn_addlsh1_n)

;  Copyright 2008 Jason Moxham
;
;  This file is part of the MPIR Library.
;
;  Windows Conversion Copyright 2008 Brian Gladman
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
;  mp_limb_t mpn_addlsh1_n(mp_ptr, mp_ptr, mp_ptr, mp_size_t)
;  rax                        rdi     rsi     rdx        rcx
;  rax                        rcx     rdx      r8         r9

%include "yasm_mac.inc"

%define reg_save_list rbx, rsi, rdi

    CPU  Core2
    BITS 64

    FRAME_PROC mpn_addlsh1_n, 0, reg_save_list
    lea     rdx, [rdx+r9*8]
    lea     r8, [r8+r9*8]
    lea     rcx, [rcx+r9*8]
    neg     r9
    xor     rsi, rsi
    xor     rax, rax
    test    r9, 3
    jz      .2
.1:	mov     rdi, [r8+r9*8]
    add     rsi, 1
    adc     rdi, rdi
    sbb     rsi, rsi
    add     rax, 1
    adc     rdi, [rdx+r9*8]
    sbb     rax, rax
    mov     [rcx+r9*8], rdi
    add     r9, 1           ; ***
    test    r9, 3
    jnz     .1
.2: cmp     r9, 0
    jz      .4

    xalign  16
.3: mov     rdi, [r8+r9*8]
    mov     rbx, [r8+r9*8+8]
    mov     r10, [r8+r9*8+16]
    mov     r11, [r8+r9*8+24]
    add     rsi, 1
    adc     rdi, rdi
    adc     rbx, rbx
    adc     r10, r10
    adc     r11, r11
    sbb     rsi, rsi
    add     rax, 1
    adc     rdi, [rdx+r9*8]
    adc     rbx, [rdx+r9*8+8]
    adc     r10, [rdx+r9*8+16]
    adc     r11, [rdx+r9*8+24]
    sbb     rax, rax
    mov     [rcx+r9*8], rdi
    mov     [rcx+r9*8+8], rbx
    mov     [rcx+r9*8+16], r10
    mov     [rcx+r9*8+24], r11
    add     r9, 4
    jnz     .3
.4: add     rax, rsi
    neg     rax
    END_PROC reg_save_list

    end
