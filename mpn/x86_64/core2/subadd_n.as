
;  AMD64 mpn_subadd_n
;  Copyright 2009 Jason Moxham
;  This file is part of the MPIR Library.
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

;	INPUT PARAMETERS
;	rp	rdi
;	xp	rsi
;	yp	rdx
;	zp	rcx
;	n	r8

;	(rdi, r8) = (rsi, r8) - ( (rdx, r8) + (rcx, r8) )
;	return carry+borrow

;	or you can think of it as

;	(rdi, r8) = (rsi, r8) - (rdx, r8) - (rcx, r8) 
;	return borrow+borrow

%include 'yasm_mac.inc'

    BITS    64

   GLOBAL_FUNC mpn_subadd_n
	lea     rsi, [rsi+r8*8]
	lea     rdx, [rdx+r8*8]
	lea     rdi, [rdi+r8*8]
	lea     rcx, [rcx+r8*8]
	neg     r8
	xor     rax, rax
	xor     r10, r10
	test    r8, 3
	jz      next
lp1:
	mov     r9, [rsi+r8*8]
	add     rax, 1
	sbb     r9, [rcx+r8*8]
	sbb     rax, rax
	add     r10, 1
	sbb     r9, [rdx+r8*8]
	sbb     r10, r10
	mov     [rdi+r8*8], r9
	add     r8, 1
	test    r8, 3
	jnz     lp1
next:
	cmp     r8, 0
	jz      end
	push    rbx
	push    rbp
	align   16
lp:
	add     rax, 1
	mov     r9,  [rsi+r8*8]
	mov     rbx, [rsi+r8*8+8]
	mov     rbp, [rsi+r8*8+16]
	mov     r11, [rsi+r8*8+24]
	sbb     r9,  [rcx+r8*8]
	sbb     rbx, [rcx+r8*8+8]
	sbb     rbp, [rcx+r8*8+16]
	sbb     r11, [rcx+r8*8+24]
	sbb     rax, rax
	add     r10, 1
	sbb     r9,  [rdx+r8*8]
	sbb     rbx, [rdx+r8*8+8]
	sbb     rbp, [rdx+r8*8+16]
	sbb     r11, [rdx+r8*8+24]
	mov     [rdi+r8*8], r9
	mov     [rdi+r8*8+8], rbx
	mov     [rdi+r8*8+16], rbp
	mov     [rdi+r8*8+24], r11
	sbb     r10, r10
	add     r8, 4
	jnz     lp
	pop     rbp
	pop     rbx
end:
	add     rax, r10
	neg	rax
	ret
