; PROLOGUE(mpn_addadd_n)

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
;  mp_limb_t mpn_addadd_n(mp_ptr, mp_ptr, mp_ptr, mp_ptr, mp_size_t)
;  rax                       rdi     rsi     rdx     rcx         r8
;  rax                       rcx     rdx      r8      r9   [rsp+40]

%include "yasm_mac.inc"

%define reg_save_list   rbx, rbp, rsi, rdi

    CPU  Athlon64
    BITS 64

    FRAME_PROC mpn_addadd_n, 0, reg_save_list
    mov     rbx, qword [rsp+stack_use+40]
	lea     rdi, [rcx+rbx*8-56]
	lea     rsi, [rdx+rbx*8-56]
	lea     rdx, [r8+rbx*8-56]
	lea     rcx, [r9+rbx*8-56]
	mov     r9, 3
	xor     rax, rax
	xor     r10, r10
	sub     r9, rbx
	jge     .3
	add     r9, 4
	mov     rbp, [rdx+r9*8+16]
	mov     r11, [rdx+r9*8+24]
	mov     r8, [rdx+r9*8]
	mov     rbx, [rdx+r9*8+8]
	jc      .2
	
	xalign  16
.1:	add     rax, 1
	adc     r8, [rcx+r9*8]
	adc     rbx, [rcx+r9*8+8]
	adc     rbp, [rcx+r9*8+16]
	adc     r11, [rcx+r9*8+24]
	sbb     rax, rax
	add     r10, 1
	adc     r8, [rsi+r9*8]
	adc     rbx, [rsi+r9*8+8]
	adc     rbp, [rsi+r9*8+16]
	adc     r11, [rsi+r9*8+24]
	sbb     r10, r10
	mov     [rdi+r9*8], r8
	mov     [rdi+r9*8+24], r11
	mov     [rdi+r9*8+8], rbx
	mov     [rdi+r9*8+16], rbp
	mov     rbp, [rdx+r9*8+48]
	mov     r11, [rdx+r9*8+56]
	add     r9, 4
	mov     r8, [rdx+r9*8]
	mov     rbx, [rdx+r9*8+8]
	jnc     .1
.2:	add     rax, 1
	adc     r8, [rcx+r9*8]
	adc     rbx, [rcx+r9*8+8]
	adc     rbp, [rcx+r9*8+16]
	adc     r11, [rcx+r9*8+24]
	sbb     rax, rax
	add     r10, 1
	adc     r8, [rsi+r9*8]
	adc     rbx, [rsi+r9*8+8]
	adc     rbp, [rsi+r9*8+16]
	adc     r11, [rsi+r9*8+24]
	sbb     r10, r10
	mov     [rdi+r9*8], r8
	mov     [rdi+r9*8+24], r11
	mov     [rdi+r9*8+8], rbx
	mov     [rdi+r9*8+16], rbp
.3:	cmp     r9, 2
	ja      .7
	jz      .6
	jp      .5
.4:	mov     rbp, [rdx+r9*8+48]
	mov     r8, [rdx+r9*8+32]
	mov     rbx, [rdx+r9*8+40]
	add     rax, 1
	adc     r8, [rcx+r9*8+32]
	adc     rbx, [rcx+r9*8+40]
	adc     rbp, [rcx+r9*8+48]
	sbb     rax, rax
	add     r10, 1
	adc     r8, [rsi+r9*8+32]
	adc     rbx, [rsi+r9*8+40]
	adc     rbp, [rsi+r9*8+48]
	mov     [rdi+r9*8+32], r8
	mov     [rdi+r9*8+40], rbx
	mov     [rdi+r9*8+48], rbp
	sbb     rax, 0
	neg     rax
    EXIT_PROC reg_save_list
	
	xalign  16
.5:	mov     r8, [rdx+r9*8+32]
	mov     rbx, [rdx+r9*8+40]
	add     rax, 1
	adc     r8, [rcx+r9*8+32]
	adc     rbx, [rcx+r9*8+40]
	sbb     rax, rax
	add     r10, 1
	adc     r8, [rsi+r9*8+32]
	adc     rbx, [rsi+r9*8+40]
	mov     [rdi+r9*8+32], r8
	mov     [rdi+r9*8+40], rbx
	sbb     rax, 0
	neg     rax
    EXIT_PROC reg_save_list
	
	xalign  16
.6:	mov     r8, [rdx+r9*8+32]
	add     rax, 1
	adc     r8, [rcx+r9*8+32]
	sbb     rax, rax
	add     r10, 1
	adc     r8, [rsi+r9*8+32]
	mov     [rdi+r9*8+32], r8
	sbb     rax, 0
	neg     rax
    EXIT_PROC reg_save_list
	
	xalign  16
.7:	add     rax, r10
.8:	neg     rax
    END_PROC reg_save_list
    
	end
