
; AMD64 mpn_addmul_1/mpn_addmul_1c -- multiply and add (with carry)
; AMD64 mpn_submul_1/mpn_submul_1c -- multiply and subtract (with carry)
; Version 1.0.3.
;
; Copyright 2008 Jason Moxham and Brian Gladman
;
; Windows x64 ABI
;
;  Calling interface:
;
;  mp_limb_t __gmpn_<op>mul_1(          <op> = add or sub
;     mp_ptr dst,               rcx
;     mp_srcptr src,            rdx
;     mp_size_t size,            r8
;     mp_limb_t mult             r9
;  )
;
;  mp_limb_t __gmpn_<op>mul_1c(
;     mp_ptr dst,               rcx
;     mp_srcptr src,            rdx
;     mp_size_t size,            r8
;     mp_limb_t mult,            r9
;     mp_limb_t carry    [rsp+0x28]
;  )
;
; Calculate src[size] multiplied by mult[1] and add to /subtract from dst[size] and
; return the carry or borrow from the top of the result
;
; This is an SEH Frame Function with two leaf prologues
;
;   %1 = __g, %2 = add, %3 = mpn_addmul_1, %4 = mpn_addmul_1c
;   %1 = __g, %2 = sub, %3 = mpn_submul_1, %4 = mpn_submul_1c

%include "..\x86_64_asm.inc"

%define reg_save_list r12,r13,r14

%macro   mac_sub  4

    global  %1%3
    global  %1%4

%ifdef DLL
    export  %1%3
    export  %1%4
%endif

%1%3:
    xor     r11, r11        ; carry (0)
    movsxd  r8, r8d
    mov     rax, [rdx]
    cmp     r8, 1
    jnz     %%1		
	mul     r9
	%2      [rcx], rax
	adc     rdx, 0
	mov     rax, rdx
	ret
    
%1%4:
    mov     r11, [rsp+0x28] ; carry value
    movsxd  r8, r8d
    mov     rax, [rdx]
    cmp     r8, 1
    jnz     %%1		
	mul     r9
	add     rax, r11
	adc     rdx, 0
	%2      [rcx], rax
	adc     rdx, 0
	mov     rax, rdx
	ret

%%1:prologue %2xx, reg_save_list, 0
    xor     r13, r13
    mov     r10, rdx
    sub     r8, 5
    lea     r10, [r10+r8*8]
    lea     rcx, [rcx+r8*8]
    neg     r8
    mul     r9
    add     r11, rax
    mov     rax, [8+r10+r8*8]
    adc     r13, rdx
    cmp     r8, 0
    jge     %%3

    align   16
%%2:mov     r12d, 0
	mul     r9
	%2      [rcx+r8*8], r11
	adc     r13, rax
	adc     r12, rdx
	mov     rax, [16+r10+r8*8]
	mul     r9
	%2     [8+rcx+r8*8], r13
	adc     r12, rax
	mov     r14d, 0
	adc     r14, rdx
	mov     rax, [24+r10+r8*8]
	mov     r11d, 0
	mov     r13d, 0
	mul     r9
	%2      [16+rcx+r8*8], r12
	adc     r14, rax
	adc     r11, rdx
	mov     rax, [32+r10+r8*8]
 	mul     r9
	%2      [24+rcx+r8*8], r14
	adc     r11, rax
	adc     r13, rdx
	add     r8, 4
	mov     rax, [8+r10+r8*8]
	jnc     %%2

%%3:jz      %%7
    jp      %%5
    cmp     r8, 1
    je      %%6

%%4:mov     r12d, 0
	mul     r9
	%2      [rcx+r8*8],r11
	adc     r13, rax
	adc     r12, rdx
	mov     rax, [16+r10+r8*8]
	mul     r9
	%2      [8+rcx+r8*8], r13
	adc     r12, rax
	mov     r14d, 0
	adc     r14, rdx
	%2      [16+rcx+r8*8], r12
	adc     r14, 0
	mov     rax, r14
	jmp     %%8
	
    align   16
%%5:mov     r12d, 0
	mul     r9
	%2      [rcx+r8*8], r11
	adc     r13, rax
	adc     r12, rdx
	%2      [8+rcx+r8*8], r13
	adc     r12, 0
	mov     rax, r12
	jmp     %%8
	
	align   16
%%6:mov     r12d, 0
	mul     r9
	%2      [rcx+r8*8], r11
	adc     r13, rax
	adc     r12, rdx
	mov     rax, [16+r10+r8*8]
	mul     r9
	%2      [8+rcx+r8*8], r13
	adc     r12, rax
	mov     r14d, 0
	adc     r14, rdx
	mov     rax, [24+r10+r8*8]
	mov     r11d, 0
	mul     r9
	%2      [16+rcx+r8*8], r12
	adc     r14, rax
	adc     r11, rdx
	%2      [24+rcx+r8*8], r14
	adc     r11, 0
	mov     rax, r11
	jmp     %%8

    align   16	
%%7:mov     r12d, 0
	mul     r9
	%2      [rcx+r8*8], r11
	adc     r13, rax
	adc     r12, rdx
	mov     rax, [16+r10+r8*8]
	mul     r9
	%2      [8+rcx+r8*8], r13
	adc     r12, rax
	mov     r14d, 0
	adc     r14, rdx
	mov     rax, [24+r10+r8*8]
	mov     r11d, 0
	mov     r13d, 0
	mul     r9
	%2      [16+rcx+r8*8], r12
	adc     r14, rax
	adc     r11, rdx
	mov     rax, [32+r10+r8*8]
	mul     r9
	%2      [24+rcx+r8*8], r14
	adc     r11, rax
	adc     r13, rdx
	%2      [32+rcx+r8*8], r11
	adc     r13, 0
	mov     rax, r13
%%8:epilogue reg_save_list, 0 

%endmacro

    bits    64
    section .text

    mac_sub __g,add,mpn_addmul_1,mpn_addmul_1c
    mac_sub __g,sub,mpn_submul_1,mpn_submul_1c

    end
