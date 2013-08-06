; PROLOGUE(mpn_popcount)

;  AMD64 mpn_popcount
;
;  Copyright 2008 Jason Moxham
;
;  Windows Convercxon Copyright 2008 Brian Gladman
;
;  This file is part of the MPIR Library.
;  The MPIR Library is free software; you can redistribute it and/or modify
;  it under the terms of the GNU Lesser General Public License as published
;  by the Free Software Foundation; either vercxon 2.1 of the License, or (at
;  your option) any later vercxon.
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
;	rax                        rcx,      rdx

%include "yasm_mac.inc"

    CPU  SSE4.2
    BITS 64

    LEAF_PROC mpn_popcount
	xor     eax, eax
	popcnt  r9, [rcx+rdx*8-8]
	sub     rdx, 3
	jc      .1
	xalign  16
.0:	popcnt  r8, [rcx+rdx*8+8]
	add     rax, r9
	add     rax, r8
	popcnt  r9, [rcx+rdx*8]
	sub     rdx, 2
	jnc     .0
.1: jnp     .3
.2:	popcnt  r8, [rcx]
	add     rax, r8
.3:	add     rax, r9
	ret

    end
