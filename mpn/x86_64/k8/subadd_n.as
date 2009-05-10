
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

%include 'yasm_mac.inc'

;	(rdi,r8)=(rsi,r8)-(rdx,r8)-(rcx,r8) return borrow+borrow

	GLOBAL_FUNC mpn_subadd_n
	lea     rdx, [rdx+r8*8-56]
	lea     rcx, [rcx+r8*8-56]
	lea     rsi, [rsi+r8*8-56]
	lea     rdi, [rdi+r8*8-56]
	mov     r9, 3
	xor     rax, rax
	xor     r10, r10
	sub     r9, r8
	push    r12
	push    rbp
	jge     L_skip
	add     r9, 4
	mov     rbp, [rsi+r9*8+16]
	mov     r11, [rsi+r9*8+24]
	mov     r12, [rsi+r9*8]
	mov     r8, [rsi+r9*8+8]
	jc      L_skiplp
	align   16
L_lp:
	add     rax, 1
	sbb     r12, [rcx+r9*8]
	sbb     r8, [rcx+r9*8+8]
	sbb     rbp, [rcx+r9*8+16]
	sbb     r11, [rcx+r9*8+24]
	sbb     rax, rax
	add     r10, 1
	sbb     r12, [rdx+r9*8]
	sbb     r8, [rdx+r9*8+8]
	sbb     rbp, [rdx+r9*8+16]
	sbb     r11, [rdx+r9*8+24]
	sbb     r10, r10
	mov     [rdi+r9*8], r12
	mov     [rdi+r9*8+24], r11
	mov     [rdi+r9*8+8], r8
	mov     [rdi+r9*8+16], rbp
	mov     rbp, [rsi+r9*8+48]
	mov     r11, [rsi+r9*8+56]
	add     r9, 4
	mov     r12, [rsi+r9*8]
	mov     r8, [rsi+r9*8+8]
	jnc     L_lp
L_skiplp:
	add     rax, 1
	sbb     r12, [rcx+r9*8]
	sbb     r8, [rcx+r9*8+8]
	sbb     rbp, [rcx+r9*8+16]
	sbb     r11, [rcx+r9*8+24]
	sbb     rax, rax
	add     r10, 1
	sbb     r12, [rdx+r9*8]
	sbb     r8, [rdx+r9*8+8]
	sbb     rbp, [rdx+r9*8+16]
	sbb     r11, [rdx+r9*8+24]
	sbb     r10, r10
	mov     [rdi+r9*8], r12
	mov     [rdi+r9*8+24], r11
	mov     [rdi+r9*8+8], r8
	mov     [rdi+r9*8+16], rbp
L_skip:
	cmp     r9, 2
	ja      L_case0
	jz      L_case1
	jp      L_case2
L_case3:
	mov     rbp, [rsi+r9*8+48]
	mov     r12, [rsi+r9*8+32]
	mov     r8, [rsi+r9*8+40]
	add     rax, 1
	sbb     r12, [rcx+r9*8+32]
	sbb     r8, [rcx+r9*8+40]
	sbb     rbp, [rcx+r9*8+48]
	sbb     rax, rax
	add     r10, 1
	sbb     r12, [rdx+r9*8+32]
	sbb     r8, [rdx+r9*8+40]
	sbb     rbp, [rdx+r9*8+48]
	mov     [rdi+r9*8+32], r12
	mov     [rdi+r9*8+40], r8
	mov     [rdi+r9*8+48], rbp
	sbb     rax, 0
	neg     rax
	pop     rbp
	pop     r12
	ret
	align   16
L_case2:
	mov     r12, [rsi+r9*8+32]
	mov     r8, [rsi+r9*8+40]
	add     rax, 1
	sbb     r12, [rcx+r9*8+32]
	sbb     r8, [rcx+r9*8+40]
	sbb     rax, rax
	add     r10, 1
	sbb     r12, [rdx+r9*8+32]
	sbb     r8, [rdx+r9*8+40]
	mov     [rdi+r9*8+32], r12
	mov     [rdi+r9*8+40], r8
	sbb     rax, 0
	neg     rax
	pop     rbp
	pop     r12
	ret
	align   16
L_case1:
	mov     r12, [rsi+r9*8+32]
	add     rax, 1
	sbb     r12, [rcx+r9*8+32]
	sbb     rax, rax
	add     r10, 1
	sbb     r12, [rdx+r9*8+32]
	mov     [rdi+r9*8+32], r12
	sbb     rax, 0
	neg     rax
	pop     rbp
	pop     r12
	ret
	align   16
L_case0:
	add     rax, r10
	neg     rax
	pop     rbp
	pop     r12
	ret
	end
