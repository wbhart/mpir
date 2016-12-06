; PROLOGUE(mpn_copyi)

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
;  void mpn_copyi(mp_ptr, mp_ptr, mp_size_t)
;                    rdi     rsi        rdx
;                    rcx     rdx         r8

%define SMALL_LOOP  1

%include "yasm_mac.inc"

    CPU  Core2
    BITS 64

	LEAF_PROC mpn_copyi
    cmp     r8, 0
	jz      .9
%if SMALL_LOOP <> 0
	cmp     r8, 10
	jge     .2
	
	xalign  16
.1:	mov     rax, [rdx]
	mov     [rcx], rax
	lea     rdx, [rdx+8]
	lea     rcx, [rcx+8]
	sub     r8, 1
	jnz     .1
	ret
%endif

.2:	mov     rax, rcx
	sub     rax, rdx
	test    rax, 0xF
	jz      .17
	test    rcx, 0xF
	jz      .10
	mov     r9, 5
	sub     r9, r8
	lea     rdx, [rdx+r8*8-40]
	lea     rcx, [rcx+r8*8-40]
	movapd  xmm1, [rdx+r9*8]
	movq    [rcx+r9*8], xmm1
	add     rcx, 8
%if SMALL_LOOP = 0
	cmp     r8, 1
	jz      .9
%endif
	cmp     r9, 0
	jge     .4

	xalign  16
.3:	movapd  xmm0, [rdx+r9*8+16]
	add     r9, 4
	shufpd  xmm1, xmm0, 1
	movapd  [rcx+r9*8-32], xmm1
	movapd  xmm1, [rdx+r9*8+0]
	shufpd  xmm0, xmm1, 1
	movapd  [rcx+r9*8-16], xmm0
	jnc     .3
.4:	cmp     r9, 2
	ja      .8
	jz      .7
	jp      .6

	xalign  16
.5:	movapd  xmm0, [rdx+r9*8+16]
	shufpd  xmm1, xmm0, 1
	movapd  [rcx+r9*8], xmm1
	movapd  xmm1, [rdx+r9*8+32]
	shufpd  xmm0, xmm1, 1
	movapd  [rcx+r9*8+16], xmm0
	ret

	xalign  16
.6:	movapd  xmm0, [rdx+r9*8+16]
	shufpd  xmm1, xmm0, 1
	movapd  [rcx+r9*8], xmm1
	movhpd  [rcx+r9*8+16], xmm0
	ret

	xalign  16
.7:	movapd  xmm0, [rdx+r9*8+16]
	shufpd  xmm1, xmm0, 1
	movapd  [rcx+r9*8], xmm1
	ret

	xalign  16
.8:	movhpd  [rcx+r9*8], xmm1
.9:	ret

.10:mov     r9, 4
	sub     r9, r8
	lea     rdx, [rdx+r8*8-32]
	lea     rcx, [rcx+r8*8-32]
	movapd  xmm1, [rdx+r9*8-8]
	sub     rdx, 8
	cmp     r9, 0
	jge     .12

	xalign  16
.11:movapd  xmm0, [rdx+r9*8+16]
	add     r9, 4
	shufpd  xmm1, xmm0, 1
	movapd  [rcx+r9*8-32], xmm1
	movapd  xmm1, [rdx+r9*8+0]
	shufpd  xmm0, xmm1, 1
	movapd  [rcx+r9*8-16], xmm0
	jnc     .11
.12:cmp     r9, 2
	ja      .16
	jz      .15
	jp      .14

	xalign  16
.13:movapd  xmm0, [rdx+r9*8+16]
	shufpd  xmm1, xmm0, 1
	movapd  [rcx+r9*8], xmm1
	movapd  xmm1, [rdx+r9*8+32]
	shufpd  xmm0, xmm1, 1
	movapd  [rcx+r9*8+16], xmm0
	ret

	xalign  16
.14:movapd  xmm0, [rdx+r9*8+16]
	shufpd  xmm1, xmm0, 1
	movapd  [rcx+r9*8], xmm1
	movhpd  [rcx+r9*8+16], xmm0
	ret
	
	xalign  16
.15:movapd  xmm0, [rdx+r9*8+16]
	shufpd  xmm1, xmm0, 1
	movapd  [rcx+r9*8], xmm1
	ret

	xalign  16
.16:movhpd  [rcx+r9*8], xmm1
	ret

	xalign  16
.17:mov     r9, 3
	sub     r9, r8
	test    rcx, 0xF
	lea     rdx, [rdx+r8*8-24]
	lea     rcx, [rcx+r8*8-24]
	jz      .18
	mov     rax, [rdx+r9*8]
	mov     [rcx+r9*8], rax
	add     r9, 1
.18:cmp     r9, 0
	jge     .20

	xalign  16
.19:add     r9, 4
	movapd  xmm0, [rdx+r9*8-32]
	movapd  [rcx+r9*8-32], xmm0
	movapd  xmm1, [rdx+r9*8-16]
	movapd  [rcx+r9*8-16], xmm1
	jnc     .19
.20:cmp     r9, 2
	ja      .22
	je      .24
	jp      .23

.21:movapd  xmm0, [rdx+r9*8]
	movapd  [rcx+r9*8], xmm0
	mov     rax, [rdx+r9*8+16]
	mov     [rcx+r9*8+16], rax
.22:ret

	xalign  16
.23:movapd  xmm0, [rdx+r9*8]
	movapd  [rcx+r9*8], xmm0
	ret

	xalign  16
.24:mov     rax, [rdx+r9*8]
	mov     [rcx+r9*8], rax
	ret

	end
