
;  AMD64 mpn_sublsh1_n
;  Verdxon 1.0.3
;
;  Copyright 2008 Jason Moxham
;
;  Windows Conversion Copyright 2008 Brian Gladman
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
;  Calling interface:
;
;  rcx[r10] = rdx[r10] - 2 * r14[r10]
;
;  void __gmpn_sublsh1_n(
;     mp_ptr rp,          rcx
;     mp_srcptr xp,       rdx
;     mp_srcptr yp,        r8
;     mp_size_t  n,        r9
;  )
;
; This is an SEH Frame Function

%include "..\x86_64_asm.inc"

%define reg_save_list rbx, rsi, rdi, r12, r13, r14, r15

   bits 64
   section .text

   global   __gmpn_sublsh1_n

%ifdef DLL
   export   __gmpn_sublsh1_n
%endif

    prologue __gmpn_sublsh1_n, 0, reg_save_list
    mov     r9d, r9d

	lea     rdx, [rdx+r9*8]
	lea     r8, [r8+r9*8]
	lea     rcx, [rcx+r9*8]
	neg     r9
	xor     r10, r10
	xor     rax, rax
	test    r9, 3
	jz      .2

.1: mov     r11, [rdx+r9*8]
	add     r10, 1
	sbb     r11, [r8+r9*8]
	sbb     r10, r10
	add     rax, 1
	sbb     r11, [r8+r9*8]
	sbb     rax, rax
	mov     [rcx+r9*8], r11
	inc     r9
	test    r9, 3
	jnz     .1

.2: cmp     r9, 0
	jz      .4

	alignb  16, nop
.3: mov     r11, [rdx+r9*8]
	mov     r12, [rdx+r9*8+8]
	mov     r13, [rdx+r9*8+16]
	mov     r14, [rdx+r9*8+24]
	mov     r15, [r8+r9*8]
	mov     rbx, [r8+r9*8+8]
	mov     rsi, [r8+r9*8+16]
	mov     rdi, [r8+r9*8+24]
	add     r10, 1
	sbb     r11, r15
	sbb     r12, rbx
	sbb     r13, rsi
	sbb     r14, rdi
	sbb     r10, r10
	add     rax, 1
	sbb     r11, r15
	sbb     r12, rbx
	sbb     r13, rsi
	sbb     r14, rdi
	sbb     rax, rax
	mov     [rcx+r9*8], r11
	mov     [rcx+r9*8+8], r12
	mov     [rcx+r9*8+16], r13
	mov     [rcx+r9*8+24], r14
	add     r9, 4
	jnz     .3

.4:	add     rax, r10
	neg     rax
    epilogue reg_save_list

	end
