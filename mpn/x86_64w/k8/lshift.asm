; PROLOGUE(mpn_lshift)

;  Version 1.0.3.
;
;  Copyright 2008 Jason Moxham
;
;  Windows Conversion Copyright 2008 Brian Gladman
;
;  This file is part of the MPIR Library.
;
;  The MPIR Library is free software; you can redistribute it and/or modify
;  it under the terms of the GNU Lesser General Public License as published
;  by the Free Software Foundation; either version 2.1 of the License, or (at
;  your option) any later version.
;
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
;  mp_limb_t mpn_lshift(mp_ptr, mp_ptr, mp_size_t, mp_uint)
;  rax                     rdi     rsi        rdx      rcx
;  rax                     rcx     rdx         r8      r9d

%include "yasm_mac.inc"

    CPU  Athlon64
    BITS 64

    LEAF_PROC mpn_lshift
    mov     r9d, r9d
    mov     eax, 64
    sub     rax, r9
    movq    mm0, r9
    sub     r8, 4
    movq    mm1, rax
    movq    mm5, [rdx+r8*8+24]
    movq    mm3, mm5
    psrlq   mm5, mm1
    movq    rax, mm5
    psllq   mm3, mm0
    jbe     .2

    xalign  16
.1:	movq    mm2, [rdx+r8*8+16]
    movq    mm4, mm2
    psrlq   mm2, mm1
    por     mm3, mm2
    movq    [rcx+r8*8+24], mm3
    psllq   mm4, mm0
    movq    mm5, [rdx+r8*8+8]
    movq    mm3, mm5
    psrlq   mm5, mm1
    por     mm4, mm5
    movq    [rcx+r8*8+16], mm4
    psllq   mm3, mm0
    movq    mm2, [rdx+r8*8]
    movq    mm4, mm2
    psrlq   mm2, mm1
    por     mm3, mm2
    movq    [rcx+r8*8+8], mm3
    psllq   mm4, mm0
    movq    mm5, [rdx+r8*8-8]
    movq    mm3, mm5
    psrlq   mm5, mm1
    por     mm4, mm5
    movq    [rcx+r8*8], mm4
    psllq   mm3, mm0
    sub     r8, 4
    ja      .1

; r8 is 0,-1,-2,-3 here , so we have 3+r8 limbs to do
.2: cmp     r8, -1
    jl      .3
    movq    mm2, [rdx+r8*8+16]
    movq    mm4, mm2
    psrlq   mm2, mm1
    por     mm3, mm2
    movq    [rcx+r8*8+24], mm3
    psllq   mm4, mm0
    movq    mm5, [rdx+r8*8+8]
    movq    mm3, mm5
    psrlq   mm5, mm1
    por     mm4, mm5
    movq    [rcx+r8*8+16], mm4
    psllq   mm3, mm0
    sub     r8, 2
.3: test    r8, 1
    jnz     .4
    movq    mm2, [rdx+r8*8+16]
    movq    mm4, mm2
    psrlq   mm2, mm1
    por     mm3, mm2
    movq    [rcx+r8*8+24], mm3
    psllq   mm4, mm0
    movq    [rcx+r8*8+16], mm4
    emms
    ret
.4: movq    [rcx+r8*8+24], mm3
    emms
    ret

    end
