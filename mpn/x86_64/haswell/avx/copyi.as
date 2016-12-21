
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

; mpn_copyi(mp_ptr Op2, mp_srcptr Op1, mp_size_t Size1)
; Linux     RDI         RSI            RDX
; Win7      RCX         RDX            R8
;
; Description:
; The function copies a given number of limb from source to destination (while
; moving low to high in memory) and hands back the size (in limb) of the
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
;   - AVX can be faster in L1$ (30%), L2$ (10%) if dest. is aligned on 32 byte
;   - AVX is generally faster on small sized operands (<=100 limb) due too
;     start-up overhead of "rep movsq" - however this could also be achieved by
;     simple copy loop
;   - the break-even between AVX and "rep movsq" is around 10,000 limb
; - the prologue & epilogue can still be optimized!

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

GLOBAL_FUNC mpn_copyi
    mov     RAX, Size1
    cmp     Op1, Op2
    je      .Exit               ; no copy required =>

    or      RAX, RAX
    je      .Exit               ; size=0 =>

    ; align the destination (Op2) to 32 byte
    test    Op2, 8
    je      .lCpyIncA32

    mov     Limb, [Op1]
    mov     [Op2], Limb
    dec     Size1
    je      .Exit

    add     Op1, 8
    add     Op2, 8

  .lCpyIncA32:

    test    Op2, 16
    je      .lCpyIncAVX

    mov     Limb, [Op1]
    mov     [Op2], Limb
    dec     Size1
    je      .Exit

    mov     Limb, [Op1+8]
    mov     [Op2+8], Limb
    dec     Size1
    je      .Exit

    add     Op1, 16
    add     Op2, 16

  .lCpyIncAVX:

    mov     Offs, 128
    jmp     .lCpyIncAVXCheck

    ; main loop (prefetching disabled; unloaded cache)
    ; - lCpyInc is slightly slower than lCpyDec through all cache levels?!
    ; - 0.30      cycles / limb in L1$
    ; - 0.60      cycles / limb in L2$
    ; - 0.70-0.90 cycles / limb in L3$
    align   16
  .lCpyIncAVXLoop:

    vmovdqu QLimb0, [Op1]
    vmovdqu QLimb1, [Op1+32]
    vmovdqu QLimb2, [Op1+64]
    vmovdqu QLimb3, [Op1+96]
    vmovdqa [Op2], QLimb0
    vmovdqa [Op2+32], QLimb1
    vmovdqa [Op2+64], QLimb2
    vmovdqa [Op2+96], QLimb3

    add     Op1, Offs
    add     Op2, Offs

  .lCpyIncAVXCheck:

    sub     Size1, 16
    jnc     .lCpyIncAVXLoop

    add     Size1, 16
    je      .Exit               ; AVX copied operand fully =>

    ; copy remaining max. 15 limb
    test    Size1, 8
    je      .lCpyIncFour

    vmovdqu QLimb0, [Op1]
    vmovdqu QLimb1, [Op1+32]
    vmovdqa [Op2], QLimb0
    vmovdqa [Op2+32], QLimb1

    add     Op1, 64
    add     Op2, 64

  .lCpyIncFour:

    test    Size1, 4
    je      .lCpyIncTwo

    vmovdqu QLimb0, [Op1]
    vmovdqa [Op2], QLimb0

    add     Op1, 32
    add     Op2, 32

  .lCpyIncTwo:

    test    Size1, 2
    je      .lCpyIncOne

%if 1
    ; Avoid SSE2 instruction due to stall on Haswell
    mov     Limb, [Op1]
    mov     [Op2], Limb
    mov     Limb, [Op1+8]
    mov     [Op2+8], Limb
%else
    movdqu  DLimb0, [Op1]
    movdqa  [Op2], DLimb0
%endif

    add     Op1, 16
    add     Op2, 16

  .lCpyIncOne:

    test    Size1, 1
    je      .Exit

    mov     Limb, [Op1]
    mov     [Op2], Limb

  .Exit:

    vzeroupper
    ret
.end:
