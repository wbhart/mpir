; PROLOGUE(mpn_add_err2_n)
;
;  AMD64 mpn_add_err2_n, mpn_sub_err2_n
;
;  Copyright (C) 2009, David Harvey
;
;  Windows Conversion Copyright 2008 Brian Gladman
;
;  All rights reserved.
;
;  Redistribution and use in source and binary forms, with or without
;  modification, are permitted provided that the following conditions are
;  met:
;
;  1. Redistributions of source code must retain the above copyright notice,
;  this list of conditions and the following disclaimer.
;
;  2. Redistributions in binary form must reproduce the above copyright
;  notice, this list of conditions and the following disclaimer in the
;  documentation and/or other materials provided with the distribution.
;
;  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
;  ``AS IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
;  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A
;  PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
;  HOLDERS OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
;  SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED
;  TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
;  PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
;  LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
;  NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
;  SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
;
;  mp_limb_t mpn_add_err2_n (mp_ptr, mp_ptr, mp_ptr, mp_ptr, mp_ptr,  mp_ptr, mp_size_t, mp_limb_t);
;  mp_limb_t mpn_sub_err2_n (mp_ptr, mp_ptr, mp_ptr, mp_ptr, mp_ptr,  mp_ptr, mp_size_t, mp_limb_t);
;  rax                          rdi     rsi     rdx     rcx       r8      r9     8(rsp)    16(rsp)
;  rax                          rcx     rdx      r8      r9 [rsp+40] [rsp+48]  [rsp+56]   [rsp+64]

%include "yasm_mac.inc"

%define reg_save_list   rbx, rsi, rdi, rbp, r12, r13, r14

%macro fun 2
	xalign  16
    FRAME_PROC %1, 0, reg_save_list
    mov     rax, qword [rsp+stack_use+56]
    lea     rdi, [rcx+rax*8]
    lea     rsi, [rdx+rax*8]
    lea     rdx, [r8+rax*8]
    mov     rcx, r9
    mov     r10, rax
    mov     r8, [rsp+stack_use+40]
    mov     r9, [rsp+stack_use+48]
    mov     rax, [rsp+stack_use+64]
    
	xor     rbp, rbp
	xor     r11, r11
	xor     r12, r12
	xor     r13, r13
	sub     r9, r8
	test    r10, 1
	jnz     %%1
	lea     r8, [r8+r10*8-8]
	neg     r10
	jmp     %%2

	xalign  16
%%1:lea     r8, [r8+r10*8-16]
	neg     r10
	shr     rax, 1
	mov     rbx, [rsi+r10*8]
	%2      rbx, [rdx+r10*8]
	cmovc   rbp, [r8+8]
	cmovc   r12, [r8+r9+8]
	mov     [rdi+r10*8], rbx
	sbb     rax, rax
	inc     r10
	jz      %%3

	xalign  16
%%2:mov     rbx, [rsi+r10*8]
	shr     rax, 1
	%2      rbx, [rdx+r10*8]
	mov     [rdi+r10*8], rbx
	sbb     r14, r14
	mov     rbx, [rsi+r10*8+8]
	%2      rbx, [rdx+r10*8+8]
	mov     [rdi+r10*8+8], rbx
	sbb     rax, rax
    mov     rbx, [r8]
	and     rbx, r14
	add     rbp, rbx
	adc     r11, 0
    and     r14, [r8+r9]
	add     r12, r14
	adc     r13, 0
    mov     rbx, [r8-8]
	and     rbx, rax
	add     rbp, rbx
	adc     r11, 0
    mov     rbx, [r8+r9-8]
	and     rbx, rax
	add     r12, rbx
	adc     r13, 0
	add     r10, 2
	lea     r8, [r8-16]
	jnz     %%2
%%3:mov     [rcx], rbp
	mov     [rcx+8], r11
	mov     [rcx+16], r12
	mov     [rcx+24], r13
	and     eax, 1
    END_PROC reg_save_list
%endmacro

    CPU  Athlon64
    BITS 64
;		global __gmpn_add_err2_n

    fun mpn_add_err2_n, adc
    
    end
