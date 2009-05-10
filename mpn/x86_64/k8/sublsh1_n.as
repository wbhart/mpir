
;  mpn_sublsh1_n

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

;	ret mpn_sublsh1_n(mp_ptr,mp_ptr,mp_ptr,mp_size_t)

	GLOBAL_FUNC mpn_sublsh1_n
	push    rbx
	lea     rdi, [rdi+rcx*8-56]
	lea     rsi, [rsi+rcx*8-56]
	lea     rdx, [rdx+rcx*8-56]
	xor     rax, rax
	xor     r10, r10
	mov     r8, 3
	sub     r8, rcx
	jge     L_skip
	add     r8, 4
	mov     r11, [rsi+r8*8+24]
	mov     rcx, [rsi+r8*8+16]
	mov     r9, [rsi+r8*8]
	mov     rbx, [rsi+r8*8+8]
	jc      L_skiplp
	align   16
L_lp:
	add     rax, 1
	sbb     r9, [rdx+r8*8]
	sbb     rbx, [rdx+r8*8+8]
	sbb     rcx, [rdx+r8*8+16]
	sbb     r11, [rdx+r8*8+24]
	sbb     rax, rax
	add     r10, 1
	sbb     r9, [rdx+r8*8]
	sbb     rbx, [rdx+r8*8+8]
	sbb     rcx, [rdx+r8*8+16]
	sbb     r11, [rdx+r8*8+24]
	sbb     r10, r10
	mov     [rdi+r8*8], r9
	mov     [rdi+r8*8+8], rbx
	mov     [rdi+r8*8+16], rcx
	mov     [rdi+r8*8+24], r11
	mov     r11, [rsi+r8*8+56]
	mov     rcx, [rsi+r8*8+48]
	add     r8, 4
	mov     r9, [rsi+r8*8]
	mov     rbx, [rsi+r8*8+8]
	jnc     L_lp
L_skiplp:
	add     rax, 1
	sbb     r9, [rdx+r8*8]
	sbb     rbx, [rdx+r8*8+8]
	sbb     rcx, [rdx+r8*8+16]
	sbb     r11, [rdx+r8*8+24]
	sbb     rax, rax
	add     r10, 1
	sbb     r9, [rdx+r8*8]
	sbb     rbx, [rdx+r8*8+8]
	sbb     rcx, [rdx+r8*8+16]
	sbb     r11, [rdx+r8*8+24]
	sbb     r10, r10
	mov     [rdi+r8*8], r9
	mov     [rdi+r8*8+8], rbx
	mov     [rdi+r8*8+16], rcx
	mov     [rdi+r8*8+24], r11
L_skip:
	cmp     r8, 2
	ja      L_case0
	jz      L_case1
	jp      L_case2
L_case3:
	mov     rcx, [rsi+r8*8+48]
	mov     r9, [rsi+r8*8+32]
	mov     rbx, [rsi+r8*8+40]
	add     rax, 1
	sbb     r9, [rdx+r8*8+32]
	sbb     rbx, [rdx+r8*8+40]
	sbb     rcx, [rdx+r8*8+48]
	sbb     rax, rax
	add     r10, 1
	sbb     r9, [rdx+r8*8+32]
	sbb     rbx, [rdx+r8*8+40]
	sbb     rcx, [rdx+r8*8+48]
	mov     [rdi+r8*8+32], r9
	mov     [rdi+r8*8+40], rbx
	mov     [rdi+r8*8+48], rcx
	sbb     rax, 0
	neg     rax
	pop     rbx
	ret
	align   16
L_case2:
	mov     r9, [rsi+r8*8+32]
	mov     rbx, [rsi+r8*8+40]
	add     rax, 1
	sbb     r9, [rdx+r8*8+32]
	sbb     rbx, [rdx+r8*8+40]
	sbb     rax, rax
	add     r10, 1
	sbb     r9, [rdx+r8*8+32]
	sbb     rbx, [rdx+r8*8+40]
	mov     [rdi+r8*8+32], r9
	mov     [rdi+r8*8+40], rbx
	sbb     rax, 0
	neg     rax
	pop     rbx
	ret
	align   16
L_case1:
	mov     r9, [rsi+r8*8+32]
	add     rax, 1
	sbb     r9, [rdx+r8*8+32]
	sbb     rax, rax
	add     r10, 1
	sbb     r9, [rdx+r8*8+32]
	mov     [rdi+r8*8+32], r9
	sbb     rax, 0
	neg     rax
	pop     rbx
	ret
	align   16
L_case0:
	add     r10, 1
	sbb     rax, 0
	neg     rax
	pop     rbx
	ret
	end
