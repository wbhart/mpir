
;  AMD64 mpn_lshift -- mpn left shift
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
;  AMD64 mpn_lshift -- mpn left shift
;
;  Calling interface:
;
; mp_limb_t mpn_lshift(
;     mp_ptr rcx,       rcx
;     mp_srcptr rdx,    rdx
;     mp_size_t size,    r8
;     unsigned shift     r9
; )
;
;  This is an SEH leaf function (no unwind support needed)

%include "..\..\yasm_mac.inc"

    BITS 64

    LEAF_PROC mpn_lshift
    mov     r10, rcx
    mov     ecx, r9d
    cmp     r8d, 2
    ja      threeormore
    jz      two

one:mov     rdx, [rdx]
    mov     rax, rdx
    shl     rdx, cl
    neg     rcx
    shr     rax, cl
    mov     [r10], rdx
    ret

two:mov     r8, [rdx]
    mov     r9, [rdx+8]
    mov     r11, r8
    mov     rax, r9
    shl     r8, cl
    shl     r9, cl
    neg     rcx
    shr     r11, cl
    shr     rax, cl
    or      r9, r11
    mov     [r10], r8
    mov     [r10+8], r9
    ret

threeormore:
    mov     r8d, r8d
    mov     eax, 64
    sub     rax, rcx
    movq    xmm0, rcx
    movq    xmm1, rax
    lea     r9, [rdx+r8*8-16]
    mov     r11, r9
    and     r9, -16
    movdqa  xmm3, [r9]
    movdqa  xmm5, xmm3
    psrlq   xmm3, xmm1
    pshufd  xmm3, xmm3, 0x4e
    movq    rax, xmm3
    cmp     r11, r9
    je      .1
    movq    xmm2, [rdx+r8*8-8]
    movq    xmm4, xmm2
    psrlq   xmm2, xmm1
    movq    rax, xmm2
    psllq   xmm4, xmm0
    por     xmm4, xmm3
    movq    [r10+r8*8-8], xmm4
    dec     r8
.1: sub     r8, 5
    jle     .3

    alignb  16, nop
.2: movdqa  xmm2, [rdx+r8*8+8]
    movdqa  xmm4, xmm2
    psllq   xmm5, xmm0
    psrlq   xmm2, xmm1
    movhlps xmm3, xmm2
    por     xmm5, xmm3
    movq    [r10+r8*8+24], xmm5
    pshufd  xmm2, xmm2, 0x4e
    movhpd  [r10+r8*8+32], xmm5
    movdqa  xmm3, [rdx+r8*8-8]
    movdqa  xmm5, xmm3
    psrlq   xmm3, xmm1
    movhlps xmm2, xmm3
    psllq   xmm4, xmm0
    pshufd  xmm3, xmm3, 0x4e
    por     xmm4, xmm2
    movq    [r10+r8*8+8], xmm4
    movhpd  [r10+r8*8+16], xmm4
    sub     r8, 4
    jg      .2
.3: cmp     r8, -1
    je      .42
    jg      .43
    jp      .41

.40:pxor    xmm2, xmm2
    psllq   xmm5, xmm0
    movhlps xmm3, xmm2
    por     xmm5, xmm3
    movq    [r10+r8*8+24], xmm5
    movhpd  [r10+r8*8+32], xmm5
    ret

    alignb  16, nop
.43:movdqa  xmm2, [rdx+r8*8+8]
    movdqa  xmm4, xmm2
    psllq   xmm5, xmm0
    psrlq   xmm2, xmm1
    movhlps xmm3, xmm2
    por     xmm5, xmm3
    movq    [r10+r8*8+24], xmm5
    pshufd  xmm2, xmm2, 0x4e
    movhpd  [r10+r8*8+32], xmm5
    movq    xmm3, [rdx+r8*8]
    pshufd  xmm3, xmm3, 0x4e
    movdqa  xmm5, xmm3
    psrlq   xmm3, xmm1
    movhlps xmm2, xmm3
    psllq   xmm4, xmm0
    por     xmm4, xmm2
    movq    [r10+r8*8+8], xmm4
    movhpd  [r10+r8*8+16], xmm4
    psllq   xmm5, xmm0
    movhpd  [r10+r8*8], xmm5
    ret

    alignb  16, nop
.42:movdqa  xmm2, [rdx+r8*8+8]
    movdqa  xmm4, xmm2
    psllq   xmm5, xmm0
    psrlq   xmm2, xmm1
    movhlps xmm3, xmm2
    por     xmm5, xmm3
    movq    [r10+r8*8+24], xmm5
    pshufd  xmm2, xmm2, 0x4e
    movhpd  [r10+r8*8+32], xmm5
    pxor    xmm3, xmm3
    movhlps xmm2, xmm3
    psllq   xmm4, xmm0
    por     xmm4, xmm2
    movq    [r10+r8*8+8], xmm4
    movhpd  [r10+r8*8+16], xmm4
    ret

    alignb  16, nop
.41:movq    xmm2, [rdx+r8*8+16]
    pshufd  xmm2, xmm2, 0x4e
    movdqa  xmm4, xmm2
    psllq   xmm5, xmm0
    psrlq   xmm2, xmm1
    movhlps xmm3, xmm2
    por     xmm5, xmm3
    movq    [r10+r8*8+24], xmm5
    movhpd  [r10+r8*8+32], xmm5
    psllq   xmm4, xmm0
    movhpd  [r10+r8*8+16], xmm4
    ret

    end
