
;  AMD64 mpn_sqr_basecase -- square an mpn number.
;  Version 1.0.3.
;
;  Copyright 2008 Jason Moxham 
;  Windows converdxon by Brian Gladman

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

;  Calling interface:
;
;  void mpn_sqr_basecase(
;     mp_ptr dst,       rcx
;     mp_srcptr src,    rdx
;     mp_size_t size     r8
;  )
;
; Squuare src[size] and write the result to dst[2 * size]
;
; This is an SEH Frame Function with a leaf prologue

; changes from standard mpn_mul_1
; change lables
; change r8 to r11
; write top limb ax straight to mem dont return
; change lea offsets to match addmul
; reorder mod4 tail parts so have "only .1 ret"
; remove ret
; todo --------  could ignore small values ie mulskiploop for dx=1,2,3 we save .1 branch
; todo ------------- alignment of jumps etc

; AMD64 mpn_sqr_basecase -- square an mpn number.
;
;  Calling interface:

%macro mpn_mul_1_int 0

	mov     r10d, 4
	sub     r10, rdx
	mov     r11d, 0
	jnc     %%2
	
	align   16
%%1:mov     rax, [rsi+r10*8+16]
	mov     r9d, 0
	mul     r13
	add     r11, rax
	mov     [rdi+r10*8+16], r11
	mov     rax, [rsi+r10*8+24]
	adc     r9, rdx
	mul     r13
	mov     r11d, 0
	add     r9, rax
	mov     rax, [rsi+r10*8+32]
	adc     r11, rdx
	mul     r13
	mov     [rdi+r10*8+24], r9
	add     r11, rax
	mov     r9d, 0
	mov     [rdi+r10*8+32], r11
	mov     rax, [rsi+r10*8+40]
	mov     r11d, 0
	adc     r9, rdx
	mul     r13
	add     r9, rax
	mov     [rdi+r10*8+40], r9
	adc     r11, rdx
	add     r10, 4
	jnc     %%1

%%2:test    r10, 2
	jnz     %%3
	mov     rax, [rsi+r10*8+16]
	mov     r9d, 0
	mul     r13
	add     r11, rax
	mov     [rdi+r10*8+16], r11
	mov     rax, [rsi+r10*8+24]
	adc     r9, rdx
	mul     r13
	mov     r11d, 0
	add     r9, rax
	adc     r11, rdx
	add     r10, 2
	mov     [rdi+r10*8+8], r9

%%3:test    r10, 1
	jnz     %%4
	mov     rax, [rsi+r10*8+16]
	mov     r9d, 0
	mul     r13
	add     r11, rax
	adc     r9, rdx
	mov     [rdi+r10*8+24], r9

%%4:mov     [rdi+r10*8+16], r11

%endmacro

; changes from standard mpn_addmul_1 internal loop
; change lables
; change r8 to r12   , rcx to r13

%macro addmulloop 1

	align   16
%%1:mov     r10d, 0
	mul     r13
	add     [rdi+r11*8], r12
	adc     r9, rax
	adc     r10, rdx
	mov     rax, [rsi+r11*8+16]
	mul     r13
	add     [rdi+r11*8+8], r9
	adc     r10, rax
	mov     ebx, 0
	adc     rbx, rdx
	mov     rax, [rsi+r11*8+24]
	mov     r12d, 0
	mov     r9d, 0
	mul     r13
	add     [rdi+r11*8+16], r10
	adc     rbx, rax
	adc     r12, rdx
	mov     rax, [rsi+r11*8+32]
	mul     r13
	add     [rdi+r11*8+24], rbx
	adc     r12, rax
	adc     r9, rdx
	add     r11, 4
	mov     rax, [rsi+r11*8+8]
	jnc     %%1
	
%endmacro

%macro addmulnext0 0

; here is 3 loads ie if r11=0
	mov     r10d, 0
	mul     r13
	add     [rdi+r11*8], r12
	adc     r9, rax
	adc     r10, rdx
	mov     rax, [rsi+r11*8+16]
	mul     r13
	add     [rdi+r11*8+8], r9
	adc     r10, rax
	mov     ebx, 0
	adc     rbx, rdx
	mov     rax, [rsi+r11*8+24]
	mov     r12d, 0
	mov     r9d, 0
	mul     r13
	add     [rdi+r11*8+16], r10
	adc     rbx, rax
	adc     r12, rdx
	mov     rax, [rsi+r11*8+32]
	mul     r13
	add     [rdi+r11*8+24], rbx
	adc     r12, rax
	adc     r9, rdx
	add     [rdi+r11*8+32], r12
	adc     r9, 0
	mov     [rdi+r11*8+40], r9

%endmacro

%macro addmulnext1 0

; here is 2 loads ie if r11=1
	mov     r10d, 0
	mul     r13
	add     [rdi+r11*8], r12
	adc     r9, rax
	adc     r10, rdx
	mov     rax, [rsi+r11*8+16]
	mul     r13
	add     [rdi+r11*8+8], r9
	adc     r10, rax
	mov     ebx, 0
	adc     rbx, rdx
	mov     rax, [rsi+r11*8+24]
	mov     r12d, 0
	mul     r13
	add     [rdi+r11*8+16], r10
	adc     rbx, rax
	adc     r12, rdx
	add     [rdi+r11*8+24], rbx
	adc     r12, 0
	mov     [rdi+r11*8+32], r12

%endmacro

%macro addmulnext2 0

; here is 1 load ie if r11=2
	mov     r10d, 0
	mul     r13
	add     [rdi+r11*8], r12
	adc     r9, rax
	adc     r10, rdx
	mov     rax, [rsi+r11*8+16]
	mul     r13
	add     [rdi+r11*8+8], r9
	adc     r10, rax
	mov     ebx, 0
	adc     rbx, rdx
	add     [rdi+r11*8+16], r10
	adc     rbx, 0
	mov     [rdi+r11*8+24], rbx

%endmacro

%macro addmulnext3 0

; here is 0 loads ie if r11=3
	mov     r10d, 0
	mul     r13
	add     [rdi+r11*8], r12
	adc     r9, rax
	adc     r10, rdx
	add     [rdi+r11*8+8], r9
	adc     r10, 0
	mov     [rdi+r11*8+16], r10

%endmacro

; changes from standard addmul_1
; change lables
; change r8 to r12
; write top limb ax straight to mem dont return  (NOTE we WRITE NOT ADD)
; remove .1 limb special .5
; remove push/pop , base.5 must save rbx
; split into mod4 types and remove rets
; surround with outer loop and pop ret
; todo ----------- can ignore small values
; this addmul MUST have a param whic is 0123 which is our r11

%macro mpn_addmul_1_int 1

%%1:mov     rax, [rsi+r14*8]
	mov     r13, [rsi+r14*8-8]
	mov     r11, r14
	mul     r13
	mov     r12, rax
	mov     rax, [rsi+r14*8+8]
	mov     r9, rdx
	cmp     r14, 0
	jge     %%2
	addmulloop %1
%%2:addmulnext%1
	inc     r14
	lea     rdi, [rdi+8]
	cmp     r14, 4
	jz      .6

%endmacro

%include "..\x86_64_asm.inc"

%define reg_save_list   rbx, rsi, rdi, r12, r13, r14

   bits     64
   section  .text

   global   __gmpn_sqr_basecase

%ifdef DLL
   export   __gmpn_sqr_basecase
%endif

__gmpn_sqr_basecase:
	cmp     r8d, 1
	ja      .2
.1:	mov     rax, [rdx]
	mul     rax
	mov     [rcx], rax
	mov     [rcx+8], rdx
	ret

.2: prologue __gmpn_sqr_bc, reg_save_list, 3
    mov     rdi, rcx
    mov     rsi, rdx
    mov     edx, r8d
    cmp     rdx, 3
    ja      .4
    jz      .3
    mov     rax, [rsi]
	mul     qword [rsi+8]
	mov     [rdi+8], rax
	mov     [rdi+16], rdx
	mov     rcx, 2
	jmp     .7

.3: mov     rax, [rsi]
	mul     qword [rsi+8]
	mov     [rdi+8], rax
	mov     rax, [rsi]
	mov     r8, rdx
	mul     qword [rsi+16]
	add     r8, rax
	adc     rdx, 0
	mov     [rdi+16], r8
	mov     [rdi+24], rdx
	mov     rax, [rsi+8]
	mul     qword [rsi+16]
	add     [rdi+24], rax
	adc     rdx, 0
	mov     [rdi+32], rdx
	mov     rcx, 3
	jmp     .7

; this code can not handle casess 3,2,1
.4:	mov     [rsp], rdi          ; STACK USE HERE
	mov     [rsp+8], rsi
	mov     [rsp+16], rdx
	
	mov     r13, [rsi]
	mov     r14, 7
	sub     r14, rdx
	lea     rdi, [rdi+rdx*8-40]
	lea     rsi, [rsi+rdx*8-40]
	mpn_mul_1_int
	lea     rdi, [rdi+8]
	mov     rax, r14
	and     rax, 3
	je      .50
	jp      .53
	cmp     rax, 1
	je      .51
	
	align   16
.5:
.52:mpn_addmul_1_int 2
.53:mpn_addmul_1_int 3
.50:mpn_addmul_1_int 0
.51:mpn_addmul_1_int 1
	jmp     .5
	
; only need to add one more line here

.6:	mov     rax, [rsi+r14*8]
	mov     r13, [rsi+r14*8-8]
	mul     r13
	add     [rdi+r14*8], rax
	adc     rdx, 0
	mov     [rdi+r14*8+8], rdx
	
; now lsh by 1 and add in the diagonal
	
	mov     rcx, [rsp+16]       ; STACK USE HERE
	mov     rsi, [rsp+8]
	mov     rdi, [rsp]
	
.7: xor     rbx, rbx
	xor     r14, r14
	lea     rsi, [rsi+rcx*8]
	mov     [rdi], r14
	lea     r10, [rdi+rcx*8]
	mov     [r10+rcx*8-8], r14
	neg     rcx

.8: mov     rax, [rsi+rcx*8]
	mul     rax
	mov     r8, [rdi]
	mov     r9, [rdi+8]
	add     rbx, 1
	adc     r8, r8
	adc     r9, r9
	sbb     rbx, rbx
	add     r14, 1
	adc     r8, rax
	adc     r9, rdx
	sbb     r14, r14
	mov     [rdi], r8
	mov     [rdi+8], r9
	inc     rcx
	lea     rdi, [rdi+16]
	jnz     .8
	epilogue reg_save_list, 3
	
	end
	
