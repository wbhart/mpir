
;  core2 mpn_sublsh1_n 
;  Copyright 2008 Jason Moxham
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

;	(rdi, rcx) = (rsi, rcx) - (rdx, rcx)<<1
;	rax = borrow
	
    BITS 64
    
   GLOBAL_FUNC mpn_sublsh1_n
	lea     rsi, [rsi+rcx*8]
	lea     rdx, [rdx+rcx*8]
	lea     rdi, [rdi+rcx*8]
	neg     rcx
	xor     r9, r9
	xor     rax, rax
	test    rcx, 3
	jz      next
lp1:
	mov     r10, [rsi+rcx*8]
	add     r9, 1
	sbb     r10, [rdx+rcx*8]
	sbb     r9, r9
	add     rax, 1
	sbb     r10, [rdx+rcx*8]
	sbb     rax, rax
	mov     [rdi+rcx*8], r10
	add     rcx, 1
	test    rcx, 3
	jnz     lp1
next:
	cmp     rcx, 0
	jz      end
	push    r15
	push    r14
	push    r13
	push    r12
	push    rbx
	align 16
lp:
	mov     r10, [rsi+rcx*8]
	mov     rbx, [rsi+rcx*8+8]
	mov     r11, [rsi+rcx*8+16]
	mov     r8, [rsi+rcx*8+24]
	mov     r12, [rdx+rcx*8]
	mov     r13, [rdx+rcx*8+8]
	mov     r14, [rdx+rcx*8+16]
	mov     r15, [rdx+rcx*8+24]
	add     r9, 1
	sbb     r10, r12
	sbb     rbx, r13
	sbb     r11, r14
	sbb     r8, r15
	sbb     r9, r9
	add     rax, 1
	sbb     r10, r12
	sbb     rbx, r13
	sbb     r11, r14
	sbb     r8, r15
	sbb     rax, rax
	mov     [rdi+rcx*8], r10
	mov     [rdi+rcx*8+8], rbx
	mov     [rdi+rcx*8+16], r11
	mov     [rdi+rcx*8+24], r8
	add     rcx, 4
	jnz     lp
	pop     rbx
	pop     r12
	pop     r13
	pop     r14
	pop     r15
end:
	add     rax, r9
	neg     rax
	ret
