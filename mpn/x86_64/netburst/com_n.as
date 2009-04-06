
;  core2 mpn_com_n
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

	BITS 64
    
   GLOBAL_FUNC mpn_com_n
	sub     rdx, 4
	jc      next
	align 8
loop1:
	mov     rax, [rsi+rdx*8+24]
	mov     rcx, [rsi+rdx*8+16]
	not     rax
	not     rcx
	mov     [rdi+rdx*8+24], rax
	mov     [rdi+rdx*8+16], rcx
	mov     rax, [rsi+rdx*8+8]
	mov     rcx, [rsi+rdx*8]
	not     rax
	not     rcx
	mov     [rdi+rdx*8+8], rax
	mov     [rdi+rdx*8], rcx
	sub     rdx, 4
	jae     loop1
next:
	add     rdx, 4
	jz      end
;	Could still have potential cache-bank conflicts in this tail part
	mov     rax, [rsi+rdx*8-8]
	not     rax
	mov     [rdi+rdx*8-8], rax
	sub     rdx, 1
	jz      end
	mov     rax, [rsi+rdx*8-8]
	not     rax
	mov     [rdi+rdx*8-8], rax
	sub     rdx, 1
	jz      end
	mov     rax, [rsi+rdx*8-8]
	not     rax
	mov     [rdi+rdx*8-8], rax
end:
	ret
