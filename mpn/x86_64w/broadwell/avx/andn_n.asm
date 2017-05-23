;  AVX mpn_andn_n
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

;   (rdi,rcx) = not(rsi,rcx) and (rdx,rcx)

; There is no initial pointer alignment lead in code below. The argument
; why not is based on some statistical reasoning and measurement points.
; All statements below strictly refer to the Intel i6xxx (Skylake) 
; microarchitecture.

; The function is intended to be used with arbitrary pointer alignment on
; entry. That is there are 8 possible cases to consider:

; - A: 1 x all pointers mis-aligned (mod 32 byte)
; - B: 3 x one pointer aligned (mod 32 byte))
; - C: 3 x two pointers aligned (mod 32 byte)
; - D: 1 x all pointers aligned (mod 32 byte)

; All sub cases under B show equivalent performance, as do all sub cases of
; C. B is 7% faster than A, C is 11% faster than A and D is 39% faster than A.

; To do a proper alignment would require a complex decision tree to allways 
; advance the alignment situation in the best possible manner - e.g. pointer
; 1 is off by 8 while pointer 2 & 3 are off by 16. To do the alignment
; requires some arith and at least one branch in the function proloque - a
; reasonable impact for small sized operands. And all this for a small gain
; (around 6% all summed up) in the average case.

; In a specific application scenario this might be the wrong choice.

; The execution speed of VMOVDQU is equivalent to VMOVDQA in case of aligned
; pointers. This may be different for earlier generations of Intel core 
; architectures like Broadwell, Haswell, ...

; cycles per limb with all operands aligned and in:

;                   LD1$      LD2$
;   Haswell         ???       ???
;   Broadwell       ???       ???
;   Skylake         0.29-0.31 0.39-0.40

%include 'yasm_mac.inc'

; definition according to Linux 64 bit ABI

%define ResP    RCX
%define Src1P   RDX
%define Src2P    R8
%define Size     R9
%define SizeD   R9D
%define Count   RAX
%define CountD  EAX
%define Limb0   R10
%define Limb0D R10D
%define QLimb0 YMM0

    align   32
    BITS    64

LEAF_PROC   mpn_andn_n

    mov     CountD, 3
    mov     Limb0, Size
    sub     Count, Size
    jnc     .PostGPR            ; dispatch size 0-3 immediately

    mov     SizeD, 3
    shr     Limb0, 2
    or      Count, -4
    sub     Size, Limb0
    jnc     .PostAVX            ; dispatch size 4, 8 & 12 immediately

    mov     Limb0D, 128

  .Loop:

    vmovdqu QLimb0, [Src1P]
    vpandn  QLimb0, QLimb0, [Src2P]
    vmovdqu [ResP], QLimb0
    vmovdqu QLimb0, [Src1P+32]
    vpandn  QLimb0, QLimb0, [Src2P+32]
    vmovdqu [ResP+32], QLimb0
    vmovdqu QLimb0, [Src1P+64]
    vpandn  QLimb0, QLimb0, [Src2P+64]
    vmovdqu [ResP+64], QLimb0
    vmovdqu QLimb0, [Src1P+96]
    vpandn  QLimb0, QLimb0, [Src2P+96]
    vmovdqu [ResP+96], QLimb0

    lea     Src1P, [Src1P+Limb0]
    lea     Src2P, [Src2P+Limb0]
    lea     ResP, [ResP+Limb0]

    add     Size, 4
    jnc     .Loop

  .PostAVX:

    mov     Limb0D, 0           ; to allow pointer correction on exit
    cmp     Size, 2             ; fastest way to dispatch values 0-3
    ja      .PostAVX0
    je      .PostAVX1
    jp      .PostAVX2

  .PostAVX3:

    add     Limb0, 32
    vmovdqu QLimb0, [Src1P+64]
    vpandn  QLimb0, QLimb0, [Src2P+64]
    vmovdqu [ResP+64], QLimb0

  .PostAVX2:

    add     Limb0, 32
    vmovdqu QLimb0, [Src1P+32]
    vpandn  QLimb0, QLimb0, [Src2P+32]
    vmovdqu [ResP+32], QLimb0

  .PostAVX1:

    add     Limb0, 32
    vmovdqu QLimb0, [Src1P]
    vpandn  QLimb0, QLimb0, [Src2P]
    vmovdqu [ResP], QLimb0

  .PostAVX0:

    add     Src1P, Limb0
    add     Src2P, Limb0
    add     ResP, Limb0
    add     Count, 4

  .PostGPR:

    cmp     Count, 2            ; fastest way to dispatch values 0-3
    ja      .Exit
    je      .PostGPR1
    jp      .PostGPR2

  .PostGPR3:

    mov     Limb0, [Src1P+16]
    andn    Limb0, Limb0, [Src2P+16]
    mov     [ResP+16], Limb0

  .PostGPR2:

    mov     Limb0, [Src1P+8]
    andn    Limb0, Limb0, [Src2P+8]
    mov     [ResP+8], Limb0

  .PostGPR1:

    mov     Limb0, [Src1P]
    andn    Limb0, Limb0, [Src2P]
    mov     [ResP], Limb0

  .Exit:

    ret
