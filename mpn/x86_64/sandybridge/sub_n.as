
;  AMD64 mpn_sub_n
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

;	(rdi,rcx)=(rsi,rcx)-(rdx,rcx)
;	rax=borrow
	
%include 'yasm_mac.inc'

    BITS    64

   GLOBAL_FUNC mpn_sub_n
	mov     rax, rcx
	and     rax, 3
	shr     rcx, 2
	cmp     rcx, 0
;	carry flag is clear here
	jnz     loop1
	mov     r11, [rsi]
	sub     r11, [rdx]
	mov     [rdi], r11
	dec     rax
	jz      end1
	mov     r11, [rsi+8]
	sbb     r11, [rdx+8]
	mov     [rdi+8], r11
	dec     rax
	jz      end1
	mov     r11, [rsi+16]
	sbb     r11, [rdx+16]
	mov     [rdi+16], r11
	dec     rax
end1:
	adc     rax, rax
	ret
	align   8
loop1:
	mov     r11, [rsi]
	mov     r8, [rsi+8]
	lea     rsi, [rsi+32]
	sbb     r11, [rdx]
	sbb     r8, [rdx+8]
	lea     rdx, [rdx+32]
	mov     [rdi], r11
	mov     [rdi+8], r8
	lea     rdi, [rdi+32]
	mov     r9, [rsi-16]
	mov     r10, [rsi-8]
	sbb     r9, [rdx-16]
	sbb     r10, [rdx-8]
	mov     [rdi-16], r9
	dec     rcx
	mov     [rdi-8], r10
	jnz     loop1
	inc     rax
	dec     rax
	jz      end
	mov     r11, [rsi]
	sbb     r11, [rdx]
	mov     [rdi], r11
	dec     rax
	jz      end
	mov     r11, [rsi+8]
	sbb     r11, [rdx+8]
	mov     [rdi+8], r11
	dec     rax
	jz      end
	mov     r11, [rsi+16]
	sbb     r11, [rdx+16]
	mov     [rdi+16], r11
	dec     rax
end:
	adc     rax, rax
	ret
