
;  AMD64 mpn_hamdist
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

        BITS    64

   GLOBAL_FUNC mpn_hamdist
	xor     eax, eax
	sub     rdx, 4
	jc      skiploop
	align   16
loop1:
	mov     r8, [rdi+rdx*8+24]
	mov     r9, [rdi+rdx*8+16]
	xor     r8, [rsi+rdx*8+24]
	popcnt  r8, r8
	add     rax, r8
	xor     r9, [rsi+rdx*8+16]
	popcnt  r9, r9
	add     rax, r9
	mov     r10, [rdi+rdx*8+8]
	mov     r11, [rdi+rdx*8]
	xor     r10, [rsi+rdx*8+8]
	popcnt  r10, r10
	add     rax, r10
	xor     r11, [rsi+rdx*8]
	popcnt  r11, r11
	add     rax, r11
	sub     rdx, 4
	jnc     loop1
skiploop:
	add     rdx, 4
	jz      end
	mov     r8, [rdi+rdx*8-8]
	xor     r8, [rsi+rdx*8-8]
	popcnt  r8, r8
	add     rax, r8
	dec     rdx
	jz      end
	mov     r9, [rdi+rdx*8-8]
	xor     r9, [rsi+rdx*8-8]
	popcnt  r9, r9
	add     rax, r9
	dec     rdx
	jz      end
	mov     r10, [rdi+rdx*8-8]
	xor     r10, [rsi+rdx*8-8]
	popcnt  r10, r10
	add     rax, r10
end:
	ret
