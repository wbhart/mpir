; PROLOGUE(mpn_lshift1)

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
;  mp_limb_t mpn_lshift1(mp_ptr, mp_ptr, mp_size_t)
;  rax                      rdi     rsi        rdx
;  rax                      rcx     rdx        r8d

%include "yasm_mac.inc"

%define reg_save_list r12, r13

    BITS 64

    FRAME_PROC mpn_lshift1, 0, reg_save_list
    xor     rax, rax
    mov     r9, r8
    and     r9, 7
    inc     r9
    shr     r8, 3
; and clear carry flag
    cmp     r8, 0
    jz      .2

    alignb  16, nop
.1: 
	mov     r10, [rdx]
    mov     r11, [rdx+8]
    mov     r12, [rdx+16]
    mov     r13, [rdx+24]
    adc     r10, r10
    adc     r11, r11
    adc     r12, r12
    adc     r13, r13
    mov     [rcx], r10
    mov     [rcx+8], r11
    mov     [rcx+16], r12
    mov     [rcx+24], r13
    mov     r10, [rdx+32]
    mov     r11, [rdx+40]
    mov     r12, [rdx+48]
    mov     r13, [rdx+56]
    adc     r10, r10
    adc     r11, r11
    adc     r12, r12
    adc     r13, r13
    mov     [rcx+32], r10
    mov     [rcx+40], r11
    mov     [rcx+48], r12
    mov     [rcx+56], r13
    lea     rcx, [rcx+64]
    dec     r8
    lea     rdx, [rdx+64]
    jnz     .1
.2:
    dec     r9
    jz      .3
; Could still have cache-bank conflicts in this tail part
    mov     r10, [rdx]
    adc     r10, r10
    mov     [rcx], r10
    dec     r9
    jz      .3
    mov     r10, [rdx+8]
    adc     r10, r10
    mov     [rcx+8], r10
    dec     r9
    jz      .3
    mov     r10, [rdx+16]
    adc     r10, r10
    mov     [rcx+16], r10
    dec     r9
    jz      .3
    mov     r10, [rdx+24]
    adc     r10, r10
    mov     [rcx+24], r10
    dec     r9
    jz      .3
    mov     r10, [rdx+32]
    adc     r10, r10
    mov     [rcx+32], r10
    dec     r9
    jz      .3
    mov     r10, [rdx+40]
    adc     r10, r10
    mov     [rcx+40], r10
    dec     r9
    jz      .3
    mov     r10, [rdx+48]
    adc     r10, r10
    mov     [rcx+48], r10
.3:
    adc     rax, rax
    END_PROC reg_save_list

    end
