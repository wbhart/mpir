
;  mpn_rshift

;  Copyright 2009 Jason Moxham

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
;	mpn_rshift(mp_ptr rdi, mp_ptr rsi, mp_size_t rdx, mp_limb_t rcx)
;                     rcx         rdx            r8d             r9
;
;	rax=carry

%include "..\yasm_mac.inc"

%define reg_save_list rbx, rsi, rdi

    CPU  Athlon64
    BITS 64

    FRAME_PROC mpn_rshift, 0, reg_save_list
; when n=1 mod4 seem to have different runtimes
    movsxd  rax, r8d
	mov     rbx, 4
	lea     rsi, [rdx+rax*8-24]
	lea     rdi, [rcx+rax*8-24]
	mov     rcx, r9
	sub     rbx, rax
	
	xor     rax, rax
	mov     rdx, [rsi+rbx*8-8]
	shrd    rax, rdx, cl
	cmp     rbx, 0
	jge     L_skiplp
	xalign  16
L_lp:
	mov     r8, [rsi+rbx*8]
	mov     r11, [rsi+rbx*8+24]
	shrd    rdx, r8, cl
	mov     r9, [rsi+rbx*8+8]
	shrd    r8, r9, cl
	mov     [rdi+rbx*8-8], rdx
	mov     [rdi+rbx*8], r8
	mov     r10, [rsi+rbx*8+16]
	shrd    r9, r10, cl
	mov     rdx, r11
	mov     [rdi+rbx*8+8], r9
	shrd    r10, r11, cl
	add     rbx, 4
	mov     [rdi+rbx*8-16], r10
	jnc     L_lp
L_skiplp:
	cmp     rbx, 2
	ja      L_case0
	je      L_case1
	jp      L_case2
L_case3:
	mov     r8, [rsi+rbx*8]
	shrd    rdx, r8, cl
	mov     r9, [rsi+rbx*8+8]
	shrd    r8, r9, cl
	mov     [rdi+rbx*8-8], rdx
	mov     [rdi+rbx*8], r8
	mov     r10, [rsi+rbx*8+16]
	shrd    r9, r10, cl
	mov     [rdi+rbx*8+8], r9
	shr     r10, cl
	mov     [rdi+rbx*8+16], r10
	jmp     L_xit

	xalign  16
L_case2:
	mov     r8, [rsi+rbx*8]
	shrd    rdx, r8, cl
	mov     r9, [rsi+rbx*8+8]
	shrd    r8, r9, cl
	mov     [rdi+rbx*8-8], rdx
	mov     [rdi+rbx*8], r8
	shr     r9, cl
	mov     [rdi+rbx*8+8], r9
    jmp     L_xit

	xalign  16
L_case1:
	mov     r8, [rsi+rbx*8]
	shrd    rdx, r8, cl
	shr     r8, cl
	mov     [rdi+rbx*8-8], rdx
	mov     [rdi+rbx*8], r8
    jmp     L_xit

	xalign  16
L_case0:
	shr     rdx, cl
	mov     [rdi+rbx*8-8], rdx
L_xit:
    END_PROC reg_save_list

    end
