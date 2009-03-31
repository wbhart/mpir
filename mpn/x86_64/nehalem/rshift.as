
;  core2 mpn_rshift
; Copyright 2009 Jason Moxham
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

;	(rdi, rdx) = (rsi,rdx)>>rcx
;	rax = carry
;	decent assmeblers understand what movq means ,except
;	microsofts/apple masm (what a suprise there) so for the broken old masm
;	assembler.  Needed for movq reg64,mediareg and movq mediareg,reg64
;	only , where mediareg is xmm or mm

%define MOVQ movd

	BITS 64

   GLOBAL_FUNC mpn_rshift
	cmp     rdx, 2
	ja      threeormore
	jz      two
one:
	mov     rdx, [rsi]
	mov     rax, rdx
	shr     rdx, cl
	neg     rcx
	shl     rax, cl
	mov     [rdi], rdx
	ret
two:
	mov     r8, [rsi]
	mov     r9, [rsi+8]
	mov     rax, r8
	mov     r11, r9
	shr     r8, cl
	shr     r9, cl
	neg     rcx
	shl     r11, cl
	shl     rax, cl
	or      r8, r11
	mov     [rdi], r8
	mov     [rdi+8], r9
	ret
threeormore:
	mov     eax, 64
	lea     r9, [rsi+8]
	sub     rax, rcx
	and     r9, -16
	MOVQ    xmm0, rcx
	MOVQ    xmm1, rax
	movdqa  xmm5, [r9]
	movdqa  xmm3, xmm5
	psllq   xmm5, xmm1
	MOVQ    rax, xmm5
	cmp     rsi, r9
	lea     rsi, [rsi+rdx*8-40]
	je      aligned
	movq    xmm2, [r9-8]
	movq    xmm4, xmm2
	psllq   xmm2, xmm1
	psrlq   xmm4, xmm0
	por     xmm4, xmm5
	movq    [rdi], xmm4
	lea     rdi, [rdi+8]
	sub     rdx, 1
	MOVQ    rax, xmm2
aligned:
	lea     rdi, [rdi+rdx*8-40]
	psrlq   xmm3, xmm0
	mov     r8d, 5
	sub     r8, rdx
	jnc     skiploop
	align 16
loop1:
	movdqa  xmm2, [rsi+r8*8+16]
	movdqa  xmm4, xmm2
	psllq   xmm2, xmm1
	shufpd  xmm5, xmm2, 1
	por     xmm3, xmm5
	movq    [rdi+r8*8], xmm3
	movhpd  [rdi+r8*8+8], xmm3
	psrlq   xmm4, xmm0
	movdqa  xmm5, [rsi+r8*8+32]
	movdqa  xmm3, xmm5
	psllq   xmm5, xmm1
	shufpd  xmm2, xmm5, 1
	psrlq   xmm3, xmm0
	por     xmm4, xmm2
	movq    [rdi+r8*8+16], xmm4
	movhpd  [rdi+r8*8+24], xmm4
	add     r8, 4
	jnc     loop1
skiploop:
	cmp     r8, 2
	ja      left0
	jz      left1
	jp      left2
left3:
	movdqa  xmm2, [rsi+r8*8+16]
	movdqa  xmm4, xmm2
	psllq   xmm2, xmm1
	shufpd  xmm5, xmm2, 1
	por     xmm3, xmm5
	movq    [rdi+r8*8], xmm3
	movhpd  [rdi+r8*8+8], xmm3
	psrlq   xmm4, xmm0
	movq    xmm5, [rsi+r8*8+32]
	movq    xmm3, xmm5
	psllq   xmm5, xmm1
	shufpd  xmm2, xmm5, 1
	psrlq   xmm3, xmm0
	por     xmm4, xmm2
	movq    [rdi+r8*8+16], xmm4
	movhpd  [rdi+r8*8+24], xmm4
	psrldq  xmm5, 8
	por     xmm3, xmm5
	movq    [rdi+r8*8+32], xmm3
	ret
	align 16
left2:
	movdqa  xmm2, [rsi+r8*8+16]
	movdqa  xmm4, xmm2
	psllq   xmm2, xmm1
	shufpd  xmm5, xmm2, 1
	por     xmm3, xmm5
	movq    [rdi+r8*8], xmm3
	movhpd  [rdi+r8*8+8], xmm3
	psrlq   xmm4, xmm0
	psrldq  xmm2, 8
	por     xmm4, xmm2
	movq    [rdi+r8*8+16], xmm4
	movhpd  [rdi+r8*8+24], xmm4
	ret
	align 16
left1:
	movq    xmm2, [rsi+r8*8+16]
	movq    xmm4, xmm2
	psllq   xmm2, xmm1
	shufpd  xmm5, xmm2, 1
	por     xmm3, xmm5
	movq    [rdi+r8*8], xmm3
	movhpd  [rdi+r8*8+8], xmm3
	psrlq   xmm4, xmm0
	psrldq  xmm2, 8
	por     xmm4, xmm2
	movq    [rdi+r8*8+16], xmm4
	ret
	align 16
left0:
	psrldq  xmm5, 8
	por     xmm3, xmm5
	movq    [rdi+r8*8], xmm3
	movhpd  [rdi+r8*8+8], xmm3
	ret
