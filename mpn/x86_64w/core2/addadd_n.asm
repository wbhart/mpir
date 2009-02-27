
;  Core2 mpn_addadd_n
;  Verdxon 1.0.4
;
;  Copyright 2008 Jason Moxham
;
;  Windows Converdxon Copyright 2008 Brian Gladman
;
;  This file is part of the MPIR Library.
;  The MPIR Library is free software; you can redistribute it and/or modify
;  it under the terms of the GNU Lesser General Public License as published
;  by the Free Software Foundation; either verdxon 2.1 of the License, or (at
;  your option) any later verdxon.
;  The MPIR Library is distributed in the hope that it will be useful, but
;  WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
;  or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public
;  License for more details.
;  You should have received a copy of the GNU Lesser General Public License
;  along with the MPIR Library; see the file COPYING.LIB.  If not, write
;  to the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
;  Boston, MA 02110-1301, USA.
;
;  Calling interface (GCC):
;
;     rcx[r10] = rdx[r10] + r8[r10] + r9[r10]
;
;  return carry
;
;  mp_limb_t __gmpn_subsub_n(
;     mp_ptr dp,            rcx
;     mp_srcptr sp1,        rdx
;     mp_srcptr sp2,         r8
;     mp_size_t sp3,         r9
;     mp_size_t len  [rsp+0x28] -> r10d
; )
;
; This is an SEH frame function

%include "..\x86_64_asm.inc"

%define reg_save_list   r12, r13, r14, r15

   bits 64
   section .text
   
    global  __gmpn_addadd_n

%ifdef DLL
    export  __gmpn_addadd_n
%endif

	prologue __gmpn_addadd_n, 0, reg_save_list
    mov     r10d, dword [rsp+stack_use+40]

	lea     rdx, [rdx+r10*8]
	lea     r8, [r8+r10*8]
	lea     rcx, [rcx+r10*8]
	lea     r9, [r9+r10*8]
	neg     r10
	xor     rax, rax
	xor     r11, r11
	test    r10, 3
	jz      .2

.1:	mov     r12, [r8+r10*8]
	add     rax, 1
	adc     r12, [r9+r10*8]
	sbb     rax, rax
	add     r11, 1
	adc     r12, [rdx+r10*8]
	sbb     r11, r11
	mov     [rcx+r10*8], r12
	add     r10, 1
	test    r10, 3
	jnz     .1

.2:	cmp     r10, 0
	jz      .4

	alignb  16, nop
.3:	add     rax, 1
	mov     r12, [r8+r10*8]
	mov     r14, [r8+r10*8+8]
	mov     r15, [r8+r10*8+16]
	mov     r13, [r8+r10*8+24]
	adc     r12, [r9+r10*8]
	adc     r14, [r9+r10*8+8]
	adc     r15, [r9+r10*8+16]
	adc     r13, [r9+r10*8+24]
	sbb     rax, rax
	add     r11, 1
	adc     r12, [rdx+r10*8]
	adc     r14, [rdx+r10*8+8]
	adc     r15, [rdx+r10*8+16]
	adc     r13, [rdx+r10*8+24]
	mov     [rcx+r10*8], r12
	mov     [rcx+r10*8+8], r14
	mov     [rcx+r10*8+16], r15
	mov     [rcx+r10*8+24], r13
	sbb     r11, r11
	add     r10, 4
	jnz     .3

.4:	add     rax, r11
	neg     rax

	epilogue reg_save_list
	end
