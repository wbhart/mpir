
;  AMD64 mpn_rsh1sub_n

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

;	(rdi,rcx)=((rsi,rcx)-(rdx,rcx))/2
; return bottom bit of difference
; subtraction treated as two compliment

	GLOBAL_FUNC mpn_rsh1sub_n
	lea     rdi, [rdi+rcx*8-32]
	lea     rsi, [rsi+rcx*8-32]
	lea     rdx, [rdx+rcx*8-32]
	push    r12
	push    rbx
	mov     r8, 4
	sub     r8, rcx
	mov     r12, [rsi+r8*8]
	sub     r12, [rdx+r8*8]
	sbb     rbx, rbx
	mov     rax, r12
	and     rax, 1
	cmp     r8, 0
	jge     L_skiplp
L_lp:
	mov     r11, [rsi+r8*8+32]
	mov     r10, [rsi+r8*8+24]
	add     rbx, 1
	mov     rcx, [rsi+r8*8+8]
	mov     r9, [rsi+r8*8+16]
	sbb     rcx, [rdx+r8*8+8]
	sbb     r9, [rdx+r8*8+16]
	sbb     r10, [rdx+r8*8+24]
	sbb     r11, [rdx+r8*8+32]
	sbb     rbx, rbx
	bt      r11, 0
	rcr     r10, 1
	rcr     r9, 1
	rcr     rcx, 1
	rcr     r12, 1
	mov     [rdi+r8*8], r12
	mov     [rdi+r8*8+8], rcx
	mov     [rdi+r8*8+16], r9
	mov     [rdi+r8*8+24], r10
	mov     r12, r11
	add     r8, 4
	jnc     L_lp
L_skiplp:
	cmp     r8, 2
	ja      L_case0
	jz      L_case1
	jp      L_case2
L_case3:
	mov     r10, [rsi+r8*8+24]
	add     rbx, 1
	mov     rcx, [rsi+r8*8+8]
	mov     r9, [rsi+r8*8+16]
	sbb     rcx, [rdx+r8*8+8]
	sbb     r9, [rdx+r8*8+16]
	sbb     r10, [rdx+r8*8+24]
	rcr     r10, 1
	rcr     r9, 1
	rcr     rcx, 1
	rcr     r12, 1
	mov     [rdi+r8*8], r12
	mov     [rdi+r8*8+8], rcx
	mov     [rdi+r8*8+16], r9
	mov     [rdi+r8*8+24], r10
	pop     rbx
	pop     r12
	ret
	align   16
L_case2:
	add     rbx, 1
	mov     rcx, [rsi+r8*8+8]
	mov     r9, [rsi+r8*8+16]
	sbb     rcx, [rdx+r8*8+8]
	sbb     r9, [rdx+r8*8+16]
	rcr     r9, 1
	rcr     rcx, 1
	rcr     r12, 1
	mov     [rdi+r8*8], r12
	mov     [rdi+r8*8+8], rcx
	mov     [rdi+r8*8+16], r9
	pop     rbx
	pop     r12
	ret
	align   16
L_case1:
	add     rbx, 1
	mov     rcx, [rsi+r8*8+8]
	sbb     rcx, [rdx+r8*8+8]
	rcr     rcx, 1
	rcr     r12, 1
	mov     [rdi+r8*8], r12
	mov     [rdi+r8*8+8], rcx
	pop     rbx
	pop     r12
	ret
	align   16
L_case0:
	add     rbx, 1
	rcr     r12, 1
	mov     [rdi+r8*8], r12
	pop     rbx
	pop     r12
	ret
	end
