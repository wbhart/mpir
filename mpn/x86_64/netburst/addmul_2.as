;  k8 mpn_addmul_2

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

	GLOBAL_FUNC mpn_addmul_2

; (rdi,rdx+1) = (rdi,rdx) + (rsi,rdx)*(rcx,2) return carrylimb

push    rbx
push    r12
mov     r8, [rcx+8]
mov     rcx, [rcx]
mov     rbx, 4
sub     rbx, rdx
lea     rsi, [rsi+rdx*8-32]
lea     rdi, [rdi+rdx*8-32]
mov     r10, 0
mov     rax, [rsi+rbx*8]
mul     rcx
mov     r12, rax
mov     r9, rdx
cmp     rbx, 0
jge     skiplp
align   16
lp:
	mov     rax, [rsi+rbx*8]
	mul     r8
	add     r9, rax
	mov     rax, [rsi+rbx*8+8]
	adc     r10, rdx
	mov     r11, 0
	mul     rcx
	add     [rdi+rbx*8], r12
	adc     r9, rax
	mov     r12, 0
	adc     r10, rdx
	mov     rax, [rsi+rbx*8+8]
	adc     r11, 0
	mul     r8
	add     [rdi+rbx*8+8], r9
	adc     r10, rax
	adc     r11, rdx
	mov     rax, [rsi+rbx*8+16]
	mul     rcx
	add     r10, rax
	mov     rax, [rsi+rbx*8+16]
	adc     r11, rdx
	adc     r12, 0
	mul     r8
	add     [rdi+rbx*8+16], r10
	mov     r9, 0
	adc     r11, rax
	mov     r10, 0
	mov     rax, [rsi+rbx*8+24]
	adc     r12, rdx
	mov     r15, r15
	mul     rcx
	add     r11, rax
	mov     rax, [rsi+rbx*8+24]
	adc     r12, rdx
	adc     r9, 0
	mul     r8
	add     [rdi+rbx*8+24], r11
	adc     r12, rax
	adc     r9, rdx
	mov     rax, [rsi+rbx*8+32]
	mul     rcx
	add     r12, rax
	adc     r9, rdx
	adc     r10, 0
	add     rbx, 4
	jnc     lp
skiplp:
mov     rax, [rsi+rbx*8]
mul     r8
cmp     rbx, 2
ja      case0
jz      case1
jp      case2
case3:
	add     r9, rax
	mov     rax, [rsi+rbx*8+8]
	adc     r10, rdx
	mov     r11, 0
	mul     rcx
	add     [rdi+rbx*8], r12
	adc     r9, rax
	mov     r12, 0
	adc     r10, rdx
	mov     rax, [rsi+rbx*8+8]
	adc     r11, 0
	mul     r8
	add     [rdi+rbx*8+8], r9
	adc     r10, rax
	adc     r11, rdx
	mov     rax, [rsi+rbx*8+16]
	mul     rcx
	add     r10, rax
	mov     rax, [rsi+rbx*8+16]
	adc     r11, rdx
	adc     r12, 0
	mul     r8
	add     [rdi+rbx*8+16], r10
	mov     r9, 0
	adc     r11, rax
	; padding
	mov     r10, 0
	mov     rax, [rsi+rbx*8+24]
	adc     r12, rdx
	; padding
	mov     r15, r15
	mul     rcx
	add     r11, rax
	mov     rax, [rsi+rbx*8+24]
	adc     r12, rdx
	adc     r9, 0
	mul     r8
	add     [rdi+rbx*8+24], r11
	adc     r12, rax
	adc     r9, rdx
	mov     [rdi+rbx*8+32], r12
	mov     rax, r9
	pop     r12
	pop     rbx
	ret
align   16
case2:
	add     r9, rax
	mov     rax, [rsi+rbx*8+8]
	adc     r10, rdx
	mov     r11, 0
	mul     rcx
	add     [rdi+rbx*8], r12
	adc     r9, rax
	mov     r12, 0
	adc     r10, rdx
	mov     rax, [rsi+rbx*8+8]
	adc     r11, 0
	mul     r8
	add     [rdi+rbx*8+8], r9
	adc     r10, rax
	adc     r11, rdx
	mov     rax, [rsi+rbx*8+16]
	mul     rcx
	add     r10, rax
	mov     rax, [rsi+rbx*8+16]
	adc     r11, rdx
	adc     r12, 0
	mul     r8
	add     [rdi+rbx*8+16], r10
	adc     r11, rax
	adc     r12, rdx
	mov     [rdi+rbx*8+24], r11
	mov     rax, r12
	pop     r12
	pop     rbx
	ret
align   16
case1:
	add     r9, rax
	mov     rax, [rsi+rbx*8+8]
	adc     r10, rdx
	mov     r11, 0
	mul     rcx
	add     [rdi+rbx*8], r12
	adc     r9, rax
	mov     r12, 0
	adc     r10, rdx
	mov     rax, [rsi+rbx*8+8]
	adc     r11, 0
	mul     r8
	add     [rdi+rbx*8+8], r9
	adc     r10, rax
	adc     r11, rdx
	mov     [rdi+rbx*8+16], r10
	mov     rax, r11
	pop     r12
	pop     rbx
	ret
align   16
case0:
	add     [rdi+rbx*8], r12
	adc     r9, rax
	adc     r10, rdx
	mov     [rdi+rbx*8+8], r9
	mov     rax, r10
	pop     r12
	pop     rbx
	ret
	end
