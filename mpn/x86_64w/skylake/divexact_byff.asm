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

    CPU  Athlon64
    BITS 64

    LEAF_PROC mpn_divexact_byff
	mov     rax, 3
	and     rax, r8
	mov     [rsp+24], rax
	xor     eax, eax
	shr     r8, 2
	cmp     r8, 0
	je      .2
; want carry clear here
	xalign  16
.1:	sbb     rax, [rdx]
	lea     rcx, [rcx+32]
	mov     r9, rax
	sbb     rax, [rdx+8]
	mov     r10, rax
	sbb     rax, [rdx+16]
	mov     r11, rax
	sbb     rax, [rdx+24]
	dec     r8
	mov     [rcx-32], r9
	mov     [rcx-24], r10
	mov     [rcx-16], r11
	mov     [rcx-8], rax
	lea     rdx, [rdx+32]
	jnz     .1
.2:	mov     r8, [rsp+24]
; dont want to change the carry
	inc     r8
	dec     r8
	jz      .3
	sbb     rax, [rdx]
	mov     [rcx], rax
	dec     r8
	jz      .3
	sbb     rax, [rdx+8]
	mov     [rcx+8], rax
	dec     r8
	jz      .3
	sbb     rax, [rdx+16]
	mov     [rcx+16], rax
.3:	sbb     rax, 0
	ret
	
	end
