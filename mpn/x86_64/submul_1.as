;  AMD64 mpn_submul_1 -- Multiply a limb vector with a limb and subtract the
;  result from a second limb vector.
;
;  Copyright 2004 Free Software Foundation, Inc.
;
;  Copyright 2008 William Hart
;
;  This file is part of the MPIR Library.
;
;  The MPIR Library is free software; you can redistribute it and/or
;  modify it under the terms of the GNU Lesser General Public License as
;  published by the Free Software Foundation; either version 2.1 of the
;  License, or (at your option) any later version.
;
;  The MPIR Library is distributed in the hope that it will be useful,
;  but WITHOUT ANY WARRANTY; without even the implied warranty of
;  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
;  Lesser General Public License for more details.
;
;  You should have received a copy of the GNU Lesser General Public
;  License along with the MPIR Library; see the file COPYING.LIB.  If
;  not, write to the Free Software Foundation, Inc., 51 Franklin Street,
;  Fifth Floor, Boston, MA 02110-1301, USA.
;
;         cycles/limb
; Hammer:     4.7
;
;
; INPUT PARAMETERS
; rp	rdi
; up	rsi
; n	    rdx
; vl	rcx

%include 'yasm_mac.inc'

    BITS    64
    
GLOBAL_FUNC mpn_submul_1
	mov	r11, rdx
	lea	rsi, [rsi+r11*8]
	lea	rdi, [rdi+r11*8]
	neg	r11
	xor	r8d, r8d

	align 8                 		; minimal alignment for claimed speed
loop1:	
    mov	rax, [rsi+r11*8]
	mov	r10, [rdi+r11*8]            ; load limb from rp
	mul	rcx                         ; multiply
	sub	r10, r8                     ; subtract any previous high limb (stored in r8)
	mov	r8d, 0                      ; clear r8 again
	adc	r8d, r8d                    ; put any borrow from this iteration in r8
	sub	r10, rax                    ; subtract low limb of product
	adc	r8, rdx                     ; add high limb and any borrow to r8 for next iteration
	mov	[rdi+r11*8], r10            ; store low limb
	inc	r11
	jne	loop1

	mov	rax, r8
	ret
