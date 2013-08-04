; PROLOGUE(mpn_popcount)

;  mpn_popcount

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

;	mpn_limb_t mpn_popcount(mp_ptr,mp_size_t)
;	rax                        rdi,      rsi
;	rax                        rcx,      rdx

%include "yasm_mac.inc"

    CPU  Athlon64
    BITS 64

%define reg_save_list   rsi, rdi, r12, r14, r15

	FRAME_PROC mpn_popcount, 0, reg_save_list
	mov	rdi, rcx
	mov   	rsi, rdx

	mov     r8, 0x5555555555555555
	mov     r9, 0x3333333333333333
	mov     r10, 0x0f0f0f0f0f0f0f0f
	mov     r11, 0x0101010101010101
	xor     rax, rax
	sub     rsi, 3
	jc      .2
	mov     rcx, [rdi+rsi*8+16]
	mov     r12, [rdi+rsi*8+8]
	mov     r14, [rdi+rsi*8]
	sub     rsi, 3
	jc      .1
	xalign  16
.0:
	mov     rdx, rcx
	shr     rcx, 1
	and     rcx, r8
	sub     rdx, rcx
	mov     rcx, rdx
	shr     rdx, 2
	and     rcx, r9
	and     rdx, r9
	add     rdx, rcx
	mov     rcx, r12
	shr     r12, 1
	and     r12, r8
	sub     rcx, r12
	mov     r12, rcx
	shr     rcx, 2
	and     r12, r9
	and     rcx, r9
	add     r12, rcx
	mov     r15, r14
	shr     r14, 1
	and     r14, r8
	mov     rcx, [rdi+rsi*8+16]
	sub     r15, r14
	mov     r14, r15
	shr     r15, 2
	and     r14, r9
	and     r15, r9
	add     r15, r14
	add     r12, rdx
	add     r12, r15
	mov     r14, r12
	shr     r12, 4
	and     r14, r10
	and     r12, r10
	add     r14, r12
	imul    r14, r11
	shr     r14, 56
	add     rax, r14
	mov     r12, [rdi+rsi*8+8]
	sub     rsi, 3
	mov     r14, [rdi+rsi*8+24-0]
	jnc     .0
.1:
	mov     rdx, rcx
	shr     rcx, 1
	and     rcx, r8
	sub     rdx, rcx
	mov     rcx, rdx
	shr     rdx, 2
	and     rcx, r9
	and     rdx, r9
	add     rdx, rcx
	mov     rcx, r12
	shr     r12, 1
	and     r12, r8
	sub     rcx, r12
	mov     r12, rcx
	shr     rcx, 2
	and     r12, r9
	and     rcx, r9
	add     r12, rcx
	mov     r15, r14
	shr     r14, 1
	and     r14, r8
	sub     r15, r14
	mov     r14, r15
	shr     r15, 2
	and     r14, r9
	and     r15, r9
	add     r15, r14
	add     r12, rdx
	add     r12, r15
	mov     r14, r12
	shr     r12, 4
	and     r14, r10
	and     r12, r10
	add     r14, r12
	imul    r14, r11
	shr     r14, 56
	add     rax, r14
.2:	cmp     rsi, -2
	jl      .5
	jz      .4
.3:
	mov     rcx, [rdi+rsi*8+16]
	mov     rdx, rcx
	shr     rcx, 1
	and     rcx, r8
	sub     rdx, rcx
	mov     rcx, rdx
	shr     rdx, 2
	and     rcx, r9
	and     rdx, r9
	add     rdx, rcx
	mov     r14, rdx
	shr     rdx, 4
	and     r14, r10
	and     rdx, r10
	add     r14, rdx
	imul    r14, r11
	shr     r14, 56
	add     rax, r14
	dec     rsi
.4:	mov     rcx, [rdi+rsi*8+16]
	mov     rdx, rcx
	shr     rcx, 1
	and     rcx, r8
	sub     rdx, rcx
	mov     rcx, rdx
	shr     rdx, 2
	and     rcx, r9
	and     rdx, r9
	add     rdx, rcx
	mov     r14, rdx
	shr     rdx, 4
	and     r14, r10
	and     rdx, r10
	add     r14, rdx
	imul    r14, r11
	shr     r14, 56
	add     rax, r14
.5:	END_PROC reg_save_list

	end
