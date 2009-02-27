
;  Copyright 1999, 2000, 2001, 2002 Free Software Foundation, Inc.
;
;  Copyright 2008 Brian Gladman
;
;  This file is part of the MPIR Library.
;
;  The MPIR Library is free software; you can redistribute it and/or
;  modify it under the terms of the GNU Lesser General Public License as
;  published by the Free Software Foundation; either version 2.1 of the
;  License, or (at your option) any later version.
;
;  The MPIR Library is distributed in the hope that it will be useful,
;  but WITHOUT ANY WARRANTY; without even the implied warranty of
;  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
;  Lesser General Public License for more details.
;
;  You should have received a copy of the GNU Lesser General Public
;  License along with the MPIR Library; see the file COPYING.LIB.  If
;  not, write to the Free Software Foundation, Inc., 51 Franklin Street,
;  Fifth Floor, Boston, MA 02110-1301, USA.
;
; mp_limb_t mpn_modexact_1_odd (mp_srcptr src, mp_size_t size,
;                               mp_limb_t divisor);
; mp_limb_t mpn_modexact_1c_odd (mp_srcptr src, mp_size_t size,
;                                mp_limb_t divisor, mp_limb_t carry);
;
;  AMD64 mpn_modexact_1_odd -- exact division style remainder.
;
; mp_limb_t mpn_modexact_1_odd (
;  mp_srcptr src,           rcx
;  mp_size_t size,          rdx
;  mp_limb_t divisor         r8
; );
; mp_limb_t mpn_modexact_1c_odd (
;  mp_srcptr src,           rcx
;  mp_size_t size,          rdx
;  mp_limb_t divisor,        r8
;  mp_limb_t carry           r9
; );
;
; This is an SEH Frame Function with a leaf prologue

%include "x86_64_asm.inc"

%define reg_save_list       rsi, rdi

    bits    64
    section .text
    align   32

    global  __gmpn_modexact_1_odd
    global  __gmpn_modexact_1c_odd
    extern  __gmp_modlimb_invert_table

%ifdef DLL
    export  __gmpn_modexact_1_odd
    export  __gmpn_modexact_1c_odd
%endif

__gmpn_modexact_1_odd:
    mov     r9, 0               ; carry

prologue    __gmpn_modexact_1c_odd, 0, reg_save_list
    
    ; first use Newton's iteration to invert the divisor limb (d) using 
    ; f(x) = 1/x - d  and x[i+1] = x[i] - f(x[i]) / f'(x[i]) to give
    ; the iteration formula: x[i+1] = x[i] * (2 - d * x[i])
    
    movsxd  rsi, edx
    mov     rdx, r8    
    shr     edx, 1              ; div / 2
    lea     r10, [rel __gmp_modlimb_invert_table]
    and     edx, 127
    movzx   edx, byte [rdx+r10] ; inv -> rdx (8-bit approx)

    mov     rax, [rcx]          ; first limb of numerator
    lea     r11, [rcx+rsi*8]    ; pointer to top of src
    mov     rdi, r8             ; save divisor

    lea     ecx, [rdx+rdx]
    imul    edx, edx
    neg     rsi                 ; limb offset from top of src
    imul    edx, edi
    sub     ecx, edx            ; inv -> rcx (16-bit approx)

    lea     edx, [rcx+rcx]
    imul    ecx, ecx
    imul    ecx, edi
    sub     edx, ecx            ; inv -> rdx (32-bit approx)
    xor     ecx, ecx

    lea     r10, [rdx+rdx]
    imul    rdx, rdx
    imul    rdx, r8
    sub     r10, rdx            ; inv -> r10 (64-bit approx)

    mov     rdx, r9             ; intial carry -> rdx
    add     rsi, 1              ; adjust limb offset
    jz      .1

    mov     r9, r11
    lea     rsi,[r11+rsi*8]
    align    16

.0: sub     rax, rdx
    adc     rcx, 0
    imul    rax, r10
    mul     r8
    lodsq
    sub     rax, rcx
    setc    cl
    cmp     r9, rsi
    jne     .0

.1: sub     rax, rdx
    adc     rcx, 0
    imul    rax, r10
    mul     r8
    lea     rax, [rcx+rdx]
    epilogue    reg_save_list

    end
