; PROLOGUE(mpn_rsh1add_n)

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
;  mp_limb_t mpn_rsh1add_n(mp_ptr, mp_ptr, mp_ptr, mp_size_t)
;  rax                        rdi     rsi     rdx        rcx
;  rax                        rcx     rdx      r8         r9

%include "yasm_mac.inc"

%define reg_save_list rbx, rsi, rdi, r12

    CPU  Athlon64
    BITS 64

    FRAME_PROC mpn_rsh1add_n, 0, reg_save_list
	mov     rax, r9
	lea     rdi, [rcx+rax*8-32]
	lea     rsi, [rdx+rax*8-32]
	lea     rdx, [r8+rax*8-32]
	mov     rcx, rax

	mov     r8, 4
	sub     r8, rcx
	mov     r12, [rsi+r8*8]
	add     r12, [rdx+r8*8]
	sbb     rbx, rbx
	mov     rax, r12
	and     rax, 1
	cmp     r8, 0
	jge     .2
.1:	mov     r11, [rsi+r8*8+32]
	mov     r10, [rsi+r8*8+24]
	add     rbx, 1
	mov     rcx, [rsi+r8*8+8]
	mov     r9, [rsi+r8*8+16]
	adc     rcx, [rdx+r8*8+8]
	adc     r9, [rdx+r8*8+16]
	adc     r10, [rdx+r8*8+24]
	adc     r11, [rdx+r8*8+32]
	sbb     rbx, rbx
	bt      r11, 0
	rcr     r10, 1
	rcr     r9, 1
	rcr     rcx, 1
	rcr     r12, 1
	mov     [rdi+r8*8], r12
	mov     [rdi+r8*8+8], rcx
	mov     [rdi+r8*8+16], r9
	mov     [rdi+r8*8+24], r10
	mov     r12, r11
	add     r8, 4
	jnc     .1
.2:	cmp     r8, 2
	ja      .6
	jz      .5
	jp      .4
.3:	mov     r10, [rsi+r8*8+24]
	add     rbx, 1
	mov     rcx, [rsi+r8*8+8]
	mov     r9, [rsi+r8*8+16]
	adc     rcx, [rdx+r8*8+8]
	adc     r9, [rdx+r8*8+16]
	adc     r10, [rdx+r8*8+24]
	rcr     r10, 1
	rcr     r9, 1
	rcr     rcx, 1
	rcr     r12, 1
	mov     [rdi+r8*8], r12
	mov     [rdi+r8*8+8], rcx
	mov     [rdi+r8*8+16], r9
	mov     [rdi+r8*8+24], r10
    EXIT_PROC reg_save_list
	
	xalign  16
.4:	add     rbx, 1
	mov     rcx, [rsi+r8*8+8]
	mov     r9, [rsi+r8*8+16]
	adc     rcx, [rdx+r8*8+8]
	adc     r9, [rdx+r8*8+16]
	rcr     r9, 1
	rcr     rcx, 1
	rcr     r12, 1
	mov     [rdi+r8*8], r12
	mov     [rdi+r8*8+8], rcx
	mov     [rdi+r8*8+16], r9
    EXIT_PROC reg_save_list
	
	xalign  16
.5:	add     rbx, 1
	mov     rcx, [rsi+r8*8+8]
	adc     rcx, [rdx+r8*8+8]
	rcr     rcx, 1
	rcr     r12, 1
	mov     [rdi+r8*8], r12
	mov     [rdi+r8*8+8], rcx
    EXIT_PROC reg_save_list
	
	xalign  16
.6:	add     rbx, 1
	rcr     r12, 1
	mov     [rdi+r8*8], r12
.7:	END_PROC reg_save_list
    
	end
