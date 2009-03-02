
;  Core2 mpn_rshift -- mpn right shift
;  Version 1.0.4.
;
;  Copyright 2008 Jason Moxham
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
;  Calling interface:
;
; mp_limb_t mpn_rshift(
;     mp_ptr dst,       rcx
;     mp_srcptr src,    rdx
;     mp_size_t size,    r8
;     unsigned shift     r9
; )
;
;  This is an SEH leaf function (no unwind support needed)

   bits 64
   section .text

   global __gmpn_rshift

%ifdef DLL
   export   __gmpn_rshift
%endif

__gmpn_rshift:
    mov     r10, rcx
    mov     ecx, r9d
	cmp     r8d, 2
	ja      threeormore
	jz      two

one:mov     rdx, [rdx]
	mov     rax, rdx
	shr     rdx, cl
	neg     rcx
	shl     rax, cl
	mov     [r10], rdx
	ret

two:mov     r8, [rdx]
	mov     r9, [rdx+8]
	mov     rax, r8
	mov     r11, r9
	shr     r8, cl
	shr     r9, cl
	neg     rcx
	shl     r11, cl
	shl     rax, cl
	or      r8, r11
	mov     [r10], r8
	mov     [r10+8], r9
	ret

threeormore:
    mov     r11, rdx
	mov     edx, r8d
	
	mov     eax, 64
	lea     r9, [r11+8]
	sub     rax, rcx
	and     r9, -16
	movq    xmm0, rcx
	movq    xmm1, rax
	movdqa  xmm5, [r9]
	movdqa  xmm3, xmm5
	psllq   xmm5, xmm1
	movq    rax, xmm5
	cmp     r11, r9
	lea     r11, [r11+rdx*8-40]
	je      .1
	movq    xmm2, [r9-8]
	movq    xmm4, xmm2
	psllq   xmm2, xmm1
	psrlq   xmm4, xmm0
	por     xmm4, xmm5
	movq    [r10], xmm4
	lea     r10, [r10+8]
	dec     rdx
	movq    rax, xmm2
.1:	lea     r10, [r10+rdx*8-40]
	psrlq   xmm3, xmm0
	mov     r8d, 5
	sub     r8, rdx
	jnc     .3
	
	alignb  16, nop
.2: movdqa  xmm2, [r11+r8*8+16]
	movdqa  xmm4, xmm2
	psllq   xmm2, xmm1
	shufpd  xmm5, xmm2, 1
	por     xmm3, xmm5
	movq    [r10+r8*8], xmm3
	movhpd  [r10+r8*8+8], xmm3
	psrlq   xmm4, xmm0
	movdqa  xmm5, [r11+r8*8+32]
	movdqa  xmm3, xmm5
	psllq   xmm5, xmm1
	shufpd  xmm2, xmm5, 1
	psrlq   xmm3, xmm0
	por     xmm4, xmm2
	movq    [r10+r8*8+16], xmm4
	movhpd  [r10+r8*8+24], xmm4
	add     r8, 4
	jnc     .2
.3:	cmp     r8, 2
	ja      .40
	jz      .41
	jp      .42

.43:movdqa  xmm2, [r11+r8*8+16]
	movdqa  xmm4, xmm2
	psllq   xmm2, xmm1
	shufpd  xmm5, xmm2, 1
	por     xmm3, xmm5
	movq    [r10+r8*8], xmm3
	movhpd  [r10+r8*8+8], xmm3
	psrlq   xmm4, xmm0
	movq    xmm5, [r11+r8*8+32]
	movq    xmm3, xmm5
	psllq   xmm5, xmm1
	shufpd  xmm2, xmm5, 1
	psrlq   xmm3, xmm0
	por     xmm4, xmm2
	movq    [r10+r8*8+16], xmm4
	movhpd  [r10+r8*8+24], xmm4
	psrldq  xmm5, 8
	por     xmm3, xmm5
	movq    [r10+r8*8+32], xmm3
	ret
	
	alignb  16, nop
.42:movdqa  xmm2, [r11+r8*8+16]
	movdqa  xmm4, xmm2
	psllq   xmm2, xmm1
	shufpd  xmm5, xmm2, 1
	por     xmm3, xmm5
	movq    [r10+r8*8], xmm3
	movhpd  [r10+r8*8+8], xmm3
	psrlq   xmm4, xmm0
	psrldq  xmm2, 8
	por     xmm4, xmm2
	movq    [r10+r8*8+16], xmm4
	movhpd  [r10+r8*8+24], xmm4
	ret
	
	alignb  16, nop
.41:movq    xmm2, [r11+r8*8+16]
	movq    xmm4, xmm2
	psllq   xmm2, xmm1
	shufpd  xmm5, xmm2, 1
	por     xmm3, xmm5
	movq    [r10+r8*8], xmm3
	movhpd  [r10+r8*8+8], xmm3
	psrlq   xmm4, xmm0
	psrldq  xmm2, 8
	por     xmm4, xmm2
	movq    [r10+r8*8+16], xmm4
	ret
	
	alignb  16, nop
.40:psrldq  xmm5, 8
	por     xmm3, xmm5
	movq    [r10+r8*8], xmm3
	movhpd  [r10+r8*8+8], xmm3
	ret
	
	end
