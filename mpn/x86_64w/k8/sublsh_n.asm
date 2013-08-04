; PROLOGUE(mpn_sublsh_n)

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
;  mp_limb_t  mpn_sublsh_n(mp_ptr, mp_ptr, mp_ptr, mp_size_t, mp_uint, mp_limb_t)
;  mp_limb_t mpn_sublsh_nc(mp_ptr, mp_ptr, mp_ptr, mp_size_t, mp_uint)
;  rax                        rdi     rsi     rdx        rcx       r8         r9
;  rax                        rcx     rdx      r8         r9 [rsp+40]   [rsp+48]

%include "yasm_mac.inc"

    CPU  Athlon64
    BITS 64

%define reg_save_list rbx, rsi, rdi, rbp, r12, r13, r14, r15

	LEAF_PROC mpn_sublsh_n
	mov     r10, r9
	xor     r9, r9
    jmp     entry
    
	LEAF_PROC mpn_sublsh_nc
	mov     r10, r9
	mov     r9, [rsp+48]
entry:
	FRAME_PROC ?mpn_sublsh, 0, reg_save_list
	lea     rdi, [rcx+r10*8]
	lea     rsi, [rdx+r10*8]
	lea     rdx, [r8+r10*8]
	mov     ecx, dword [rsp+stack_use+40]
	
	neg     rcx
	shr     r9, cl
	neg     r10
	xor     rax, rax
	test    r10, 3
	jz      .2
.1:	mov     r8, [rdx+r10*8]
	mov     r11, r8
	neg     rcx
	shl     r8, cl
	neg     rcx
	shr     r11, cl
	or      r8, r9
	mov     r9, r11
	add     rax, 1
	mov     r11, [rsi+r10*8]
	sbb     r11, r8
	sbb     rax, rax
	mov     [rdi+r10*8], r11
	inc     r10
	test    r10, 3
	jnz     .1
.2:	cmp     r10, 0
	jz      .4

	xalign  16
.3:	mov     r8, [rdx+r10*8]
	mov     rbp, [rdx+r10*8+8]
	mov     rbx, [rdx+r10*8+16]
	mov     r12, [rdx+r10*8+24]
	mov     r11, r8
	mov     r13, rbp
	mov     r14, rbx
	mov     r15, r12
	neg     rcx
	shl     r8, cl
	shl     rbp, cl
	shl     rbx, cl
	shl     r12, cl
	neg     rcx
	shr     r11, cl
	shr     r13, cl
	shr     r14, cl
	shr     r15, cl
	or      r8, r9
	or      rbp, r11
	or      rbx, r13
	or      r12, r14
	mov     r9, r15
	add     rax, 1
	mov     r11, [rsi+r10*8]
	mov     r13, [rsi+r10*8+8]
	mov     r14, [rsi+r10*8+16]
	mov     r15, [rsi+r10*8+24]
	sbb     r11, r8
	sbb     r13, rbp
	sbb     r14, rbx
	sbb     r15, r12
	sbb     rax, rax
	mov     [rdi+r10*8], r11
	mov     [rdi+r10*8+8], r13
	mov     [rdi+r10*8+16], r14
	mov     [rdi+r10*8+24], r15
	add     r10, 4
	jnz     .3
.4:	neg     rax
	add     rax, r9
    END_PROC reg_save_list
    
    end
