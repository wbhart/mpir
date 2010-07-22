
;  AMD64 mpn_popcount
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

;	mpn_limb_t mpn_popcount(mp_ptr,mp_size_t)
;	rax                        rdi,      rsi
;	eax                        rcx,      rdx

%include '..\yasm_mac.inc'

%define TR4	r10

    CPU  nehalem
    BITS 64

	LEAF_PROC mpn_popcount
	xor     eax, eax
	sub     rdx, 4
	jc      .2
	
	xalign   16
.1:	popcnt  r9, [rcx+rdx*8+24]
	add     rax, r9
	popcnt  r10, [rcx+rdx*8+16]
	add     rax, r10
	popcnt  r11, [rcx+rdx*8+8]
	add     rax, r11
	popcnt  TR4, [rcx+rdx*8]
	add     rax, TR4
	sub     rdx, 4
	jnc     .1
.2:	add     rdx, 4
	jz      .3
	popcnt  r9, [rcx+rdx*8-8]
	add     rax, r9
	dec     rdx
	jz      .3
	popcnt  r10, [rcx+rdx*8-8]
	add     rax, r10
	dec     rdx
	jz      .3
	popcnt  r11, [rcx+rdx*8-8]
	add     rax, r11
.3:	ret

	end
