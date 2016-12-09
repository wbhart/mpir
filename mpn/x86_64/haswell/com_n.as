
;  AMD64 mpn_com_n
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

BITS 64

%include 'yasm_mac.inc'

%ifdef USE_WIN64
    %define Op2     RCX
    %define Op1     RDX
    %define Size1   R8
    %define Limb    R9
    %define Offs    R10
    %define FFFF    RSI
%else
    %define Op2     RDI
    %define Op1     RSI
    %define Size1   RDX
    %define Limb    RCX
    %define Offs    R10
    %define FFFF    R8
%endif

%define DLimb0  XMM0
%define QLimb0  YMM0
%define QLimb1  YMM1
%define QLimb2  YMM2
%define QLimb3  YMM3
%define QFFFF   YMM4
%define DFFFF   XMM4

    align   32

GLOBAL_FUNC mpn_com_n
    mov     RAX, Size1
    or      RAX, RAX
    je      .Exit      ;ajs:notshortform
                       ; size=0 =>

    ; Set a GPR to 0xFF...FF
    mov     FFFF, -1

    ; align the destination (Op2) to 32 byte
    test    Op2, 8
    je      .A32

    mov     Limb, [Op1]
    xor     Limb, FFFF
    mov     [Op2], Limb
    dec     Size1
    je      .Exit      ;ajs:notshortform

    add     Op1, 8
    add     Op2, 8

  .A32:

    test    Op2, 16
    je      .AVX

    mov     Limb, [Op1]
    xor     Limb, FFFF
    mov     [Op2], Limb
    dec     Size1
    je      .Exit       ;ajs:notshortform

    mov     Limb, [Op1+8]
    xor     Limb, FFFF
    mov     [Op2+8], Limb
    dec     Size1
    je      .Exit       ;ajs:notshortform

    add     Op1, 16
    add     Op2, 16

  .AVX:

    ; Set an AVX2 reg to 0xFF...FF
    movq    DFFFF, FFFF
    vbroadcastsd QFFFF, DFFFF

    mov     Offs, 128
    jmp     .AVXCheck

    ; main loop (prefetching disabled; unloaded cache)
    ; - lCpyInc is slightly slower than lCpyDec through all cache levels?!
    ; - 0.30      cycles / limb in L1$
    ; - 0.60      cycles / limb in L2$
    ; - 0.70-0.90 cycles / limb in L3$
    align   16
  .AVXLoop:

    vmovdqu QLimb0, [Op1]
    vpxor   QLimb0, QLimb0, QFFFF
    vmovdqu QLimb1, [Op1+32]
    vpxor   QLimb1, QLimb1, QFFFF
    vmovdqu QLimb2, [Op1+64]
    vpxor   QLimb2, QLimb2, QFFFF
    vmovdqu QLimb3, [Op1+96]
    vpxor   QLimb3, QLimb3, QFFFF
    vmovdqa [Op2], QLimb0
    vmovdqa [Op2+32], QLimb1
    vmovdqa [Op2+64], QLimb2
    vmovdqa [Op2+96], QLimb3

    add     Op1, Offs
    add     Op2, Offs

  .AVXCheck:

    sub     Size1, 16
    jnc     .AVXLoop

    add     Size1, 16
    je      .Exit ;ajs:notshortform
                  ; AVX copied operand fully =>

    ; copy remaining max. 15 limb
    test    Size1, 8
    je      .Four

    vmovdqu QLimb0, [Op1]
    vpxor   QLimb0, QLimb0, QFFFF
    vmovdqu QLimb1, [Op1+32]
    vpxor   QLimb1, QLimb1, QFFFF
    vmovdqa [Op2], QLimb0
    vmovdqa [Op2+32], QLimb1

    add     Op1, 64
    add     Op2, 64

  .Four:

    test    Size1, 4
    je      .Two

    vmovdqu QLimb0, [Op1]
    vpxor   QLimb0, QLimb0, QFFFF
    vmovdqa [Op2], QLimb0

    add     Op1, 32
    add     Op2, 32

  .Two:

    test    Size1, 2
    je      .One

%if 1
    ; Avoid SSE2 instruction due to stall on Haswell
    mov     Limb, [Op1]
    xor     Limb, FFFF
    mov     [Op2], Limb
    mov     Limb, [Op1+8]
    xor     Limb, FFFF
    mov     [Op2+8], Limb
%else
    movdqu  DLimb0, [Op1]
    pxor    DLimb0, DLimb0, DFFFF
    movdqa  [Op2], DLimb0
%endif

    add     Op1, 16
    add     Op2, 16

  .One:

    test    Size1, 1
    je      .Exit

    mov     Limb, [Op1]
    xor     Limb, FFFF
    mov     [Op2], Limb

  .Exit:

    vzeroupper
    ret
.end:
