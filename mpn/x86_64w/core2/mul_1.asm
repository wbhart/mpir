
; Core2 mpn_mul_1 -- mpn by limb multiply (with carry)
; Version 1.0.3.
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
;  mp_limb_t mpn_mul_1 (
;     mp_ptr dst,                  rcx
;     mp_srcptr src,               rdx
;     mp_size_t size,               r8
;     mp_limb_t multiplier          r9
;  )
;
;  mp_limb_t mpn_mul_1c (
;     mp_ptr dst,                  rcx
;     mp_srcptr src,               rdx
;     mp_size_t size,               r8
;     mp_limb_t multiplier,         r9
;     mp_limb_t carry       [rsp+0x28]
;  )
;
;  This is an SEH frame function with two leaf prologues

%include "..\yasm_mac.inc"

    BITS 64

    LEAF_PROC mpn_mul_1c
    mov     r11, [rsp+0x28]
    jmp     start

    LEAF_PROC mpn_mul_1
    xor     r11, r11
    jmp     start

    align   16
start:
    FRAME_PROC mulmm, 0, rbx
    movsxd  rax, r8d
    mov     r8d, 3
    lea     r10, [rdx+rax*8-24]
    sub     r8, rax
    lea     rcx, [rcx+rax*8-24]
    jc      .1
    jmp     .2

    alignb  16, nop
.1: mov     rax, [r10+r8*8]
    mov     ebx, 0
    mul     r9
    add     r11, rax
    mov     [rcx+r8*8], r11
    mov     rax, [r10+r8*8+8]
    adc     rbx, rdx
    mul     r9
    mov     r11d, 0
    add     rbx, rax
    mov     rax, [r10+r8*8+16]
    adc     r11, rdx
    mul     r9
    mov     [rcx+r8*8+8], rbx
    add     r11, rax
    mov     ebx, 0
    mov     [rcx+r8*8+16], r11
    mov     rax, [r10+r8*8+24]
    mov     r11d, 0
    adc     rbx, rdx
    mul     r9
    add     rbx, rax
    mov     [rcx+r8*8+24], rbx
    adc     r11, rdx
    add     r8, 4
    jnc     .1

.2: test    r8, 2
    jnz     .3
    mov     rax, [r10+r8*8]
    mov     ebx, 0
    mul     r9
    add     r11, rax
    mov     [rcx+r8*8], r11
    mov     rax, [r10+r8*8+8]
    adc     rbx, rdx
    mul     r9
    mov     r11d, 0
    add     rbx, rax
    adc     r11, rdx
    add     r8, 2
    mov     [rcx+r8*8-8], rbx

.3: test    r8, 1
    mov     rax, r11
    jnz     .4
    mov     rax, [r10+r8*8]
    mov     ebx, 0
    mul     r9
    add     r11, rax
    mov     [rcx+r8*8], r11
    adc     rbx, rdx
    mov     rax, rbx
.4: END_PROC rbx

    end
