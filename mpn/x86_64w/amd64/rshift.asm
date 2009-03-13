
;  AMD64 mpn_rshift -- mpn right shift
;  Version 1.0.3.
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

%include "..\yasm_mac.inc"

    BITS 64

    LEAF_PROC mpn_rshift
    movsxd  r8, r8d
    mov     r9d, r9d
    mov     eax, 64
    lea     rdx, [rdx+r8*8-32]
    lea     rcx, [rcx+r8*8-32]
    sub     rax, r9
    movq    mm0, r9
    mov     r9d, 4
    sub     r9, r8
    movq    mm1, rax
    movq    mm5, [rdx+r9*8]
    movq    mm3, mm5
    psllq   mm5, mm1
    movq    rax, mm5
    psrlq   mm3, mm0
    jnc     .2

    alignb  16, nop
.1: movq    mm2, [rdx+r9*8+8]
    movq    mm4, mm2
    psllq   mm2, mm1
    por     mm3, mm2
    movq    [rcx+r9*8], mm3
    psrlq   mm4, mm0
    movq    mm5, [rdx+r9*8+16]
    movq    mm3, mm5
    psllq   mm5, mm1
    por     mm4, mm5
    movq    [rcx+r9*8+8], mm4
    psrlq   mm3, mm0

; got room here for another jump out , if we can arrange our r9 to be
; slightly different , so we can use a jz or jp here

    movq    mm2, [rdx+r9*8+24]
    movq    mm4, mm2
    psllq   mm2, mm1
    por     mm3, mm2
    movq    [rcx+r9*8+16], mm3
    psrlq   mm4, mm0
    movq    mm5, [rdx+r9*8+32]
    movq    mm3, mm5
    psllq   mm5, mm1
    por     mm4, mm5
    movq    [rcx+r9*8+24], mm4
    psrlq   mm3, mm0
    add     r9, 4
    jnc     .1

; r9 is 0,1,2,3 here , so we have 3-r9 limbs to do

.2: test    r9, 2
    jnz     .3
    movq    mm2, [rdx+r9*8+8]
    movq    mm4, mm2
    psllq   mm2, mm1
    por     mm3, mm2
    movq    [rcx+r9*8], mm3
    psrlq   mm4, mm0
    movq    mm5, [rdx+r9*8+16]
    movq    mm3, mm5
    psllq   mm5, mm1
    por     mm4, mm5
    movq    [rcx+r9*8+8], mm4
    psrlq   mm3, mm0
    add     r9, 2

.3: test    r9, 1
    jnz     .4
    movq    mm2, [rdx+r9*8+8]
    movq    mm4, mm2
    psllq   mm2, mm1
    por     mm3, mm2
    movq    [rcx+r9*8], mm3
    psrlq   mm4, mm0
    movq    [rcx+r9*8+8], mm4
    emms
    ret

.4: movq    [rcx+r9*8], mm3
    emms
    ret

    end
