
; AMD64 mpn_mul_1 -- mpn by limb multiply.
; Version 1.0.3.
;
; Copyright 2008 Jason Moxham 
; Windows conversion by Brian Gladman
;
; Windows x64 ABI
;
; Calling interface:
;
;  mp_limb_t mpn_mul_1 (
;     mp_ptr dst,                  rcx
;     mp_srcptr src,               rdx
;     mp_size_t size,               r8
;     mp_limb_t multiplier          r9
;  )
;
;  mp_limb_t mpn_mul_1c (
;     mp_ptr dst,                  rcx
;     mp_srcptr src,               rdx
;     mp_size_t size,               r8
;     mp_limb_t multiplier,         r9
;     mp_limb_t carry       [rsp+0x28]
;  )

%include "..\x86_64_asm.inc"

    bits    64
    section .text

    global  __gmpn_mul_1
    global  __gmpn_mul_1c

%ifdef DLL
    export  __gmpn_mul_1
    export  __gmpn_mul_1c
%endif

__gmpn_mul_1c:
    mov     r11, [rsp+0x28]
    jmp     start

__gmpn_mul_1:
    xor     r11, r11

start:
    prologue _gmulmm, r12, 0
    movsxd  rax, r8d
    mov     r8d, 3
    lea     r10, [rdx+rax*8-24]
    sub     r8, rax
    lea     rcx, [rcx+rax*8-24]
    jc      .1
    jmp     .2
    
    align   16
.1:	mov     rax, [r10+r8*8]
	mov     r12d, 0
	mul     r9
	add     r11, rax
	mov     [rcx+r8*8], r11
	mov     rax, [r10+r8*8+8]
	adc     r12, rdx
	mul     r9
	mov     r11d, 0
	add     r12, rax
	mov     rax, [r10+r8*8+16]
	adc     r11, rdx
	mul     r9
	mov     [rcx+r8*8+8], r12
	add     r11, rax
	mov     r12d, 0
	mov     [rcx+r8*8+16], r11
	mov     rax, [r10+r8*8+24]
	mov     r11d, 0
	adc     r12, rdx
	mul     r9
	add     r12, rax
	mov     [rcx+r8*8+24], r12
	adc     r11, rdx
	add     r8, 4
	jnc     .1

.2: test    r8, 2
    jnz     .3
	mov     rax, [r10+r8*8]
	mov     r12d, 0
	mul     r9
	add     r11, rax
	mov     [rcx+r8*8], r11
	mov     rax, [r10+r8*8+8]
	adc     r12, rdx
	mul     r9
	mov     r11d, 0
	add     r12, rax
	adc     r11, rdx
	add     r8, 2
	mov     [rcx+r8*8-8], r12

.3: test    r8, 1
    mov     rax, r11
    jnz     .4
	mov     rax, [r10+r8*8]
	mov     r12d, 0
	mul     r9
	add     r11, rax
	mov     [rcx+r8*8], r11
	adc     r12, rdx
	mov     rax, r12
.4: epilogue r12, 0 

    end
