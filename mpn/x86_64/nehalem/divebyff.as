
;  core2 mpn_divexact_byff
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

%include 'yasm_mac.inc'

;	(rdi, rdx) = (rsi, rdx)/0xFFFFFFFFFFFFFFFF
;	rax = "remainder"
;	where (rsi, rdx) = (rdi, rdx)*(B - 1) -rax*B^rdx    and 0 <= rax < B - 1      B = 0xFFFFFFFFFFFFFFFF

    BITS 64

   GLOBAL_FUNC mpn_divexact_byff
;	this is good but suffers from alignment slowdown
;	we dont seem to have much freedom to re-arrange the instructions to avoid
;	it , I suppose we could detect alignment at the start and have different
;	routines for different alignments
	xor     eax, eax
	mov     rcx, rdx
	and     rcx, 3
	shr     rdx, 2
	cmp     rdx, 0
;	carry flag is clear here
	jnz     loop1
	sbb     rax, [rsi]
	mov     [rdi], rax
	dec     rcx
	jz      end1
	sbb     rax, [rsi+8]
	mov     [rdi+8], rax
	dec     rcx
	jz      end1
	sbb     rax, [rsi+16]
	mov     [rdi+16], rax
	dec     rcx
end1:
	sbb     rax, 0
	ret
	align 16
loop1:
	sbb     rax, [rsi]
	mov     [rdi], rax
	sbb     rax, [rsi+8]
	mov     [rdi+8], rax
	sbb     rax, [rsi+16]
	mov     [rdi+16], rax
	sbb     rax, [rsi+24]
	mov     [rdi+24], rax
	lea     rsi, [rsi+32]
	dec     rdx
	lea     rdi, [rdi+32]
	jnz     loop1
	inc     rcx
	dec     rcx
	jz      end
	sbb     rax, [rsi]
	mov     [rdi], rax
	dec     rcx
	jz      end
	sbb     rax, [rsi+8]
	mov     [rdi+8], rax
	dec     rcx
	jz      end
	sbb     rax, [rsi+16]
	mov     [rdi+16], rax
	dec     rcx
end:
	sbb     rax, 0
	ret
