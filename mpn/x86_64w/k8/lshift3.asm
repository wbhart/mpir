; PROLOGUE(mpn_lshift3)

;  mpn_lshift3

;  Copyright 2009 Jason Moxham

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

;  mp_limb_t mpn_lshift3(mp_ptr, mp_ptr, mp_size_t)
;  rax                      rdi     rsi        rdx
;  rax                      rcx     rdx         r8

%include "yasm_mac.inc"

%define reg_save_list rsi, rdi

    CPU  Athlon64
    BITS 64

    FRAME_PROC mpn_lshift3, 0, reg_save_list
    mov     rax, r8
	lea     rsi, [rdx+rax*8-24]
	lea     rdi, [rcx+rax*8-24]
	mov     ecx, 3
	sub     rcx, rax
	mov     r8, 0
	jnc     .2
	
	xalign  16
.1:	mov     r9, [rsi+rcx*8]
	lea     r8, [r8+r9*8]
	shr     r9, 61
	mov     r10, [rsi+rcx*8+8]
	lea     r9, [r9+r10*8]
	shr     r10, 61
	mov     r11, [rsi+rcx*8+16]
	mov     [rdi+rcx*8], r8
	lea     r10, [r10+r11*8]
	mov     [rdi+rcx*8+16], r10
	shr     r11, 61
	mov     r8, [rsi+rcx*8+24]
	lea     r11, [r11+r8*8]
	mov     [rdi+rcx*8+24], r11
	shr     r8, 61
	add     rcx, 4
	mov     [rdi+rcx*8+8-32], r9
	jnc     .1
.2:	cmp     rcx, 2
	ja      .6
	je      .5
	jp      .4
.3:	mov     r9, [rsi+rcx*8]
	lea     r8, [r8+r9*8]
	shr     r9, 61
	mov     r10, [rsi+rcx*8+8]
	lea     r9, [r9+r10*8]
	shr     r10, 61
	mov     r11, [rsi+rcx*8+16]
	mov     [rdi+rcx*8], r8
	lea     r10, [r10+r11*8]
	mov     [rdi+rcx*8+16], r10
	shr     r11, 61
	mov     rax, r11
	mov     [rdi+rcx*8+8], r9
	jmp		.7
	
	xalign  16
.4:	mov     r9, [rsi+rcx*8]
	lea     r8, [r8+r9*8]
	shr     r9, 61
	mov     r10, [rsi+rcx*8+8]
	lea     r9, [r9+r10*8]
	shr     r10, 61
	mov     [rdi+rcx*8], r8
	mov     rax, r10
	mov     [rdi+rcx*8+8], r9
    EXIT_PROC reg_save_list
	
	xalign  16
.5:	mov     r9, [rsi+rcx*8]
	lea     r8, [r8+r9*8]
	shr     r9, 61
	mov     [rdi+rcx*8], r8
	mov     rax, r9
    EXIT_PROC reg_save_list

	xalign  16
.6:	mov     rax, r8
.7:	END_PROC reg_save_list

    end
