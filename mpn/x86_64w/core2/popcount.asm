
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

;	mp_limb_t mpn_popcount(mp_ptr,mp_size_t)
;	rax                       rdi,      rsi
;	rax                       rcx,      rdx

%include "..\yasm_mac.inc"

    CPU  Core2
    BITS 64

%define reg_save_list   rbx, rsi, rdi, rbp, r12

	FRAME_PROC mpn_popcount, 0, reg_save_list
	mov		rdi, rcx
	mov  	rsi, rdx

	mov     r8, 0x5555555555555555
	mov     r9, 0x3333333333333333
	mov     r10, 0x0f0f0f0f0f0f0f0f
	mov     r11, 0x0101010101010101
	mov     rax, 0
	sub     rsi, 2
	jc      .3
	mov     rcx, [rdi+rsi*8+8]
	or      rcx, [rdi+rsi*8+8]
	mov     r12, [rdi+rsi*8]
	or      r12, [rdi+rsi*8]
	sub     rsi, 2
	jc      .2
	
	xalign  16
.1:	mov     rbp, rcx
	shr     rcx, 1
	and     rcx, r8
	sub     rbp, rcx
	mov     rcx, rbp
	shr     rbp, 2
	and     rcx, r9
	and     rbp, r9
	add     rbp, rcx

	mov     rbx, r12
	shr     r12, 1
	and     r12, r8
	sub     rbx, r12
	mov     rcx, [rdi+rsi*8+8]
	mov     r12, rbx
	shr     rbx, 2
	and     r12, r9
	or      rcx, [rdi+rsi*8+8]
	and     rbx, r9
	add     rbx, r12

	add     rbx, rbp
	mov     rdx, rbx
	mov     r12, [rdi+rsi*8]
	or      r12, [rdi+rsi*8]
	shr     rbx, 4
	and     rdx, r10
	and     rbx, r10
	add     rdx, rbx
	imul    rdx, r11
	shr     rdx, 56
	add     rax, rdx
	sub     rsi, 2
	jnc     .1
.2:	mov     rbp, rcx
	shr     rcx, 1
	and     rcx, r8
	sub     rbp, rcx
	mov     rcx, rbp
	shr     rbp, 2
	and     rcx, r9
	and     rbp, r9
	add     rbp, rcx

	mov     rbx, r12
	shr     r12, 1
	and     r12, r8
	sub     rbx, r12
	mov     r12, rbx
	shr     rbx, 2
	and     r12, r9
	and     rbx, r9
	add     rbx, r12

	add     rbx, rbp
	mov     rdx, rbx
	shr     rbx, 4
	and     rdx, r10
	and     rbx, r10
	add     rdx, rbx
	imul    rdx, r11
	shr     rdx, 56
	add     rax, rdx
.3:	cmp     rsi, -2
	jz      .5
.4:	mov     rcx, [rdi+rsi*8+8]
	or      rcx, [rdi+rsi*8+8]
	mov     rbp, rcx
	shr     rcx, 1
	and     rcx, r8
	sub     rbp, rcx
	mov     rcx, rbp
	shr     rbp, 2
	and     rcx, r9
	and     rbp, r9
	add     rbp, rcx
	mov     rdx, rbp
	shr     rbp, 4
	add     rdx, rbp
	and     rdx, r10
	imul    rdx, r11
	shr     rdx, 56
	add     rax, rdx
.5:	END_PROC reg_save_list

	end
