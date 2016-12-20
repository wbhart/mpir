
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

; mp_limb_t mpn_lshift1(mp_ptr Op2, mp_srcptr Op1, mp_size_t Size1 )
; Linux     RAX         RDI         RSI            RDX
; Win7      RAX         RCX         RDX            R8
;
; Description:
; The function shifts Op1 left by one bit, stores the result in Op2 (non-
; destructive shl) and hands back the shifted-out most significant bit of Op1.
; The function operates decreasing in memory supporting in-place operation.
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
; - implemented, tested and benched on 21.02.2016 by jn
; - includes cache prefetching


%include 'yasm_mac.inc'

BITS 64

%ifdef USE_WIN64

    %define Op2         RCX
    %define Op1         RDX
    %define Size1       R8
    %define Limb1       R9
    %define Limb2       R10
    %define Offs        -512    ; used direct def. to stay in Win scratch regs

    %define ShlDL0      XMM2    ; ATTN: this must match ShlQL0 definition
    %define ShrDL0      XMM3    ; ATTN: this must match ShrQL0 definition

    %define QLimb0      YMM0
    %define QLimb1      YMM1
    %define ShlQL0      YMM2
    %define ShrQL0      YMM3
    %define ShlQL1      YMM4
    %define ShrQL1      YMM5

%else

    %define Op2         RDI
    %define Op1         RSI
    %define Size1       RDX
    %define Limb1       R8
    %define Limb2       R9
    %define Offs        -512    ; used direct def. to stay in Win scratch regs

    %define ShlDL0      XMM2    ; ATTN: this must match ShlQL0 definition
    %define ShrDL0      XMM3    ; ATTN: this must match ShrQL0 definition

    %define QLimb0      YMM0
    %define QLimb1      YMM1
    %define ShlQL0      YMM2
    %define ShrQL0      YMM3
    %define ShlQL1      YMM4
    %define ShrQL1      YMM5

%endif

    align   32

GLOBAL_FUNC mpn_lshift1

    xor     EAX, EAX
    sub      Size1, 1
    jc      .Exit               ;ajs:notshortform ; Size1=0 =>

    lea     Op1, [Op1+8*Size1]
    lea     Op2, [Op2+8*Size1]

    mov     Limb1, [Op1]
    shld    RAX, Limb1, 1

    or      Size1, Size1
    je      .lShl1EquPost       ;ajs:notshortform ; Size1=1 =>

    cmp     Size1, 8
    jc      .lShl1EquFour       ;ajs:notshortform ; AVX inefficient =>

    ; first align Op2 to 32 bytes
    test    Op2, 8
    jne     .lShl1EquA16

    mov     Limb2, [Op1-8]
    shld    Limb1, Limb2, 1
    mov     [Op2], Limb1
    mov     Limb1, Limb2

    sub     Op1, 8
    sub     Op2, 8
    sub     Size1, 1

  .lShl1EquA16:

    test    Op2, 16
    jne     .lShl1EquAVX

    mov     Limb2, [Op1-8]
    shld    Limb1, Limb2, 1
    mov     [Op2], Limb1
    mov     Limb1, [Op1-16]
    shld    Limb2, Limb1, 1
    mov     [Op2-8], Limb2

    sub     Op1, 16
    sub     Op2, 16
    sub     Size1, 2

  .lShl1EquAVX:

    ; pre-fetch first quad-limb
    vmovdqu QLimb0, [Op1-24]
    vpsrlq  ShrQL0, QLimb0, 63
    vpermq  ShrQL0, ShrQL0, 147		; 0b10010011

    sub     Op1, 32
    sub     Size1, 4
    jmp     .lShl1EquAVXCheck

    ; main loop requires on entry:
    ; - 0.60      cycles per limb in LD1$
    ; - 0.60-0.75 cycles per limb in LD2$
    ; - 0.75-1.00 cycles per limb in LD3$
    align   16
  .lShl1EquAVXLoop:

  %ifdef USE_PREFETCH
    prefetchnta [Op1+Offs]
  %endif

    vmovdqu   QLimb1, [Op1-24]
    vpsllq    ShlQL0, QLimb0, 1
    vmovdqu   QLimb0, [Op1-56]
    vpsrlq    ShrQL1, QLimb1, 63
    vpermq    ShrQL1, ShrQL1, 147	; 0b10010011
    vpblendd  ShrQL0, ShrQL0, ShrQL1, 3	; 0b00000011
    vpor      ShlQL0, ShlQL0, ShrQL0
    vpsllq    ShlQL1, QLimb1, 1
    vpsrlq    ShrQL0, QLimb0, 63
    vpermq    ShrQL0, ShrQL0, 147	; 0b10010011
    vpblendd  ShrQL1, ShrQL1, ShrQL0, 3	; 0b00000011
    vmovdqa   [Op2-24], ShlQL0
    vpor      ShlQL1, ShlQL1, ShrQL1
    vmovdqa   [Op2-56], ShlQL1

    sub     Op1, 64
    sub     Op2, 64

  .lShl1EquAVXCheck:

    sub     Size1, 8
    jnc     .lShl1EquAVXLoop

    mov     Limb2, [Op1]
    mov     Limb1, Limb2
    shr     Limb2, 63
%if 1
    vmovq ShlDL0, Limb2
    vpblendd ShrQL0, ShrQL0, ShlQL0, 3
%else
    ; I am mixing in a single SSE4.1 instruction into otherwise pure AVX2
    ; this is generating stalls on Haswell & Broadwell architecture (Agner Fog)
    ; but it is only executed once and there is no AVX2 based alternative

    ; Insert value of Limb2 into the 0-th qword of ShrDL0
    pinsrq  ShrDL0, Limb2, 0        ; SSE4.1
%endif
    vpsllq  ShlQL0, QLimb0, 1
    vpor    ShlQL0, ShlQL0, ShrQL0
    vmovdqa [Op2-24], ShlQL0

    sub     Op2, 32
    add     Size1, 8

    ; shift remaining max. 7 limbs with SHLD mnemonic
  .lShl1EquFour:

    sub     Op1, 8
    test    Size1, 4
    je      .lShl1EquTwo

    mov     Limb2, [Op1]
    shld    Limb1, Limb2, 1
    mov     [Op2], Limb1
    mov     Limb1, [Op1-8]
    shld    Limb2, Limb1, 1
    mov     [Op2-8], Limb2
    mov     Limb2, [Op1-16]
    shld    Limb1, Limb2, 1
    mov     [Op2-16], Limb1
    mov     Limb1, [Op1-24]
    shld    Limb2, Limb1, 1
    mov     [Op2-24], Limb2

    sub     Op1, 32
    sub     Op2, 32

  .lShl1EquTwo:

    test    Size1, 2
    je      .lShl1EquOne

    mov     Limb2, [Op1]
    shld    Limb1, Limb2, 1
    mov     [Op2], Limb1
    mov     Limb1, [Op1-8]
    shld    Limb2, Limb1, 1
    mov     [Op2-8], Limb2

    sub     Op1, 16
    sub     Op2, 16

  .lShl1EquOne:

    test    Size1, 1
    je      .lShl1EquPost

    mov     Limb2, [Op1]
    shld    Limb1, Limb2, 1
    mov     [Op2], Limb1
    mov     Limb1, Limb2

    sub     Op2, 8

  .lShl1EquPost:

    shl     Limb1, 1
    mov     [Op2], Limb1

  .Exit:

    vzeroupper
    ret
.end:
