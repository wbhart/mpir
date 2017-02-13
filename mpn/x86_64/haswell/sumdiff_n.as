; ============================================================================
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
; mp_limb_t mpn_sumdiff_n(mp_ptr Op3, mp_ptr Op4, mp_srcptr Op1, mp_srcptr Op2, mp_size_t Size)
; Linux     RAX           RDI         RSI         RDX            RCX            R8
; Win7      RAX           RCX         RDX         R8             R9             Stack
;
; Description:
; The function adds Op2 to Op1 and stores the result in Op3 while at the same
; time subtracting Op2 from Op1 with result in Op4. The final carries from
; addition and subtraction are handed back as a combined mp_limb_t. There is a
; gain in execution speed compared to separate addition and subtraction by
; reducing memory access. The factor depends on the size of the operands (the
; cache hierarchy in which the operands can be handled).
;
; Comments:
; - asm version implemented, tested & benched on 10.06.2015 by jn
; - On Nehalem per limb saving is 0.5 cycle in LD1$, LD2$ and LD3$
; - includes prefetching
; - includes XMM save & restore
; ============================================================================

%include 'yasm_mac.inc'

%ifdef USE_WIN64

    %define Op3     RCX
    %define Op4     RDX
    %define Op1     R8
    %define Op2     R9
    %define Size    RBX             ; SAVE!

    %define Limb0   RBP             ; SAVE!
    %define Limb1   RSI             ; SAVE!
    %define Limb2   RDI             ; SAVE!
    %define Limb3   R10
    %define Limb4   R11
    %define Limb5   R12             ; SAVE!
    %define Limb6   R13             ; SAVE!
    %define Limb7   R14             ; SAVE!
    %define Limb8   R15             ; SAVE!

  %ifdef USE_PREFETCH
    %define Offs    PREFETCH_STRIDE ; no more regs avail. => fallback to const
  %endif

    %define SaveRBX XMM0            ; use scratch XMM for fast save & restore
    %define SaveRBP XMM1            ; R14 and R15 handled via stack
    %define SaveRSI XMM2
    %define SaveRDI XMM3
    %define SaveR12 XMM4
    %define SaveR13 XMM5

%else

    %define Op3     RDI
    %define Op4     RSI
    %define Op1     RDX
    %define Op2     RCX
    %define Size    R8

    %define Limb0   RBP             ; SAVE!
    %define Limb1   RBX             ; SAVE!
    %define Limb2   R9
    %define Limb3   R10
    %define Limb4   R11
    %define Limb5   R12             ; SAVE!
    %define Limb6   R13             ; SAVE!
    %define Limb7   R14             ; SAVE!
    %define Limb8   R15             ; SAVE!

  %ifdef USE_PREFETCH
    %define Offs    PREFETCH_STRIDE ; no more regs avail. => fallback to const
  %endif

    %define SaveRBX XMM0            ; use scratch XMM for fast save & restore
    %define SaveRBP XMM1
    %define SaveR12 XMM2
    %define SaveR13 XMM3
    %define SaveR14 XMM4
    %define SaveR15 XMM5

%endif

%define SaveAC  setc    AL
%define LoadAC  shr     AL, 1

%define SaveSC  sbb     AH, AH
%define LoadSC  add     AH, AH

BITS 64

