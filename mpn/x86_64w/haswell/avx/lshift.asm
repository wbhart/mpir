
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

; mp_limb_t mpn_lshift(mp_ptr Op2, mp_srcptr Op1, mp_size_t Size1, unsigned int Shift)
; Linux     RAX        RDI         RSI            RDX              RCX
; Win7      RAX        RCX         RDX            R8               R9
;
; Description:
; The function shifts Op1 left by n bit, stores the result in Op2 (non-
; destructive shl) and hands back the shifted-out most significant bits of Op1.
; The function operates decreasing in memory supporting in-place operation.
;
; Result:
; - Op2[ Size1-1..0 ] := ( Op1[ Size1-1..0 ]:ShlIn ) << 1
; - Op1[ 0 ] >> 63
;
; Caveats:
; - caller must ensure that Shift is in [ 1..63 ]!
; - currently Linux64 support only!
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
; - implemented, tested and benched on 31.03.2016 by jn
; - includes prefetching
; ============================================================================

%define USE_WIN64

%include 'yasm_mac.inc'

BITS 64

%ifdef USE_WIN64
    %define Op2         R11
    %define Op1         RDX
    %define Size1       R8
    %define Shift       RCX
    %define Limb1       R9
    %define Limb2       R10
  %ifdef USE_PREFETCH
    %define Offs        -512    ; No caller-saves regs left, use immediate
  %endif
    %define reg_save_list XMM, 6, 7
%else
    %define Op2         RDI
    %define Op1         RSI
    %define Size1       RDX
    %define Shift       RCX
    %define Limb1       R8
    %define Limb2       R9
  %ifdef USE_PREFETCH
    %define OFFS_REG 1
    %define Offs        R10
  %endif
%endif

%define ShlDL0      XMM2    ; Attn: this must match ShlQL0 definition
%define ShrDL0      XMM3    ; Attn: this must match ShrQL0 definition
%define ShlDLCnt    XMM6    ; Attn: this must match ShlQlCnt definition
%define ShrDLCnt    XMM7    ; Attn: this must match ShrQlCnt definition

%define QLimb0      YMM0
%define QLimb1      YMM1
%define ShlQL0      YMM2
%define ShrQL0      YMM3
%define ShlQL1      YMM4
%define ShrQL1      YMM5
%define ShlQLCnt    YMM6
%define ShrQLCnt    YMM7

    align   32
FRAME_PROC  mpn_lshift, 0, reg_save_list
%ifdef USE_WIN64
    mov     r11, rcx
	mov     rcx, r9
