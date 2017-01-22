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
; mp_limb_t mpn_nsumdiff_n(mp_ptr Op3, mp_ptr Op4, mp_srcptr Op1, mp_srcptr Op2, mp_size_t Size)
; Linux     RAX           RDI         RSI         RDX            RCX            R8
; Win7      RAX           RCX         RDX         R8             R9             Stack
;
; Description:
; The function computes -(Op2+Op1) and stores the result in Op3 while at the
; same time subtracting Op2 from Op1 with result in Op4. The final carries from
; addition and subtraction are handed back as a combined mp_limb_t. There is a
; gain in execution speed compared to separate addition and subtraction by
; reducing memory access. The factor depends on the size of the operands (the
; cache hierarchy in which the operands can be handled).
;
; Equivalent to, assuming no overlap:
; cy1 = mpn_add_n(r1, s1, s2, n);
; cy2 = mpn_neg_n(r1, r1, n); /* cy2 = [{r1,n} != 0] */
; cy3 = mpn_sub_n(r2, s1, s2, n);
; return 2*(cy1 + cy2) + cy3;
; ============================================================================


%include 'yasm_mac.inc'

%define reg_save_list rsi, rdi, rbx, rbp, r12, r13, r14, r15

%define Op3     RDI
%define Op4     RSI
%define Op1     RDX
%define Op2     RCX
%define Size    R8

%define Limb0   RBP
%define Limb1   RBX
%define Limb2   R9
%define Limb3   R10
%define Limb4   R11
%define Limb5   R12
%define Limb6   R13
%define Limb7   R14
%define Limb8   R15

%ifdef USE_PREFETCH
%define Offs    PREFETCH_STRIDE ; no more regs avail. => fallback to const
%endif

%define SaveAC  setc    AL
%define LoadAC  shr     AL, 1

%define SaveSC  sbb     AH, AH
%define LoadSC  add     AH, AH

    BITS 64

    align   32

    FRAME_PROC mpn_nsumdiff_n, 0, reg_save_list
    mov     rdi, rcx
    mov     rsi, rdx
    mov     rdx, r8
    mov     rcx, r9
    mov     r8, [rsp+stack_use+40]

    xor     EAX, EAX            ; clear add & sub carry

; First we handle any words whose sum = 0
    mov     Limb1, [Op1]
    mov     Limb5, [Op2]
    mov     Limb2, Limb1
    add     Limb2, Limb5
    SaveAC
    neg	    Limb2
    jz      .zero_sum ; ajs:notshortform

