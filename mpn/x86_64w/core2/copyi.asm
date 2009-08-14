
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
;
;	ret mpn_copyi(mp_ptr,mp_ptr,mp_size_t)
;	rax             rcx,   rdx,      r8
;
;  for <20 limbs this is slower than core2/copyi for rev 2257 ;
;  probaly want to tweek it , that should do most of the work ;
;  below small loop is not much help

%define SMALL_LOOP  0

%include "..\yasm_mac.inc"

    CPU  Core2
    BITS 64

	LEAF_PROC mpn_copyi
	movsxd  r8, r8d

%if SMALL_LOOP <> 0
	cmp     r8, 10
	jge     .1
	xalign  16
.0:	mov     rax, [rdx]
	mov     [rcx], rax
	lea     rdx, [rdx+8]
	lea     rcx, [rcx+8]
	sub     r8, 1
	jnz     .0
	ret
%else	
    cmp     r8, 0
	jz      .8
%endif

.1:	mov     rax, rcx
	sub     rax, rdx
	test    rax, 0xF
	jz      .16
	test    rcx, 0xF
	jz      .9
	mov     r9, 5
	sub     r9, r8
	lea     rdx, [rdx+r8*8-40]
	lea     rcx, [rcx+r8*8-40]
	movapd  xmm1, [rdx+r9*8]
	movq    [rcx+r9*8], xmm1
	add     rcx, 8
%if SMALL_LOOP = 0
	cmp     r8, 1
	jz      .8
%endif
	cmp     r9, 0
	jge     .3
	
	xalign  16
.2:	movapd  xmm0, [rdx+r9*8+16]
	add     r9, 4
	shufpd  xmm1, xmm0, 1
	movapd  [rcx+r9*8-32], xmm1
	movapd  xmm1, [rdx+r9*8+0]
	shufpd  xmm0, xmm1, 1
	movapd  [rcx+r9*8-16], xmm0
	jnc     .2

.3:	cmp     r9, 2
	ja      .7
	jz      .6
	jp      .5

	xalign  16
.4:	movapd  xmm0, [rdx+r9*8+16]
	shufpd  xmm1, xmm0, 1
	movapd  [rcx+r9*8], xmm1
	movapd  xmm1, [rdx+r9*8+32]
	shufpd  xmm0, xmm1, 1
	movapd  [rcx+r9*8+16], xmm0
	ret

	xalign  16
.5:	movapd  xmm0, [rdx+r9*8+16]
	shufpd  xmm1, xmm0, 1
	movapd  [rcx+r9*8], xmm1
	movhpd  [rcx+r9*8+16], xmm0
	ret

	xalign  16
.6:	movapd  xmm0, [rdx+r9*8+16]
	shufpd  xmm1, xmm0, 1
	movapd  [rcx+r9*8], xmm1
	ret

	xalign  16
.7:	movhpd  [rcx+r9*8], xmm1
.8:	ret

.9:	mov     r9, 4
	sub     r9, r8
	lea     rdx, [rdx+r8*8-32]
	lea     rcx, [rcx+r8*8-32]
	movapd  xmm1, [rdx+r9*8-8]
	sub     rdx, 8
	cmp     r9, 0
	jge     .11

	xalign  16
.10:movapd  xmm0, [rdx+r9*8+16]
	add     r9, 4
	shufpd  xmm1, xmm0, 1
	movapd  [rcx+r9*8-32], xmm1
	movapd  xmm1, [rdx+r9*8+0]
	shufpd  xmm0, xmm1, 1
	movapd  [rcx+r9*8-16], xmm0
	jnc     .10

.11:cmp     r9, 2
	ja      .15
	jz      .14
	jp      .13
	
	xalign  16
.12:movapd  xmm0, [rdx+r9*8+16]
	shufpd  xmm1, xmm0, 1
	movapd  [rcx+r9*8], xmm1
	movapd  xmm1, [rdx+r9*8+32]
	shufpd  xmm0, xmm1, 1
	movapd  [rcx+r9*8+16], xmm0
	ret
	
	xalign  16
.13:movapd  xmm0, [rdx+r9*8+16]
	shufpd  xmm1, xmm0, 1
	movapd  [rcx+r9*8], xmm1
	movhpd  [rcx+r9*8+16], xmm0
	ret
	xalign  16
.14:movapd  xmm0, [rdx+r9*8+16]
	shufpd  xmm1, xmm0, 1
	movapd  [rcx+r9*8], xmm1
	ret
	
	xalign  16
.15:movhpd  [rcx+r9*8], xmm1
	ret

	xalign  16
.16:mov     r9, 3
	sub     r9, r8
	test    rcx, 0xF
	lea     rdx, [rdx+r8*8-24]
	lea     rcx, [rcx+r8*8-24]
	jz      .17
	mov     rax, [rdx+r9*8]
	mov     [rcx+r9*8], rax
	add     r9, 1
.17:cmp     r9, 0
	jge     .19
	
	xalign  16
.18:add     r9, 4
	movapd  xmm0, [rdx+r9*8-32]
	movapd  [rcx+r9*8-32], xmm0
	movapd  xmm1, [rdx+r9*8-16]
	movapd  [rcx+r9*8-16], xmm1
	jnc     .18

.19:cmp     r9, 2
	ja      .21
	je      .23
	jp      .22
	
.20:movapd  xmm0, [rdx+r9*8]
	movapd  [rcx+r9*8], xmm0
	mov     rax, [rdx+r9*8+16]
	mov     [rcx+r9*8+16], rax
.21:ret

	xalign  16
.22:movapd  xmm0, [rdx+r9*8]
	movapd  [rcx+r9*8], xmm0
	ret
	
	xalign  16
.23:mov     rax, [rdx+r9*8]
	mov     [rcx+r9*8], rax
	ret
	
	end
