;  AMD64 mpn_subadd_n
;
;  Copyright 2017 Jens Nurmann
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

;   (rdi,r8) = (rsi,r8)-(rdx,r8)-(rcx,r8)
;   rax = summed borrow in range [ 0..2 ]

; the main loop has been enhanced with the MPIR SuperOptimizer
; the gain was roughly 4% execution speed for operands in LD1$
;
;  mp_limb_t mpn_subadd_n(mp_ptr, mp_ptr, mp_ptr, mp_ptr, mp_size_t)
;  rax                       rdi     rsi     rdx     rcx         r8
;  rax                       rcx     rdx      r8      r9   [rsp+40]

; cycles per limb with all operands in:

;                   LD1$      LD2$
;   Haswell         ???       ???
;   Broadwell       ???       ???
;   Skylake         1.6-1.7   1.7-1.85

%include 'yasm_mac.inc'

%define reg_save_list rsi, rdi, r12
; definition according to Linux 64 bit ABI

%define ResP    rdi
%define Src1P   rsi
%define Src2P   rdx
%define Src3P   rcx
%define Size    r8

%define Spills  eax
%define Carry   al
%define Borrow  ah

%define Limb0   r9
%define Limb1   r10
%define Limb2   r11
%define Limb3   r12

    align   32
    BITS    64

    FRAME_PROC mpn_subadd_n, 0, reg_save_list
	mov		rdi, rcx
	mov		rsi, rdx
	mov		rdx, r8
 	mov		rcx, r9
	mov		r8, [rsp+stack_use+40]

    sub     Src3P, 32
    sub     ResP, 32

    xor     Spills, Spills

    jmp     .Check

    align   16
  .Loop:

    ; do not delete!
    ; this seemingly unreasoned AVX instruction optimizes the allocation of
    ; read/write operations to ports 2, 3 & 7 (write allways ending up
    ; on port 7) which allows a sustained 2r1w execution per cycle
    vpor    ymm0, ymm0, ymm0

    shr     Carry, 1
    mov     Limb0, [Src1P]
    mov     Limb1, [Src1P+8]
    mov     Limb2, [Src1P+16]
    mov     Limb3, [Src1P+24]
    lea     Src3P, [Src3P+32]
    lea     ResP, [ResP+32]
    sbb     Limb0, [Src2P]
    sbb     Limb1, [Src2P+8]
    sbb     Limb2, [Src2P+16]
    sbb     Limb3, [Src2P+24]
    setc    Carry

    lea     Src2P, [Src2P+32]
    shr     Borrow, 1
    sbb     Limb0, [Src3P]
    sbb     Limb1, [Src3P+8]
    lea     Src1P, [Src1P+32]
    mov     [ResP], Limb0
    sbb     Limb2, [Src3P+16]
    mov     [ResP+8], Limb1
    mov     [ResP+16], Limb2
    sbb     Limb3, [Src3P+24]
    setc    Borrow
    mov     [ResP+24], Limb3

    ; label @ $a (mod $10) seems ok from benchmark figures
  .Check:

    sub     Size, 4
    jnc     .Loop

  .Post:

    add     Src3P, 32
    add     ResP, 32

    add     Size, 4
    je      .Exit

    shr     Carry, 1
    mov     Limb0, [Src1P]
    sbb     Limb0, [Src2P]
    setc    Carry
    shr     Borrow, 1
    sbb     Limb0, [Src3P]
    setc    Borrow
    mov     [ResP], Limb0
    dec     Size
    je      .Exit

    shr     Carry, 1
    mov     Limb0, [Src1P+8]
    sbb     Limb0, [Src2P+8]
    setc    Carry
    shr     Borrow, 1
    sbb     Limb0, [Src3P+8]
    setc    Borrow
    mov     [ResP+8], Limb0
    dec     Size
    je      .Exit

    shr     Carry, 1
    mov     Limb0, [Src1P+16]
    sbb     Limb0, [Src2P+16]
    setc    Carry
    shr     Borrow, 1
    sbb     Limb0, [Src3P+16]
    mov     [ResP+16], Limb0
    setc    Borrow

    ; label @ $2 (mod $10) is ok
.Exit:

    add     Carry, Borrow
    movsx   rax, Carry
	END_PROC reg_save_list

