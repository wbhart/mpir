; PROLOGUE(mpn_and_n)

;  mpn_and_n

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

;  void mpn_and_n(mp_ptr, mp_srcptr, mp_srcptr, mp_size_t)
;                    rcx        rdx         r8         r9
;                    rdi        rsi        rdx        rcx 

%include 'yasm_mac.inc'

    CPU  nehalem
    BITS 64

	LEAF_PROC mpn_and_n
	movsxd	r9, r9d
	mov     r10, 3
	lea     rdx, [rdx+r9*8-24]
	lea     r8, [r8+r9*8-24]
	lea     rcx, [rcx+r9*8-24]
	sub     r10, r9
	jnc     .2
	
	xalign  16
.1:	movdqu  xmm0, [r8+r10*8]
	movdqu  xmm1, [r8+r10*8+16]
	movdqu  xmm3, [rdx+r10*8+16]
	movdqu  xmm2, [rdx+r10*8]
	pand    xmm0, xmm2
	movdqu  [rcx+r10*8], xmm0
	pand    xmm1, xmm3
	add     r10, 4
	movdqu  [rcx+r10*8+16-32], xmm1
	jnc     .1
.2:	cmp     r10, 2
	ja      .4
	je      .6
	jp      .5
.3:	movdqu  xmm0, [r8+r10*8]
	mov     rax, [r8+r10*8+16]
	mov     r9, [rdx+r10*8+16]
	movdqu  xmm2, [rdx+r10*8]
	pand    xmm0, xmm2
	movdqu  [rcx+r10*8], xmm0
	and     rax, r9
	mov     [rcx+r10*8+16], rax
.4:	ret

.5:	movdqu  xmm0, [r8+r10*8]
	movdqu  xmm2, [rdx+r10*8]
	pand    xmm0, xmm2
	movdqu  [rcx+r10*8], xmm0
	ret

.6:	mov     rax, [r8+r10*8]
	mov     r9, [rdx+r10*8]
	and     rax, r9
	mov     [rcx+r10*8], rax
	ret
	
	end
