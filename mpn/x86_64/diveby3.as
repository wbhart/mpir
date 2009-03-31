
;  AMD64 mpn_diveby3
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

;	(rdi, rdx) = (rsi, rdx)  rcx = carry in
;	rax = carry out
;	NOTE could pass 55555...555 as next param so this would
;	be mpn_divexact_by_ff_over_c , and change imul at end , or
;	drop backwards compatibilty and just dump the two imuls
	
%include 'yasm_mac.inc'

    BITS    64

    GLOBAL_FUNC mpn_divexact_by3c
	mov     r9d, 3
	lea     rsi, [rsi+rdx*8-24]
	lea     rdi, [rdi+rdx*8-24]
	mov     r8, 0x5555555555555555
	imul    rcx, r8
	sub     r9, rdx
	jnc     skiploop
	align   16
loop1:
	mov     rax, [rsi+r9*8]
	mul     r8
	sub     rcx, rax
	mov     [rdi+r9*8], rcx
	sbb     rcx, rdx
	mov     rax, [rsi+r9*8+8]
	mul     r8
	sub     rcx, rax
	mov     [rdi+r9*8+8], rcx
	sbb     rcx, rdx
	mov     rax, [rsi+r9*8+16]
	mul     r8
	sub     rcx, rax
	mov     [rdi+r9*8+16], rcx
	sbb     rcx, rdx
	mov     rax, [rsi+r9*8+24]
	mul     r8
	sub     rcx, rax
	mov     [rdi+r9*8+24], rcx
	sbb     rcx, rdx
	add     r9, 4
	jnc     loop1
skiploop:
	test    r9, 2
	jnz     skip
	mov     rax, [rsi+r9*8]
	mul     r8
	sub     rcx, rax
	mov     [rdi+r9*8], rcx
	sbb     rcx, rdx
	mov     rax, [rsi+r9*8+8]
	mul     r8
	sub     rcx, rax
	mov     [rdi+r9*8+8], rcx
	sbb     rcx, rdx
	add     r9, 2
skip:
	test    r9, 1
	jnz     end
	mov     rax, [rsi+r9*8]
	mul     r8
	sub     rcx, rax
	mov     [rdi+r9*8], rcx
	sbb     rcx, rdx
end:
	; below is the same as   imul    rax, rcx, -3
	lea     rax, [rcx+rcx*2]
	neg     rax
	ret
