
;  Copyright 2016, 2017 Jens Nurmann

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

;  mp_limb_t  mpn_sub_n(mp_ptr, mp_srcptr, mp_srcptr, mp_size_t)
;  mp_limb_t mpn_sub_nc(mp_ptr, mp_srcptr, mp_srcptr, mp_size_t, mp_limb_t)
;  rax                     rdi        rsi        rdx        rcx         r8
;  rax                     rcx        rdx         r8         r9   [rsp+40]

%include 'yasm_mac.inc'

%define Op1     RDX
%define Op2     R8
%define Size    R9
%define Op3     RCX

%define BorrowD EAX
%define BorrowB AL

%define Limb0   R10
%define SizeB   R9B         ; check if this fits to code alignment!
%define Count   R11

    align   32
	LEAF_PROC mpn_sub_nc
    mov     BorrowD, [rsp+40]
    mov     Count, Size
    shr     Count, 3
    inc     Count
    vpor    YMM0, YMM0, YMM0    ; see comment in main loop below
    jmp     One

    align   32
	LEAF_PROC mpn_sub_n
    xor     BorrowD, BorrowD
    mov     Count, Size
    shr     Count, 3
    inc     Count
    vpor    YMM0, YMM0, YMM0    ; see comment in main loop below

    ; unrolling the loop from small to high gives better timings
    ; when considering all sizes 1-100 limb
  One:
    test    SizeB, 1
    je      .Two
    shr     BorrowB, 1
    mov     Limb0, [Op1]        ; I am using implicit code alignment through-
    sbb     Limb0, [Op2]        ; out the following to get all branch targets
    mov     [Op3], Limb0        ; on 16 byte alignments - check this if non-
    setc    BorrowB             ; Linux register allocation is used!

    add     Op1, 8
    add     Op2, 8
    add     Op3, 8

  .Two:

    test    SizeB, 2
    je      .Four

    shr     BorrowB, 1
    mov     Limb0, [dword Op1]
    sbb     Limb0, [Op2]
    mov     [Op3], Limb0
    mov     Limb0, [Op1+8]
    sbb     Limb0, [Op2+8]
    mov     [Op3+8], Limb0
    setc    BorrowB

    add     Op1, 16
    add     Op2, 16
    add     Op3, 16

  .Four:

    test    SizeB, 4
    bt      BorrowD, 0
    je      .Check

    mov     Limb0, [Op1]
    sbb     Limb0, [Op2]
    mov     [Op3], Limb0
    mov     Limb0, [Op1+8]
    sbb     Limb0, [Op2+8]
    mov     [Op3+8], Limb0
    mov     Limb0, [Op1+16]
    sbb     Limb0, [Op2+16]
    mov     [Op3+16], Limb0
    mov     Limb0, [Op1+24]
    sbb     Limb0, [Op2+24]
    mov     [Op3+24], Limb0

    lea     Op1, [Op1+32]
    lea     Op2, [Op2+32]
    lea     Op3, [Op3+32]
    jmp     .Check

    ; main loop:
    ; - 1.03-1.05 cycles per limb in L1D$
    ; - 1.13-1.15 cycles per limb in L2D$
    ; - 1.50-1.75 cycles per limb in L3D$
    align   16
  .Loop:

    mov     Limb0, [Op1]
    sbb     Limb0, [Op2]
    mov     [Op3], Limb0

    ; do not delete!
    ; this seemingly unreasoned AVX instruction optimizes the allocation of
    ; read/write operations to ports 2, 3 & 7 (write always ending up
    ; on port 7) which allows a sustained 2r1w execution per cycle
    vpor    YMM0, YMM0, YMM0

    mov     Limb0, [dword Op1+8]
    sbb     Limb0, [Op2+8]
    mov     [Op3+8], Limb0
    mov     Limb0, [Op1+16]
    sbb     Limb0, [Op2+16]
    mov     [Op3+16], Limb0
    mov     Limb0, [Op1+24]
    sbb     Limb0, [Op2+24]
    mov     [Op3+24], Limb0
    mov     Limb0, [Op1+32]
    sbb     Limb0, [Op2+32]
    mov     [Op3+32], Limb0
    mov     Limb0, [Op1+40]
    sbb     Limb0, [Op2+40]
    mov     [Op3+40], Limb0
    mov     Limb0, [Op1+48]
    sbb     Limb0, [Op2+48]
    mov     [Op3+48], Limb0
    mov     Limb0, [Op1+56]
    sbb     Limb0, [Op2+56]
    mov     [Op3+56], Limb0

    lea     Op1, [Op1+64]
    lea     Op2, [Op2+64]
    lea     Op3, [Op3+64]

  .Check:

    dec     Count
    jne     .Loop

  .Exit:

    setc    BorrowB             ; move total borrow to RAX
    ret
