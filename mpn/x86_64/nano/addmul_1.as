;  AMD64 mpn_addmul_1 -- Multiply a limb vector with a limb and add the
;  result to a second limb vector.
;
;  Copyright 2004 Free Software Foundation, Inc.
;
;  Copyright 2008 William Hart
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
;         cycles/limb
; Hammer:     4.0
;
;
; INPUT PARAMETERS
; rp	rdi
; up	rsi
; n	    rdx
; vl	rcx

%include 'yasm_mac.inc'

    BITS    64
GLOBAL_FUNC mpn_addmul_1
	mov	r11, rdx
	lea	rsi, [rsi+rdx*8]
	lea	rdi, [rdi+rdx*8]
	neg	r11
	xor	r8d, r8d
	xor	r10d, r10d

	align 4       		          ; minimal alignment for claimed speed
loop1:	
    mov	rax, [rsi+r11*8]
	mul	rcx                       ; multiply by vl
	add	rax, [rdi+r11*8]          ; add to limb from rp[]
	adc	rdx, r10                  ; add any carry to high limb
	add	rax, r8                   ; add any previous limb (stored in r8) to low limb
	mov	r8, r10                   ; zero r8 again
	mov	[rdi+r11*8], rax          ; store resulting low limb
	adc	r8, rdx                   ; set r8 to high limb plus carry from this iteration
	inc	r11
	jne	loop1

	mov	rax, r8
	ret