%endif
    xor     EAX, EAX
    sub     Size1, 1
    jc      .Exit               ; Size1=0 =>

    lea     Op1, [Op1+8*Size1]
    lea     Op2, [Op2+8*Size1]

    mov     Limb1, [Op1]
    shld    RAX, Limb1, CL

    or      Size1, Size1
    je      .lShlEquPost        ; Size1=1 =>

  %ifdef USE_PREFETCH
  %ifdef OFFS_REG
    mov     Offs, -512
  %endif
  %endif

    cmp     Size1, 8
    jc      .lShlEquFour        ; AVX inefficient =>

    ; first align Op2 to 32 bytes
    test    Op2, 8
    jne     .lShlEquA16

    mov     Limb2, [Op1-8]
    shld    Limb1, Limb2, CL
    mov     [Op2], Limb1
    mov     Limb1, Limb2

    sub     Op1, 8
    sub     Op2, 8
    sub     Size1, 1

  .lShlEquA16:

    test    Op2, 16
    jne     .lShlEquAVX

    mov     Limb2, [Op1-8]
    shld    Limb1, Limb2, CL
    mov     [Op2], Limb1
    mov     Limb1, [Op1-16]
    shld    Limb2, Limb1, CL
    mov     [Op2-8], Limb2

    sub     Op1, 16
    sub     Op2, 16
    sub     Size1, 2

  .lShlEquAVX:

    ; initialize AVX shift counter
    vmovq   ShlDLCnt, RCX
    neg     RCX
    and     RCX, 63             ; must do, as AVX shifts set result=0 if Shift>63!
    vmovq   ShrDLCnt, RCX
    neg     RCX
    and     RCX, 63             ; must do, as AVX shifts set result=0 if Shift>63!
    vpbroadcastq ShlQLCnt, ShlDLCnt
    vpbroadcastq ShrQLCnt, ShrDLCnt

    ; pre-fetch first quad-limb
    vmovdqu QLimb0, [Op1-24]
    vpsrlvq ShrQL0, QLimb0, ShrQLCnt
    vpermq  ShrQL0, ShrQL0, 10010011b

    sub     Op1, 32
    sub     Size1, 4
    jmp     .lShlEquAVXCheck

    ; main loop (prefetching enabled; unloaded cache)
    ; - 0.60      cycles per limb in LD1$
    ; - 0.60-0.70 cycles per limb in LD2$
    ; - 0.70-0.90 cycles per limb in LD3$
    align   16
  .lShlEquAVXLoop:

  %ifdef USE_PREFETCH
    prefetchnta [Op1+Offs]
  %endif

    vmovdqu   QLimb1, [Op1-24]
    vpsllvq   ShlQL0, QLimb0, ShlQLCnt
    vmovdqu   QLimb0, [Op1-56]
    vpsrlvq   ShrQL1, QLimb1, ShrQLCnt
    vpermq    ShrQL1, ShrQL1, 10010011b
    vpblendd  ShrQL0, ShrQL0, ShrQL1, 00000011b
    vpor      ShlQL0, ShlQL0, ShrQL0
    vpsllvq   ShlQL1, QLimb1, ShlQLCnt
    vpsrlvq   ShrQL0, QLimb0, ShrQLCnt
    vpermq    ShrQL0, ShrQL0, 10010011b
    vpblendd  ShrQL1, ShrQL1, ShrQL0, 00000011b
    vmovdqa   [Op2-24], ShlQL0
    vpor      ShlQL1, ShlQL1, ShrQL1
    vmovdqa   [Op2-56], ShlQL1

    sub     Op1, 64
    sub     Op2, 64

  .lShlEquAVXCheck:

    sub     Size1, 8
    jnc     .lShlEquAVXLoop

    mov     Limb1, [Op1]
    xor     Limb2, Limb2
    shld    Limb2, Limb1, CL
%if 1
    vmovq   ShlDL0, Limb2
    vpblendd ShrQL0, ShrQL0, ShlQL0, 3
%else
    ; I am mixing in a single SSE4.1 instruction into otherwise pure AVX2
    ; this is generating stalls on Haswell & Broadwell architecture (Agner Fog)
    ; but it is only executed once and there is no AVX2 based alternative
    pinsrq  ShrDL0, Limb2, 0        ; SSE4.1
%endif
    vpsllvq ShlQL0, QLimb0, ShlQLCnt
    vpor    ShlQL0, ShlQL0, ShrQL0
    vmovdqa [Op2-24], ShlQL0

    sub     Op2, 32
    add     Size1, 8

    ; shift remaining max. 7 limbs with SHLD mnemonic
  .lShlEquFour:

    sub     Op1, 8
    test    Size1, 4
    je      .lShlEquTwo

    mov     Limb2, [Op1]
    shld    Limb1, Limb2, CL
    mov     [Op2], Limb1
    mov     Limb1, [Op1-8]
    shld    Limb2, Limb1, CL
    mov     [Op2-8], Limb2
    mov     Limb2, [Op1-16]
    shld    Limb1, Limb2, CL
    mov     [Op2-16], Limb1
    mov     Limb1, [Op1-24]
    shld    Limb2, Limb1, CL
    mov     [Op2-24], Limb2

    sub     Op1, 32
    sub     Op2, 32

  .lShlEquTwo:

    test    Size1, 2
    je      .lShlEquOne

    mov     Limb2, [Op1]
    shld    Limb1, Limb2, CL
    mov     [Op2], Limb1
    mov     Limb1, [Op1-8]
    shld    Limb2, Limb1, CL
    mov     [Op2-8], Limb2

    sub     Op1, 16
    sub     Op2, 16

  .lShlEquOne:

    test    Size1, 1
    je      .lShlEquPost

    mov     Limb2, [Op1]
    shld    Limb1, Limb2, CL
    mov     [Op2], Limb1
    mov     Limb1, Limb2

    sub     Op2, 8

  .lShlEquPost:

    shl    Limb1, CL
    mov     [Op2], Limb1

  .Exit:

    vzeroupper
END_PROC reg_save_list
.end: