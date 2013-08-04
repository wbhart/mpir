; PROLOGUE(mpn_store)
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
;	mpn_store(mp_ptr, mp_size_t, mp_limb_t)
;   r10          rdi        rsi        rdx
;	r10          rcx        rdx         r8

%include "yasm_mac.inc"

    CPU  Athlon64
    BITS 64

	LEAF_PROC mpn_store
	mov     rax, rdx
	and     rax, 7
	mov     r9, 8
	sub     r9, rax
    mov     rax, r8
	lea     r9, [r9+r9*4]
	lea     r10, [rel .0]
	add     r10, r9
	and     rdx, -8
	add     rcx, 48
	jmp     r10
	xalign  16
.0:	mov     [rcx+rdx*8+8], rax
	mov     [byte rcx+rdx*8+0], rax
	mov     [rcx+rdx*8-8], rax
	mov     [rcx+rdx*8-16], rax
	mov     [rcx+rdx*8-24], rax
	mov     [rcx+rdx*8-32], rax
	mov     [rcx+rdx*8-40], rax
	mov     [rcx+rdx*8-48], rax
	nop
	sub     rdx, 8
	jnc     .0
	nop
	ret

    end