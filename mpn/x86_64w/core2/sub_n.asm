; PROLOGUE(mpn_sub_n)

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
;  The MPIR Library is distributed in the hope that it will be useful, but
;  WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
;  or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public
;  License for more details.
;  You should have received a copy of the GNU Lesser General Public License
;  along with the MPIR Library; see the file COPYING.LIB.  If not, write
;  to the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
;  Boston, MA 02110-1301, USA.
;
;  mp_limb_t  mpn_sub_n(mp_ptr, mp_srcptr, mp_srcptr, mp_size_t)
;  mp_limb_t mpn_sub_nc(mp_ptr, mp_srcptr, mp_srcptr, mp_size_t, mp_limb_t)
;  rax                     rdi        rsi        rdx        rcx         r8
;  rax                     rcx        rdx         r8         r9   [rsp+40]

%include "yasm_mac.inc"

%define TR2 r10
%define TR4 r10

    CPU  Core2
    BITS 64

	LEAF_PROC mpn_sub_nc
    mov     r10, [rsp+0x28]
    jmp     mpn_sub_entry

	LEAF_PROC mpn_sub_n
	xor     r10, r10

mpn_sub_entry:
	mov     rax, r9
	mov     r9, rcx
	mov     rcx, rax
	and     rax, 3
	sub     rcx, rax
	lea     r9, [r9+rcx*8]
	lea     rdx, [rdx+rcx*8]
	lea     r8, [r8+rcx*8]
	neg     rcx
	lea     rcx, [r10+rcx*2]
	sar     rcx, 1
	jz      .2

	xalign  16
.1:	mov     r10, [rdx+rcx*8]
	mov     r11, [rdx+rcx*8+16]
	sbb     r10, [r8+rcx*8]
	mov     [r9+rcx*8], r10
	mov     TR2, [rdx+rcx*8+8]
	sbb     TR2, [r8+rcx*8+8]
	mov     [r9+rcx*8+8], TR2
	lea     rcx, [rcx+4]
	mov     TR4, [rdx+rcx*8-8]
	sbb     r11, [r8+rcx*8-16]
	sbb     TR4, [r8+rcx*8-8]
	mov     [r9+rcx*8-16], r11
	mov     [r9+rcx*8-8], TR4
	jrcxz   .2
	jmp     .1
.2:	sbb     rcx, rcx
.3:	cmp     rax, 2
	ja      .6
	jz      .7
	jp      .5
.4:	sub     rax, rcx
	ret
	
	xalign  16
.5:	add     rcx, rcx
	mov     r10, [rdx]
	sbb     r10, [r8]
	mov     [r9], r10
	sbb     rax, rax
	neg     rax
	ret
	
	xalign  16
.6:	add     rcx, rcx
	mov     r10, [rdx]
	mov     r11, [rdx+16]
	sbb     r10, [r8]
	mov     [r9], r10
	mov     TR2, [rdx+8]
	sbb     TR2, [r8+8]
	mov     [r9+8], TR2
	sbb     r11, [r8+16]
	mov     [r9+16], r11
	sbb     rax, rax
	neg     rax
	ret
	
	xalign  16
.7:	add     rcx, rcx
	mov     r10, [rdx]
	sbb     r10, [r8]
	mov     [r9], r10
	mov     TR2, [rdx+8]
	sbb     TR2, [r8+8]
	mov     [r9+8], TR2
	sbb     rax, rax
	neg     rax
	ret

	end
