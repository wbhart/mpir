; PROLOGUE(mpn_sublsh1_n)

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
;  mp_limb_t mpn_sublsh1_n(mp_ptr, mp_ptr, mp_ptr, mp_size_t)
;  rax                        rdi     rsi     rdx        rcx  
;  rax                        rcx     rdx      r8         r9

%include "yasm_mac.inc"

%define reg_save_list rbx, rsi, rdi

    CPU  Athlon64
    BITS 64

    FRAME_PROC mpn_sublsh1_n, 0, reg_save_list
    mov     rax, r9

    lea     rdi, [rcx+rax*8-56]
    lea     rsi, [rdx+rax*8-56]
    lea     rdx, [ r8+rax*8-56]
    mov     rcx, rax

	xor     rax, rax
	xor     r10, r10
	mov     r8, 3
	sub     r8, rcx
	jge     .3
	add     r8, 4
	mov     r11, [rsi+r8*8+24]
	mov     rcx, [rsi+r8*8+16]
	mov     r9, [rsi+r8*8]
	mov     rbx, [rsi+r8*8+8]
	jc      .2
	
	xalign   16
.1: add     rax, 1
	sbb     r9, [rdx+r8*8]
	sbb     rbx, [rdx+r8*8+8]
	sbb     rcx, [rdx+r8*8+16]
	sbb     r11, [rdx+r8*8+24]
	sbb     rax, rax
	add     r10, 1
	sbb     r9, [rdx+r8*8]
	sbb     rbx, [rdx+r8*8+8]
	sbb     rcx, [rdx+r8*8+16]
	sbb     r11, [rdx+r8*8+24]
	sbb     r10, r10
	mov     [rdi+r8*8], r9
	mov     [rdi+r8*8+8], rbx
	mov     [rdi+r8*8+16], rcx
	mov     [rdi+r8*8+24], r11
	mov     r11, [rsi+r8*8+56]
	mov     rcx, [rsi+r8*8+48]
	add     r8, 4
	mov     r9, [rsi+r8*8]
	mov     rbx, [rsi+r8*8+8]
	jnc     .1
.2: add     rax, 1
	sbb     r9, [rdx+r8*8]
	sbb     rbx, [rdx+r8*8+8]
	sbb     rcx, [rdx+r8*8+16]
	sbb     r11, [rdx+r8*8+24]
	sbb     rax, rax
	add     r10, 1
	sbb     r9, [rdx+r8*8]
	sbb     rbx, [rdx+r8*8+8]
	sbb     rcx, [rdx+r8*8+16]
	sbb     r11, [rdx+r8*8+24]
	sbb     r10, r10
	mov     [rdi+r8*8], r9
	mov     [rdi+r8*8+8], rbx
	mov     [rdi+r8*8+16], rcx
	mov     [rdi+r8*8+24], r11
.3: cmp     r8, 2
	ja      .7
	jz      .6
	jp      .5
.4: mov     rcx, [rsi+r8*8+48]
	mov     r9, [rsi+r8*8+32]
	mov     rbx, [rsi+r8*8+40]
	add     rax, 1
	sbb     r9, [rdx+r8*8+32]
	sbb     rbx, [rdx+r8*8+40]
	sbb     rcx, [rdx+r8*8+48]
	sbb     rax, rax
	add     r10, 1
	sbb     r9, [rdx+r8*8+32]
	sbb     rbx, [rdx+r8*8+40]
	sbb     rcx, [rdx+r8*8+48]
	mov     [rdi+r8*8+32], r9
	mov     [rdi+r8*8+40], rbx
	mov     [rdi+r8*8+48], rcx
	sbb     rax, 0
	neg     rax
	EXIT_PROC reg_save_list
	
	xalign   16
.5: mov     r9, [rsi+r8*8+32]
	mov     rbx, [rsi+r8*8+40]
	add     rax, 1
	sbb     r9, [rdx+r8*8+32]
	sbb     rbx, [rdx+r8*8+40]
	sbb     rax, rax
	add     r10, 1
	sbb     r9, [rdx+r8*8+32]
	sbb     rbx, [rdx+r8*8+40]
	mov     [rdi+r8*8+32], r9
	mov     [rdi+r8*8+40], rbx
	sbb     rax, 0
	neg     rax
	EXIT_PROC reg_save_list
	
	xalign   16
.6: mov     r9, [rsi+r8*8+32]
	add     rax, 1
	sbb     r9, [rdx+r8*8+32]
	sbb     rax, rax
	add     r10, 1
	sbb     r9, [rdx+r8*8+32]
	mov     [rdi+r8*8+32], r9
	sbb     rax, 0
	neg     rax
	EXIT_PROC reg_save_list
	
	xalign   16
.7: add     r10, 1
.8: sbb     rax, 0
	neg     rax
	END_PROC reg_save_list

	end
	