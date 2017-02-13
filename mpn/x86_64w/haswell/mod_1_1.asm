; PROLOGUE(mpn_mod_1_1)

;  Copyright 2009 Jason Moxham
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
;  mp_limb_t  mpn_mod_1_1(mp_ptr, mp_ptr, mp_size_t, mp_ptr)
;  rax                       rdi     rsi        rdx     rcx
;  rax                       rcx     rdx         r8      r9

%include 'yasm_mac.inc'

    CPU  Athlon64
    BITS 64

%define reg_save_list rsi, rdi, r13

    FRAME_PROC mpn_mod_1_1, 0, reg_save_list
    mov     rsi, rdx
    mov     rdx, r8
    
	mov     r13, [rsi+rdx*8-8]
	mov     rax, [rsi+rdx*8-16]
	mov     r8, [r9]
	mov     r9, [r9+8]
	mov     rdi, rdx
	sub     rdi, 2
	
	xalign  16
.1:	mov     r10, [rsi+rdi*8-8]
	mul     r8
	add     r10, rax
	mov     r11, 0
	adc     r11, rdx
	mov     rax, r13
	mul     r9
	add     rax, r10
	mov     r13, r11
	adc     r13, rdx
	dec     rdi
	jnz     .1

	mov     [rcx], rax
	mov     rax, r8
	mul     r13
	add     [rcx], rax
	adc     rdx, 0
	mov     [rcx+8], rdx
	END_PROC reg_save_list
	
	end
