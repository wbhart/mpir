; PROLOGUE(mpn_lshift2)

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

;  The MPIR Library is distributed in the hope that it will be useful, but
;  WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
;  or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public
;  License for more details.

;  You should have received a copy of the GNU Lesser General Public License
;  along with the MPIR Library; see the file COPYING.LIB.  If not, write
;  to the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
;  Boston, MA 02110-1301, USA.
;
;  mp_limb_t mpn_lshift1(mp_ptr, mp_ptr, mp_size_t)
;  rax                      rdi     rsi        rdx
;  rax                      rcx     rdx         r8

%include "yasm_mac.inc"

%define reg_save_list rsi, rdi

    CPU  Athlon64
    BITS 64

    FRAME_PROC mpn_lshift2, 0, reg_save_list
    mov     rax, r8
	lea     rsi, [rdx+rax*8-24]
	lea     rdi, [rcx+rax*8-24]
	mov     ecx, 3
	sub     rcx, rax
	xor     eax, eax
	xor     edx, edx
	cmp     rcx, 0
	jge     .2
	
	xalign  16
.1:	mov     r8, [rsi+rcx*8]
	mov     r9, [rsi+rcx*8+8]
	mov     r10, [rsi+rcx*8+16]
	mov     r11, [rsi+rcx*8+24]
	add     rax, rax
	adc     r8, r8
	adc     r9, r9
	adc     r10, r10
	adc     r11, r11
	sbb     rax, rax
	add     rdx, rdx
	adc     r8, r8
	adc     r9, r9
	adc     r10, r10
	adc     r11, r11
	mov     [rdi+rcx*8+24], r11
	sbb     rdx, rdx
	mov     [rdi+rcx*8], r8
	add     rcx, 4
	mov     [rdi+rcx*8-24], r9
	mov     [rdi+rcx*8-16], r10
	jnc     .1
.2:	cmp     rcx, 2
	ja      .6
	je      .5
	jp      .4
.3:	mov     r8, [rsi+rcx*8]
	mov     r9, [rsi+rcx*8+8]
	mov     r10, [rsi+rcx*8+16]
	add     rax, rax
	adc     r8, r8
	adc     r9, r9
	adc     r10, r10
	sbb     rax, rax
	add     rdx, rdx
	adc     r8, r8
	adc     r9, r9
	adc     r10, r10
	sbb     rdx, rdx
	mov     [rdi+rcx*8], r8
	mov     [rdi+rcx*8+8], r9
	mov     [rdi+rcx*8+16], r10
	lea     rax, [rdx+rax*2]
	neg     rax
    EXIT_PROC reg_save_list
	
	xalign  16
.4:	mov     r8, [rsi+rcx*8]
	mov     r9, [rsi+rcx*8+8]
	add     rax, rax
	adc     r8, r8
	adc     r9, r9
	sbb     rax, rax
	add     rdx, rdx
	adc     r8, r8
	adc     r9, r9
	sbb     rdx, rdx
	mov     [rdi+rcx*8], r8
	mov     [rdi+rcx*8+8], r9
	lea     rax, [rdx+rax*2]
	neg     rax
    EXIT_PROC reg_save_list
	
	xalign  16
.5:	mov     r8, [rsi+rcx*8]
	add     rax, rax
	adc     r8, r8
	sbb     rax, rax
	add     rdx, rdx
	adc     r8, r8
	sbb     rdx, rdx
	mov     [rdi+rcx*8], r8
.6:	lea     rax, [rdx+rax*2]
	neg     rax
    END_PROC reg_save_list

    end
