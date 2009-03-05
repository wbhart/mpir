
;  AMD64 mpn_rshift
; Copyright 2008 Jason Moxham
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

;	(rdi,rdx) = (rsi,rdx)>>rcx
;	rax = carry

%include 'yasm_mac.inc'

    BITS    64

   GLOBAL_FUNC mpn_rshift
	mov     eax, 64
	lea     rsi, [rsi+rdx*8-32]
	lea     rdi, [rdi+rdx*8-32]
	sub     rax, rcx
	movq    mm0, rcx
	mov     r8d, 4
	sub     r8, rdx
	movq    mm1, rax
	movq    mm5, [rsi+r8*8]
	movq    mm3, mm5
	psllq   mm5, mm1
	movq    rax, mm5
	psrlq   mm3, mm0
	jnc     skiploop
	align   16
loop1:
	movq    mm2, [rsi+r8*8+8]
	movq    mm4, mm2
	psllq   mm2, mm1
	por     mm3, mm2
	movq    [rdi+r8*8], mm3
	psrlq   mm4, mm0
	movq    mm5, [rsi+r8*8+16]
	movq    mm3, mm5
	psllq   mm5, mm1
	por     mm4, mm5
	movq    [rdi+r8*8+8], mm4
	psrlq   mm3, mm0
;	got room here for another jump out , if we can arrange our r8 to be
;	slightly different , so we can use a jz or jp here
	movq    mm2, [rsi+r8*8+24]
	movq    mm4, mm2
	psllq   mm2, mm1
	por     mm3, mm2
	movq    [rdi+r8*8+16], mm3
	psrlq   mm4, mm0
	movq    mm5, [rsi+r8*8+32]
	movq    mm3, mm5
	psllq   mm5, mm1
	por     mm4, mm5
	movq    [rdi+r8*8+24], mm4
	psrlq   mm3, mm0
	add     r8, 4
	jnc     loop1
skiploop:
	test    r8, 2
	jnz     next
	movq    mm2, [rsi+r8*8+8]
	movq    mm4, mm2
	psllq   mm2, mm1
	por     mm3, mm2
	movq    [rdi+r8*8], mm3
	psrlq   mm4, mm0
	movq    mm5, [rsi+r8*8+16]
	movq    mm3, mm5
	psllq   mm5, mm1
	por     mm4, mm5
	movq    [rdi+r8*8+8], mm4
	psrlq   mm3, mm0
	add     r8, 2
next:
	test    r8, 1
	jnz     end
	movq    mm2, [rsi+r8*8+8]
	movq    mm4, mm2
	psllq   mm2, mm1
	por     mm3, mm2
	movq    [rdi+r8*8], mm3
	psrlq   mm4, mm0
	movq    [rdi+r8*8+8], mm4
	emms
	ret
end:
	movq    [rdi+r8*8], mm3
	emms
	ret
