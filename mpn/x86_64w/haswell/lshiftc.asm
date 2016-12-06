; PROLOGUE(mpn_lshiftc)

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
;  void lshiftc(mp_ptr, mp_ptr, mp_size_t, mp_uint)
;                  rdi     rsi        rdx      rcx
;                  rcx     rdx         r8      r9d

%include "yasm_mac.inc"

    CPU  Athlon64
    BITS 64

    LEAF_PROC mpn_lshiftc
	mov     r9d, r9d
	movq    mm0, r9
	mov     rax, 64
	sub     rax, r9
	pcmpeqb mm6, mm6
	movq    mm1, rax
	lea     rdx, [rdx+8]
	lea     rcx, [rcx+8]
	sub     r8, 5
	movq    mm5, [rdx+r8*8+24]
	movq    mm3, mm5
	psrlq   mm5, mm1
	movq    rax, mm5
	psllq   mm3, mm0
	jc      .2
	
	xalign  16
.1: movq    mm2, [rdx+r8*8+16]
	movq    mm4, mm2
	psrlq   mm2, mm1
	por     mm3, mm2
	movq    mm2, [rdx+r8*8]
	pxor    mm3, mm6
	movq    [rcx+r8*8+24], mm3
	psllq   mm4, mm0
	movq    mm5, [rdx+r8*8+8]
	movq    mm3, mm5
	psrlq   mm5, mm1
	por     mm4, mm5
	pxor    mm4, mm6
	movq    [rcx+r8*8+16], mm4
	psllq   mm3, mm0
	movq    mm4, mm2
	movq    mm5, [rdx+r8*8-8]
	sub     r8, 4
	psrlq   mm2, mm1
	por     mm3, mm2
	pxor    mm3, mm6
	movq    [rcx+r8*8+40], mm3
	psllq   mm4, mm0
	movq    mm3, mm5
	psrlq   mm5, mm1
	por     mm4, mm5
	pxor    mm4, mm6
	movq    [rcx+r8*8+32], mm4
	psllq   mm3, mm0
	jnc     .1
.2: cmp     r8, -2
	jz      .4
	jp      .5
	js      .6
.3:	movq    mm2, [rdx+r8*8+16]
	movq    mm4, mm2
	psrlq   mm2, mm1
	por     mm3, mm2
	movq    mm2, [rdx+r8*8]
	pxor    mm3, mm6
	movq    [rcx+r8*8+24], mm3
	psllq   mm4, mm0
	movq    mm5, [rdx+r8*8+8]
	movq    mm3, mm5
	psrlq   mm5, mm1
	por     mm4, mm5
	pxor    mm4, mm6
	movq    [rcx+r8*8+16], mm4
	psllq   mm3, mm0
	movq    mm4, mm2
	psrlq   mm2, mm1
	por     mm3, mm2
	pxor    mm3, mm6
	movq    [rcx+r8*8+8], mm3
	psllq   mm4, mm0
	pxor    mm4, mm6
	movq    [rcx+r8*8], mm4
	emms
	ret

	xalign  16
.4:	movq    mm2, [rdx+r8*8+16]
	movq    mm4, mm2
	psrlq   mm2, mm1
	por     mm3, mm2
	pxor    mm3, mm6
	movq    [rcx+r8*8+24], mm3
	psllq   mm4, mm0
	movq    mm5, [rdx+r8*8+8]
	movq    mm3, mm5
	psrlq   mm5, mm1
	por     mm4, mm5
	pxor    mm4, mm6
	movq    [rcx+r8*8+16], mm4
	psllq   mm3, mm0
	pxor    mm3, mm6
	movq    [rcx+r8*8+8], mm3
	emms
	ret

	xalign  16
.5:	movq    mm2, [rdx+r8*8+16]
	movq    mm4, mm2
	psrlq   mm2, mm1
	por     mm3, mm2
	pxor    mm3, mm6
	movq    [rcx+r8*8+24], mm3
	psllq   mm4, mm0
	pxor    mm4, mm6
	movq    [rcx+r8*8+16], mm4
	emms
	ret

	xalign  16
.6:	pxor    mm3, mm6
	movq    [rcx+r8*8+24], mm3
	emms
	ret
	
