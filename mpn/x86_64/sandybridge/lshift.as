
;  AMD64 mpn_lshift
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

;	(rdi,rdx) = (rsi,rdx)<<rcx
;	rax = carry
;	decent assmeblers understand what movq means ,except
;	microsofts/apple masm (what a suprise there) so for the broken old masm
;	assembler.  Needed for movq reg64,mediareg and movq mediareg,reg64
;	only , where mediareg is xmm or mm

%include 'yasm_mac.inc'

%define MOVQ movd

	BITS 64

   GLOBAL_FUNC  mpn_lshift
	cmp     rdx, 2
	ja      threeormore
	jz      two
one:
	mov     rdx, [rsi]
	mov     rax, rdx
	shl     rdx, cl
	neg     rcx
	shr     rax, cl
	mov     [rdi], rdx
	ret
two:
	mov     r8, [rsi]
	mov     r9, [rsi+8]
	mov     r11, r8
	mov     rax, r9
	shl     r8, cl
	shl     r9, cl
	neg     rcx
	shr     r11, cl
	shr     rax, cl
	or      r9, r11
	mov     [rdi], r8
	mov     [rdi+8], r9
	ret
threeormore:
	mov     eax, 64
	sub     rax, rcx
	MOVQ    xmm0, rcx
	MOVQ    xmm1, rax
	mov     r8, rdx
	lea     r9, [rsi+r8*8-16]
	mov     r10, r9
	and     r9, -9
	movdqa  xmm3, [r9]
	movdqa  xmm5, xmm3
	psrlq   xmm3, xmm1
	pshufd  xmm3, xmm3, 0x4E
	MOVQ    rax, xmm3
	cmp     r10, r9
	je      aligned
	movq    xmm2, [rsi+r8*8-8]
	movq    xmm4, xmm2
	psrlq   xmm2, xmm1
	MOVQ    rax, xmm2
	psllq   xmm4, xmm0
	por     xmm4, xmm3
	movq    [rdi+r8*8-8], xmm4
	dec     r8
aligned:
	sub     r8, 5
	jle     skiploop
	align   16
loop1:
	movdqa  xmm2, [rsi+r8*8+8]
	movdqa  xmm4, xmm2
	psllq   xmm5, xmm0
	psrlq   xmm2, xmm1
	movhlps xmm3, xmm2
	por     xmm5, xmm3
	movq    [rdi+r8*8+24], xmm5
	pshufd  xmm2, xmm2, 0x4E
	movhpd  [rdi+r8*8+32], xmm5
	movdqa  xmm3, [rsi+r8*8-8]
	movdqa  xmm5, xmm3
	psrlq   xmm3, xmm1
	movhlps xmm2, xmm3
	psllq   xmm4, xmm0
	pshufd  xmm3, xmm3, 0x4E
	por     xmm4, xmm2
	movq    [rdi+r8*8+8], xmm4
	movhpd  [rdi+r8*8+16], xmm4
	sub     r8, 4
	jg      loop1
skiploop:
	cmp     r8, -1
	je      left2
	jg      left3
	jp      left1
left0:
;	may be easier to bswap xmm5 first , same with other cases
	pxor    xmm2, xmm2
	psllq   xmm5, xmm0
	movhlps xmm3, xmm2
	por     xmm5, xmm3
	movq    [rdi+r8*8+24], xmm5
	movhpd  [rdi+r8*8+32], xmm5
	ret
	align   16
left3:
	movdqa  xmm2, [rsi+r8*8+8]
	movdqa  xmm4, xmm2
	psllq   xmm5, xmm0
	psrlq   xmm2, xmm1
	movhlps xmm3, xmm2
	por     xmm5, xmm3
	movq    [rdi+r8*8+24], xmm5
	pshufd  xmm2, xmm2, 0x4E
	movhpd  [rdi+r8*8+32], xmm5
	movq    xmm3, [rsi+r8*8]
	pshufd  xmm3, xmm3, 0x4E
	movdqa  xmm5, xmm3
	psrlq   xmm3, xmm1
	movhlps xmm2, xmm3
	psllq   xmm4, xmm0
	por     xmm4, xmm2
	movq    [rdi+r8*8+8], xmm4
	movhpd  [rdi+r8*8+16], xmm4
	psllq   xmm5, xmm0
	movhpd  [rdi+r8*8], xmm5
	ret
	align   16
left2:
	movdqa  xmm2, [rsi+r8*8+8]
	movdqa  xmm4, xmm2
	psllq   xmm5, xmm0
	psrlq   xmm2, xmm1
	movhlps xmm3, xmm2
	por     xmm5, xmm3
	movq    [rdi+r8*8+24], xmm5
	pshufd  xmm2, xmm2, 0x4E
	movhpd  [rdi+r8*8+32], xmm5
	pxor    xmm3, xmm3
	movhlps xmm2, xmm3
	psllq   xmm4, xmm0
	por     xmm4, xmm2
	movq    [rdi+r8*8+8], xmm4
	movhpd  [rdi+r8*8+16], xmm4
	ret
	align   16
left1:
	movq    xmm2, [rsi+r8*8+16]
	pshufd  xmm2, xmm2, 0x4E
	movdqa  xmm4, xmm2
	psllq   xmm5, xmm0
	psrlq   xmm2, xmm1
	movhlps xmm3, xmm2
	por     xmm5, xmm3
	movq    [rdi+r8*8+24], xmm5
	movhpd  [rdi+r8*8+32], xmm5
	psllq   xmm4, xmm0
	movhpd  [rdi+r8*8+16], xmm4
	ret
