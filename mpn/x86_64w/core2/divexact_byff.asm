; PROLOGUE(mpn_divexact_byff)
;  Copyright 2009 Jason Moxham
;
;  Windows Conversion Copyright 2008 Brian Gladman
;
;  This file is part of the MPIR Library.
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
;  mp_limb_t mpn_divexact_byff(mp_ptr, mp_ptr, mp_size_t)
;  rax                           rdi     rsi         rdx
;  rax                           rcx     rdx          r8

%include "yasm_mac.inc"

    CPU  Core2
    BITS 64

    LEAF_PROC mpn_divexact_byff

	xor     eax, eax
	mov     r9, r8
	and     r9, 3
	shr     r8, 2
	cmp     r8, 0
;	carry flag is clear here
	jnz     .2
	sbb     rax, [rdx]
	mov     [rcx], rax
	dec     r9
	jz      .1
	sbb     rax, [rdx+8]
	mov     [rcx+8], rax
	dec     r9
	jz      .1
	sbb     rax, [rdx+16]
	mov     [rcx+16], rax
	dec     r9
.1:	sbb     rax, 0
	ret
	
	xalign  16
.2:	sbb     rax, [rdx]
	mov     [rcx], rax
	sbb     rax, [rdx+8]
	mov     [rcx+8], rax
	sbb     rax, [rdx+16]
	mov     [rcx+16], rax
	sbb     rax, [rdx+24]
	mov     [rcx+24], rax
	lea     rdx, [rdx+32]
	dec     r8
	lea     rcx, [rcx+32]
	jnz     .2
	inc     r9
	dec     r9
	jz      .3
	sbb     rax, [rdx]
	mov     [rcx], rax
	dec     r9
	jz      .3
	sbb     rax, [rdx+8]
	mov     [rcx+8], rax
	dec     r9
	jz      .3
	sbb     rax, [rdx+16]
	mov     [rcx+16], rax
	dec     r9
.3:	sbb     rax, 0
	ret

	end
