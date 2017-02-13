
; void mpn_sqr_basecase(mp_ptr, mp_srcptr, mp_size_t)
; Linux                    rdi        rsi        rdx
; Win64                    rcx        rdx         r8

%include 'yasm_mac.inc'

%define reg_save_list rbx, rsi, rdi, rbp, r12, r13, r14
    
    text
	xalign  32
	LEAF_PROC mpn_sqr_basecase 
	cmp     r8, 2
	jae     .0
	mov     rdx, [rdx]
	mulx    rdx, rax, rdx
	mov     [rcx], rax
	mov     [rcx+8], rdx
	ret
.0:	jne     .1
	mov     r11, [rdx+8]
	mov     rdx, [rdx]
	mulx    r10, r9, r11
	mulx    r8, rax, rdx
	mov     rdx, r11
	mulx    rdx, r11, rdx
	add     r9, r9
	adc     r10, r10
	adc     rdx, 0
	add     r8, r9
	adc     r10, r11
	adc     rdx, 0
	mov     [rcx], rax
	mov     [rcx+8], r8
	mov     [rcx+16], r10
	mov     [rcx+24], rdx
	ret

	xalign  32
.1:	FRAME_PROC mpn_sqr_basec1, 0, reg_save_list
    mov     rdi, rcx
    mov     rsi, rdx
    mov     rdx, r8  
    cmp     rdx, 4
	jae     .2
	mov     r8, [rsi]
	mov     rdx, [rsi+8]
	mov     r9, rdx
	mulx    rax, r11, r8
	mov     rdx, [rsi+16]
	mulx    rcx, r10, r8
	mov     r8, r11
	add     r10, rax
	adc     rcx, 0
	mulx    rax, rdx, r9
	add     rdx, rcx
	mov     [rdi+24], rdx
	adc     rax, 0
	mov     [rdi+32], rax
	xor     rcx, rcx
	mov     rdx, [rsi]
	mulx    r11, rax, rdx
	mov     [rdi], rax
	add     r8, r8
	adc     r10, r10
	setc    cl
	mov     rdx, [rsi+8]
	mulx    rdx, rax, rdx
	add     r8, r11
	adc     r10, rax
	mov     [rdi+8], r8
	mov     [rdi+16], r10
	mov     r8, [rdi+24]
	mov     r10, [rdi+32]
	lea     r11, [rdx+rcx]
	adc     r8, r8
	adc     r10, r10
	setc    cl
	mov     rdx, [rsi+16]
	mulx    rdx, rax, rdx
	add     r8, r11
	adc     r10, rax
	mov     [rdi+24], r8
	mov     [rdi+32], r10
	adc     rdx, rcx
	mov     [rdi+40], rdx
	EXIT_PROC reg_save_list
.2:
.3:	mov     r12, 0
	sub     r12, rdx
	mov     [rsp+stack_use+8], r12
	mov     r8, [rsi]
	mov     rdx, [rsi+8]
	lea     rcx, [r12+2]
	sar     rcx, 2
	inc     r12
	mov     r9, rdx
	test    r12b, 1
	jnz     .7
.4:	mulx    r11, rbx, r8
	mov     rdx, [rsi+16]
	mov     [rdi+8], rbx
	xor     rbx, rbx
	mulx    rbp, r10, r8
	test    r12b, 2
	jz      .6
.5:	lea     rdi, [rdi-8]
	lea     rsi, [rsi-8]
	jmp     .13
.6:	lea     rsi, [rsi+8]
	lea     rdi, [rdi+8]
	jmp     .11
.7:	mulx    rbp, r10, r8
	mov     rdx, [rsi+16]
	mov     [rdi+8], r10
	xor     r10, r10
	mulx    r11, rbx, r8
	test    r12b, 2
	jz      .12
.8:	lea     rdi, [rdi+16]
	lea     rsi, [rsi+16]
	jmp     .10
	xalign  32
.9:	mulx    r10, rax, r9
	add     rbx, rax
	mov     rdx, [rsi]
	mulx    r11, rax, r8
	adc     r10, 0
	add     rbx, rax
.10:adc     r11, 0
	add     rbx, rbp
	mov     [rdi], rbx
	adc     r11, 0
	mulx    rbx, rax, r9
	add     r10, rax
	mov     rdx, [rsi+8]
	adc     rbx, 0
	mulx    rbp, rax, r8
	add     r10, rax
	adc     rbp, 0
.11:add     r10, r11
	mov     [rdi+8], r10
	adc     rbp, 0
	mulx    r10, rax, r9
	add     rbx, rax
	mov     rdx, [rsi+16]
	mulx    r11, rax, r8
	adc     r10, 0
	add     rbx, rax
	adc     r11, 0
.12:add     rbx, rbp
	mov     [rdi+16], rbx
	adc     r11, 0
	mulx    rbx, rax, r9
	add     r10, rax
	mov     rdx, [rsi+24]
	adc     rbx, 0
	mulx    rbp, rax, r8
	add     r10, rax
	adc     rbp, 0
.13:add     r10, r11
	lea     rsi, [rsi+32]
	mov     [rdi+24], r10
	adc     rbp, 0
	inc     rcx
	lea     rdi, [rdi+32]
	jnz     .9
.14:mulx    rax, rdx, r9
	add     rbx, rdx
	adc     rax, 0
	add     rbx, rbp
	mov     [rdi], rbx
	adc     rax, 0
	mov     [rdi+8], rax
	lea     rsi, [rsi+16]
	lea     rdi, [rdi-16]
