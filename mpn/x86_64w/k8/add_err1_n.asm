; PROLOGUE(mpn_add_err1_n)

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
;
;  The MPIR Library is distributed in the hope that it will be useful, but
;  WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
;  or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public
;  License for more details.
;
;  You should have received a copy of the GNU Lesser General Public License
;  along with the MPIR Library; see the file COPYING.LIB.  If not, write
;  to the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
;  Boston, MA 02110-1301, USA.
;
;  mp_limb_t mpn_add_err1(mp_ptr, mp_ptr, mp_ptr, mp_ptr,  mp_ptr, mp_size_t, mp_limb_t)
;  rax                       rdi     rsi     rdx     rcx       r8         r9     8(rsp)
;  rax                       rcx     rdx      r8      r9 [rsp+40]   [rsp+48]   [rsp+56]

%include "yasm_mac.inc"

    CPU  Athlon64
    BITS 64

%define reg_save_list rbx, rsi, rdi, rbp, r12, r13, r14, r15

    FRAME_PROC mpn_add_err1_n, 0, reg_save_list
	mov     [rsp+stack_use+32], r9
    mov     r9, qword [rsp+stack_use+48]
	mov     r10, [rsp++stack_use+56]
	lea     rdi, [rcx+r9*8-24]
	lea     rsi, [rdx+r9*8-24]
	lea     rdx, [r8+r9*8-24]
	mov     r8, [rsp+stack_use+40]
	
	mov     r11, 3
	shl     r10, 63
	lea     r8, [r8+r9*8]
	sub     r11, r9
	mov     r9, 0
	mov     rax, 0
	mov     rbx, 0
	jnc     .2

	xalign  16
.1:	mov     r12, [rsi+r11*8]
	mov     r13, [rsi+r11*8+8]
	mov     r14, [rsi+r11*8+16]
	mov     r15, [rsi+r11*8+24]
	mov     rbp, 0
	shl     r10, 1
	adc     r12, [rdx+r11*8]
	cmovc   rax, [r8-8]
	adc     r13, [rdx+r11*8+8]
	cmovc   rbx, [r8-16]
	mov     rcx, 0
	adc     r14, [rdx+r11*8+16]
	cmovc   rcx, [r8-24]
	adc     r15, [rdx+r11*8+24]
	cmovc   rbp, [r8-32]
	rcr     r10, 1
	add     r9, rax
	adc     r10, 0
	add     r9, rbx
	adc     r10, 0
	add     r9, rcx
	mov     rax, 0
	adc     r10, 0
	lea     r8, [r8-32]
	add     r9, rbp
	adc     r10, 0
	mov     [rdi+r11*8], r12
	mov     [rdi+r11*8+8], r13
	mov     [rdi+r11*8+16], r14
	mov     [rdi+r11*8+24], r15
	mov     rbx, 0
	add     r11, 4
	jnc     .1
.2: cmp     r11, 2
	ja      .6
	je      .5
	jp      .4
.3: mov     r12, [rsi+r11*8]
	mov     r13, [rsi+r11*8+8]
	mov     r14, [rsi+r11*8+16]
	shl     r10, 1
	adc     r12, [rdx+r11*8]
	cmovc   rax, [r8-8]
	adc     r13, [rdx+r11*8+8]
	cmovc   rbx, [r8-16]
	mov     rcx, 0
	adc     r14, [rdx+r11*8+16]
	cmovc   rcx, [r8-24]
	rcr     r10, 1
	add     r9, rax
	adc     r10, 0
	add     r9, rbx
	adc     r10, 0
	add     r9, rcx
	adc     r10, 0
	mov     [rdi+r11*8], r12
	mov     [rdi+r11*8+8], r13
	mov     [rdi+r11*8+16], r14
    jmp     .6
    
	xalign  16
.4: mov     r12, [rsi+r11*8]
	mov     r13, [rsi+r11*8+8]
	shl     r10, 1
	adc     r12, [rdx+r11*8]
	cmovc   rax, [r8-8]
	adc     r13, [rdx+r11*8+8]
	cmovc   rbx, [r8-16]
	rcr     r10, 1
	add     r9, rax
	adc     r10, 0
	add     r9, rbx
	adc     r10, 0
	mov     [rdi+r11*8], r12
	mov     [rdi+r11*8+8], r13
	jmp     .6

	xalign  16
.5: mov     r12, [rsi+r11*8]
	shl     r10, 1
	adc     r12, [rdx+r11*8]
	cmovc   rax, [r8-8]
	rcr     r10, 1
	add     r9, rax
	adc     r10, 0
	mov     [rdi+r11*8], r12
.6:	mov     rcx, [rsp+stack_use+32]
	mov     [rcx], r9
	btr     r10, 63
	mov     [rcx+8], r10
    mov     rax, 0
	adc     rax, 0
    END_PROC reg_save_list

    end
