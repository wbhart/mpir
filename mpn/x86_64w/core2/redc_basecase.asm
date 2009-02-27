

;  Core2 mpn_redc_basecase
;  Version 1.0.4
;
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
;  Calling interface:
;
;  mp_limb_t __gmpn_redc_basecase(
;     mp_ptr dst,               rcx
;     mp_srcptr src,            rdx
;     mp_size_t len,             r8
;     mp_limb_t npr,             r9 (r9)
;     mp_ptr tp          [rsp + 40] (r10)
; )
;

%include "..\x86_64_asm.inc"

%define reg_save_list   rsi, rdi, rbx, rbp, r12, r13, r14, r15

%macro mpn_add 0

	mov     rax, r9
	and     rax, 3
	shr     r9, 2
	cmp     r9, 0
	jnz     %%1         ; carry flag is clear here
	mov     rbp, [rsi]
	add     rbp, [rdx]
	mov     [rcx], rbp
	dec     rax
	jz      %%2
	mov     rbp, [rsi+8]
	adc     rbp, [rdx+8]
	mov     [rcx+8], rbp
	dec     rax
	jz      %%2
	mov     rbp, [rsi+16]
	adc     rbp, [rdx+16]
	mov     [rcx+16], rbp
	jmp     %%2
	
	alignb  16, nop
%%1:mov     rbp, [rsi]
	mov     r10, [rsi+8]
	lea     rsi, [rsi+32]
	adc     rbp, [rdx]
	adc     r10, [rdx+8]
	lea     rdx, [rdx+32]
	mov     [rcx], rbp
	mov     [rcx+8], r10
	lea     rcx, [rcx+32]
	mov     r11, [rsi-16]
	mov     r12, [rsi-8]
	adc     r11, [rdx-16]
	adc     r12, [rdx-8]
	mov     [rcx-16], r11
	dec     r9
	mov     [rcx-8], r12
	jnz     %%1
	inc     rax
	dec     rax
	jz      %%2
	mov     rbp, [rsi]
	adc     rbp, [rdx]
	mov     [rcx], rbp
	dec     rax
	jz      %%2
	mov     rbp, [rsi+8]
	adc     rbp, [rdx+8]
	mov     [rcx+8], rbp
	dec     rax
	jz      %%2
	mov     rbp, [rsi+16]
	adc     rbp, [rdx+16]
	mov     [rcx+16], rbp
%%2:

%endmacro

%macro mpn_sub 0

	mov     rax, r8
	and     rax, 3
	shr     r8, 2
	cmp     r8, 0
	jnz     %%1         ; carry flag is clear here
	mov     rbp, [rsi]
	sub     rbp, [rdx]
	mov     [r15], rbp
	dec     rax
	jz      %%2
	mov     rbp, [rsi+8]
	sbb     rbp, [rdx+8]
	mov     [r15+8], rbp
	dec     rax
	jz      %%2
	mov     rbp, [rsi+16]
	sbb     rbp, [rdx+16]
	mov     [r15+16], rbp
	jmp     %%2

	alignb  16, nop
%%1:mov     rbp, [rsi]
	mov     r10, [rsi+8]
	lea     rsi, [rsi+32]
	sbb     rbp, [rdx]
	sbb     r10, [rdx+8]
	lea     rdx, [rdx+32]
	mov     [r15], rbp
	mov     [r15+8], r10
	lea     r15, [r15+32]
	mov     r11, [rsi-16]
	mov     r12, [rsi-8]
	sbb     r11, [rdx-16]
	sbb     r12, [rdx-8]
	mov     [r15-16], r11
	dec     r8
	mov     [r15-8], r12
	jnz     %%1
	inc     rax
	dec     rax
	jz      %%2
	mov     rbp, [rsi]
	sbb     rbp, [rdx]
	mov     [r15], rbp
	dec     rax
	jz      %%2
	mov     rbp, [rsi+8]
	sbb     rbp, [rdx+8]
	mov     [r15+8], rbp
	dec     rax
	jz      %%2
	mov     rbp, [rsi+16]
	sbb     rbp, [rdx+16]
	mov     [r15+16], rbp
