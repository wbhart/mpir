;  X86_64 mpn_diveby (B-1)/f   where f=1  special case

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

%include "yasm_mac.inc"

;	(rdi,rdx)=(rsi,rdx)/rcx where r8=(B-1)/rcx
;	rax=carry out

;	special case rcx=ffff  r8=1

;	The two imul's are only needed if want strict compatibility with
;	mpn_divexact_1 when the division is not exact

	GLOBAL_FUNC mpn_divexact_byff

mov     r10d, 3
lea     rsi, [rsi+rdx*8-24]
lea     rdi, [rdi+rdx*8-24]
; r9 is our carry in
mov     r9, 0
mov	r8, 1
mov	rcx,0xFFFFFFFFFFFFFFFF
; imul %r8,%r9 this is needed if we have non-zero carry in
sub     r10, rdx
jnc     skiploop
align 16
lp:
	mov     rax, [rsi+r10*8]
	mul     r8
	sub     r9, rax
	mov     [rdi+r10*8], r9
	sbb     r9, rdx
	mov     rax, [rsi+r10*8+8]
	mul     r8
	sub     r9, rax
	mov     [rdi+r10*8+8], r9
	sbb     r9, rdx
	mov     rax, [rsi+r10*8+16]
	mul     r8
	sub     r9, rax
	mov     [rdi+r10*8+16], r9
	sbb     r9, rdx
	mov     rax, [rsi+r10*8+24]
	mul     r8
	sub     r9, rax
	mov     [rdi+r10*8+24], r9
	sbb     r9, rdx
	add     r10, 4
	jnc     lp
skiploop:
test    r10, 2
jnz     skip
	mov     rax, [rsi+r10*8]
	mul     r8
	sub     r9, rax
	mov     [rdi+r10*8], r9
	sbb     r9, rdx
	mov     rax, [rsi+r10*8+8]
	mul     r8
	sub     r9, rax
	mov     [rdi+r10*8+8], r9
	sbb     r9, rdx
	add     r10, 2
skip:
test    r10, 1
jnz     fin
	mov     rax, [rsi+r10*8]
	mul     r8
	sub     r9, rax
	mov     [rdi+r10*8], r9
	sbb     r9, rdx
fin:
imul    r9, rcx
mov     rax, r9
neg     rax
ret
end
