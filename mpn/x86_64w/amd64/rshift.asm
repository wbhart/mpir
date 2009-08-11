
;  AMD64 mpn_rshift -- mpn right shift
;  Verdxon 1.0.3.
;
;  Copyright 2008 Jason Moxham
;
;  Windows Converdxon Copyright 2008 Brian Gladman
;
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
;  Calling interface:
;
; mp_limb_t mpn_rshift(
;     mp_ptr dst,        r8
;     mp_srcptr src,    rdx
;     mp_size_t size,    r8
;     unsigned shift     r9
; )
;
;  This is an SEH leaf function (no unwind support needed)

%include "..\yasm_mac.inc"

    CPU  Athlon64
    BITS 64

    LEAF_PROC mpn_rshift
    mov     r9d, r9d	
	movq    mm0, r9
	mov     rax, 64
	sub     rax, r9
	movq    mm1, rax
    movsxd  rax, r8d
	mov     r8, 4	
	lea     rdx, [rdx+rax*8-32]
	lea     rcx, [rcx+rax*8-32]
	sub     r8, rax
	
	movq    mm5, [rdx+r8*8]
	movq    mm3, mm5
	psllq   mm5, mm1
	movq    rax, mm5
	psrlq   mm3, mm0
	jge     .1
	
	xalign  16
.0: movq    mm2, [rdx+r8*8+8]
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
	jnc     .0

.1:	cmp     r8, 2
	ja      .5
	jz      .4
	jp      .3

.2:	movq    mm2, [rdx+r8*8+8]
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
.3: movq    mm2, [rdx+r8*8+8]
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
.4: movq    mm2, [rdx+r8*8+8]
	movq    mm4, mm2
	psllq   mm2, mm1
	por     mm3, mm2
	movq    [rcx+r8*8], mm3
	psrlq   mm4, mm0
	movq    [rcx+r8*8+8], mm4
	emms
	ret
	
	xalign  16
.5: movq    [rcx+r8*8], mm3
	emms
	ret
	win64_gcc_end