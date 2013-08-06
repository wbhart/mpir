; PROLOGUE(mpn_add_err1_n)

;  Copyright (C) 2009, David Harvey
;
;  Windows Conversion Copyright 2008 Brian Gladman
;
;  All rights reserved.
;
;  Redistribution and use in source and binary forms, with or without
;  modification, are permitted provided that the following conditions are
;  met:
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
;  mp_limb_t mpn_add_err1_n(mp_ptr, mp_ptr, mp_ptr, mp_ptr, mp_ptr, mp_size_t, mp_limb_t)
;  mp_limb_t mpn_sub_err1_n(mp_ptr, mp_ptr, mp_ptr, mp_ptr, mp_ptr, mp_size_t, mp_limb_t)
;  rax                         rdi     rsi     rdx     rcx       r8        r9     8(rsp)
;  rax                         rcx     rdx      r8      r9 [rsp+40]  [rsp+48]   [rsp+56]

%include "yasm_mac.inc"

%define reg_save_list   rbx, rsi, rdi, rbp, r12

%macro fun 2
	xalign  16
    FRAME_PROC %1, 0, reg_save_list
    mov     rax, qword [rsp+stack_use+48]
    lea     rdi, [rcx+rax*8]
    lea     rsi, [rdx+rax*8]
    lea     rdx, [r8+rax*8]
    mov     rcx, r9
    mov     r9, rax
    mov     r8, [rsp+stack_use+40]
    mov     rax, [rsp+stack_use+56]
    
	xor     rbx, rbx
	xor     rbp, rbp
	test    r9, 1
	jnz     %%2
%%1:lea     r8, [r8+r9*8-8]
	neg     r9
	jmp     %%3

	xalign  16
%%2:lea     r8, [r8+r9*8-16]
	neg     r9
	shr     rax, 1
	mov     r12, [rsi+r9*8]
	%2      r12, [rdx+r9*8]
	cmovc   rbx, [r8+8]
	mov     [rdi+r9*8], r12
	setc    al
	inc     r9
	jz      %%4

	xalign  16
%%3:mov     r12, [rsi+r9*8]
	shr     rax, 1
	%2      r12, [rdx+r9*8]
	mov     r11, 0
	mov     [rdi+r9*8], r12
	mov     r10, 0
	mov     r12, [rsi+r9*8+8]
	cmovc   r10, [r8]
	%2      r12, [rdx+r9*8+8]
	cmovc   r11, [r8-8]
	setc    al
	add     rbx, r10
	adc     rbp, 0
	add     rbx, r11
	mov     [rdi+r9*8+8], r12
	adc     rbp, 0
	add     r9, 2
	lea     r8, [r8-16]
	jnz     %%3
%%4:mov     [rcx], rbx
	mov     [rcx+8], rbp
    END_PROC reg_save_list
%endmacro


    CPU  Athlon64
    BITS 64
;   global __gmpn_add_err1_n

    fun mpn_add_err1_n, adc
    
    end
