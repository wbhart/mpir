;  AMD64 mpn_sub_n -- Subtract two limb vectors of the same length > 0 and
;  store difference in a third limb vector.
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
; Hammer:     3.0
;
;
; INPUT PARAMETERS
; rp	rdi
; up	rsi
; vp	rdx
; n	rcx

%include 'yasm_mac.inc'

    BITS    64
    
GLOBAL_FUNC mpn_sub_n
	lea	rsi, [rsi+rcx*8]
	lea	rdi, [rdi+rcx*8]
	lea	rdx, [rdx+rcx*8]
	neg	rcx
	xor	eax, eax		               ; clear cy

	align 4			                   ; minimal alignment for claimed speed
loop1:	                               ; do the subtraction in a loop
    mov	rax, [rsi+rcx*8]
	mov	r10, [rdx+rcx*8]
	sbb	rax, r10
	mov	[rdi+rcx*8], rax
	inc	rcx
	jne	loop1

	mov	rax, rcx		               ; zero rax
	adc	rax, rax                       ; return borrow
	ret
