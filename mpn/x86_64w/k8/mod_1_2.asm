; PROLOGUE(mpn_mod_1_2)

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
;  mp_limb_t  mpn_mod_1_2(mp_ptr, mp_ptr, mp_size_t, mp_ptr)
;  rax                       rdi     rsi        rdx     rcx
;  rax                       rcx     rdx         r8      r9

%include 'yasm_mac.inc'

    CPU  Athlon64
    BITS 64

%define reg_save_list rsi, rdi, r12, r13, r14

        FRAME_PROC mpn_mod_1_2, 0, reg_save_list
        mov     rdi, rcx
        mov     rsi, rdx
        mov     rdx, r8

        mov     r14, [rsi+rdx*8-8]
        mov     r13, [rsi+rdx*8-16]
        mov     r8, [r9]
        mov     r10, [r9+16]
        mov     r9, [r9+8]
        mov     rcx, rdx
        mov     rax, [rsi+rdx*8-24]
        mul     r8
        mov     r11, [rsi+rcx*8-32]
        xor     r12, r12
        sub     rcx, 6
        jc      .2
    
        align   16
.1:     add     r11, rax
        adc     r12, rdx
        mov     rax, r9
        mul     r13
        add     r11, rax
        adc     r12, rdx
        mov     r13, r11
        mov     rax, r10
        mul     r14
        add     r13, rax
        mov     rax, [rsi+rcx*8+8]
        mov     r14, r12
        adc     r14, rdx
        mul     r8
        mov     r12d, 0
        mov     r11, [rsi+rcx*8+0]
        sub     rcx, 2
        jnc     .1
.2:     add     r11, rax
        adc     r12, rdx
        mov     rax, r9
        mul     r13
        add     r11, rax
        adc     r12, rdx
        mov     r13, r11
        mov     rax, r10
        mul     r14
        add     r13, rax
        mov     r14, r12
        adc     r14, rdx
        cmp     rcx, -2
        je      .4
.3:     mov     r11, [rsi+rcx*8+8]
        xor     r12, r12
        mov     rax, r8
        mul     r13
        add     r11, rax
        adc     r12, rdx
        mov     r13, r11
        mov     rax, r9
        mul     r14
        add     r13, rax
        mov     r14, r12
        adc     r14, rdx
.4:     mov     rax, r8
        mul     r14
        add     r13, rax
        adc     rdx, 0
        mov     [rdi], r13
        mov     [rdi+8], rdx
    	END_PROC reg_save_list
	
	    end
