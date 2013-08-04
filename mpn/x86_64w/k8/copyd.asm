; PROLOGUE(mpn_copyd)

;  Copyright 2009 Jason Moxham
;
;  Windows Conversion Copyright 2008 Brian Gladman
;
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
;
;  void mpn_copyd(mp_ptr, mp_ptr, mp_size_t)
;                    rdi     rsi        rdx
;                    rcx     rdx         r8

%include "yasm_mac.inc"

    CPU  Athlon64
    BITS 64

;	mpn_copyd(mp_ptr rcx ,mp_ptr rdx ,mp_size_t r8)
;	(rcx,r8)=(rdx,r8)

	LEAF_PROC mpn_copyd
	lea     rdx, [rdx+r8*8-8]
	lea     rcx, [rcx+r8*8-8]
	sub     r8, 4
	jl      .2
	
	xalign  16
.1:	mov     rax, [rdx]
	mov     r9, [rdx-8]
	mov     r10, [rdx-16]
	lea     rcx, [rcx-32]
	mov     r11, [rdx-24]
	mov     [rcx+32], rax
	sub     r8, 4
	mov     [rcx+24], r9
	mov     [rcx+16], r10
	lea     rdx, [rdx-32]
	mov     [rcx+8], r11
	jns     .1
.2:	add     r8, 2
	jz      .5
	jns     .6
	jp      .4
.3:	ret
	
	xalign  16
.4:	mov     rax, [rdx]
	mov     [rcx], rax
	ret
	
	xalign  16
.5:	mov     rax, [rdx]
	mov     r9, [rdx-8]
	mov     [rcx], rax
	mov     [rcx-8], r9
	ret
	
	xalign  16
.6:	mov     rax, [rdx]
	mov     r9, [rdx-8]
	mov     r10, [rdx-16]
	mov     [rcx], rax
	mov     [rcx-8], r9
	mov     [rcx-16], r10
	ret
	
	end
