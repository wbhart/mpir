
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

; mp_limb_t mpn_rshift1(mp_ptr Op2, mp_srcptr Op1, mp_size_t Size1 )
; Linux     RAX         RDI         RSI            RDX
; Win7      RAX         RCX         RDX            R8
;
; Description:
; The function shifts Op1 right by one bit, stores the result in Op2 (non-
; destructive shr) and hands back the shifted-out least significant bit of Op1.
; The function operates increasing in memory supporting in place shifts.
;
; Caveats:
; - the AVX version uses mnemonics only available on Haswell, Broadwell and
;   Skylake cores
; - the behaviour of cache prefetching in combination with AVX shifting seems
;   somewhat erratic
;    - slight (a few clock cycles) degradation for 1/2 LD1$ sizes
;    - slight (a few percent) improvement for full LD1$ sizes
;    - substantial (>10%) improvement for 1/2 LD2$ sizes
;    - slight (a few percent) improvement for full LD2$ sizes
;    - slight (a few percent) degradation for 1/2 LD3$ sizes
;    - substantial (around 10%) degradation for full LD3$ sizes
;
; Comments:
; - AVX based version implemented, tested & benched on 21.02.2016 by jn
; - includes cache prefetching

%define USE_WIN64

%include 'yasm_mac.inc'

BITS 64

%ifdef USE_WIN64

    %define Op2         RCX
    %define Op1         RDX
    %define Size1       R8
    %define Limb1       R9
    %define Limb2       R10
    %define Offs        512     ; used direct def. to stay in Win scratch regs

    %define ShrDL0      XMM2    ; Attn: this must match ShrQL0 definition
    %define ShlDL0      XMM3    ; Attn: this must match ShlQL0 definition

    %define QLimb0      YMM0
    %define QLimb1      YMM1
    %define ShrQL0      YMM2
    %define ShlQL0      YMM3
    %define ShrQL1      YMM4
    %define ShlQL1      YMM5

%else

    %define Op2         RDI
    %define Op1         RSI
    %define Size1       RDX
    %define Limb1       R8
    %define Limb2       R9
    %define Offs        512     ; used direct def. to stay in Win scratch regs

    %define ShrDL0      XMM2    ; Attn: this must match ShrQL0 definition
    %define ShlDL0      XMM3    ; Attn: this must match ShlQL0 definition

    %define QLimb0      YMM0
    %define QLimb1      YMM1
    %define ShrQL0      YMM2
    %define ShlQL0      YMM3
    %define ShrQL1      YMM4
    %define ShlQL1      YMM5

%endif

    align   32

