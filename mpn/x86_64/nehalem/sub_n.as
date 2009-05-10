
;  mpn_sub_n

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

;	ret mpn_sub_n(mp_ptr,mp_ptr,mp_ptr,mp_size_t)
;	(rdi,rcx)=(rsi,rcx)-(rdx,rcx) return rax=borrow

	GLOBAL_FUNC mpn_sub_n
	mov     rax, rcx
	and     rax, 3
	sub     rcx, rax
	lea     rdi, [rdi+rcx*8]
	lea     rsi, [rsi+rcx*8]
	lea     rdx, [rdx+rcx*8]
	neg     rcx
	cmp     rcx, 0
	jz      L_skiplp
	align   16
L_lp:
	mov     r8, [rsi+rcx*8]
	mov     r10, [rsi+rcx*8+16]
	sbb     r8, [rdx+rcx*8]
	mov     [rdi+rcx*8], r8
	mov     r9, [rsi+rcx*8+8]
	sbb     r9, [rdx+rcx*8+8]
	mov     [rdi+rcx*8+8], r9
	lea     rcx, [rcx+4]
	mov     r11, [rsi+rcx*8-8]
	sbb     r10, [rdx+rcx*8-16]
	sbb     r11, [rdx+rcx*8-8]
	mov     [rdi+rcx*8-16], r10
	mov     [rdi+rcx*8-8], r11
	jrcxz   L_exitlp
	jmp     L_lp
L_exitlp:
	sbb     rcx, rcx
L_skiplp:
	cmp     rax, 2
	ja      L_case3
	jz      L_case2
	jp      L_case1
L_case0:
	sub     rax, rcx
	ret
	align   16
L_case1:
	add     rcx, rcx
	mov     r8, [rsi]
	sbb     r8, [rdx]
	mov     [rdi], r8
	sbb     rax, rax
	neg     rax
	ret
	align   16
L_case3:
	add     rcx, rcx
	mov     r8, [rsi]
	mov     r10, [rsi+16]
	sbb     r8, [rdx]
	mov     [rdi], r8
	mov     r9, [rsi+8]
	sbb     r9, [rdx+8]
	mov     [rdi+8], r9
	sbb     r10, [rdx+16]
	mov     [rdi+16], r10
	sbb     rax, rax
	neg     rax
	ret
	align   16
L_case2:
	add     rcx, rcx
	mov     r8, [rsi]
	sbb     r8, [rdx]
	mov     [rdi], r8
	mov     r9, [rsi+8]
	sbb     r9, [rdx+8]
	mov     [rdi+8], r9
	sbb     rax, rax
	neg     rax
	ret
	end
