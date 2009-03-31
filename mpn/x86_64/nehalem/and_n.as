
;  core2 mpn_and_n
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

;	(rdi, rcx) = (rsi, rcx) & (rdx, rcx)
	
    BITS 64
    
   GLOBAL_FUNC mpn_and_n
	mov     rax, rcx
	and     rax, 3
	shr     rcx, 2
	jz      skiploop
	align 8
loop1:
	mov     r11, [rsi]
	mov     r8, [rsi+8]
	lea     rsi, [rsi+32]
	and     r11, [rdx]
	and     r8, [rdx+8]
	lea     rdx, [rdx+32]
	mov     [rdi], r11
	mov     [rdi+8], r8
	lea     rdi, [rdi+32]
	mov     r9, [rsi-16]
	mov     r10, [rsi-8]
	and     r9, [rdx-16]
	and     r10, [rdx-8]
	mov     [rdi-16], r9
	sub     rcx, 1
	mov     [rdi-8], r10
	jnz     loop1
skiploop:
	cmp     rax, 0
	jz      end
	mov     r11, [rsi]
	and     r11, [rdx]
	mov     [rdi], r11
	sub     rax, 1
	jz      end
	mov     r11, [rsi+8]
	and     r11, [rdx+8]
	mov     [rdi+8], r11
	sub     rax, 1
	jz      end
	mov     r11, [rsi+16]
	and     r11, [rdx+16]
	mov     [rdi+16], r11
end:
	ret
