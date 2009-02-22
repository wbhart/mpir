
;  AMD64 mpn_addlsh1_n
;  Version 1.0.3
;
;  Copyright 2008 Jason Moxham
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
;  Calling interface:
;
;  rcx[r9] = rdx[r9] + 2 * r8[r9]
;
;  void __gmpn_addlsh1_n(
;     mp_ptr rp,          rcx
;     mp_srcptr xp,       rdx
;     mp_srcptr yp,        r8
;     mp_size_t  n,        r9
;  )
;
; This is an SEH frame function

%include "..\x86_64_asm.inc"

%define reg_save_list r12, r13, r14

   bits 64
   section .text

   global   __gmpn_addlsh1_n

%ifdef DLL
   export   __gmpn_addlsh1_n
%endif

    prologue __gmpn_addlsh1_n, 0, reg_save_list
    mov     r9d, r9d
	lea     rdx, [rdx+r9*8]
	lea     r8, [r8+r9*8]
	lea     rcx, [rcx+r9*8]
	neg     r9
	xor     r10, r10
	xor     rax, rax
	test    r9, 3
	jz      .2

.1: mov     r11, [r8+r9*8]
	add     r10, 1
	adc     r11, r11
	sbb     r10, r10
	add     rax, 1
	adc     r11, [rdx+r9*8]
	sbb     rax, rax
	mov     [rcx+r9*8], r11
	inc     r9
	test    r9, 3
	jnz     .1

.2: cmp     r9, 0
	jz      .4
; push/pop can be moved to pro/epilog

	align   16
.3: mov     r11, [r8+r9*8]
	mov     r12, [r8+r9*8+8]
	mov     r13, [r8+r9*8+16]
	mov     r14, [r8+r9*8+24]
	add     r10, 1
	adc     r11, r11
	adc     r12, r12
	adc     r13, r13
	adc     r14, r14
	sbb     r10, r10
	add     rax, 1
	adc     r11, [rdx+r9*8]
	adc     r12, [rdx+r9*8+8]
	adc     r13, [rdx+r9*8+16]
	adc     r14, [rdx+r9*8+24]
	sbb     rax, rax
	mov     [rcx+r9*8], r11
	mov     [rcx+r9*8+8], r12
	mov     [rcx+r9*8+16], r13
	mov     [rcx+r9*8+24], r14
	add     r9, 4
	jnz     .3

.4: add     rax, r10
	neg     rax
    epilogue reg_save_list

    end
