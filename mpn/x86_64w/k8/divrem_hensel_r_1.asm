; PROLOGUE(mpn_divrem_hensel_r_1)

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
;  mp_limb_t  mpn_divrem_hensel_r_1(mp_ptr, mp_size_t, mp_limb_t)
;  rax                                 rdi        rsi        rdx
;  rax                                 rcx        rdx         r8

%include "yasm_mac.inc"

    CPU  Athlon64
    BITS 64

	LEAF_PROC mpn_divrem_hensel_r_1
    movsxd  rax, edx
	mov     rdx, r8
	lea     r10, [rcx+rax*8]
	mov     r9, 0
	sub     r9, rax

    mov     rcx, rdx    
	mov     rax, rdx
	imul    edx, ecx
	mov     r11, 2
	sub     r11, rdx
	imul    r11d, eax

	mov     rax, r11
	imul    r11d, ecx
	mov     rdx, 2
	sub     rdx, r11
	imul    edx, eax

	mov     rax, rdx
	imul    edx, ecx
	mov     r11, 2
	sub     r11, rdx
	imul    r11d, eax

	mov     rax, r11
	imul    r11d, ecx
	mov     rdx, 2
	sub     rdx, r11
	imul    edx, eax

	mov     rax, rdx
	imul    rdx, rcx
	mov     r11, 2
	sub     r11, rdx
	imul    r11, rax
	xor     rdx, rdx

	xalign  16
.1:	mov     rax, [r10+r9*8]
	sbb     rax, rdx
	sbb     r8, r8
	imul    rax, r11
	mul     rcx
	add     r8, 1
	inc     r9
	jnz     .1
	mov     rax, 0
	adc     rax, rdx
	ret
	
    end
