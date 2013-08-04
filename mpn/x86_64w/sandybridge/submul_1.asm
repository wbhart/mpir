; PROLOGUE(mpn_submul_1)
        
;  Copyright 2011 The Code Cavern  
;
;  Windows Conversion Copyright 2008 Brian Gladman
;
;  This file is part of the MPIR Library.
;
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
;
;  mp_limb_t mpn_submul_1(mp_ptr, mp_ptr, mp_size_t, mp_limb_t)
;  mp_limb_t mpn_declsh_n(mp_ptr, mp_ptr, mp_size_t,   mp_uint)
;  rax                       rdi     rsi        rdx        rcx
;  rax                       rcx     rdx         r8         r9

%include "yasm_mac.inc"

    CPU  Athlon64
    BITS 64

%define reg_save_list rsi, rdi, r12

  
        xalign 16
        LEAF_PROC mpn_submul_1
        mov     rax, [rdx]
        cmp     r8, 1
        jnz     .1
        mul     r9
        sub     [rcx], rax
        adc     rdx, 0
        mov     rax, rdx
        ret

        xalign   16
.1:	    FRAME_PROC ?mpn_sandybridge_submul, 0, reg_save_list
        mov     r11, 5
        lea     rsi, [rdx+r8*8-40]
        lea     rdi, [rcx+r8*8-40]
        mov     rcx, r9
        sub     r11, r8
        mul     rcx
        db      0x26
        mov     r8, rax
        db      0x26
        mov     rax, [rsi+r11*8+8]
        db      0x26
        mov     r9, rdx
        db      0x26
        cmp     r11, 0
        db      0x26
        mov     [rsp-8], r12
        db      0x26
        jge     .3
.2:     xor     r10, r10
        mul     rcx
        sub     [rdi+r11*8], r8
        adc     r9, rax
        adc     r10, rdx
        mov     rax, [rsi+r11*8+16]
        mul     rcx
        sub     [rdi+r11*8+8], r9
        adc     r10, rax
        mov     r12d, 0
        adc     r12, rdx
        mov     rax, [rsi+r11*8+24]
        xor     r8, r8
        xor     r9, r9
        mul     rcx
        sub     [rdi+r11*8+16], r10
        adc     r12, rax
        adc     r8, rdx
        mov     rax, [rsi+r11*8+32]
        mul     rcx
        sub     [rdi+r11*8+24], r12
        adc     r8, rax
        adc     r9, rdx
        add     r11, 4
        mov     rax, [rsi+r11*8+8]
        jnc     .2
.3:     xor     r10, r10
        mul     rcx
        sub     [rdi+r11*8], r8
        adc     r9, rax
        adc     r10, rdx
        cmp     r11, 2
        ja      .7
        jz      .6
        jp      .5
.4:     mov     rax, [rsi+16]
        mul     rcx
        sub     [rdi+8], r9
        adc     r10, rax
        mov     r12d, 0
        adc     r12, rdx
        mov     rax, [rsi+24]
        xor     r8, r8
        xor     r9, r9
        mul     rcx
        sub     [rdi+16], r10
        adc     r12, rax
        adc     r8, rdx
        mov     rax, [rsi+32]
        mul     rcx
        sub     [rdi+24], r12
        adc     r8, rax
        adc     r9, rdx
        sub     [rdi+32], r8
        adc     r9, 0
        mov     rax, r9
        EXIT_PROC reg_save_list
.5:     mov     rax, [rsi+24]
        mul     rcx
        sub     [rdi+16], r9
        adc     r10, rax
        mov     r12d, 0
        adc     r12, rdx
        mov     rax, [rsi+32]
        xor     r8, r8
        mul     rcx
        sub     [rdi+24], r10
        adc     r12, rax
        adc     r8, rdx
        sub     [rdi+32], r12
        adc     r8, 0
        mov     rax, r8
        EXIT_PROC reg_save_list

        align   16
.6:     mov     rax, [rsi+32]
        mul     rcx
        sub     [rdi+24], r9
        adc     r10, rax
        mov     r12d, 0
        adc     r12, rdx
        sub     [rdi+32], r10
        adc     r12, 0
        mov     rax, r12
        EXIT_PROC reg_save_list
.7:     sub     [rdi+32], r9
        adc     r10, 0
        mov     rax, r10
        END_PROC reg_save_list

        end