LEAF_PROC mpn_rshift1

    xor     EAX, EAX
    or      Size1, Size1
    je      .Exit

    mov     RAX, [Op1]
    mov     Limb1, RAX
    shl     RAX, 63

    sub     Size1, 1
    je      .lShr1EquPost       ; Size1=1 =>

    cmp     Size1, 8
    jc      .lShr1EquFour       ; AVX inefficient =>

    ; first align Op2 to 32 bytes
    test    Op2, 8
    je      .lShr1EquAlign16

    mov     Limb2, [Op1+8]
    shrd    Limb1, Limb2, 1
    mov     [Op2], Limb1
    mov     Limb1, Limb2

    add     Op1, 8
    add     Op2, 8
    sub     Size1, 1

  .lShr1EquAlign16:

    test    Op2, 16
    je      .lShr1EquAVX

    mov     Limb2, [Op1+8]
    shrd    Limb1, Limb2, 1
    mov     [Op2], Limb1
    mov     Limb1, [Op1+16]
    shrd    Limb2, Limb1, 1
    mov     [Op2+8], Limb2

    add     Op1, 16
    add     Op2, 16
    sub     Size1, 2

  .lShr1EquAVX:

    ; pre-fetch first quad-limb
    vmovdqu QLimb0, [Op1]
    vpsllq  ShlQL0, QLimb0, 63

    add     Op1, 32
    sub     Size1, 4
    jmp     .lShr1EquAVXCheck

    ; main loop (prefetching enabled, unloaded data cache)
    ; - 0.60      cycles per limb in LD1$
    ; - 0.60-0.75 cycles per limb in LD2$
    ; - 0.75-1.00 cycles per limb in LD3$
    align   16
  .lShr1EquAVXLoop:

  %ifdef USE_PREFETCH
    prefetchnta [Op1+Offs]
  %endif

    vmovdqu   QLimb1, [Op1]
    vpsrlq    ShrQL0, QLimb0, 1
    vmovdqu   QLimb0, [Op1+32]
    vpsllq    ShlQL1, QLimb1, 63
    vpblendd  ShlQL0, ShlQL0, ShlQL1, 00000011b
    vpermq    ShlQL0, ShlQL0, 00111001b
    vpor      ShrQL0, ShrQL0, ShlQL0
    vpsrlq    ShrQL1, QLimb1, 1
    vpsllq    ShlQL0, QLimb0, 63
    vpblendd  ShlQL1, ShlQL1, ShlQL0, 00000011b
    vpermq    ShlQL1, ShlQL1, 00111001b
    vmovdqa   [Op2], ShrQL0
    vpor      ShrQL1, ShrQL1, ShlQL1
    vmovdqa   [Op2+32], ShrQL1

    add     Op1, 64
    add     Op2, 64

  .lShr1EquAVXCheck:

    sub     Size1, 8
    jnc     .lShr1EquAVXLoop

    mov     Limb2, [Op1]
    mov     Limb1, Limb2
    shl     Limb2, 63
%if 1
    vmovq ShrDL0, Limb2
    vpblendd ShlQL0, ShlQL0, ShrQL0, 3
%else
    ; I am mixing in a single SSE4.1 instruction into otherwise pure AVX2
    ; this is generating stalls on Haswell & Broadwell architecture (Agner Fog)
    ; but it is only executed once and there is no AVX2 based alternative
    pinsrq  ShlDL0, Limb2, 0            ; SSE4.1
%endif
    vpsrlq  ShrQL0, QLimb0, 1
    vpermq  ShlQL0, ShlQL0, 00111001b
    vpor    ShrQL0, ShrQL0, ShlQL0
    vmovdqa [Op2], ShrQL0

    add     Op2, 32
    add     Size1, 8

    ; shift remaining max. 7 limbs with SHRD mnemonic
  .lShr1EquFour:

    add     Op1, 8
    test    Size1, 4
    je      .lShr1EquTwo

    mov     Limb2, [Op1]
    shrd    Limb1, Limb2, 1
    mov     [Op2], Limb1
    mov     Limb1, [Op1+8]
    shrd    Limb2, Limb1, 1
    mov     [Op2+8], Limb2
    mov     Limb2, [Op1+16]
    shrd    Limb1, Limb2, 1
    mov     [Op2+16], Limb1
    mov     Limb1, [Op1+24]
    shrd    Limb2, Limb1, 1
    mov     [Op2+24], Limb2

    add     Op1, 32
    add     Op2, 32

  .lShr1EquTwo:

    test    Size1, 2
    je      .lShr1EquOne

    mov     Limb2, [Op1]
    shrd    Limb1, Limb2, 1
    mov     [Op2], Limb1
    mov     Limb1, [Op1+8]
    shrd    Limb2, Limb1, 1
    mov     [Op2+8], Limb2

    add     Op1, 16
    add     Op2, 16

  .lShr1EquOne:

    test    Size1, 1
    je      .lShr1EquPost

    mov     Limb2, [Op1]
    shrd    Limb1, Limb2, 1
    mov     [Op2], Limb1
    mov     Limb1, Limb2

    add     Op2, 8

  .lShr1EquPost:

    shr     Limb1, 1
    mov     [Op2], Limb1

  .Exit:

    ret
.end:
