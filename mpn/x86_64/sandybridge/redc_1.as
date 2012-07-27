
;  core2 mpn_redc_1
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

%include 'yasm_mac.inc'

;	(rdi, rcx) = (rsi, rcx) + (rdx, rcx)   with the carry flag set for the carry
;	this is the usual mpn_add_n with the final dec rax;adc rax,rax;ret  removed 
;	and a jump where we have two rets

%macro mpn_add 0
	mov     rax, rcx
	and     rax, 3
	shr     rcx, 2
	cmp     rcx, 0
;	carry flag is clear here
	jnz     %%1
	mov     r11, [rsi]
	add     r11, [rdx]
	mov     [rdi], r11
	dec     rax
	jz      %%2
	mov     r11, [rsi+8]
	adc     r11, [rdx+8]
	mov     [rdi+8], r11
	dec     rax
	jz      %%2
	mov     r11, [rsi+16]
	adc     r11, [rdx+16]
	mov     [rdi+16], r11
	jmp     %%2
	align 16
%%1:
	mov     r11, [rsi]
	mov     r8, [rsi+8]
	lea     rsi, [rsi+32]
	adc     r11, [rdx]
	adc     r8, [rdx+8]
	lea     rdx, [rdx+32]
	mov     [rdi], r11
	mov     [rdi+8], r8
	lea     rdi, [rdi+32]
	mov     r9, [rsi-16]
	mov     r10, [rsi-8]
	adc     r9, [rdx-16]
	adc     r10, [rdx-8]
	mov     [rdi-16], r9
	dec     rcx
	mov     [rdi-8], r10
	jnz     %%1
	inc     rax
	dec     rax
	jz      %%2
	mov     r11, [rsi]
	adc     r11, [rdx]
	mov     [rdi], r11
	dec     rax
	jz      %%2
	mov     r11, [rsi+8]
	adc     r11, [rdx+8]
	mov     [rdi+8], r11
	dec     rax
	jz      %%2
	mov     r11, [rsi+16]
	adc     r11, [rdx+16]
	mov     [rdi+16], r11
%%2:
%endmacro

;	(rbx, rbp) = (rsi, rbp) - (rdx, rbp)
%macro mpn_sub 0
	mov     rax, rbp
	and     rax, 3
	shr     rbp, 2
	cmp     rbp, 0
;	carry flag is clear here
	jnz     %%1
	mov     r11, [rsi]
	sub     r11, [rdx]
	mov     [rbx], r11
	dec     rax
	jz      %%2
	mov     r11, [rsi+8]
	sbb     r11, [rdx+8]
	mov     [rbx+8], r11
	dec     rax
	jz      %%2
	mov     r11, [rsi+16]
	sbb     r11, [rdx+16]
	mov     [rbx+16], r11
	jmp     %%2
	align 16
%%1:
	mov     r11, [rsi]
	mov     r8, [rsi+8]
	lea     rsi, [rsi+32]
	sbb     r11, [rdx]
	sbb     r8, [rdx+8]
	lea     rdx, [rdx+32]
	mov     [rbx], r11
	mov     [rbx+8], r8
	lea     rbx, [rbx+32]
	mov     r9, [rsi-16]
	mov     r10, [rsi-8]
	sbb     r9, [rdx-16]
	sbb     r10, [rdx-8]
	mov     [rbx-16], r9
	dec     rbp
	mov     [rbx-8], r10
	jnz     %%1
	inc     rax
	dec     rax
	jz      %%2
	mov     r11, [rsi]
	sbb     r11, [rdx]
	mov     [rbx], r11
	dec     rax
	jz      %%2
	mov     r11, [rsi+8]
	sbb     r11, [rdx+8]
	mov     [rbx+8], r11
	dec     rax
	jz      %%2
	mov     r11, [rsi+16]
	sbb     r11, [rdx+16]
	mov     [rbx+16], r11
%%2:
%endmacro

;	changes from standard addmul
;	change  r8 to r12   and rcx to r13   and rdi to r8
;	reemove ret and write last limb but to beginning
%macro addmulloop 1
	align 16
%%1:
	mov     r10, 0
	mul     r13
	add     [r8+r11*8], r12
	adc     r9, rax
	db      0x26
	adc     r10, rdx
	mov     rax, [rsi+r11*8+16]
	mul     r13
	add     [r8+r11*8+8], r9
	adc     r10, rax
	mov     ebx, 0
	adc     rbx, rdx
	mov     rax, [rsi+r11*8+24]
	mov     r12, 0
	mov     r9, 0
	mul     r13
	add     [r8+r11*8+16], r10
	db      0x26
	adc     rbx, rax
	db      0x26
	adc     r12, rdx
	mov     rax, [rsi+r11*8+32]
	mul     r13
	add     [r8+r11*8+24], rbx
	db      0x26
	adc     r12, rax
	db      0x26
	adc     r9, rdx
	add     r11, 4
	mov     rax, [rsi+r11*8+8]
	jnc     %%1
%endmacro

%macro addmulpropro0 0
	imul    r13, rcx
	lea     r8, [r8-8]
%endmacro

%macro addmulpro0 0
	mov     r11, r14
	lea     r8, [r8+8]
	mov     rax, [rsi+r14*8]
	mul     r13
	mov     r12, rax
	mov     rax, [rsi+r14*8+8]
	mov     r9, rdx
	cmp     r14, 0
%endmacro

%macro addmulnext0 0
	mov     r10d, 0
	mul     r13
	add     [r8+r11*8], r12
	adc     r9, rax
	adc     r10, rdx
	mov     rax, [rsi+r11*8+16]
	mul     r13
	add     [r8+r11*8+8], r9
	adc     r10, rax
	mov     ebx, 0
	adc     rbx, rdx
	mov     rax, [rsi+r11*8+24]
	mov     r12d, 0
	mov     r9d, 0
	mul     r13
	add     [r8+r11*8+16], r10
	adc     rbx, rax
	adc     r12, rdx
	mov     rax, [rsi+r11*8+32]
	mul     r13
	add     [r8+r11*8+24], rbx
	mov     r13, [r8+r14*8+8]
	adc     r12, rax
	adc     r9, rdx
	imul    r13, rcx
	add     [r8+r11*8+32], r12
	adc     r9, 0
	sub     r15, 1
	mov     [r8+r14*8], r9
%endmacro

%macro addmulpropro1 0
%endmacro

%macro addmulpro1 0
	imul    r13, rcx
	mov     rax, [rsi+r14*8]
	mov     r11, r14
	mul     r13
	mov     r12, rax
	mov     rax, [rsi+r14*8+8]
	mov     r9, rdx
	cmp     r14, 0
%endmacro

%macro addmulnext1 0
	mov     r10d, 0
	mul     r13
	add     [r8+r11*8], r12
	adc     r9, rax
	adc     r10, rdx
	mov     rax, [rsi+r11*8+16]
	mul     r13
	add     [r8+r11*8+8], r9
	adc     r10, rax
	mov     ebx, 0
	adc     rbx, rdx
	mov     rax, [rsi+r11*8+24]
	mov     r12d, 0
	mul     r13
	add     [r8+r11*8+16], r10
	adc     rbx, rax
	adc     r12, rdx
	add     [r8+r11*8+24], rbx
	mov     r13, [r8+r14*8+8]
	adc     r12, 0
	sub     r15, 1
	mov     [r8+r14*8], r12
	lea     r8, [r8+8]
%endmacro

%macro addmulpropro2 0
%endmacro

%macro addmulpro2 0
	imul    r13, rcx
	mov     rax, [rsi+r14*8]
	mov     r11, r14
	mul     r13
	mov     r12, rax
	mov     rax, [rsi+r14*8+8]
	mov     r9, rdx
	cmp     r14, 0
%endmacro

%macro addmulnext2 0
	mul     r13
	add     [r8+r11*8], r12
	adc     r9, rax
	mov     r10d, 0
	adc     r10, rdx
	mov     rax, [rsi+r11*8+16]
	mul     r13
	add     [r8+r11*8+8], r9
	adc     r10, rax
	mov     ebx, 0
	adc     rbx, rdx
	mov     r13, [r8+r14*8+8]
	add     [r8+r11*8+16], r10
	adc     rbx, 0
	mov     [r8+r14*8], rbx
	sub     r15, 1
	lea     r8, [r8+8]
%endmacro

%macro addmulpropro3 0
%endmacro

%macro addmulpro3 0
	imul    r13, rcx
	mov     rax, [rsi+r14*8]
	mov     r11, r14
	mul     r13
	mov     r12, rax
	mov     rax, [rsi+r14*8+8]
	mov     r9, rdx
	cmp     r14, 0
%endmacro

%macro addmulnext3 0
	mul     r13
	add     [r8+r11*8], r12
	adc     r9, rax
	mov     r10d, 0
	adc     r10, rdx
	add     [r8+r11*8+8], r9
	adc     r10, 0
	mov     r13, [r8+r14*8+8]
	mov     [r8+r14*8], r10
	lea     r8, [r8+8]
	sub     r15, 1
%endmacro

;	change r8 to r12
;	write top limb ax straight to mem dont return  (NOTE we WRITE NOT ADD)
%macro mpn_addmul_1_int 1
	addmulpropro%1
	align 16
%%1:
	addmulpro%1
	jge     %%2
	addmulloop %1
%%2:
	addmulnext%1
	jnz     %%1
	jmp     end
%endmacro

	BITS 64
    
   GLOBAL_FUNC mpn_redc_1
	mov r9,r8
	mov r8,rsi
	mov rsi,rdx
	mov rdx,rcx
	mov rcx,r9

	cmp     rdx, 1
	je      one
	push    r13
	push    r14
	push    rbx
	push    r12
	push    r15
	push    rbp
	mov     r14, 5
	sub     r14, rdx
;	store copys
	push    rsi
	push    r8
	lea     r8, [r8+rdx*8-40]
	lea     rsi, [rsi+rdx*8-40]
	mov     rbp, rdx
	mov     r15, rdx
	mov     rax, r14
	and     rax, 3
	mov     r13, [r8+r14*8]
	je      case0
	jp      case3
	cmp     rax, 1
	je      case1
case2:
	mpn_addmul_1_int 2
	align 16
case0:
	mpn_addmul_1_int 0
	align 16
case1:
	mpn_addmul_1_int 1
	align 16
case3:
	mpn_addmul_1_int 3
	align 16
end:
	mov     rcx, rbp
	pop     rdx
	lea     rsi, [rdx+rbp*8]
	mov     rbx, rdi
	mpn_add
;	mpnadd(rdi,rsi,rdx,rcx)
	pop     rdx
	jnc     skip
	mov     rsi, rbx
	mpn_sub
;	mpn_sub_n(rbx,rsi,rdx,rbp) we can certainly improve this sub
skip:
	pop     rbp
	pop     r15
	pop     r12
	pop     rbx
	pop     r14
	pop     r13
	ret
	align 16
one:
	mov     r9, [r8]
	mov     r11, [rsi]
	imul    rcx, r9
	mov     rax, rcx
	mul     r11
	add     rax, r9
;	rax is zero here
	adc     rdx, [r8+8]
	cmovnc  r11, rax
	sub     rdx, r11
	mov     [rdi], rdx
	ret
