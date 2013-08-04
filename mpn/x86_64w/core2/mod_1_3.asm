; PROLOGUE(mpn_mod_1_3)

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
;  mp_limb_t  mpn_mod_1_3(mp_ptr, mp_ptr, mp_size_t, mp_ptr)
;  rax                       rdi     rsi        rdx     rcx
;  rax                       rcx     rdx         r8      r9

;	(rdi,2)= not fully reduced remainder of (rsi,rdx) / divisor , and top limb <d
;	where (rcx,4)  contains B^i % divisor

%include 'yasm_mac.inc'

    CPU  Athlon64
    BITS 64

%define reg_save_list rsi, rdi, r12, r13, r14, r15

    FRAME_PROC mpn_mod_1_3, 0, reg_save_list
    mov     rsi, rdx
    mov     rdi, r8
	mov     r15, [rsi+rdi*8-8]
	mov     r14, [rsi+rdi*8-16]
	mov     rax, [rsi+rdi*8-32]
	mov     r12, [rsi+rdi*8-40]
	mov     r8, [r9]
	mov     r10, [r9+16]
	mov     r11, [r9+24]
	mov     r9, [r9+8]
	sub     rdi, 8
	jc      .2
	
; // r15 r14 -8() -16()=rax -24()=r12
	xalign  16
.1:	mul     r8
	add     r12, rax
	mov     rax, [rsi+rdi*8+40]
	mov     r13, 0
	adc     r13, rdx
	mul     r9
	add     r12, rax
	nop
	adc     r13, rdx
	mov     rax, r10
	mul     r14
	add     r12, rax
	adc     r13, rdx
	mov     r14, r12
	mov     rax, r11
	mul     r15
	add     r14, rax
	mov     r12, [rsi+rdi*8+0]
	mov     r15, r13
	mov     rax, [rsi+rdi*8+8]
	adc     r15, rdx
	sub     rdi, 3
	jnc     .1

; // we have loaded up the next two limbs
; // but because they are out of order we can have to do 3 limbs min
.2:	cmp     rdi, -2
	jl      .5
	je      .4

	; //two more limbs is 4 limbs
	; // r15 r14 40() 8+24()=rax 0+24()=r12
.3:	mul     r8
	add     r12, rax
	mov     rax, [rsi+rdi*8+40]
	mov     r13, 0
	adc     r13, rdx
	mul     r9
	add     r12, rax
	nop
	adc     r13, rdx
	mov     rax, r10
	mul     r14
	add     r12, rax
	adc     r13, rdx
	mov     r14, r12
	mov     rax, r11
	mul     r15
	add     r14, rax
	mov     r12, [rsi+rdi*8+8]
	mov     r15, r13
	mov     rax, [rsi+rdi*8+16]
	adc     r15, rdx
	; // r15 r14 rax r12
	mov     r13, 0
	mul     r8
	add     r12, rax
	adc     r13, rdx
	mov     rax, r9
	mul     r14
	add     r12, rax
	adc     r13, rdx
	mov     rax, r10
	mul     r15
	add     r12, rax
	adc     r13, rdx
	; // r13 r12
	mov     rax, r8
	mul     r13
	jmp     .6

	; //two more limbs is 4 limbs
	; // r15 r14 40() 8+24()=rax 0+24()=r12
	xalign  16
.4:	mul     r8
	add     r12, rax
	mov     rax, [rsi+rdi*8+40]
	mov     r13, 0
	adc     r13, rdx
	mul     r9
	add     r12, rax
	nop
	adc     r13, rdx
	mov     rax, r10
	mul     r14
	add     r12, rax
	adc     r13, rdx
	mov     r14, r12
	mov     rax, r11
	mul     r15
	add     r14, rax
	mov     r12, [rsi+rdi*8+16]
	mov     r15, r13
	adc     r15, rdx
	; // r15 r14 r12
	mov     r13, 0
	mov     rax, r8
	mul     r14
	add     r12, rax
	adc     r13, rdx
	mov     rax, r9
	mul     r15
	add     r12, rax
	adc     r13, rdx
	; // r13 r12
	mov     rax, r8
	mul     r13
	jmp     .6
	
	; // one more is 3 limbs
	; // r15 r14 40() 8+24()=rax 0+24()=r12 
	xalign  16
.5:	mul     r8
	add     r12, rax
	mov     rax, [rsi+rdi*8+40]
	mov     r13, 0
	adc     r13, rdx
	mul     r9
	add     r12, rax
	nop
	adc     r13, rdx
	mov     rax, r10
	mul     r14
	add     r12, rax
	adc     r13, rdx
	mov     rax, r11
	mul     r15
	add     r12, rax
	mov     r15, r13
	adc     r15, rdx
	mov     rax, r8
	mul     r15
.6:	add     r12, rax
	adc     rdx, 0
	mov     [rcx], r12
	mov     [rcx+8], rdx
    END_PROC reg_save_list

	end
