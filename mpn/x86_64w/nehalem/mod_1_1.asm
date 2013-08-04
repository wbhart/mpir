; PROLOGUE(mpn_mod_1_1)
;  Copyright 2011 The Code Cavern
;
;  Windows Conversion Copyright 2011 Brian Gladman
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
;  mp_limb_t  mpn_mod_1_1(mp_ptr, mp_ptr, mp_size_t, mp_ptr)
;  rax                       rdi     rsi        rdx     rcx
;  rax                       rcx     rdx         r8      r9

%include 'yasm_mac.inc'

    CPU  Athlon64
    BITS 64

%define reg_save_list rsi, rdi, r13

        FRAME_PROC mpn_mod_1_1, 0, reg_save_list
        mov     rdi, rcx
        mov     rsi, rdx
        mov     rcx, r8

        mov     r13, [rsi+rcx*8-8]
        mov     rax, [rsi+rcx*8-16]
        mov     r8, [r9]
        mov     r9, [r9+8]
        xor     r11, r11
        mov     r10, [rsi+rcx*8-24]
        sub     rcx, 3
        jz      .2

        align   16
.1:     mul     r8
        add     r10, rax
        adc     r11, rdx
        lea     rax, [r13]
        lea     r13, [r11]
        mul     r9
        add     rax, r10
        adc     r13, rdx
        xor     r11, r11
        mov     r10, [rsi+rcx*8-8]
        lea     r8, [r8]
        dec     rcx
        lea     r9, [r9]
        jnz     .1
.2:     mul     r8
        add     r10, rax
        adc     r11, rdx
        lea     rax, [r13]
        lea     r13, [r11]
        mul     r9
        add     rax, r10
        adc     r13, rdx
        mov     [rdi], rax
        mov     rax, r8
        mul     r13
        add     [rdi], rax
        adc     rdx, 0
        mov     [rdi+8], rdx
    	END_PROC reg_save_list
	
	    end
