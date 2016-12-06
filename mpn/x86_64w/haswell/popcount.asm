; PROLOGUE(mpn_popcount)

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

%include 'yasm_mac.inc'

    CPU  nehalem
    BITS 64

	LEAF_PROC mpn_popcount
	mov     r8, 5
	lea     rcx, [rcx+rdx*8-40]
	xor     eax, eax
	sub     r8, rdx
	jnc     .1
	xalign  16
.0:
	popcnt  r9, [rcx+r8*8]
	popcnt  r10, [rcx+r8*8+8]
	popcnt  r11, [rcx+r8*8+16]
	popcnt  rdx, [rcx+r8*8+24]
	add     rax, r9
	add     rax, rdx
	add     rax, r10
	popcnt  r9, [rcx+r8*8+32]
	popcnt  r10, [rcx+r8*8+40]
	add     rax, r9
	add     rax, r11
	add     rax, r10
	add     r8, 6
	jnc     .0
.1:
	lea     rdx, [rel .2]
	lea     r8, [r8+r8*8]
	add     rdx, r8
	jmp     rdx
.2:
	nop
	popcnt  r9, [rcx]
	add     rax, r9
.3:
	popcnt  r10, [rcx+8]
	add     rax, r10
.4:	popcnt  r11, [rcx+16]
	add     rax, r11
.5:	popcnt  rdx, [rcx+24]
	add     rax, rdx
.6:	popcnt  r9, [rcx+32]
	add     rax, r9
.7:	ret

    end
