; PROLOGUE(mpn_hamdist)

;  mpn_hamdist

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

;	mp_limb_t mpn_hamdist(mp_ptr, mp_ptr, mp_size_t)
;	rax                      rdi,    rsi,       rdx
;	rax                      rcx,    rdx,        r8

%include "yasm_mac.inc"

    CPU  Athlon64
    BITS 64

%define reg_save_list   rsi, rdi, rbp, r12, r14

	FRAME_PROC mpn_hamdist, 0, reg_save_list
	mov	rdi, rcx
	mov	rsi, rdx
	mov   	rdx, r8

	mov     r8, 0x5555555555555555
	mov     r9, 0x3333333333333333
	mov     r10, 0x0f0f0f0f0f0f0f0f
	mov     r11, 0x0101010101010101
	xor     eax, eax
	sub     rdx, 3
	jc      .2
	mov     rcx, [rdi+rdx*8+16]
	xor     rcx, [rsi+rdx*8+16]
	mov     r12, [rdi+rdx*8+8]
	xor     r12, [rsi+rdx*8+8]
	mov     r14, [rdi+rdx*8]
	xor     r14, [rsi+rdx*8]
	sub     rdx, 3
	jc      .1
	xalign  16
.0:	mov     rbp, rcx
	shr     rcx, 1
	and     rcx, r8
	sub     rbp, rcx
	mov     rcx, rbp
	shr     rbp, 2
	and     rcx, r9
	and     rbp, r9
	add     rcx, rbp
	mov     rbp, r12
	shr     r12, 1
	and     r12, r8
	sub     rbp, r12
	mov     r12, rbp
	shr     rbp, 2
	and     r12, r9
	and     rbp, r9
	add     rbp, r12
	mov     r12, r14
	shr     r14, 1
	and     r14, r8
	sub     r12, r14
	mov     r14, r12
	shr     r12, 2
	and     r14, r9
	and     r12, r9
	add     r12, r14
	add     rbp, rcx
	add     rbp, r12
	mov     rcx, [rdi+rdx*8+16]
	mov     r14, rbp
	shr     rbp, 4
	and     r14, r10
	xor     rcx, [rsi+rdx*8+16]
	mov     r12, [rdi+rdx*8+8]
	xor     r12, [rsi+rdx*8+8]
	and     rbp, r10
	add     r14, rbp
	imul    r14, r11
	shr     r14, 56
	add     rax, r14
	mov     r14, [rdi+rdx*8]
	xor     r14, [rsi+rdx*8]
	sub     rdx, 3
	jnc     .0
.1:
	mov     rbp, rcx
	shr     rcx, 1
	and     rcx, r8
	sub     rbp, rcx
	mov     rcx, rbp
	shr     rbp, 2
	and     rcx, r9
	and     rbp, r9
	add     rcx, rbp
	mov     rbp, r12
	shr     r12, 1
	and     r12, r8
	sub     rbp, r12
	mov     r12, rbp
	shr     rbp, 2
	and     r12, r9
	and     rbp, r9
	add     rbp, r12
	mov     r12, r14
	shr     r14, 1
	and     r14, r8
	sub     r12, r14
	mov     r14, r12
	shr     r12, 2
	and     r14, r9
	and     r12, r9
	add     r12, r14
	add     rbp, rcx
	add     rbp, r12
	mov     r14, rbp
	shr     rbp, 4
	and     r14, r10
	and     rbp, r10
	add     r14, rbp
	imul    r14, r11
	shr     r14, 56
	add     rax, r14
.2:
	cmp     rdx, -2
	jl      .5
	jz      .4
.3:
	mov     rcx, [rdi+rdx*8+16]
	xor     rcx, [rsi+rdx*8+16]
	mov     rbp, rcx
	shr     rcx, 1
	and     rcx, r8
	sub     rbp, rcx
	mov     rcx, rbp
	shr     rbp, 2
	and     rcx, r9
	and     rbp, r9
	add     rcx, rbp
	mov     r14, rcx
	shr     rcx, 4
	and     r14, r10
	and     rcx, r10
	add     r14, rcx
	imul    r14, r11
	shr     r14, 56
	add     rax, r14
	dec     rdx
.4:
	mov     rcx, [rdi+rdx*8+16]
	xor     rcx, [rsi+rdx*8+16]
	mov     rbp, rcx
	shr     rcx, 1
	and     rcx, r8
	sub     rbp, rcx
	mov     rcx, rbp
	shr     rbp, 2
	and     rcx, r9
	and     rbp, r9
	add     rcx, rbp
	mov     r14, rcx
	shr     rcx, 4
	and     r14, r10
	and     rcx, r10
	add     r14, rcx
	imul    r14, r11
	shr     r14, 56
	add     rax, r14
.5:	END_PROC reg_save_list

    	end
