
;  core2 mpn_xor_n
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

	BITS 64
    
   GLOBAL_FUNC mpn_xor_n
	sub     rcx, 4
	jb      skiploop
	align 16
loop1:
	mov     r8, [rdx+rcx*8+24]
	mov     r9, [rdx+rcx*8+16]
	or      r8, r8
	or      r9, r9
	xor     r8, [rsi+rcx*8+24]
	xor     r9, [rsi+rcx*8+16]
	mov     [rdi+rcx*8+24], r8
	mov     [rdi+rcx*8+16], r9
	mov     r10, [rdx+rcx*8+8]
	mov     r11, [rdx+rcx*8]
	or      r10, r10
	or      r11, r11
	xor     r10, [rsi+rcx*8+8]
	xor     r11, [rsi+rcx*8]
	mov     [rdi+rcx*8+8], r10
	mov     [rdi+rcx*8], r11
	sub     rcx, 4
	jnc     loop1
skiploop:
	add     rcx, 4
	jz      end
	mov     r8, [rdx+rcx*8-8]
	or      r8, r8
	xor     r8, [rsi+rcx*8-8]
	mov     [rdi+rcx*8-8], r8
	sub     rcx, 1
	jz      end
	mov     r8, [rdx+rcx*8-8]
	or      r8, r8
	xor     r8, [rsi+rcx*8-8]
	mov     [rdi+rcx*8-8], r8
	sub     rcx, 1
	jz      end
	mov     r8, [rdx+rcx*8-8]
	or      r8, r8
	xor     r8, [rsi+rcx*8-8]
	mov     [rdi+rcx*8-8], r8
end:
	ret
