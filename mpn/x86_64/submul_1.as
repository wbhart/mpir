;  AMD64 mpn_submul_1 -- Multiply a limb vector with a limb and subtract the
;  result from a second limb vector.
;
;  Copyright 2004 Free Software Foundation, Inc.
;
;  This file is part of the GNU MP Library.
;
;  The GNU MP Library is free software; you can redistribute it and/or modify
;  it under the terms of the GNU Lesser General Public License as published
;  by the Free Software Foundation; either version 2.1 of the License, or (at
;  your option) any later version.
;
;  The GNU MP Library is distributed in the hope that it will be useful, but
;  WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
;  or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public
;  License for more details.
;
;  You should have received a copy of the GNU Lesser General Public License
;  along with the GNU MP Library; see the file COPYING.LIB.  If not, write
;  to the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
;  Boston, MA 02110-1301, USA.
;
;         cycles/limb
; Hammer:     4.7
;
;
; INPUT PARAMETERS
; rp	rdi
; up	rsi
; n	rdx
; vl	rcx

%include '../yasm_mac.inc'

    bits    64
    section .text

    G_EXPORT __gmpn_submul_1
 
G_LABEL __gmpn_submul_1
	mov	r11, rdx
	lea	rsi, [rsi+r11*8]
	lea	rdi, [rdi+r11*8]
	neg	r11
	xor	r8d, r8d

	align 8                 		; minimal alignment for claimed speed
.loop:	
      mov	rax, [rsi+r11*8]
	mov	r10, [rdi+r11*8]
	mul	rcx
	sub	r10, r8
	mov	r8d, 0
	adc	r8d, r8d
	sub	r10, rax
	adc	r8, rdx
	mov	[rdi+r11*8], r10
	inc	r11
	jne	.loop

	mov	rax, r8
	ret

    end