align   32

   GLOBAL_FUNC mpn_sumdiff_n

  %ifdef USE_WIN64
    sub     RSP, 16
    mov     [RSP+8], R15
    mov     [RSP], R14

    movq    SaveR13, R13
    movq    SaveR12, R12
    movq    SaveRDI, RDI
    movq    SaveRSI, RSI
    movq    SaveRBP, RBP
    movq    SaveRBX, RBX
  %else
    movq    SaveR15, R15
    movq    SaveR14, R14
    movq    SaveR13, R13
    movq    SaveR12, R12
    movq    SaveRBP, RBP
    movq    SaveRBX, RBX
  %endif

    xor     EAX, EAX            ; clear add & sub carry

    shr     Size, 1
    jnc     .sumdiff_n_two

    mov     Limb1, [Op1]
    mov     Limb5, [Op2]
    mov     Limb2, Limb1
    add     Limb2, Limb5
    mov     [Op3], Limb2

    SaveAC

    sub     Limb1, Limb5
    mov     [Op4], Limb1

    SaveSC

    add     Op1, 8
    add     Op2, 8
    add     Op3, 8
    add     Op4, 8

  .sumdiff_n_two:

    shr     Size, 1
    jnc     .sumdiff_n_four

    mov     Limb1, [Op1]
    mov     Limb2, [Op1+8]
    mov     Limb5, [Op2]
    mov     Limb6, [Op2+8]

    LoadAC

    mov     Limb3, Limb1
    adc     Limb3, Limb5
    mov     [Op3], Limb3
    mov     Limb4, Limb2
    adc     Limb4, Limb6
    mov     [Op3+8], Limb4

    SaveAC
    LoadSC

    sbb     Limb1, Limb5
    mov     [Op4], Limb1
    sbb     Limb2, Limb6
    mov     [Op4+8], Limb2

    SaveSC

    add     Op1, 16
    add     Op2, 16
    add     Op3, 16
    add     Op4, 16

  .sumdiff_n_four:

    shr     Size, 1
    jnc     .sumdiff_n_loop_pre ;ajs:notshortform

    LoadAC

    ; slight change of scheme here - avoid too many
    ; memory to reg or reg to memory moves in a row
    mov     Limb1, [Op1]
    mov     Limb5, [Op2]
    mov     Limb0, Limb1
    adc     Limb0, Limb5
    mov     [Op3], Limb0
    mov     Limb2, [Op1+8]
    mov     Limb6, [Op2+8]
    mov     Limb0, Limb2
    adc     Limb0, Limb6
    mov     [Op3+8], Limb0
    mov     Limb3, [Op1+16]
    mov     Limb7, [Op2+16]
    mov     Limb0, Limb3
    adc     Limb0, Limb7
    mov     [Op3+16], Limb0
    mov     Limb4, [Op1+24]
    mov     Limb8, [Op2+24]
    mov     Limb0, Limb4
    adc     Limb0, Limb8
    mov     [Op3+24], Limb0

    SaveAC
    LoadSC

    sbb     Limb1, Limb5
    mov     [Op4], Limb1
    sbb     Limb2, Limb6
    mov     [Op4+8], Limb2
    sbb     Limb3, Limb7
    mov     [Op4+16], Limb3
    sbb     Limb4, Limb8
    mov     [Op4+24], Limb4

    SaveSC

    add     Op1, 32
    add     Op2, 32
    add     Op3, 32
    add     Op4, 32
 
    test   Size, Size
  .sumdiff_n_loop_pre:		; If we jump here, ZF=1 iff Size=0
    jz     .sumdiff_n_post      ;ajs:notshortform
    LoadAC              ; set carry for addition

    ; main loop - values below are best case - up to 50% fluctuation possible!
    ; - 3.50      cycles per limb in LD1$
    ; - 3.50      cycles per limb in LD2$
    ; - 5.10-5.50 cycles per limb in LD3$
    align   16
  .sumdiff_n_loop:

  %ifdef USE_PREFETCH
    prefetchnta [Op1+Offs]
    prefetchnta [Op2+Offs]
  %endif

    mov     Limb1, [Op1]        ; add the first quad-limb
    mov     Limb5, [Op2]
    mov     Limb0, Limb1
    adc     Limb0, Limb5
    mov     [Op3], Limb0
    mov     Limb2, [Op1+8]
    mov     Limb6, [Op2+8]
    mov     Limb0, Limb2
    adc     Limb0, Limb6
    mov     [Op3+8], Limb0
    mov     Limb3, [Op1+16]
    mov     Limb7, [Op2+16]
    mov     Limb0, Limb3
    adc     Limb0, Limb7
    mov     [Op3+16], Limb0
    mov     Limb4, [Op1+24]
    mov     Limb8, [Op2+24]
    mov     Limb0, Limb4
    adc     Limb0, Limb8
    mov     [Op3+24], Limb0
    lea     Op3, [Op3 + 64]

    SaveAC              ; memorize add-carry
    LoadSC              ; set carry for subtraction

    sbb     Limb1, Limb5        ; now sub the first quad-limb
    mov     [Op4], Limb1
    sbb     Limb2, Limb6
    mov     [Op4+8], Limb2
    sbb     Limb3, Limb7
    mov     [Op4+16], Limb3
    sbb     Limb4, Limb8
    mov     [Op4+24], Limb4

    mov     Limb1, [Op1+32]     ; sub the second quad-limb
    mov     Limb5, [Op2+32]
    mov     Limb0, Limb1
    sbb     Limb0, Limb5
    mov     [Op4+32], Limb0
    mov     Limb2, [Op1+40]
    mov     Limb6, [Op2+40]
    mov     Limb0, Limb2
    sbb     Limb0, Limb6
    mov     [Op4+40], Limb0
    mov     Limb3, [Op1+48]
    mov     Limb7, [Op2+48]
    mov     Limb0, Limb3
    sbb     Limb0, Limb7
    mov     [Op4+48], Limb0
    mov     Limb4, [Op1+56]
    mov     Limb8, [Op2+56]
    mov     Limb0, Limb4
    sbb     Limb0, Limb8
    mov     [Op4+56], Limb0
    lea     Op4, [Op4 + 64]

    SaveSC                      ; memorize sub-carry
    LoadAC                      ; set carry for addition

    adc     Limb1, Limb5        ; add the second quad-limb
    mov     [Op3+32-64], Limb1
    adc     Limb2, Limb6
    mov     [Op3+40-64], Limb2
    adc     Limb3, Limb7
    mov     [Op3+48-64], Limb3
    adc     Limb4, Limb8
    mov     [Op3+56-64], Limb4

    lea     Op1, [Op1 + 64]
    lea     Op2, [Op2 + 64]

    dec     Size
    jnz     .sumdiff_n_loop     ;ajs:notshortform

    SaveAC                      ; memorize add-carry
    ; hand back carries
  .sumdiff_n_post:
				; AL = add_carry, AH = -sub_carry
    LoadSC			; AL = add_carry, CY = sub_carry
    adc     AL, AL		; AL = 2*add_carry + sub_carry
    movsx   RAX, AL

  .Exit:

  %ifdef USE_WIN64
    movq    RBX, SaveRBX
    movq    RBP, SaveRBP
    movq    RSI, SaveRSI
    movq    RDI, SaveRDI
    movq    R12, SaveR12
    movq    R13, SaveR13

    mov     R14, [RSP]
    mov     R15, [RSP+8]
    add     RSP, 16
  %else
    movq    R15, SaveR15
    movq    R14, SaveR14
    movq    R13, SaveR13
    movq    R12, SaveR12
    movq    RBP, SaveRBP
    movq    RBX, SaveRBX
  %endif

    ret
.end:
