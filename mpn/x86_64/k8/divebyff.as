
;  mpn_divexact_byff

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

;	ret mpn_divexact_byff(mp_ptr,mp_ptr,mp_size_t)

	GLOBAL_FUNC mpn_divexact_byff
	xor     eax, eax
	mov     rcx, rdx
	and     rdx, 3
	shr     rcx, 2
	cmp     rcx, 0
	je      L_skiplp
; want carry clear here
	align   16
L_lp:
	sbb     rax, [rsi]
	lea     rdi, [rdi+32]
	mov     r8, rax
	sbb     rax, [rsi+8]
	mov     r9, rax
	sbb     rax, [rsi+16]
	mov     r10, rax
	sbb     rax, [rsi+24]
	dec     rcx
	mov     [rdi-32], r8
	mov     [rdi-24], r9
	mov     [rdi-16], r10
	mov     [rdi-8], rax
	lea     rsi, [rsi+32]
	jnz     L_lp
L_skiplp:
; dont want to change the carry
	inc     rdx
	dec     rdx
	jz      L_end
	sbb     rax, [rsi]
	mov     [rdi], rax
	dec     rdx
	jz      L_end
	sbb     rax, [rsi+8]
	mov     [rdi+8], rax
	dec     rdx
	jz      L_end
	sbb     rax, [rsi+16]
	mov     [rdi+16], rax
L_end:
	sbb     rax, 0
	ret
	end
