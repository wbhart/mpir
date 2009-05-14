
;  mpn_store

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

%include 'yasm_mac.inc'

;	mpn_store(mp_ptr rdi ,mp_size_t rsi, mp_limb_t rdx)
;	(rdi,rsi)=rdx

	GLOBAL_FUNC mpn_store
	mov	rcx,3
	lea	rdi,[rdi+rsi*8-24]
	sub     rcx,rsi
	jnc      L_skiplp
	align   16
L_lp:
	add	rcx,4
	mov	[rdi+rcx*8-32],rdx
	mov	[rdi+rcx*8+8-32],rdx
	mov	[rdi+rcx*8+16-32],rdx
	mov	[rdi+rcx*8+24-32],rdx
	jnc     L_lp
L_skiplp:
	cmp     rcx, 2
	ja	theend
	mov	[rdi+rcx*8],rdx
	je	theend
	mov	[rdi+rcx*8+8],rdx
	jp	theend
	mov	[rdi+rcx*8+16],rdx
theend:
	ret
	end
