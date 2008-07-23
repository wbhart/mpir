;  AMD64 mpn_mul_1 -- Multiply a limb vector with a limb and store the result
;  in a second limb vector.
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
; Hammer:     3.0
;
;
; INPUT PARAMETERS
; rp	rdi
; up	rsi
; n	    rdx
; vl	rcx

%include '../yasm_mac.inc'

    BITS    64
 
    align 16
    db 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0

GLOBAL_FUNC mpn_mul_1
	mov	  r11, rdx
	lea	  rsi, [rsi+rdx*8]
	lea	  rdi, [rdi+rdx*8]
	neg	  r11
	xor	  r8d, r8d

label0:	
    mov	  rax, [rsi+r11*8]
	mul	  rcx
	add	  rax, r8
	mov	  r8d, 0
	adc	  r8, rdx
	mov	  [rdi+r11*8], rax
	inc	  r11
	jne	  label0

	mov   rax, r8
	ret
