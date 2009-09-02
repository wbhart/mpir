
;  Copyright 2009 Jason Moxham
;
;  This file is part of the MPIR Library.
;
;  Windows Conversion Copyright 2008 Brian Gladman
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
;  mp_limb_t pn_addlsh_n(mp_ptr, mp_ptr, mp_ptr, mp_size_t, mp_uint)
;  rax                      rdi     rsi     rdx        rcx       r8
;  rax                      rcx     rdx      r8         r9 [rsp+40]

%include '..\yasm_mac.inc'

%define reg_save_list   rbx, rsi, rdi, r12

    CPU  Core2
    BITS 64

	FRAME_PROC mpn_addlsh_n, 0, reg_save_list
	movsxd  rax, r9d
	lea     rdi, [rcx+rax*8-32]
	lea     rsi, [rdx+rax*8-32]
	lea     rdx, [r8+rax*8-32]
	mov     rcx, rax
	mov     r8d, dword [rsp+stack_use+40]

	mov     rbx, 4
	sub     rbx, rcx
	mov     rcx, 64
	sub     rcx, r8
	mov     r12, 0
	mov     rax, 0
	mov     r8, [rdx+rbx*8]
	cmp     rbx, 0
	jge     L_skiplp

	xalign  16
L_lp:
	mov     r9, [rdx+rbx*8+8]
	mov     r10, [rdx+rbx*8+16]
	mov     r11, [rdx+rbx*8+24]
	shrd    r12, r8, cl
	shrd    r8, r9, cl
	shrd    r9, r10, cl
	shrd    r10, r11, cl
	sahf
	adc     r12, [rsi+rbx*8]
	mov     [rdi+rbx*8], r12
	adc     r8, [rsi+rbx*8+8]
	mov     r12, r11
	mov     [rdi+rbx*8+8], r8
	adc     r9, [rsi+rbx*8+16]
	adc     r10, [rsi+rbx*8+24]
	mov     [rdi+rbx*8+24], r10
	mov     [rdi+rbx*8+16], r9
	lahf
	mov     r8, [rdx+rbx*8+32]
	add     rbx, 4
	jnc     L_lp
	
	xalign  16
L_skiplp:
	cmp     rbx, 2
	ja      L_case0
	je      L_case1
	jp      L_case2
L_case3:
	shrd    r12, r8, cl
	mov     r9, [rdx+rbx*8+8]
	mov     r10, [rdx+rbx*8+16]
	mov     r11, [rdx+rbx*8+24]
	shrd    r8, r9, cl
	shrd    r9, r10, cl
	shrd    r10, r11, cl
	sahf
	adc     r12, [rsi+rbx*8]
	mov     [rdi+rbx*8], r12
	adc     r8, [rsi+rbx*8+8]
	mov     r12, r11
	mov     [rdi+rbx*8+8], r8
	adc     r9, [rsi+rbx*8+16]
	adc     r10, [rsi+rbx*8+24]
	mov     [rdi+rbx*8+24], r10
	mov     [rdi+rbx*8+16], r9
	lahf
	shr     r12, cl
	sahf
	adc     r12, 0
	mov     rax, r12
	jmp     L_xit

L_case2:
	shrd    r12, r8, cl
	mov     r9, [rdx+rbx*8+8]
	shrd    r8, r9, cl
	mov     r10, [rdx+rbx*8+16]
	shrd    r9, r10, cl
	shr     r10, cl
	sahf
	adc     r12, [rsi+rbx*8]
	mov     [rdi+rbx*8], r12
	adc     r8, [rsi+rbx*8+8]
	mov     rax, 0
	mov     [rdi+rbx*8+8], r8
	adc     r9, [rsi+rbx*8+16]
	adc     rax, r10
	mov     [rdi+rbx*8+16], r9
	jmp     L_xit

L_case1:
	shrd    r12, r8, cl
	mov     r9, [rdx+rbx*8+8]
	shrd    r8, r9, cl
	shr     r9, cl
	sahf
	adc     r12, [rsi+rbx*8]
	mov     [rdi+rbx*8], r12
	adc     r8, [rsi+rbx*8+8]
	mov     rax, 0
	mov     [rdi+rbx*8+8], r8
	adc     rax, r9
	jmp     L_xit
L_case0:
	shrd    r12, r8, cl
	shr     r8, cl
	sahf
	adc     r12, [rsi+rbx*8]
	mov     [rdi+rbx*8], r12
	adc     r8, 0
	mov     rax, r8
L_xit:
    END_PROC reg_save_list
    
    end