.15:
.16:
	lea     rsi, [rsi+r12*8]
	lea     rdi, [rdi+r12*8+48]
	mov     r8, [rsi-8]
	add     r12, 2
	cmp     r12, -2
	jge     .30
	mov     r9, [rsi]
	lea     rcx, [r12+1]
	sar     rcx, 2
	mov     rdx, r9
	test    r12b, 1
	jnz     .20
.17:mov     r13, [rdi]
	mov     r14, [rdi+8]
	mulx    r11, rax, r8
	add     r13, rax
	adc     r11, 0
	mov     [rdi], r13
	xor     rbx, rbx
	test    r12b, 2
	jnz     .19
.18:mov     rdx, [rsi+8]
	lea     rdi, [rdi+16]
	lea     rsi, [rsi+16]
	jmp     .26
.19:mov     rdx, [rsi+8]
	mov     r13, [rdi+16]
	lea     rsi, [rsi+32]
	inc     rcx
	mulx    rbp, rax, r8
	jz      .29
	jmp     .24
.20:mov     r14, [rdi]
	mov     r13, [rdi+8]
	mulx    rbp, rax, r8
	mov     rdx, [rsi+8]
	add     r14, rax
	adc     rbp, 0
	xor     r10, r10
	mov     [rdi], r14
	mulx    r11, rax, r8
	test    r12b, 2
	jz      .22
.21:mov     r14, [rdi+16]
	lea     rdi, [rdi+24]
	lea     rsi, [rsi+24]
	jmp     .25
.22:lea     rdi, [rdi+8]
	lea     rsi, [rsi+8]
	jmp     .27
	xalign  32
.23:mulx    rbp, rax, r8
	add     r14, r10
	adc     rbx, 0
.24:add     r14, rax
	adc     rbp, 0
	mulx    r10, rax, r9
	add     r13, rax
	adc     r10, 0
	lea     rdi, [rdi+32]
	add     r14, r11
	mov     rdx, [rsi-16]
	mov     [rdi-24], r14
	adc     rbp, 0
	add     r13, rbx
	mov     r14, [rdi-8]
	mulx    r11, rax, r8
	adc     r10, 0
.25:add     r13, rax
	mulx    rbx, rax, r9
	adc     r11, 0
	add     r13, rbp
	mov     [rdi-16], r13
	adc     r11, 0
	add     r14, rax
	adc     rbx, 0
	add     r14, r10
	mov     rdx, [rsi-8]
	adc     rbx, 0
.26:mulx    rbp, rax, r8
	add     r14, rax
	adc     rbp, 0
	mov     r13, [rdi]
	mulx    r10, rax, r9
	add     r13, rax
	adc     r10, 0
	add     r14, r11
	mov     [rdi-8], r14
	adc     rbp, 0
	mov     rdx, [rsi]
	add     r13, rbx
	mulx    r11, rax, r8
	adc     r10, 0
.27:add     r13, rax
	adc     r11, 0
	mulx    rbx, rax, r9
	add     r13, rbp
	mov     r14, [rdi+8]
	mov     [rdi], r13
	mov     r13, [rdi+16]
	adc     r11, 0
	add     r14, rax
	adc     rbx, 0
	mov     rdx, [rsi+8]
	lea     rsi, [rsi+32]
	inc     rcx
	jnz     .23
.28:mulx    rbp, rax, r8
	add     r14, r10
	adc     rbx, 0
.29:add     r14, rax
	adc     rbp, 0
	mulx    rax, rdx, r9
	add     r14, r11
	mov     [rdi+8], r14
	adc     rbp, 0
	add     rdx, rbx
	adc     rax, 0
	add     rbp, rdx
	mov     [rdi+16], rbp
	adc     rax, 0
	mov     [rdi+24], rax
	jmp     .16
.30:mov     r12, [rsp+stack_use+8]
	mov     rdx, [rsi]
	jg      .31
	mov     r9, rdx
	mov     r13, [rdi]
	mov     r14, rax
	mulx    r11, rax, r8
	add     r13, rax
	adc     r11, 0
	mov     [rdi], r13
	mov     rdx, [rsi+8]
	mulx    rbp, rax, r8
	add     r14, rax
	adc     rbp, 0
	mulx    rax, rdx, r9
	add     r14, r11
	mov     [rdi+8], r14
	adc     rbp, 0
	add     rdx, rbp
	mov     [rdi+16], rdx
	adc     rax, 0
	mov     [rdi+24], rax
	lea     rdi, [rdi+32]
	lea     rsi, [rsi+16]
	jmp     .32
.31:mulx    rbp, r14, r8
	add     r14, rax
	adc     rbp, 0
	mov     [rdi], r14
	mov     [rdi+8], rbp
	lea     rdi, [rdi+16]
	lea     rsi, [rsi+8]
.32:
.33:lea     rsi, [rsi+r12*8+8]
	lea     rdi, [rdi+r12*8]
	lea     rdi, [rdi+r12*8]
	inc     r12
	mov     rdx, [rsi-8]
	xor     rbx, rbx
	mulx    r10, rax, rdx
	mov     [rdi+8], rax
	mov     r8, [rdi+16]
	mov     r9, [rdi+24]
	jmp     .35
	xalign  16
.34:mov     r8, [rdi+32]
	mov     r9, [rdi+40]
	lea     rdi, [rdi+16]
	lea     r10, [rdx+rbx]
.35:adc     r8, r8
	adc     r9, r9
	setc    bl
	mov     rdx, [rsi]
	lea     rsi, [rsi+8]
	mulx    rdx, rax, rdx
	add     r8, r10
	adc     r9, rax
	mov     [rdi+16], r8
	mov     [rdi+24], r9
	inc     r12
	jnz     .34
.36:adc     rdx, rbx
	mov     [rdi+32], rdx
	END_PROC reg_save_list
