; PROLOGUE(mpn_lshift1)

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
;  mp_limb_t mpn_lshift1(mp_ptr, mp_ptr, mp_size_t)
;  rax                      rdi     rsi        rdx
;  rax                      rcx     rdx         r8

%include "yasm_mac.inc"

    CPU  Athlon64
    BITS 64

    LEAF_PROC mpn_lshift1
    mov     rax, r8
	and     r8, 7
	inc     r8
	mov     [rsp+0x18], r8
	shr     rax, 3
	cmp     rax, 0
	jz      .2
	
	xalign  16
.1:	mov     r8, [rdx]
	mov     r9, [rdx+8]
	mov     r10, [rdx+16]
	mov     r11, [rdx+24]
	adc     r8, r8
	adc     r9, r9
	adc     r10, r10
	adc     r11, r11
	mov     [rcx], r8
	mov     [rcx+8], r9
	mov     [rcx+16], r10
	mov     [rcx+24], r11
	mov     r8, [rdx+32]
	mov     r9, [rdx+40]
	mov     r10, [rdx+48]
	mov     r11, [rdx+56]
	adc     r8, r8
	adc     r9, r9
	adc     r10, r10
	adc     r11, r11
	mov     [rcx+32], r8
	mov     [rcx+40], r9
	mov     [rcx+48], r10
	mov     [rcx+56], r11
	lea     rcx, [rcx+64]
	dec     rax
	lea     rdx, [rdx+64]
	jnz     .1
.2:	mov     rax, [rsp+0x18]
	dec     rax
	jz      .3
;	Could still have cache-bank conflicts in this tail part
	mov     r8, [rdx]
	adc     r8, r8
	mov     [rcx], r8
	dec     rax
	jz      .3
	mov     r8, [rdx+8]
	adc     r8, r8
	mov     [rcx+8], r8
	dec     rax
	jz      .3
	mov     r8, [rdx+16]
	adc     r8, r8
	mov     [rcx+16], r8
	dec     rax
	jz      .3
	mov     r8, [rdx+24]
	adc     r8, r8
	mov     [rcx+24], r8
	dec     rax
	jz      .3
	mov     r8, [rdx+32]
	adc     r8, r8
	mov     [rcx+32], r8
	dec     rax
	jz      .3
	mov     r8, [rdx+40]
	adc     r8, r8
	mov     [rcx+40], r8
	dec     rax
	jz      .3
	mov     r8, [rdx+48]
	adc     r8, r8
	mov     [rcx+48], r8
.3:	sbb     rax, rax
	neg     rax
	ret

	end
