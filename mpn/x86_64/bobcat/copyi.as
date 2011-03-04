
;  mpn_copyi

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

;	mpn_copyi(mp_ptr rdi ,mp_ptr rsi ,mp_size_t rdx)
;	(rdi,rdx)=(rdi,rdx)

	GLOBAL_FUNC mpn_copyi
	sub     rdx, 4
	jl      L_skiplp
	align   16
L_lp:
	mov     rax, [rsi]
	mov     rcx, [rsi+8]
	sub     rdx, 4
	mov     r8, [rsi+16]
	mov     r9, [rsi+24]
	lea     rdi, [rdi+32]
	mov     [rdi-32], rax
	mov     [rdi-24], rcx
	mov     [rdi-16], r8
	lea     rsi, [rsi+32]
	mov     [rdi-8], r9
	jns     L_lp
L_skiplp:
	add     rdx, 2
	jz      L_case2
	jns     L_case3
	jp      L_case1
L_case0:
	ret
	align   16
L_case1:
	mov     rax, [rsi]
	mov     [rdi], rax
	ret
	align   16
L_case2:
	mov     rax, [rsi]
	mov     rcx, [rsi+8]
	mov     [rdi], rax
	mov     [rdi+8], rcx
	ret
	align   16
L_case3:
	mov     rax, [rsi]
	mov     rcx, [rsi+8]
	mov     r8, [rsi+16]
	mov     [rdi], rax
	mov     [rdi+8], rcx
	mov     [rdi+16], r8
	ret
	end
