; PROLOGUE(mpn_copyi)

;  mpn_copyi

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

	LEAF_PROC mpn_copyi
	mov     r9, 3
	lea     rdx, [rdx+r8*8-24]
	lea     rcx, [rcx+r8*8-24]
	sub     r9, r8
	jnc     .2
	
	xalign  16
.1:	movdqu  xmm0, [rdx+r9*8]
	movdqu  xmm1, [rdx+r9*8+16]
	add     r9, 4
	movdqu  [rcx+r9*8+16-32], xmm1
	movdqu  [rcx+r9*8-32], xmm0
	jnc     .1
.2:	cmp     r9, 2
	ja      .6
	je      .5
	jp      .4
.3:	movdqu  xmm0, [rdx+r9*8]
	mov     rax, [rdx+r9*8+16]
	mov     [rcx+r9*8+16], rax
	movdqu  [rcx+r9*8], xmm0
	ret

.4:	movdqu  xmm0, [rdx+r9*8]
	movdqu  [rcx+r9*8], xmm0
	ret

.5:	mov     rax, [rdx+r9*8]
	mov     [rcx+r9*8], rax
.6:	ret
	
	end
