
; Core2 mpn_divexact_by3c -- divide exact by 3
; Version 1.0.4.
;
;  Copyright 2008 Jason Moxham and Brian Gladman
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
; mp_limb_t mpn_divexact_by3c (
;     mp_ptr rp,                rcx
;     mp_srcptr up,             rdx
;     mp_size_t un,              r8
;     mp_limb_t c                r9
; )
;
; This is an SEH leaf function

%include "..\yasm_mac.inc"

    BITS 64

%define MLT1 0x5555555555555555

%macro  mul_sub1 1
    mov     rax, [r10+r8*8+8*%1]
    mul     r11
    sub     r9, rax
    mov     [rcx+r8*8+8*%1], r9
    sbb     r9, rdx
%endmacro

%define MLT2 0xaaaaaaaaaaaaaaab

%macro  mul_sub2 1
    mov     rax, [r10+r8*8+8*%1]
    sub     rax, r9
    mov     r9, 0
    adc     r9, r9
    mul     r11
    mov     [rcx+r8*8+8*%1], rax
    mov     rdx, r11
    dec     rdx
    cmp     rdx, rax
    adc     r9, 0
    shr     rdx, 1
    cmp     rdx, rax
    adc     r9, 0
%endmacro

;  div_by_3 name, algorithm, carry_input (if present use carry)

%macro div_by_3 2-3

    LEAF_PROC %1
    movsxd  rax, r8d
    mov     r8d, 3
    lea     r10, [rdx+rax*8-24]
    lea     rcx, [rcx+rax*8-24]
    mov     r11, MLT%2
%if %0 == 2
    xor     r9, r9
%else
    imul    r9, r11
%endif
    sub     r8, rax
    jnc     %%2

    alignb  16, nop
%%1:mul_sub%2 0
    mul_sub%2 1
    mul_sub%2 2
    mul_sub%2 3
    add     r8, 4
    jnc     %%1

; so have 3-r8 limbs left to do

%%2:test    r8, 2
    jnz     %%3
    mul_sub%2 0
    mul_sub%2 1
    add     r8, 2

%%3:test    r8, 1
    jnz     %%4
    mul_sub%2 0

%%4:lea     rax, [r9+r9*2]
    neg     rax
    ret

%endmacro

    div_by_3  mpn_divexact_by3,  1
    div_by_3  mpn_divexact_by3c, 1, 1

    end
