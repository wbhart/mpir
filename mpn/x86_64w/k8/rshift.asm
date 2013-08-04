; PROLOGUE(mpn_rshift)

;  Verdxon 1.1.4.
;
;  Copyright 2008 Jason Moxham
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
;
;  You should have received a copy of the GNU Lesser General Public License
;  along with the MPIR Library; see the file COPYING.LIB.  If not, write
;  to the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
;  Boston, MA 02110-1301, USA.
;
;  mp_limb_t mpn_rshift(mp_ptr, mp_ptr, mp_size_t, mp_uint)
;  rax                     rdi     rsi        rdx      rcx
;  rax                     rcx     rdx         r8      r9d

%include "yasm_mac.inc"

    CPU  Athlon64
    BITS 64

    LEAF_PROC mpn_rshift
    mov     r9d, r9d
	movq    mm0, r9
	mov     rax, 64
	sub     rax, r9
	movq    mm1, rax
    mov     rax, r8
	mov     r8, 4
	lea     rdx, [rdx+rax*8-32]
	lea     rcx, [rcx+rax*8-32]
	sub     r8, rax

	movq    mm5, [rdx+r8*8]
	movq    mm3, mm5
	psllq   mm5, mm1
	movq    rax, mm5
	psrlq   mm3, mm0
	jge     .2

	xalign  16
.1: movq    mm2, [rdx+r8*8+8]
	movq    mm4, mm2
	psllq   mm2, mm1
	por     mm3, mm2
	movq    [rcx+r8*8], mm3
	psrlq   mm4, mm0
	movq    mm5, [rdx+r8*8+16]
	movq    mm3, mm5
	psllq   mm5, mm1
	por     mm4, mm5
	movq    [rcx+r8*8+8], mm4
	psrlq   mm3, mm0
	movq    mm2, [rdx+r8*8+24]
	movq    mm4, mm2
	psllq   mm2, mm1
	por     mm3, mm2
	movq    mm5, [rdx+r8*8+32]
	movq    [rcx+r8*8+16], mm3
	psrlq   mm4, mm0
	movq    mm3, mm5
	psllq   mm5, mm1
	por     mm4, mm5
	movq    [rcx+r8*8+24], mm4
	psrlq   mm3, mm0
	add     r8, 4
	jnc     .1

.2:	cmp     r8, 2
	ja      .6
	jz      .5
	jp      .4

.3:	movq    mm2, [rdx+r8*8+8]
	movq    mm4, mm2
	psllq   mm2, mm1
	por     mm3, mm2
	movq    [rcx+r8*8], mm3
	psrlq   mm4, mm0
	movq    mm5, [rdx+r8*8+16]
	movq    mm3, mm5
	psllq   mm5, mm1
	por     mm4, mm5
	movq    [rcx+r8*8+8], mm4
	psrlq   mm3, mm0
	movq    mm2, [rdx+r8*8+24]
	movq    mm4, mm2
	psllq   mm2, mm1
	por     mm3, mm2
	movq    [rcx+r8*8+16], mm3
	psrlq   mm4, mm0
	movq    [rcx+r8*8+24], mm4
	emms
	ret

	xalign  16
.4: movq    mm2, [rdx+r8*8+8]
	movq    mm4, mm2
	psllq   mm2, mm1
	por     mm3, mm2
	movq    [rcx+r8*8], mm3
	psrlq   mm4, mm0
	movq    mm5, [rdx+r8*8+16]
	movq    mm3, mm5
	psllq   mm5, mm1
	por     mm4, mm5
	movq    [rcx+r8*8+8], mm4
	psrlq   mm3, mm0
	movq    [rcx+r8*8+16], mm3
	emms
	ret

	xalign  16
.5: movq    mm2, [rdx+r8*8+8]
	movq    mm4, mm2
	psllq   mm2, mm1
	por     mm3, mm2
	movq    [rcx+r8*8], mm3
	psrlq   mm4, mm0
	movq    [rcx+r8*8+8], mm4
	emms
	ret

	xalign  16
.6: movq    [rcx+r8*8], mm3
	emms
	ret
	end
	