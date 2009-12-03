
;  AMD64 mpn_hamdist
;  Copyright 2009 Jason Moxham
;  This file is part of the MPIR Library.
;  The MPIR Library is free software; you can redistribute it and/or modify
;  it under the terms of the GNU Lesser General Public License as published
;  by the Free Software Foundation; either verdxon 2.1 of the License, or (at
;  your option) any later verdxon.
;  The MPIR Library is distributed in the hope that it will be useful, but
;  WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
;  or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public
;  License for more details.
;  You should have received a copy of the GNU Lesser General Public License
;  along with the MPIR Library; see the file COPYING.LIB.  If not, write
;  to the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
;  Boston, MA 02110-1301, USA.

;	mp_limb_t mpn_hamdist(mp_ptr, mp_ptr, mp_size_t)
;	rax                      rdi,    rsi,       rdx
;	rax                      rcx,    rdx,       r8d

%include '..\yasm_mac.inc'

%define TR4	r10

    CPU  nehalem
    BITS 64

	LEAF_PROC mpn_hamdist
	movsxd	r8, r8d
	xor     eax, eax
	sub     r8, 4
	jc      .2
	
	xalign   16
.1:
	mov     r9, [rcx+r8*8+24]
	mov     r10, [rcx+r8*8+16]
	xor     r9, [rdx+r8*8+24]
	popcnt  r9, r9
	add     rax, r9
	xor     r10, [rdx+r8*8+16]
	popcnt  r10, r10
	add     rax, r10
	mov     r11, [rcx+r8*8+8]
	mov     TR4, [rcx+r8*8]
	xor     r11, [rdx+r8*8+8]
	popcnt  r11, r11
	add     rax, r11
	xor     TR4, [rdx+r8*8]
	popcnt  TR4, TR4
	add     rax, TR4
	sub     r8, 4
	jnc     .1
.2:
	add     r8, 4
	jz      .3
	mov     r9, [rcx+r8*8-8]
	xor     r9, [rdx+r8*8-8]
	popcnt  r9, r9
	add     rax, r9
	dec     r8
	jz      .3
	mov     r10, [rcx+r8*8-8]
	xor     r10, [rdx+r8*8-8]
	popcnt  r10, r10
	add     rax, r10
	dec     r8
	jz      .3
	mov     r11, [rcx+r8*8-8]
	xor     r11, [rdx+r8*8-8]
	popcnt  r11, r11
	add     rax, r11
.3:
	ret
