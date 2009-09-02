
;  Copyright 2009 Jason Moxham
;
;  This file is part of the MPIR Library.
;
;  Windows Conversion Copyright 2008 Brian Gladman
;
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
;
;	mpn_store(mp_ptr rcx ,mp_size_t rdx, mp_limb_t rax)
;	rax          rcx,      rdx,         r8

%include "..\yasm_mac.inc"

    CPU  Core2
    BITS 64

	LEAF_PROC mpn_store
	movsxd  r8, r8d

	cmp     rdx, 0
	jz      .3
	movq    xmm0, r8
	movddup xmm0, xmm0
	lea     rcx, [rcx-16]
	test    rcx, 0xF
	jz      .0
	mov     [rcx+16], r8
	lea     rcx, [rcx+8]
	sub     rdx, 1

.0:	sub     rdx, 2
	jc      .2
	xalign  16
.1:	lea     rcx, [rcx+16]
	sub     rdx, 2
	movdqa  [rcx], xmm0
	jnc     .1
.2:	jnp     .3
	mov     [rcx+16], r8
.3:	ret

    end
