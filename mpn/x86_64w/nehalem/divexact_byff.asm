; PROLOGUE(mpn_divexact_byff)
;  Copyright 2009 Jason Moxham
;
;  Windows Conver11on Copyright 2009 Brian Gladman
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
;  mp_limb_t mpn_divexact_byff(mp_ptr, mp_ptr, mp_size_t)
;  rax                           rdi     rsi         rdx
;  rax                           rcx     rdx          r8

%include "yasm_mac.inc"

        CPU  nehalem
        BITS 64

        LEAF_PROC mpn_divexact_byff
        mov     r11, rdx
        mov     r10d, 3
        lea     r11, [r11+r8*8-24]
        lea     rcx, [rcx+r8*8-24]
        sub     r10, r8
        mov     r8, 0
        mov	    r9, 1
        jnc     .2
        
        align   16
.1:     mov     rax, [r11+r10*8]
	    mul     r9
	    sub     r8, rax
	    mov     [rcx+r10*8], r8
	    sbb     r8, rdx
	    mov     rax, [r11+r10*8+8]
	    mul     r9
	    sub     r8, rax
	    mov     [rcx+r10*8+8], r8
	    sbb     r8, rdx
	    mov     rax, [r11+r10*8+16]
	    mul     r9
	    sub     r8, rax
	    mov     [rcx+r10*8+16], r8
	    sbb     r8, rdx
	    mov     rax, [r11+r10*8+24]
	    mul     r9
	    sub     r8, rax
	    mov     [rcx+r10*8+24], r8
	    sbb     r8, rdx
	    add     r10, 4
	    jnc     .1
.2:     test    r10, 2
        jnz     .3
	    mov     rax, [r11+r10*8]
	    mul     r9
	    sub     r8, rax
	    mov     [rcx+r10*8], r8
	    sbb     r8, rdx
	    mov     rax, [r11+r10*8+8]
	    mul     r9
	    sub     r8, rax
	    mov     [rcx+r10*8+8], r8
	    sbb     r8, rdx
	    add     r10, 2
.3:     test    r10, 1
        jnz     .4
	    mov     rax, [r11+r10*8]
	    mul     r9
	    sub     r8, rax
	    mov     [rcx+r10*8], r8
	    sbb     r8, rdx
.4:     mov	    rcx,0xFFFFFFFFFFFFFFFF
        imul    r8, rcx
        mov     rax, r8
        neg     rax
        ret

        end
