; PROLOGUE(mpn_karasub)
;  mpn_karasub  
;       
;  Copyright 2011 The Code Cavern  
;
;  Copyright 2012 Brian Gladman
;       
;  This file is part of the MPIR Library.  
;       
;  The MPIR Library is free software; you can redistribute it and/or modify  
;  it under the terms of the GNU Lesser General Public License as published  
;  by the Free Software Foundation; either version 2.1 of the License, or (at  
;  your option) any later version.  
;        
;  The MPIR Library is distributed in the hope that it will be useful, but  
;  WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY  
;  or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public  
;  License for more details.  
;        
;  You should have received a copy of the GNU Lesser General Public License  
;  along with the MPIR Library; see the file COPYING.LIB.  If not, write  
;  to the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,  
;  Boston, MA 02110-1301, USA.  
;
;  void mpn_karasub(mp_ptr, mp_ptr, mp_size_t)
;  rax                 rdi     rsi        rdx
;  rax                 rcx     rdx         r8
;
;  Karasuba Multiplication - split x and y into two equal length halves so
;  that x = xh.B + xl and y = yh.B + yl. Then their product is:
;
;  x.y = xh.yh.B^2 + (xh.yl + xl.yh).B + xl.yl
;      = xh.yh.B^2 + (xh.yh + xl.yl - {xh - xl}.{yh - yl}).B + xl.yl
;
; If the length of the elements is m (about n / 2), the output length is 4 * m 
; as illustrated below.  The middle two blocks involve three additions and one 
; subtraction: 
; 
;       -------------------- rp
;       |                  |-->
;       |   A:xl.yl[lo]    |   |
;       |                  |   |      (xh - xl).(yh - yl)
;       --------------------   |      -------------------- tp
;  <--  |                  |<--<  <-- |                  |
; |     |   B:xl.yl[hi]    |   |      |     E:[lo]       |
; |     |                  |   |      |                  |
; |     --------------------   |      --------------------
; >-->  |                  |-->   <-- |                  |
; |\___ |   C:xh.yh[lo]    | ____/    |     F:[hi]       |
; |     |                  |          |                  |
; |     --------------------          --------------------
;  <--  |                  |   
;       |   D:xh.yh[hi]    |
;       |                  |
;       --------------------
;
; To avoid overwriting B before it is used, we need to do two operations
; in parallel:
;
; (1)   B = B + C + A - E = (B + C) + A - E
; (2)   C = C + B + D - F = (B + C) + D - F
;
; The final carry from (1) has to be propagated into C and D, and the final
; carry from (2) has to be propagated into D. When the number of input limbs
; is odd, some extra operations have to be undertaken. 

%include "yasm_mac.inc"

%define reg_save_list   rbx, rbp, rsi, rdi, r12, r13, r14, r15

%macro add_one 1
    lea     %1, [%1 + 1]
%endmacro

    BITS 64
    TEXT
        
;       requires n >= 8  
        FRAME_PROC mpn_karasub, 2, reg_save_list
        mov     rdi, rcx
        mov     rsi, rdx
        mov     rdx, r8
        mov     [rsp], rdx
        mov     [rsp+8], rdi

;       rp is rdi, tp is rsi, L is rdi, H is rbp, tp is rsi
;       carries/borrows in rax, rbx
   
        shr     rdx, 1
        lea     rcx, [rdx+rdx*1]
        lea     rbp, [rdi+rcx*8]
        xor     rax, rax
        xor     rbx, rbx
        lea     rdi, [rdi+rdx*8-24]
        lea     rsi, [rsi+rdx*8-24]
        lea     rbp, [rbp+rdx*8-24]
        mov     ecx, 3
        sub     rcx, rdx
        mov     edx, 3

        align   16
