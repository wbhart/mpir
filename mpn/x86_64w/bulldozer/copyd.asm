; PROLOGUE(mpn_copyd)

;  mpn_copyd

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

;  void mpn_copyi(mp_ptr, mp_ptr, mp_size_t)
;                    rdi,    rsi,       rdx
;                    rcx,    rdx,        r8

%include 'yasm_mac.inc'

    CPU  nehalem
    BITS 64

	LEAF_PROC mpn_copyd
	lea     rdx, [rdx+16]
	lea     rcx, [rcx+16]
	sub     r8, 4
	jc      .2
	
	xalign  16
.1:	movdqu  xmm0, [rdx+r8*8]
	movdqu  xmm1, [rdx+r8*8-16]
	sub     r8, 4
	movdqu  [rcx+r8*8-16+32], xmm1
	movdqu  [rcx+r8*8+32], xmm0
	jnc     .1
.2:	cmp     r8, -2
	jg      .5
	je      .6
	jnp     .4
.3:	mov     rax, [rdx+r8*8+8]
	mov     [rcx+r8*8+8], rax
.4:	ret

.5:	movdqu  xmm0, [rdx+r8*8]
	mov     rax, [rdx+r8*8-8]
	mov     [rcx+r8*8-8], rax
	movdqu  [rcx+r8*8], xmm0
	ret

.6:	movdqu  xmm0, [rdx+r8*8]
	movdqu  [rcx+r8*8], xmm0
	ret
	
	end