%%2:

%endmacro

; changes from standard addmul
; change  r10 to r12   and r9 to r13   and rcx to r10
; reemove ret and write last limb but to beginning

%macro addmulloop 1

	alignb  16, nop
%%1:mov     r12, 0
	mul     rbx
	add     [r10+rbp*8], r13
	adc     r11, rax
	db      0x26
	adc     r12, rdx
	mov     rax, [rsi+rbp*8+16]
	mul     rbx
	add     [r10+rbp*8+8], r11
	adc     r12, rax
	mov     r15d, 0
	adc     r15, rdx
	mov     rax, [rsi+rbp*8+24]
	mov     r13, 0
	mov     r11, 0
	mul     rbx
	add     [r10+rbp*8+16], r12
	db      0x26
	adc     r15, rax
	db      0x26
	adc     r13, rdx
	mov     rax, [rsi+rbp*8+32]
	mul     rbx
	add     [r10+rbp*8+24], r15
	db      0x26
	adc     r13, rax
	db      0x26
	adc     r11, rdx
	add     rbp, 4
	mov     rax, [rsi+rbp*8+8]
	jnc     %%1

%endmacro

%macro addmulpropro0 0

	imul    rbx, r9
	lea     r10, [r10-8]

%endmacro

%macro addmulpro0 0

	mov     rbp, rdi
	lea     r10, [r10+8]
	mov     rax, [rsi+rdi*8]
	mul     rbx
	mov     r13, rax
	mov     rax, [rsi+rdi*8+8]
	mov     r11, rdx
	cmp     rdi, 0

%endmacro

%macro addmulnext0 0

	mov     r12d, 0
	mul     rbx
	add     [r10+rbp*8], r13
	adc     r11, rax
	adc     r12, rdx
	mov     rax, [rsi+rbp*8+16]
	mul     rbx
	add     [r10+rbp*8+8], r11
	adc     r12, rax
	mov     r15d, 0
	adc     r15, rdx
	mov     rax, [rsi+rbp*8+24]
	mov     r13d, 0
	mov     r11d, 0
	mul     rbx
	add     [r10+rbp*8+16], r12
	adc     r15, rax
	adc     r13, rdx
	mov     rax, [rsi+rbp*8+32]
	mul     rbx
	add     [r10+rbp*8+24], r15
	mov     rbx, [r10+rdi*8+8]
	adc     r13, rax
	adc     r11, rdx
	imul    rbx, r9
	add     [r10+rbp*8+32], r13
	adc     r11, 0
	sub     r14, 1
	mov     [r10+rdi*8], r11

%endmacro

%macro addmulpropro1 0

%endmacro

%macro addmulpro1 0

	imul    rbx, r9
	mov     rax, [rsi+rdi*8]
	mov     rbp, rdi
	mul     rbx
	mov     r13, rax
	mov     rax, [rsi+rdi*8+8]
	mov     r11, rdx
	cmp     rdi, 0

%endmacro

%macro addmulnext1 0

	mov     r12d, 0
	mul     rbx
	add     [r10+rbp*8], r13
	adc     r11, rax
	adc     r12, rdx
	mov     rax, [rsi+rbp*8+16]
	mul     rbx
	add     [r10+rbp*8+8], r11
	adc     r12, rax
	mov     r15d, 0
	adc     r15, rdx
	mov     rax, [rsi+rbp*8+24]
	mov     r13d, 0
	mul     rbx
	add     [r10+rbp*8+16], r12
	adc     r15, rax
	adc     r13, rdx
	add     [r10+rbp*8+24], r15
	mov     rbx, [r10+rdi*8+8]
	adc     r13, 0
	sub     r14, 1
	mov     [r10+rdi*8], r13
	lea     r10, [r10+8]

%endmacro

%macro addmulpropro2 0

%endmacro

%macro addmulpro2 0

	imul    rbx, r9
	mov     rax, [rsi+rdi*8]
	mov     rbp, rdi
	mul     rbx
	mov     r13, rax
	mov     rax, [rsi+rdi*8+8]
	mov     r11, rdx
	cmp     rdi, 0

%endmacro

%macro addmulnext2 0

	mul     rbx
	add     [r10+rbp*8], r13
	adc     r11, rax
	mov     r12d, 0
	adc     r12, rdx
	mov     rax, [rsi+rbp*8+16]
	mul     rbx
	add     [r10+rbp*8+8], r11
	adc     r12, rax
	mov     r15d, 0
	adc     r15, rdx
	mov     rbx, [r10+rdi*8+8]
	add     [r10+rbp*8+16], r12
	adc     r15, 0
	mov     [r10+rdi*8], r15
	sub     r14, 1
	lea     r10, [r10+8]

%endmacro

%macro addmulpropro3 0

%endmacro

%macro addmulpro3 0

	imul    rbx, r9
	mov     rax, [rsi+rdi*8]
	mov     rbp, rdi
	mul     rbx
	mov     r13, rax
	mov     rax, [rsi+rdi*8+8]
	mov     r11, rdx
	cmp     rdi, 0

%endmacro

%macro addmulnext3 0

	mul     rbx
	add     [r10+rbp*8], r13
	adc     r11, rax
	mov     r12d, 0
	adc     r12, rdx
	add     [r10+rbp*8+8], r11
	adc     r12, 0
	mov     rbx, [r10+rdi*8+8]
	mov     [r10+rdi*8], r12
	lea     r10, [r10+8]
	sub     r14, 1

%endmacro

; change r10 to r13
; write top limb ax straight to mem dont return  (NOTE we WRITE NOT ADD)

%macro mpn_addmul_1_int 1

	addmulpropro%1
	alignb  16, nop
%%1:addmulpro%1
	jge     %%2
	addmulloop %1
%%2:addmulnext%1
	jnz     %%1

%endmacro

   bits 64
   section .text

   global   __gmpn_redc_basecase
%ifdef DLL
   export   __gmpn_redc_basecase
%endif

__gmpn_redc_basecase:
    mov     r8d, r8d
	cmp     r8, 1
	je      L4
	
    prologue mpn_redc_1, 0, reg_save_list
    mov     rsi, rdx
    
	mov     rdi, 5
	sub     rdi, r8

	mov     [rsp+stack_use+0x10], rsi
	mov     r10, [rsp+stack_use+0x28]

	lea     r10, [r10+r8*8-40]
	lea     rsi, [rsi+r8*8-40]
	mov     r14, r8
	mov     rax, rdi
	and     rax, 3
	mov     rbx, [r10+rdi*8]
	je      .10
	jp      .13
	cmp     rax, 1
	je      .11

.12:mpn_addmul_1_int 2
	jmp     .2
	
	alignb  16, nop
.10:mpn_addmul_1_int 0
	jmp     .2

	alignb  16, nop
.11:mpn_addmul_1_int 1
	jmp     .2

	alignb  16, nop
.13:mpn_addmul_1_int 3
	
	alignb  16, nop
.2: mov     r9, r8
    mov     rdx, [rsp+stack_use+0x28]
	lea     rsi, [rdx+r8*8]
	mov     r15, rcx
	mpn_add
;   mpnadd(rcx,rsi,rdx,r9)
    mov     rdx, [rsp+stack_use+0x10]
	jnc     .3
	mov     rsi, r15
	mpn_sub
;   mpn_sub_n(r15,rsi,rdx,r8) we can certainly improve this sub

.3: epilogue reg_save_list

	alignb  16, nop
L4: mov     r8,[rsp+0x28]
	mov     r10, [r8]
	mov     r11, [rdx]
	imul    r9, r10
	mov     rax, r9
	mul     r11
	add     rax, r10
	adc     rdx, [r8+8]     ; rax is zero here
	cmovnc  r11, rax
	sub     rdx, r11
	mov     [rcx], rdx
	ret
	
	end