; Then we handle the first word whose sum !=0. The NOT of this sum needs to
; be incremented, which produces no carry (NOT(x) + 1 = NEG(x))
.not_zero:
    mov     [Op3], Limb2
    LoadSC
    sbb     Limb1, Limb5
    SaveSC
    add     Op1, 8
    add     Op2, 8
    add     Op3, 8
    dec     Size
    mov     [Op4], Limb1
    add     Op4, 8

    shr     Size, 1
    jnc     .n_two

    mov     Limb1, [Op1]
    mov     Limb5, [Op2]
    LoadAC
    mov     Limb2, Limb1
    adc     Limb2, Limb5
    SaveAC
    LoadSC
    sbb     Limb1, Limb5
    SaveSC
    not     Limb2
    add     Op1, 8
    add     Op2, 8
    add     Op3, 8
    mov     [Op3-8], Limb2
    mov     [Op4], Limb1
    add     Op4, 8

  .n_two:

    shr     Size, 1
    jnc     .n_four
    mov     Limb1, [Op1]
    mov     Limb2, [Op1+8]
    mov     Limb5, [Op2]
    mov     Limb6, [Op2+8]
    LoadAC
    mov     Limb3, Limb1
    mov     Limb4, Limb2
    adc     Limb3, Limb5
    not     Limb3
    adc     Limb4, Limb6
    SaveAC
    LoadSC
    not     Limb4
    sbb     Limb1, Limb5
    sbb     Limb2, Limb6
    SaveSC
    mov     [Op3], Limb3
    mov     [Op3+8], Limb4
    mov     [Op4], Limb1
    mov     [Op4+8], Limb2
    add     Op1, 16
    add     Op2, 16
    add     Op3, 16
    add     Op4, 16


  .n_four:

    shr     Size, 1
    jnc     .n_loop_pre ;ajs:notshortform

    LoadAC

    ; slight change of scheme here - avoid too many
    ; memory to reg or reg to memory moves in a row
    mov     Limb1, [Op1]
    mov     Limb5, [Op2]
    mov     Limb0, Limb1
    adc     Limb0, Limb5
    not     Limb0
    mov     [Op3], Limb0
    mov     Limb2, [Op1+8]
    mov     Limb6, [Op2+8]
    mov     Limb0, Limb2
    adc     Limb0, Limb6
    not     Limb0
    mov     [Op3+8], Limb0
    mov     Limb3, [Op1+16]
    mov     Limb7, [Op2+16]
    mov     Limb0, Limb3
    adc     Limb0, Limb7
    not     Limb0
    mov     [Op3+16], Limb0
    mov     Limb4, [Op1+24]
    mov     Limb8, [Op2+24]
    mov     Limb0, Limb4
    adc     Limb0, Limb8
    not     Limb0
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
  .n_loop_pre:		; If we jump here, ZF=1 iff Size=0
    jz     .n_post      ;ajs:notshortform
    LoadAC              ; set carry for addition

    ; main loop - values below are best case - up to 50% fluctuation possible!
    ; - 3.50      cycles per limb in LD1$
    ; - 3.50      cycles per limb in LD2$
    ; - 5.10-5.50 cycles per limb in LD3$
    align   16
  .n_loop:

  %ifdef USE_PREFETCH
    prefetchnta [Op1+Offs]
    prefetchnta [Op2+Offs]
  %endif

    mov     Limb1, [Op1]        ; add the first quad-limb
    mov     Limb5, [Op2]
    mov     Limb0, Limb1
    adc     Limb0, Limb5
    not     Limb0
    mov     [Op3], Limb0
    mov     Limb2, [Op1+8]
    mov     Limb6, [Op2+8]
    mov     Limb0, Limb2
    adc     Limb0, Limb6
    not     Limb0
    mov     [Op3+8], Limb0
    mov     Limb3, [Op1+16]
    mov     Limb7, [Op2+16]
    mov     Limb0, Limb3
    adc     Limb0, Limb7
    not     Limb0
    mov     [Op3+16], Limb0
    mov     Limb4, [Op1+24]
    mov     Limb8, [Op2+24]
    mov     Limb0, Limb4
    adc     Limb0, Limb8
    not     Limb0
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
    not     Limb1
    mov     [Op3+32-64], Limb1
    adc     Limb2, Limb6
    not     Limb2
    mov     [Op3+40-64], Limb2
    adc     Limb3, Limb7
    not     Limb3
    mov     [Op3+48-64], Limb3
    adc     Limb4, Limb8
    not     Limb4
    mov     [Op3+56-64], Limb4

    lea     Op1, [Op1 + 64]
    lea     Op2, [Op2 + 64]


    dec     Size
    jnz     .n_loop     ;ajs:notshortform

    SaveAC                      ; memorize add-carry
    ; hand back carries
  .n_post:
				; AL = cy1, AH = -cy3. cy2 = 1 here, as
                                ; there were non-zero words in the sum
    inc     al			; AL = cy1 + cy2 = cy1 + 1, AH = -cy3
.all_zero:
    LoadSC			; AL = cy1 + cy2, CY = cy3
    adc     AL, AL		; AL = 2*(cy1 + cy2) + cy3
    movsx   EAX, AL

  .Exit:
  END_PROC reg_save_list
.end:

.zero_sum:
    mov     [Op3], Limb2
    LoadSC
    sbb     Limb1, Limb5
    SaveSC
    mov     [Op4], Limb1
    dec     Size
    jz      .all_zero
    add     Op1, 8
    add     Op2, 8
    add     Op3, 8
    add     Op4, 8
    mov     Limb1, [Op1]
    mov     Limb5, [Op2]
    mov     Limb2, Limb1
    LoadAC
    adc     Limb2, Limb5
    SaveAC
    neg     Limb2
    jz      .zero_sum
    jmp     .not_zero
