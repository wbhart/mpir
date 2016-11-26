
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


; mpn_copyd(mp_ptr Op2, mp_srcptr Op1, mp_size_t Size1)
; Linux     RDI         RSI            RDX
; Win7      RCX         RDX            R8
;
; Description:
; The function copies a given number of limb from source to destination (while
; moving high to low in memory) and hands back the size (in limb) of the
; destination.
;
; Result:
; - Op2[ 0..size-1 ] = Op1[ 0..size-1 ]
; - number of copied limb: range [ 0..max tCounter ]
;
; Caveats:
; - if size 0 is given the content of the destination will remain untouched!
; - if Op1=Op2 no copy is done!
;
; Comments:
; - AVX-based version implemented, tested & benched on 05.01.2016 by jn
; - did some experiments with AVX based version with following results
;   - AVX can be faster in L1$-L3$ if destination is aligned on 32 byte
;   - AVX is generally faster on small sized operands (<=100 limb) due too
;     start-up overhead of "rep movsq" - however this could also be achieved by
;     simple copy loop
;   - startup overhead of "rep movsq" with negative direction is 200 cycles!!!
;   - negative direction is unfavourable compared to positive "rep movsq" and
;     to AVX.

%define USE_WIN64

%include 'yasm_mac.inc'

BITS 64

%ifdef USE_WIN64
    %define Op2     RCX
    %define Op1     RDX
    %define Size1   R8
    %define Limb    R9
    %define Offs    R10
%else
    %define Op2     RDI
    %define Op1     RSI
    %define Size1   RDX
    %define Limb    RCX
    %define Offs    R10
%endif

%define DLimb0  XMM0
%define QLimb0  YMM0
%define QLimb1  YMM1
%define QLimb2  YMM2
%define QLimb3  YMM3

    align   32

LEAF_PROC   mpn_copyd
    mov     RAX, Size1
    cmp     Op1, Op2
    je      .Exit               ; no copy required =>

    or      RAX, RAX
    je      .Exit               ; Size=0 =>

    lea     Op1, [Op1+8*Size1-8]
    lea     Op2, [Op2+8*Size1-8]

    ; align the destination (Op2) to 32 byte
    test    Op2, 8
    jne     .lCpyDecA32

    mov     Limb, [Op1]
    mov     [Op2], Limb
    dec     Size1
    je      .Exit

    sub     Op1, 8
    sub     Op2, 8

  .lCpyDecA32:

    test    Op2, 16
    jnz     .lCpyDecAVX

    mov     Limb, [Op1]
    mov     [Op2], Limb
    dec     Size1
    je      .Exit

    mov     Limb, [Op1-8]
    mov     [Op2-8], Limb
    dec     Size1
    je      .Exit

    sub     Op1, 16
    sub     Op2, 16

  .lCpyDecAVX:

    mov     Offs, 128
    jmp     .lCpyDecAVXCheck

    ; main loop (prefetching disabled; unloaded cache)
    ; - 0.30      cycles / limb in L1$
    ; - 0.60      cycles / limb in L2$
    ; - 0.70-0.90 cycles / limb in L3$
    align   16
  .lCpyDecAVXLoop:

    vmovdqu QLimb0, [Op1-24]
    vmovdqu QLimb1, [Op1-56]
    vmovdqu QLimb2, [Op1-88]
    vmovdqu QLimb3, [Op1-120]
    vmovdqa [Op2-24], QLimb0
    vmovdqa [Op2-56], QLimb1
    vmovdqa [Op2-88], QLimb2
    vmovdqa [Op2-120], QLimb3

    sub     Op1, Offs
    sub     Op2, Offs

  .lCpyDecAVXCheck:

    sub     Size1, 16
    jnc     .lCpyDecAVXLoop

    add     Size1, 16
    je      .Exit               ; AVX copied operand fully =>

    ; copy remaining max. 15 limb
    test    Size1, 8
    je      .lCpyDecFour

    vmovdqu QLimb0, [Op1-24]
    vmovdqu QLimb1, [Op1-56]
    vmovdqa [Op2-24], QLimb0
    vmovdqa [Op2-56], QLimb1

    sub     Op1, 64
    sub     Op2, 64

  .lCpyDecFour:

    test    Size1, 4
    je      .lCpyDecTwo

    vmovdqu QLimb0, [Op1-24]
    vmovdqa [Op2-24], QLimb0

    sub     Op1, 32
    sub     Op2, 32

  .lCpyDecTwo:

    test    Size1, 2
    je      .lCpyDecOne

%if 1
    ; Avoid SSE2 instruction due to stall on Haswell
    mov     Limb, [Op1]
    mov     [Op2], Limb
    mov     Limb, [Op1-8]
    mov     [Op2-8], Limb
%else
    movdqu  DLimb0, [Op1-8]
    movdqa  [Op2-8], DLimb0
%endif

    sub     Op1, 16
    sub     Op2, 16

  .lCpyDecOne:

    test    Size1, 1
    je      .Exit

    mov     Limb, [Op1]
    mov     [Op2], Limb

  .Exit:

    vzeroupper
    ret
.end:
