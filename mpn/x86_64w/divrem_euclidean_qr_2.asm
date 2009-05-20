
;  mpn_divrem_euclidean_qr_2

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
;   mpn_divrem_euclidean_qr_2(mp_ptr, mp_ptr, mp_size_t, mp_srcptr dp);	
;	rax                 rdi,   rsi,      rdx
;   t[0] is r8		t[1] is r9
;   l is   r10 		h is r11
;   divisor is rcx
;   qn is rbx
;   i is bp
;   qf is r12
;   mask, q is r13

%include "yasm_mac.inc"

%define reg_save_list rbx, rsi, rdi, rbp, r12, r13

    BITS 64

	FRAME_PROC mpn_divrem_euclidean_qr_2, 0, reg_save_list
	mov     rdi, rcx
	mov     rsi, rdx
	movsxd  rdx, r8d
    mov     rcx, r9
    
	mov     rbx, rdx
	sub     rbx, 3
	xor     r11, r11
	mov     rax, -1
	mov     rdx, [rcx+8]
	not     rdx
	div     qword [rcx+8]
	mov     rbp, rax
	mov     r10, [rsi+rbx*8+16]
	mov     r8, [rsi+rbx*8+8]
	cmp     r10, [rcx+8]
	jae     L_j4
	mov     r11, r10
	mov     r9, r10
	mov     r10, [rsi+rbx*8+8]
	mov     r8, r10
	mov     r12, 0
	jmp     L_j5
L_j4:
	mov     r12, 1
	mov     r9, r10
	sub     r9, [rcx+8]
	sub     r8, [rcx]
	sbb     r9, 0
	jnc     L_j3
	dec     r12
	add     r8, [rcx]
	adc     r9, [rcx+8]
L_j3:
	mov     r11, r9
	mov     r10, r8
L_j5:
	cmp     rbx, 0
	js      L_skiplp

    xalign  16
L_lp:
	mov     r8, [rsi+rbx*8]
	cmp     r11, [rcx+8]
	je      L_j2
	mov     r13, r10
	sar     r13, 63
	mov     rax, r11
	sub     rax, r13
	and     r13, [rcx+8]
	add     r13, r10
	mul     rbp
	add     rax, r13
	adc     rdx, r11
	mov     r13, rdx
	not     rdx
	mov     rax, [rcx+8]
	mul     rdx
	add     rax, r10
	adc     rdx, r11
	sub     rdx, [rcx+8]
	sbb     r13, -1
	and     rdx, [rcx+8]
	add     rdx, rax
	mov     rax, [rcx]
	mov     r9, rdx
	mov     [rdi+rbx*8], r13
	mul     r13
	sub     r8, rax
	sbb     r9, rdx
	jnc     L_j1
	dec     qword[rdi+rbx*8]
	add     r8, [rcx]
	adc     r9, [rcx+8]
	jc      L_j1
	dec     qword[rdi+rbx*8]
	add     r8, [rcx]
	adc     r9, [rcx+8]
	jmp     L_j1
L_j2:
	mov     qword[rdi+rbx*8], -1
	mov     r9, r10
	add     r8, [rcx]
	adc     r9, r11
	sbb     rax, rax
	sub     r9, [rcx]
	adc     rax, 0
	jz      L_j1
	dec     qword[rdi+rbx*8]
	add     r8, [rcx]
	adc     r9, r11
L_j1:
	mov     r11, r9
	mov     r10, r8
	dec     rbx
	jns     L_lp
L_skiplp:
	mov     [rsi+8], r9
	mov     [rsi], r8
	mov     rax, r12
    END_PROC reg_save_list
    end
