;  X86_64 mpn_divrem_euclidean_1

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

; (rdi,rdx)=(rsi,rdx) / rcx 
; return rax=remainder

	GLOBAL_FUNC mpn_divrem_euclidean_qr_1
push    r15
push    r14
push    r13
push    r12
push    rbp
mov     r14, rdx
mov     r8, rcx
xor     r15, r15
bsr     rcx, r8
xor     rcx, 63
shl     r8, cl
xor     r11, r11
mov     rdx, r8
not     rdx
xor     rax, rax
not     rax
div     r8
mov     r9, rax
xor     rax, rax
xor     rbp, rbp
align 16
lp:
	mov     r13, [rsi+r14*8-8]
	mov     r12, r13
	neg     rcx
	cmovnc  r13, r15
	shr     r13, cl
	neg     rcx
	shl     r12, cl
	mov     r10, r12
	sar     r10, 63
	add     rax, r13
	sub     rax, r10
	add     rax, r11
	add     r11, r13
	add     r11, rbp
	mul     r9
	mov     r13, r11
	and     r10, r8
	add     r10, r12
	add     rax, r10
	mov     r10, r11
	adc     r10, rdx
	not     r10
	mov     rax, r8
	mul     r10
	sub     r13, r8
	add     rax, r12
	mov     r11, r8
	adc     rdx, r13
	and     r11, rdx
	mov     rbp, rax
	sub     rdx, r10
	mov     [rdi+r14*8-8], rdx
	dec     r14
	jnz     lp
pop     rbp
pop     r12
pop     r13
pop     r14
pop     r15
add     r11, rax
shr     r11, cl
mov     rax, r11
ret
end
