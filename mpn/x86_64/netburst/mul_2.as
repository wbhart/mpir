;  X86_64 mpn_mul_2

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

; (rdi,rdx+1)=(rsi,rdx)*(rcx,2) return carrylimb

	GLOBAL_FUNC mpn_mul_2
push    rbx
mov     r8, [rcx]
mov     rcx, [rcx+8]
lea     rsi, [rsi+rdx*8-24]
lea     rdi, [rdi+rdx*8-24]
mov     rbx, 3
sub     rbx, rdx
mov     r10, 0
mov     rax, [rsi+rbx*8]
mul     r8
mov     r11, rax
mov     r9, rdx
cmp     rbx, 0
jge     skiplp
align 16
lp:
	mov     rax, [rsi+rbx*8]
	mov     [rdi+rbx*8], r11
	mul     rcx
	add     r9, rax
	adc     r10, rdx
	mov     r11, 0
	mov     rax, [rsi+rbx*8+8]
	mul     r8
	add     r9, rax
	mov     rax, [rsi+rbx*8+8]
	adc     r10, rdx
	adc     r11, 0
	mul     rcx
	add     r10, rax
	mov     [rdi+rbx*8+8], r9
	adc     r11, rdx
	mov     rax, [rsi+rbx*8+16]
	mul     r8
	mov     r9, 0
	add     r10, rax
	mov     rax, [rsi+rbx*8+16]
	adc     r11, rdx
	mov     [rdi+rbx*8+16], r10
	mov     r10, 0
	adc     r9, 0
	mul     rcx
	add     r11, rax
	mov     rax, [rsi+rbx*8+24]
	adc     r9, rdx
	mul     r8
	add     r11, rax
	adc     r9, rdx
	adc     r10, 0
	add     rbx, 3
	jnc     lp
skiplp:
mov     rax, [rsi+rbx*8]
mov     [rdi+rbx*8], r11
mul     rcx
add     r9, rax
adc     r10, rdx
cmp     rbx, 1
ja      case2
je      case1
case0:
	mov     r11, 0
	mov     rax, [rsi+rbx*8+8]
	mul     r8
	add     r9, rax
	mov     rax, [rsi+rbx*8+8]
	adc     r10, rdx
	adc     r11, 0
	mul     rcx
	add     r10, rax
	mov     [rdi+rbx*8+8], r9
	adc     r11, rdx
	mov     rax, [rsi+rbx*8+16]
	mul     r8
	mov     r9, 0
	add     r10, rax
	mov     rax, [rsi+rbx*8+16]
	adc     r11, rdx
	mov     [rdi+rbx*8+16], r10
	adc     r9, 0
	mul     rcx
	add     r11, rax
	adc     r9, rdx
	mov     [rdi+rbx*8+24], r11
	mov     rax, r9
	pop     rbx
	ret
align 16
case1:
	mov     r11, 0
	mov     rax, [rsi+rbx*8+8]
	mul     r8
	add     r9, rax
	mov     rax, [rsi+rbx*8+8]
	adc     r10, rdx
	adc     r11, 0
	mul     rcx
	add     r10, rax
	mov     [rdi+rbx*8+8], r9
	adc     r11, rdx
	mov     [rdi+rbx*8+16], r10
	mov     rax, r11
	pop     rbx
	ret
align 16
case2:
	mov     [rdi+rbx*8+8], r9
	mov     rax, r10
	pop     rbx
	ret
	end
