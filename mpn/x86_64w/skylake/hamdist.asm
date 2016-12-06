; PROLOGUE(mpn_hamdist)

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
;	rax                      rcx,    rdx,        r8

%include 'yasm_mac.inc'

    CPU  nehalem
    BITS 64

	LEAF_PROC mpn_hamdist
	lea     rdx, [rdx+r8*8-8]
	lea     r9, [rcx+r8*8-8]
	mov     rcx, 1
	xor     eax, eax
	sub     rcx, r8
	jnc     .1
	xalign  16
.0:	mov     r10, [r9+rcx*8]
	xor     r10, [rdx+rcx*8]
	popcnt  r10, r10
	mov     r11, [r9+rcx*8+8]
	xor     r11, [rdx+rcx*8+8]
	popcnt  r11, r11
	add     rax, r10
	add     rax, r11
	add     rcx, 2
	jnc     .0
.1: jne     .2
	mov     r10, [r9+rcx*8]
	xor     r10, [rdx+rcx*8]
	popcnt  r10, r10
	add     rax, r10
.2:	ret

    end