.1:     bt      rbx, 2
        mov     r8, [rdi+rdx*8]
        adc     r8, [rbp+rcx*8]
        mov     r12, r8
        mov     r9, [rdi+rdx*8+8]
        adc     r9, [rbp+rcx*8+8]
        mov     r10, [rdi+rdx*8+16]
        adc     r10, [rbp+rcx*8+16]
        mov     r11, [rdi+rdx*8+24]
        adc     r11, [rbp+rcx*8+24]
        rcl     rbx, 1
        bt      rax, 1
        mov     r15, r11
        adc     r8, [rdi+rcx*8]
        mov     r13, r9
        adc     r9, [rdi+rcx*8+8]
        mov     r14, r10
        adc     r10, [rdi+rcx*8+16]
        adc     r11, [rdi+rcx*8+24]
        rcl     rax, 1
        bt      rbx, 2
        adc     r12, [rbp+rdx*8]
        adc     r13, [rbp+rdx*8+8]
        adc     r14, [rbp+rdx*8+16]
        adc     r15, [rbp+rdx*8+24]
        rcl     rbx, 1
        bt      rax, 1
        sbb     r8, [rsi+rcx*8]
        sbb     r9, [rsi+rcx*8+8]
        sbb     r10, [rsi+rcx*8+16]
        sbb     r11, [rsi+rcx*8+24]
        mov     [rdi+rdx*8+16], r10
        mov     [rdi+rdx*8+24], r11
        rcl     rax, 1
        bt      rbx, 2
        mov     [rdi+rdx*8], r8
        mov     [rdi+rdx*8+8], r9
        sbb     r12, [rsi+rdx*8]
        sbb     r13, [rsi+rdx*8+8]
        sbb     r14, [rsi+rdx*8+16]
        sbb     r15, [rsi+rdx*8+24]
        rcl     rbx, 1
        add     rdx, 4
        mov     [rbp+rcx*8], r12
        mov     [rbp+rcx*8+8], r13
        mov     [rbp+rcx*8+16], r14
        mov     [rbp+rcx*8+24], r15
        add     rcx, 4
        jnc     .1
        cmp     rcx, 2
        jg      .5
        jz      .4
        jp      .3

.2:     bt      rbx, 2
        mov     r8, [rdi+rdx*8]
        adc     r8, [rbp]
        mov     r12, r8
        mov     r9, [rdi+rdx*8+8]
        adc     r9, [rbp+8]
        mov     r10, [rdi+rdx*8+16]
        adc     r10, [rbp+16]
        rcl     rbx, 1
        bt      rax, 1
        adc     r8, [rdi]
        mov     r13, r9
        adc     r9, [rdi+8]
        mov     r14, r10
        adc     r10, [rdi+16]
        rcl     rax, 1
        bt      rbx, 2
        adc     r12, [rbp+rdx*8]
        adc     r13, [rbp+rdx*8+8]
        adc     r14, [rbp+rdx*8+16]
        rcl     rbx, 1
        bt      rax, 1
        sbb     r8, [rsi]
        sbb     r9, [rsi+8]
        sbb     r10, [rsi+16]
        mov     [rdi+rdx*8+16], r10
        rcl     rax, 1
        bt      rbx, 2
        mov     [rdi+rdx*8], r8
        mov     [rdi+rdx*8+8], r9
        sbb     r12, [rsi+rdx*8]
        sbb     r13, [rsi+rdx*8+8]
        sbb     r14, [rsi+rdx*8+16]
        rcl     rbx, 1
        add     rdx, 3
        mov     [rbp], r12
        mov     [rbp+8], r13
        mov     [rbp+16], r14
        jmp     .5

.3:     bt      rbx, 2
        mov     r8, [rdi+rdx*8]
        adc     r8, [rbp+8]
        mov     r12, r8
        mov     r9, [rdi+rdx*8+8]
        adc     r9, [rbp+16]
        rcl     rbx, 1
        bt      rax, 1
        adc     r8, [rdi+8]
        mov     r13, r9
        adc     r9, [rdi+16]
        rcl     rax, 1
        bt      rbx, 2
        adc     r12, [rbp+rdx*8]
        adc     r13, [rbp+rdx*8+8]
        rcl     rbx, 1
        bt      rax, 1
        sbb     r8, [rsi+8]
        sbb     r9, [rsi+16]
        rcl     rax, 1
        bt      rbx, 2
        mov     [rdi+rdx*8], r8
        mov     [rdi+rdx*8+8], r9
        sbb     r12, [rsi+rdx*8]
        sbb     r13, [rsi+rdx*8+8]
        rcl     rbx, 1
        add     rdx, 2
        mov     [rbp+8], r12
        mov     [rbp+16], r13
        jmp     .5

