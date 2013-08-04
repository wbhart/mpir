; PROLOGUE(mpn_mul_2)
;  X86_64 mpn_mul_2
;
;  Copyright 2010 Jason Moxham
;
;  Windows Conversion Copyright 2010 Brian Gladman
;
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
;
;  mp_limb_t  mpn_mul_2(mp_ptr, mp_ptr, mp_size_t, mp_limb_t)
;  rax                     rdi     rsi        rdx        rcx
;  rax                     rcx     rdx         r8         r9

%include "yasm_mac.inc"

%define reg_save_list rsi, rdi, rbx

        CPU  nehalem
        BITS 64

    	FRAME_PROC mpn_mul_2, 0, reg_save_list
        mov     rbx, 3
        lea     rdi, [rcx+r8*8-24]
        lea     rsi, [rdx+r8*8-24]
        sub     rbx, r8
        mov     r8, [r9+8]
        mov     rcx, [r9]

        mov     r11, 0
        mov     r9, 0
        mov     rax, [rsi+rbx*8]
        mov     r10, 0
        mul     rcx
        add     r11, rax
        mov     rax, [rsi+rbx*8]
        mov     [rdi+rbx*8], r11
        adc     r9, rdx
        cmp     rbx, 0
        jge     .2

        align   16
.1:     mul     r8
        add     r9, rax
        adc     r10, rdx
        mov     rax, [rsi+rbx*8+8]
        mov     r11, 0
        mul     rcx
        add     r9, rax
        adc     r10, rdx
        adc     r11, 0
        mov     rax, [rsi+rbx*8+8]
        mul     r8
        add     r10, rax
        mov     rax, [rsi+rbx*8+16]
        adc     r11, rdx
        mul     rcx
        add     r10, rax
        mov     [rdi+rbx*8+8], r9
        adc     r11, rdx
        mov     r9, 0
        mov     rax, [rsi+rbx*8+16]
        adc     r9, 0
        mul     r8
        add     r11, rax
        mov     [rdi+rbx*8+16], r10
        mov     rax, [rsi+rbx*8+24]
        mov     r10, 0
        adc     r9, rdx
        mul     rcx
        add     r11, rax
        mov     rax, [rsi+rbx*8+24]
        mov     [rdi+rbx*8+24], r11
        adc     r9, rdx
        adc     r10, 0
        add     rbx, 3
        jnc     .1
.2:     cmp     rbx, 1
        ja      .5
        je      .4
.3:     mul     r8
        add     r9, rax
        adc     r10, rdx
        mov     rax, [rsi+rbx*8+8]
        mov     r11, 0
        mul     rcx
        add     r9, rax
        adc     r10, rdx
        adc     r11, 0
        mov     rax, [rsi+rbx*8+8]
        mul     r8
        add     r10, rax
        mov     rax, [rsi+rbx*8+16]
        adc     r11, rdx
        mul     rcx
        add     r10, rax
        mov     [rdi+rbx*8+8], r9
        adc     r11, rdx
        mov     r9, 0
        mov     rax, [rsi+rbx*8+16]
        adc     r9, 0
        mul     r8
        add     r11, rax
        mov     [rdi+rbx*8+16], r10
        adc     r9, rdx
        mov     [rdi+rbx*8+24], r11
        mov     rax, r9
        EXIT_PROC reg_save_list

.4:     mul     r8
        add     r9, rax
        adc     r10, rdx
        mov     rax, [rsi+rbx*8+8]
        mov     r11, 0
        mul     rcx
        add     r9, rax
        adc     r10, rdx
        adc     r11, 0
        mov     rax, [rsi+rbx*8+8]
        mul     r8
        add     r10, rax
        adc     r11, rdx
        mov     [rdi+rbx*8+8], r9
        mov     [rdi+rbx*8+16], r10
        mov     rax, r11
        EXIT_PROC reg_save_list

.5:     mul     r8
        add     r9, rax
        adc     r10, rdx
        mov     [rdi+rbx*8+8], r9
        mov     rax, r10
        END_PROC reg_save_list
        
        end    
        
