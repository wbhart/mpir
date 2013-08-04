; PROLOGUE(mpn_addlsh1_n)

;  Copyright 2008 Jason Moxham
;
;  This file is part of the MPIR Library.
;
;  Windows Conversion Copyright 2008 Brian Gladman
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
;  mp_limb_t mpn_addlsh1_n(mp_ptr, mp_ptr, mp_ptr, mp_size_t)
;  rax                        rdi     rsi     rdx        rcx
;  rax                        rcx     rdx      r8         r9
	
%include "yasm_mac.inc"

%define reg_save_list rbx, rsi, rdi

    CPU  Athlon64
    BITS 64

    FRAME_PROC mpn_addlsh1_n, 0, reg_save_list
    mov     rax, r9
	lea     rdi, [rcx+rax*8]
	lea     rsi, [rdx+rax*8]
	lea     rdx, [r8+rax*8]
    mov     rcx, rax
	neg     rcx
	xor     r9, r9
	xor     rax, rax
	test    rcx, 3
	jz      .2
.1:	mov     r10, [rdx+rcx*8]
	add     r9, 1
	adc     r10, r10
	sbb     r9, r9
	add     rax, 1
	adc     r10, [rsi+rcx*8]
	sbb     rax, rax
	mov     [rdi+rcx*8], r10
	inc     rcx
	test    rcx, 3
	jnz     .1
.2:	cmp     rcx, 0
	jz      .4
	
	xalign  16
.3:	mov     r10, [rdx+rcx*8]
	mov     rbx, [rdx+rcx*8+8]
	mov     r11, [rdx+rcx*8+16]
	mov     r8, [rdx+rcx*8+24]
	add     r9, 1
	adc     r10, r10
	adc     rbx, rbx
	adc     r11, r11
	adc     r8, r8
	sbb     r9, r9
	add     rax, 1
	adc     r10, [rsi+rcx*8]
	adc     rbx, [rsi+rcx*8+8]
	adc     r11, [rsi+rcx*8+16]
	adc     r8, [rsi+rcx*8+24]
	sbb     rax, rax
	mov     [rdi+rcx*8], r10
	mov     [rdi+rcx*8+8], rbx
	mov     [rdi+rcx*8+16], r11
	mov     [rdi+rcx*8+24], r8
	add     rcx, 4
	jnz     .3
.4:	add     rax, r9
	neg     rax
    END_PROC reg_save_list
    
    end
