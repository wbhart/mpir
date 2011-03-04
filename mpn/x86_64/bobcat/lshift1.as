
;  AMD64 mpn_lshift1
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

;	(rdi,rdx) = (rsi,rdx)<<1
;	rax = carry

%include 'yasm_mac.inc'

    BITS    64

   GLOBAL_FUNC mpn_lshift1
	xor     rax, rax
	mov     r11, rdx
	and     r11, 7
	inc     r11
	shr     rdx, 3
;	and clear carry flag
	cmp     rdx, 0
	jz      next
	align   16
loop1:
	mov     rcx, [rsi]
	mov     r8, [rsi+8]
	mov     r10, [rsi+16]
	mov     r9, [rsi+24]
	adc     rcx, rcx
	adc     r8, r8
	adc     r10, r10
	adc     r9, r9
	mov     [rdi], rcx
	mov     [rdi+8], r8
	mov     [rdi+16], r10
	mov     [rdi+24], r9
	mov     rcx, [rsi+32]
	mov     r8, [rsi+40]
	mov     r10, [rsi+48]
	mov     r9, [rsi+56]
	adc     rcx, rcx
	adc     r8, r8
	adc     r10, r10
	adc     r9, r9
	mov     [rdi+32], rcx
	mov     [rdi+40], r8
	mov     [rdi+48], r10
	mov     [rdi+56], r9
	lea     rdi, [rdi+64]
	dec     rdx
	lea     rsi, [rsi+64]
	jnz     loop1
next:
	dec     r11
	jz      end
;	Could still have cache-bank conflicts in this tail part
	mov     rcx, [rsi]
	adc     rcx, rcx
	mov     [rdi], rcx
	dec     r11
	jz      end
	mov     rcx, [rsi+8]
	adc     rcx, rcx
	mov     [rdi+8], rcx
	dec     r11
	jz      end
	mov     rcx, [rsi+16]
	adc     rcx, rcx
	mov     [rdi+16], rcx
	dec     r11
	jz      end
	mov     rcx, [rsi+24]
	adc     rcx, rcx
	mov     [rdi+24], rcx
	dec     r11
	jz      end
	mov     rcx, [rsi+32]
	adc     rcx, rcx
	mov     [rdi+32], rcx
	dec     r11
	jz      end
	mov     rcx, [rsi+40]
	adc     rcx, rcx
	mov     [rdi+40], rcx
	dec     r11
	jz      end
	mov     rcx, [rsi+48]
	adc     rcx, rcx
	mov     [rdi+48], rcx
end:
	adc     rax, rax
	ret
