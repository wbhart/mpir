; PROLOGUE(mpn_half)

;  Copyright 2009 Jason Moxham
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
;  mp_limb_t mpn_half(mp_ptr, mp_size_t)
;                        rdi        rsi
;                        rcx        rdx

%include "yasm_mac.inc"

        CPU  Athlon64
        BITS 64

    	LEAF_PROC mpn_half

        mov     rax, rdx
        lea     rcx, [rcx+rdx*8-8]
        shr     rdx, 2
        and     eax, 3
        jz      .1
        shr     qword [rcx], 1
        lea     rcx, [rcx-8]
        dec     rax
        jz      .1
        rcr     qword [rcx], 1
        lea     rcx, [rcx-8]
        dec     rax
        jz      .1
        rcr     qword [rcx], 1
        lea     rcx, [rcx-8]
        dec     rax
.1:     sbb     r8, r8
        cmp     rdx, 0
        jz      .3
        add     r8, r8
        
        xalign   16
.2:     rcr     qword [rcx], 1
        nop     
        rcr     qword [rcx-8], 1
        rcr     qword [rcx-16], 1
        rcr     qword [rcx-24], 1
        nop     
        dec     rdx
        lea     rcx, [rcx-32]
        jnz     .2
        sbb     r8, r8
.3:     sub     rax, r8
        shl     rax, 63
        ret     

        end
