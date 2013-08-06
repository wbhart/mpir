; PROLOGUE(mpn_com_n)

;  mpn_com_n

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

;  void mpn_com_n(mp_ptr, mp_ptr, mp_size_t)
;                    rdi,    rsi,       rdx
;                    rcx,    rdx,        r8

%include 'yasm_mac.inc'

    CPU  nehalem
    BITS 64

	LEAF_PROC mpn_com_n
	mov     r9, 3
	lea     rdx, [rdx+r8*8-24]
	pcmpeqb xmm2, xmm2
	sub     r9, r8
	jnc     .2
	
	xalign  16
.1:	movdqu  xmm0, [rdx+r9*8]
	movdqu  xmm1, [rdx+r9*8+16]
	pxor    xmm0, xmm2
	add     r9, 4
	pxor    xmm1, xmm2
	movdqu  [rcx], xmm0
	movdqu  [rcx+16], xmm1
	lea     rcx, [rcx+32]
	jnc     .1
.2:	cmp     r9, 2
	ja      .6
	je      .5
	jp      .4
.3:	movdqu  xmm0, [rdx+r9*8]
	mov     rax, [rdx+r9*8+16]
	pxor    xmm0, xmm2
	not     rax
	movdqu  [rcx], xmm0
	mov     [rcx+16], rax
	ret

.4:	movdqu  xmm0, [rdx+r9*8]
	pxor    xmm0, xmm2
	movdqu  [rcx], xmm0
	ret

.5:	mov     rax, [rdx+r9*8]
	not     rax
	mov     [rcx], rax
.6:	ret
	
	end
