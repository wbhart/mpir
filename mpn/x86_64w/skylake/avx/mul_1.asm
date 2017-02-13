;  AMD64 mpn_mul_1
;  Copyright 2016 Jens Nurmann and Alexander Kruppa
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

;	(rdi,rdx) = rcx*(rsi,rdx)
;	rax = high word of product

;  mp_limb_t  mpn_mul_1(mp_ptr, mp_ptr, mp_size_t, mp_limb_t)
;  mp_limb_t mpn_mul_1c(mp_ptr, mp_ptr, mp_size_t, mp_limb_t, mp_limb_t)
;  rax                     rdi     rsi        rdx        rcx         r8
;  rax                     rcx     rdx         r8         r9   [rsp+40]


%include 'yasm_mac.inc'

    BITS    64

; the following register allocation scheme is valid for Linux

    %define RP      RDI
    %define S1P     RSI
    %define Size    RCX
    %define S2      RDX

    %define MulLo0  R8
    %define MulHi0  R9
    %define MulLo1  R10
    %define MulHi1  R11
    %define MulLo2  R12
    %define MulHi2  R13
    %define MulLo3  R14
    %define MulHi3  RBX

%define reg_save_list rsi, rdi, rbx, r12, r13, r14

    align   32
FRAME_PROC mpn_mul_1, 0, reg_save_list
    mov     rdi, rcx
    mov     rsi, rdx
    mov     rcx, r8
    mov     rdx, r9
    mov     r8, [rsp + stack_use + 40]

    xor     MulHi3, MulHi3

    mov     RAX, Size           ; may be increased by 1 at the end
    sub     Size, 4
    jc      .Post               ; separate handling of remaining max. 3 limb =>

    ; prepare a quadlimb for main-loop entry
    mulx    MulHi0, MulLo0, [S1P]
    mulx    MulHi1, MulLo1, [S1P+8]
    mulx    MulHi2, MulLo2, [S1P+16]
    mulx    MulHi3, MulLo3, [S1P+24]
    add     S1P, 32
    add     MulLo1, MulHi0
    adc     MulLo2, MulHi1
    adc     MulLo3, MulHi2
    adc     MulHi3, 0

    jmp     .Check              ; enter main loop =>

    ; main loop (unloaded operands)
    ; - 1.25      cycles per limb in L1D$
    ; - 1.25      cycles per limb in L2D$
    ; - 1.60-1.72 cycles per limb in L3D$
    align   32
  .Loop:

    mov     [RP], MulLo0
    mov     [RP+8], MulLo1
    mov     [RP+16], MulLo2
    mov     [RP+24], MulLo3
    mulx    MulHi0, MulLo0, [S1P]
    mulx    MulHi1, MulLo1, [S1P+8]
    mulx    MulHi2, MulLo2, [S1P+16]
    add     MulLo0, MulHi3
    mov     [RP+32], MulLo0
    adc     MulLo1, MulHi0
    mov     [RP+40], MulLo1
    adc     MulLo2, MulHi1
    mov     [RP+48], MulLo2
    mulx    MulHi3, MulLo3, [S1P+24]
    mulx    MulHi0, MulLo0, [S1P+32]
    mulx    MulHi1, MulLo1, [S1P+40]
    adc     MulLo3, MulHi2	; no carry-out here
    adc     MulLo0, MulHi3
    adc     MulLo1, MulHi0
    mulx    MulHi2, MulLo2, [S1P+48]
    adc     MulLo2, MulHi1
    mov     [RP+56], MulLo3
    mulx    MulHi3, MulLo3, [S1P+56]
    adc     MulLo3, MulHi2
    adc     MulHi3, 0

    add     S1P, 64
    add     RP, 64

  .Check:

    sub     Size, 8
    jnc     .Loop

    ; core loop roll-out 8 can generate dangling quad-limb
    test    Size, 4
    je      .Store              ; no dangling quad-limb =>

    mov     [RP], MulLo0
    mulx    MulHi0, MulLo0, [S1P]
    mov     [RP+8], MulLo1
    mulx    MulHi1, MulLo1, [S1P+8]
    mov     [RP+16], MulLo2
    mulx    MulHi2, MulLo2, [S1P+16]
    add     MulLo0, MulHi3
    mov     [RP+24], MulLo3
    mulx    MulHi3, MulLo3, [S1P+24]
    adc     MulLo1, MulHi0
    adc     MulLo2, MulHi1
    adc     MulLo3, MulHi2
    adc     MulHi3, 0

    add     S1P, 32
    add     RP, 32

    ; store remaining quad-limb from main loop
  .Store:
    mov     [RP], MulLo0
    mov     [RP+8], MulLo1
    mov     [RP+16], MulLo2
    mov     [RP+24], MulLo3
    add     RP, 32

    ; handle final 0-3 single limb of S1P
  .Post:

    and     Size, 3
    je      .Post0

    cmp     Size, 2
    ja      .Post3
    je      .Post2

  .Post1:

    mulx    MulHi0, MulLo0, [S1P]
    add     MulLo0, MulHi3
    adc     MulHi0, 0
    mov     [RP], MulLo0
    mov     rax, MulHi0
    jmp     .Exit

  .Post2:

    mulx    MulHi0, MulLo0, [S1P]
    mulx    MulHi1, MulLo1, [S1P+8]
    add     MulLo0, MulHi3
    adc     MulLo1, MulHi0
    adc     MulHi1, 0
    mov     [RP], MulLo0
    mov     [RP+8], MulLo1
    mov     rax, MulHi1
    jmp     .Exit

  .Post3:

    mulx    MulHi0, MulLo0, [S1P]
    mulx    MulHi1, MulLo1, [S1P+8]
    mulx    MulHi2, MulLo2, [S1P+16]
    add     MulLo0, MulHi3
    adc     MulLo1, MulHi0
    adc     MulLo2, MulHi1
    adc     MulHi2, 0
    mov     [RP], MulLo0
    mov     [RP+8], MulLo1
    mov     [RP+16], MulLo2
    mov     rax, MulHi2
    jmp     .Exit

  .Post0:

    mov     rax, MulHi3

  .Exit:
    END_PROC reg_save_list
