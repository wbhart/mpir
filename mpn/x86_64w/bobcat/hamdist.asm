; PROLOGUE(mpn_hamdist)
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

;	mp_limb_t mpn_hamdist(mp_ptr, mp_ptr, mp_size_t)
;	rax                      rdi,    rdx,       rdx
;	rax                      rcx,    rdx,        r8

%include "yasm_mac.inc"

    CPU  SSE4.2
    BITS 64

    FRAME_PROC mpn_hamdist, 0, rdi
	xor     eax, eax
	lea     rdi, [rcx+r8*8-24]
	lea     rdx, [rdx+r8*8-24]
	mov     rcx, 3
	sub     rcx, r8
	jnc     .1
	xalign  16
.0:	mov     r8, [rdi+rcx*8]
	mov     r9, [rdi+rcx*8+8]
	xor     r8, [rdx+rcx*8]
	mov     r10, [rdi+rcx*8+16]
	popcnt  r8, r8
	xor     r9, [rdx+rcx*8+8]
	xor     r10, [rdx+rcx*8+16]
	popcnt  r9, r9
	mov     r11, [rdi+rcx*8+24]
	add     rax, r8
	popcnt  r10, r10
	xor     r11, [rdx+rcx*8+24]
	add     rax, r9
	popcnt  r11, r11
	add     rax, r10
	add     rax, r11
	add     rcx, 4
	jnc     .0
.1:
	cmp     rcx, 2
	ja      .5
	je      .4
	jp      .3
.2:	mov     r8, [rdi]
	xor     r8, [rdx]
	popcnt  r8, r8
	add     rax, r8
.3:	mov     r8, [rdi+8]
	xor     r8, [rdx+8]
	popcnt  r8, r8
	add     rax, r8
.4:	mov     r8, [rdi+16]
	xor     r8, [rdx+16]
	popcnt  r8, r8
	add     rax, r8
.5: END_PROC rdi

    end
