; PROLOGUE(mpn_rshift2)

;  Copyright 2009 Jason Moxham
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
;  mp_limb_t mpn_rshift2(mp_ptr, mp_ptr, mp_size_t)
;  rax                      rdi     rsi        rdx
;  rax                      rcx     rdx         r8

%include "yasm_mac.inc"

%define reg_save_list rsi, rdi

    CPU  Athlon64
    BITS 64

    FRAME_PROC mpn_rshift2, 0, reg_save_list
	lea     rsi, [rdx+24]
	lea     rdi, [rcx+24]
    mov     rcx, r8
	xor     eax, eax
	xor     edx, edx
	sub     rcx, 4
	jc      .2
	
	xalign  16
.1:	mov     r8, [rsi+rcx*8]
	mov     r9, [rsi+rcx*8-8]
	mov     r10, [rsi+rcx*8-16]
	mov     r11, [rsi+rcx*8-24]
	add     rax, rax
	rcr     r8, 1
	rcr     r9, 1
	rcr     r10, 1
	rcr     r11, 1
	sbb     rax, rax
	add     rdx, rdx
	rcr     r8, 1
	rcr     r9, 1
	rcr     r10, 1
	rcr     r11, 1
	mov     [rdi+rcx*8-24], r11
	sbb     rdx, rdx
	mov     [rdi+rcx*8], r8
	sub     rcx, 4
	mov     [rdi+rcx*8+24], r9
	mov     [rdi+rcx*8+16], r10
	jnc     .1
.2:	cmp     rcx, -2
	ja      .4
	je      .5
	jp      .6
.3:	lea     rax, [rax+rdx*2]
	neg     rax
	shl     rax, 62
	EXIT_PROC reg_save_list
	
	xalign  16
.4:	mov     r8, [rsi+rcx*8]
	mov     r9, [rsi+rcx*8-8]
	mov     r10, [rsi+rcx*8-16]
	add     rax, rax
	rcr     r8, 1
	rcr     r9, 1
	rcr     r10, 1
	sbb     rax, rax
	add     rdx, rdx
	rcr     r8, 1
	rcr     r9, 1
	rcr     r10, 1
	sbb     rdx, rdx
	mov     [rdi+rcx*8], r8
	mov     [rdi+rcx*8-8], r9
	mov     [rdi+rcx*8-16], r10
	lea     rax, [rax+rdx*2]
	neg     rax
	shl     rax, 62
	EXIT_PROC reg_save_list
	
	xalign  16
.5:	mov     r8, [rsi+rcx*8]
	mov     r9, [rsi+rcx*8-8]
	add     rax, rax
	rcr     r8, 1
	rcr     r9, 1
	sbb     rax, rax
	add     rdx, rdx
	rcr     r8, 1
	rcr     r9, 1
	sbb     rdx, rdx
	mov     [rdi+rcx*8], r8
	mov     [rdi+rcx*8-8], r9
	lea     rax, [rax+rdx*2]
	neg     rax
	shl     rax, 62
	EXIT_PROC reg_save_list
	
	xalign  16
.6:	mov     r8, [rsi+rcx*8]
	add     rax, rax
	rcr     r8, 1
	sbb     rax, rax
	add     rdx, rdx
	rcr     r8, 1
	sbb     rdx, rdx
	mov     [rdi+rcx*8], r8
	lea     rax, [rax+rdx*2]
	neg     rax
	shl     rax, 62
.7:	END_PROC reg_save_list

	end
