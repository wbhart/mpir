
;  AMD64 mpn_popcount
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

   GLOBAL_FUNC mpn_popcount
	xor     eax, eax
	sub     rsi, 4
	jc      skiploop
	align   16
loop1:
	popcnt  r8, [rdi+rsi*8+24]
	add     rax, r8
	popcnt  r9, [rdi+rsi*8+16]
	add     rax, r9
	popcnt  r10, [rdi+rsi*8+8]
	add     rax, r10
	popcnt  r11, [rdi+rsi*8]
	add     rax, r11
	sub     rsi, 4
	jnc     loop1
skiploop:
	add     rsi, 4
	jz      end
	popcnt  r8, [rdi+rsi*8-8]
	add     rax, r8
	dec     rsi
	jz      end
	popcnt  r9, [rdi+rsi*8-8]
	add     rax, r9
	dec     rsi
	jz      end
	popcnt  r10, [rdi+rsi*8-8]
	add     rax, r10
end:
	ret
