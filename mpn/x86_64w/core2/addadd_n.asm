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
;     rcx[rbx] = rdx[rbx] + r8[rbx] + r9[rbx]
;
;  return carry
;
;  mp_limb_t __gmpn_addadd_n(
;     mp_ptr dp,            rcx
;     mp_srcptr sp1,        rdx
;     mp_srcptr sp2,         r8
;     mp_size_t sp3,         r9
;     mp_size_t len  [rsp+0x28] -> rbxd
; )
;
; This is an SEH frame function

%include "..\x86_64_asm.inc"

%define reg_save_list   rbx, rbp, rsi, rdi

   bits 64
   section .text
   
    global  __gmpn_addadd_n

%ifdef DLL
    export  __gmpn_addadd_n
%endif

	prologue __gmpn_addadd_n, 0, reg_save_list
    mov     ebx, dword [rsp+stack_use+40]
    
	lea     rdx, [rdx+rbx*8]
	lea     r8, [r8+rbx*8]
	lea     rcx, [rcx+rbx*8]
	lea     r9, [r9+rbx*8]
	neg     rbx
	xor     rax, rax
	xor     r11, r11
	test    rbx, 3
	jz      .2
.1:	mov     rsi, [r8+rbx*8]
	add     rax, 1
	adc     rsi, [r9+rbx*8]
	sbb     rax, rax
	add     r11, 1
	adc     rsi, [rdx+rbx*8]
	sbb     r11, r11
	mov     [rcx+rbx*8], rsi
	add     rbx, 1          ; ***
	test    rbx, 3
	jnz     .1
.2:	cmp     rbx, 0
	jz      .4

	alignb  16, nop
.3: add     rax, 1
	mov     rsi, [r8+rbx*8]
	mov     rdi, [r8+rbx*8+8]
	mov     rbp, [r8+rbx*8+16]
	mov     r10, [r8+rbx*8+24]
	adc     rsi, [r9+rbx*8]
	adc     rdi, [r9+rbx*8+8]
	adc     rbp, [r9+rbx*8+16]
	adc     r10, [r9+rbx*8+24]
	sbb     rax, rax
	add     r11, 1
	adc     rsi, [rdx+rbx*8]
	adc     rdi, [rdx+rbx*8+8]
	adc     rbp, [rdx+rbx*8+16]
	adc     r10, [rdx+rbx*8+24]
	mov     [rcx+rbx*8], rsi
	mov     [rcx+rbx*8+8], rdi
	mov     [rcx+rbx*8+16], rbp
	mov     [rcx+rbx*8+24], r10
	sbb     r11, r11
	add     rbx, 4
	jnz     .3
.4: add     rax, r11
	neg     rax
.5: epilogue reg_save_list
    
    end