.4:     bt      rbx, 2
        mov     r8, [rdi+rdx*8]
        adc     r8, [rbp+16]
        mov     r12, r8
        rcl     rbx, 1
        bt      rax, 1
        adc     r8, [rdi+16]
        rcl     rax, 1
        bt      rbx, 2
        adc     r12, [rbp+rdx*8]
        rcl     rbx, 1
        bt      rax, 1
        sbb     r8, [rsi+16]
        rcl     rax, 1
        bt      rbx, 2
        mov     [rdi+rdx*8], r8
        sbb     r12, [rsi+rdx*8]
        rcl     rbx, 1
        add_one rdx
        mov     [rbp+rcx*8], r12

;       move low half rbx carry into rax
.5:     rcr     rax, 3
        bt      rbx, 2
        rcl     rax, 3
        mov     r8, [rsp]
        mov     rcx, rsi
        mov     rsi,[rsp+8]
        lea     r9, [r8+r8]
        lea     rsi, [rsi+r9*8]
        lea     r11, [rbp+24]
        sub     r11, rsi
        sar     r11, 3
        bt      r8, 0
        jnc     .9

;       if odd the do next two  
        add     r11, 2
        mov     r8, [rbp+rdx*8]
        mov     r9, [rbp+rdx*8+8]
        rcr     rbx, 2
        adc     r8,0
        adc     r9, 0
        rcl     rbx, 1
        sbb     r8, [rcx+rdx*8]
        sbb     r9, [rcx+rdx*8+8]
        rcr     rbx, 2
        adc     [rbp+24], r8
        adc     [rbp+32], r9
        rcl     rbx, 3

; Now add in any accummulated carries and/or borrows
;
; NOTE: We can't propagate individual borrows or carries from the second
; and third quarter blocks into the fourth quater block by simply waiting
; for carry (or borrow) propagation to end.  This is because a carry into
; the fourth quarter block when it contains only maximum integers or a 
; borrow when it contains all zero integers will incorrectly propagate
; beyond the end of the top quarter block.

.9:     lea     rdx, [rdi+rdx*8]
        sub     rdx, rsi
        sar     rdx, 3

; carries/borrrow from second to third quarter quarter block
;   rax{2} is the carry in (B + C)
;   rax{1} is the carry in (B + C) + A
;   rax{0} is the borrow in (B + C + A) - E

        mov     rcx, rdx
        bt      rax, 0
.10:    sbb     qword[rsi+rcx*8], 0
        add_one rcx
        jrcxz   .11
        jc      .10

.11     mov     rcx, rdx
        bt      rax, 1
.12:    adc     qword[rsi+rcx*8], 0
        add_one rcx
        jrcxz   .13
        jc      .12

.13     mov     rcx, rdx
        bt      rax, 2
.14:    adc     qword[rsi+rcx*8], 0
        add_one rcx
        jrcxz   .15
        jc      .14

; carries/borrrow from third to fourth quarter quarter block
;   rbx{2} is the carry in (B + C)
;   rbx{1} is the carry in (B + C) + D
;   rbx{0} is the borrow in (B + C + D) - F

.15:    mov     rcx, r11
        bt      rbx, 0
.16:    sbb     qword[rsi+rcx*8], 0
        add_one rcx
        jrcxz   .17
        jc      .16

.17:    mov     rcx, r11
        bt      rbx, 1
.18:    adc     qword[rsi+rcx*8], 0
        add_one rcx
        jrcxz   .19
        jc      .18

.19:    mov     rcx, r11
        bt      rbx, 2
.20:    adc     qword[rsi+rcx*8], 0
        add_one rcx
        jrcxz   .21
        jc      .20
.21:
        END_PROC reg_save_list

        end
