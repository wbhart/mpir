
; void mpn_sqr_basecase (mp_ptr, mp_srcptr, mp_size_t)
;  rax                      rdi        rsi        rdx
;  rax                      rcx        rdx         r8

%include 'yasm_mac.inc'

%define reg_save_list   rbx, rbp, rsi, rdi, r12, r13, r14

%macro jmp_tab 2
	%ifdef PIC
		dd	%1 - %2
	%else
		dq	%1
	%endif
%endmacro

%macro	do_jmp 0
	lea     rax, [rel L0]
	%ifdef PIC
		mov     r10d, [rax+rcx*4]
		add     rax, r10
		jmp     rax
	%else
		jmp     [rax+rcx*8]
	%endif
%endmacro

	text
	xalign  16
	WIN64_GCC_PROC mpn_sqr_basecase, 3
	mov     ecx, edx
	mov     r11d, edx
	and     ecx, 3
	cmp     edx, 4
	lea     r8, [rcx+4]
	cmovg   rcx, r8
	do_jmp

	xalign  8
L0:	jmp_tab L4, L0
	jmp_tab L1, L0
	jmp_tab L2, L0
	jmp_tab L3, L0
	jmp_tab	L5, L0
	jmp_tab L8, L0
	jmp_tab L12, L0
	jmp_tab L15, L0

L1:	mov     rax, [rsi]
	mul     rax
	mov     [rdi], rax
	mov     [rdi+8], rdx
	WIN64_GCC_EXIT

L2:	mov     rax, [rsi]
	mov     r8, rax
	mul     rax
	mov     r11, [rsi+8]
	mov     [rdi], rax
	mov     rax, r11
	mov     r9, rdx
	mul     rax
	mov     r10, rax
	mov     rax, r11
	mov     r11, rdx
	mul     r8
	xor     r8, r8
	add     r9, rax
	adc     r10, rdx
	adc     r11, r8
	add     r9, rax
	mov     [rdi+8], r9
	adc     r10, rdx
	mov     [rdi+16], r10
	adc     r11, r8
	mov     [rdi+24], r11
	WIN64_GCC_EXIT

L3:	mov     rax, [rsi]
	mov     r10, rax
	mul     rax
	mov     r11, [rsi+8]
	mov     [rdi], rax
	mov     rax, r11
	mov     [rdi+8], rdx
	mul     rax
	mov     rcx, [rsi+16]
	mov     [rdi+16], rax
	mov     rax, rcx
	mov     [rdi+24], rdx
	mul     rax
	mov     [rdi+32], rax
	mov     [rdi+40], rdx
	mov     rax, r11
	mul     r10
	mov     r8, rax
	mov     rax, rcx
	mov     r9, rdx
	mul     r10
	xor     r10, r10
	add     r9, rax
	mov     rax, r11
	mov     r11, r10
	adc     r10, rdx
	mul     rcx
	add     r10, rax
	adc     rdx, r11
	add     r8, r8
	adc     r9, r9
	adc     r10, r10
	adc     rdx, rdx
	adc     r11, r11
	add     [rdi+8], r8
	adc     [rdi+16], r9
	adc     [rdi+24], r10
	adc     [rdi+32], rdx
	adc     [rdi+40], r11
	WIN64_GCC_EXIT

L4:	mov     rax, [rsi]
	mov     r11, rax
	mul     rax
	mov     rbx, [rsi+8]
	mov     [rdi], rax
	mov     rax, rbx
	mov     [rdi+8], rdx
	mul     rax
	mov     [rdi+16], rax
	mov     [rdi+24], rdx
	mov     rax, [rsi+16]
	mul     rax
	mov     [rdi+32], rax
	mov     [rdi+40], rdx
	mov     rax, [rsi+24]
	mul     rax
	mov     [rdi+48], rax
	mov     rax, rbx
	mov     [rdi+56], rdx
	mul     r11
	mov     r8, rax
	mov     r9, rdx
	mov     rax, [rsi+16]
	mul     r11
	xor     r10, r10
	add     r9, rax
	adc     r10, rdx
	mov     rax, [rsi+24]
	mul     r11
	xor     r11, r11
	add     r10, rax
	adc     r11, rdx
	mov     rax, [rsi+16]
	mul     rbx
	xor     rcx, rcx
	add     r10, rax
	adc     r11, rdx
	adc     rcx, 0
	mov     rax, [rsi+24]
	mul     rbx
	add     r11, rax
	adc     rcx, rdx
	mov     rdx, [rsi+16]
	mov     rax, [rsi+24]
	mul     rdx
	add     rcx, rax
	adc     rdx, 0
	add     r8, r8
	adc     r9, r9
	adc     r10, r10
	adc     r11, r11
	adc     rcx, rcx
	mov     eax, 0
	adc     rdx, rdx
	adc     rax, rax
	add     [rdi+8], r8
	adc     [rdi+16], r9
	adc     [rdi+24], r10
	adc     [rdi+32], r11
	adc     [rdi+40], rcx
	adc     [rdi+48], rdx
	adc     [rdi+56], rax
	WIN64_GCC_EXIT

L5:	lea     r12, [rdi+r11*8-16]
	mov     r13, [rsi]
	mov     rax, [rsi+8]
	lea     rsi, [rsi+r11*8]
	lea     r8, [r11-4]
	xor     r9d, r9d
	sub     r9, r11
	mul     r13
	xor     ebp, ebp
	mov     rbx, rax
	mov     rax, [rsi+r9*8+16]
	mov     r10, rdx
	jmp     .7

	xalign  16
.6:	add     rbp, rax
	mov     [r12+r9*8], r10
	mov     rax, [rsi+r9*8]
	adc     rcx, rdx
	xor     ebx, ebx
	mul     r13
	xor     r10d, r10d
	mov     [r12+r9*8+8], rbp
	add     rcx, rax
	adc     rbx, rdx
	mov     rax, [rsi+r9*8+8]
	mov     [r12+r9*8+16], rcx
	xor     ebp, ebp
	mul     r13
	add     rbx, rax
	mov     rax, [rsi+r9*8+16]
	adc     r10, rdx
.7:	xor     ecx, ecx
	mul     r13
	add     r10, rax
	mov     rax, [rsi+r9*8+24]
	adc     rbp, rdx
	mov     [r12+r9*8+24], rbx
	mul     r13
	add     r9, 4
	js      .6
	add     rbp, rax
	mov     [r12], r10
	adc     rcx, rdx
	mov     [r12+8], rbp
	mov     [r12+16], rcx
	lea		r12, [r12+16]
	lea     rsi, [rsi-8]
	jmp     L18

L8:	lea     r12, [rdi+r11*8+8]
	mov     r13, [rsi]
	mov     rax, [rsi+8]
	lea     rsi, [rsi+r11*8+8]
	lea     r8, [r11-3]
	lea     r9, [r11-3]
	neg     r9
	mov     r14, rax
	mul     r13
	mov     rcx, rdx
	xor     ebp, ebp
	mov     [rdi+8], rax
	jmp     .10

	xalign  16
.9:	mul     r14
	add     rbx, rax
	adc     rcx, rdx
	mov     rax, [rsi+r9*8-24]
	mov     ebp, 0
	mul     r13
	add     rbx, rax
	mov     rax, [rsi+r9*8-24]
	adc     rcx, rdx
	adc     ebp, 0
	mul     r14
	add     rcx, rax
	mov     [r12+r9*8-24], rbx
	adc     rbp, rdx
.10:mov     rax, [rsi+r9*8-16]
	mul     r13
	mov     r10d, 0
	add     rcx, rax
	adc     rbp, rdx
	mov     rax, [rsi+r9*8-16]
	adc     r10d, 0
	mov     ebx, 0
	mov     [r12+r9*8-16], rcx
	mul     r14
	add     rbp, rax
	mov     rax, [rsi+r9*8-8]
	adc     r10, rdx
	mov     ecx, 0
	mul     r13
	add     rbp, rax
	mov     rax, [rsi+r9*8-8]
	adc     r10, rdx
	adc     ebx, 0
	mul     r14
	add     r10, rax
	mov     [r12+r9*8-8], rbp
	adc     rbx, rdx
.11:mov     rax, [rsi+r9*8]
	mul     r13
	add     r10, rax
	adc     rbx, rdx
	adc     ecx, 0
	add     r9, 4
	mov     rax, [rsi+r9*8-32]
	mov     [r12+r9*8-32], r10
	js      .9
	mul     r14
	add     rbx, rax
	adc     rcx, rdx
	mov     [r12-8], rbx
	mov     [r12], rcx
	lea     rsi, [rsi-16]
	jmp     L24

L12:lea     r12, [rdi+r11*8-16]
	mov     r13, [rsi]
	mov     rax, [rsi+8]
	lea     rsi, [rsi+r11*8]
	lea     r8, [r11-4]
	lea     r9, [r11-2]
	neg     r9
	mul     r13
	mov     rbp, rax
	mov     rax, [rsi+r9*8]
	mov     rcx, rdx
	jmp     .14
	
	xalign  16
.13:add     rbp, rax
	mov     [r12+r9*8], r10
	mov     rax, [rsi+r9*8]
	adc     rcx, rdx
.14:xor     ebx, ebx
	mul     r13
	xor     r10d, r10d
	mov     [r12+r9*8+8], rbp
	add     rcx, rax
	adc     rbx, rdx
	mov     rax, [rsi+r9*8+8]
	mov     [r12+r9*8+16], rcx
	xor     ebp, ebp
	mul     r13
	add     rbx, rax
	mov     rax, [rsi+r9*8+16]
	adc     r10, rdx
	xor     ecx, ecx
	mul     r13
	add     r10, rax
	mov     rax, [rsi+r9*8+24]
	adc     rbp, rdx
	mov     [r12+r9*8+24], rbx
	mul     r13
	add     r9, 4
	js      .13
	add     rbp, rax
	mov     [r12], r10
	adc     rcx, rdx
	mov     [r12+8], rbp
	mov     [r12+16], rcx
	lea		r12, [r12+16]
	lea     rsi, [rsi-8]
	jmp     L21

L15:lea     r12, [rdi+r11*8+8]
	mov     r13, [rsi]
	mov     rax, [rsi+8]
	lea     rsi, [rsi+r11*8+8]
	lea     r8, [r11-5]
	lea     r9, [r11-1]
	neg     r9
	mov     r14, rax
	mul     r13
	mov     r10, rdx
	xor     ebx, ebx
	xor     ecx, ecx
	mov     [rdi+8], rax
	jmp     .17

	xalign  16
.16:mul     r14
	add     rbx, rax
	adc     rcx, rdx
	mov     rax, [rsi+r9*8-24]
	mov     ebp, 0
	mul     r13
	add     rbx, rax
	mov     rax, [rsi+r9*8-24]
	adc     rcx, rdx
	adc     ebp, 0
	mul     r14
	add     rcx, rax
	mov     [r12+r9*8-24], rbx
	adc     rbp, rdx
	mov     rax, [rsi+r9*8-16]
	mul     r13
	mov     r10d, 0
	add     rcx, rax
	adc     rbp, rdx
	mov     rax, [rsi+r9*8-16]
	adc     r10d, 0
	mov     ebx, 0
	mov     [r12+r9*8-16], rcx
	mul     r14
	add     rbp, rax
	mov     rax, [rsi+r9*8-8]
	adc     r10, rdx
	mov     ecx, 0
	mul     r13
	add     rbp, rax
	mov     rax, [rsi+r9*8-8]
	adc     r10, rdx
	adc     ebx, 0
	mul     r14
	add     r10, rax
	mov     [r12+r9*8-8], rbp
	adc     rbx, rdx
.17:mov     rax, [rsi+r9*8]
	mul     r13
	add     r10, rax
	adc     rbx, rdx
	adc     ecx, 0
	add     r9, 4
	mov     rax, [rsi+r9*8-32]
	mov     [r12+r9*8-32], r10
	js      .16
	mul     r14
	add     rbx, rax
	adc     rcx, rdx
	mov     [r12-8], rbx
	mov     [r12], rcx
	lea     rsi, [rsi-16]
	jmp     L21
L18:lea     r9, [r8+4]
	neg     r9
	mov     r13, [rsi+r9*8+16]
	mov     r14, [rsi+r9*8+24]
	mov     rax, [rsi+r9*8+24]
	mul     r13
	xor     r10d, r10d
	add     [r12+r9*8+24], rax
	adc     r10, rdx
	xor     ebx, ebx
	xor     ecx, ecx
	jmp     .20

	xalign  16
.19:add     [r12+r9*8], r10
	adc     rbx, rax
	mov     rax, [rsi+r9*8+8]
	adc     rcx, rdx
	mov     ebp, 0
	mul     r13
	add     rbx, rax
	mov     rax, [rsi+r9*8+8]
	adc     rcx, rdx
	adc     ebp, 0
	mul     r14
	add     [r12+r9*8+8], rbx
	adc     rcx, rax
	adc     rbp, rdx
	mov     rax, [rsi+r9*8+16]
	mov     r10d, 0
	mul     r13
	add     rcx, rax
	mov     rax, [rsi+r9*8+16]
	adc     rbp, rdx
	adc     r10d, 0
	mul     r14
	add     [r12+r9*8+16], rcx
	adc     rbp, rax
	mov     rax, [rsi+r9*8+24]
	adc     r10, rdx
	mul     r13
	mov     ebx, 0
	add     rbp, rax
	adc     r10, rdx
	mov     ecx, 0
	mov     rax, [rsi+r9*8+24]
	adc     ebx, 0
	mul     r14
	add     [r12+r9*8+24], rbp
	adc     r10, rax
	adc     rbx, rdx
.20:mov     rax, [rsi+r9*8+32]
	mul     r13
	add     r10, rax
	mov     rax, [rsi+r9*8+32]
	adc     rbx, rdx
	adc     ecx, 0
	mul     r14
	add     r9, 4
	js      .19
	add     [r12], r10
	adc     rbx, rax
	adc     rcx, rdx
	mov     [r12+8], rbx
	mov     [r12+16], rcx
	lea		r12, [r12+16]
	add     r8d, -2
L21:lea     r9, [r8+2]
	neg     r9
	mov     r13, [rsi+r9*8]
	mov     r14, [rsi+r9*8+8]
	mov     rax, [rsi+r9*8+8]
	mul     r13
	xor     ecx, ecx
	add     [r12+r9*8+8], rax
	adc     rcx, rdx
	xor     ebp, ebp
	jmp     .23

	xalign  16
.22:add     [r12+r9*8], r10
	adc     rbx, rax
	mov     rax, [rsi+r9*8+8]
	adc     rcx, rdx
	mov     ebp, 0
	mul     r13
	add     rbx, rax
	mov     rax, [rsi+r9*8+8]
	adc     rcx, rdx
	adc     ebp, 0
	mul     r14
	add     [r12+r9*8+8], rbx
	adc     rcx, rax
	adc     rbp, rdx
.23:mov     rax, [rsi+r9*8+16]
	mov     r10d, 0
	mul     r13
	add     rcx, rax
	mov     rax, [rsi+r9*8+16]
	adc     rbp, rdx
	adc     r10d, 0
	mul     r14
	add     [r12+r9*8+16], rcx
	adc     rbp, rax
	mov     rax, [rsi+r9*8+24]
	adc     r10, rdx
	mul     r13
	mov     ebx, 0
	add     rbp, rax
	adc     r10, rdx
	mov     ecx, 0
	mov     rax, [rsi+r9*8+24]
	adc     ebx, 0
	mul     r14
	add     [r12+r9*8+24], rbp
	adc     r10, rax
	adc     rbx, rdx
	mov     rax, [rsi+r9*8+32]
	mul     r13
	add     r10, rax
	mov     rax, [rsi+r9*8+32]
	adc     rbx, rdx
	adc     ecx, 0
	mul     r14
	add     r9, 4
	js      .22
	add     [r12], r10
	adc     rbx, rax
	adc     rcx, rdx
	mov     [r12+8], rbx
	mov     [r12+16], rcx
	lea		r12, [r12+16]
L24:add     r8d, -2
	jne     L18
	mov     r13, [rsi-16]
	mov     r14, [rsi-8]
	mov     rax, [rsi-8]
	mul     r13
	xor     r10d, r10d
	add     [r12-8], rax
	adc     r10, rdx
	xor     ebx, ebx
	xor     ecx, ecx
	mov     rax, [rsi]
	mul     r13
	add     r10, rax
	mov     rax, [rsi]
	adc     rbx, rdx
	mul     r14
	add     [r12], r10
	adc     rbx, rax
	adc     rcx, rdx
	mov     [r12+8], rbx
	mov     [r12+16], rcx
	lea     r9, [r11+r11-4]
	mov     r11, [rdi+8]
	lea     rsi, [rsi-8]
	lea     rdi, [rdi+r9*8]
	neg     r9
	mov     rax, [rsi+r9*4]
	mul     rax
	test    r9b, 2
	jnz     .26
.25:add     r11, r11
	sbb     ebx, ebx
	add     r11, rdx
	mov     [rdi+r9*8], rax
	jmp     .28
.26:add     r11, r11
	sbb     ebp, ebp
	add     r11, rdx
	mov     [rdi+r9*8], rax
	lea     r9, [r9-2]
	jmp     .29

	xalign  16
.27:mov     rax, [rsi+r9*4]
	mul     rax
	add     ebp, ebp
	adc     r10, rax
	adc     r11, rdx
	mov     [rdi+r9*8], r10
.28:mov     [rdi+r9*8+8], r11
	mov     r10, [rdi+r9*8+16]
	adc     r10, r10
	mov     r11, [rdi+r9*8+24]
	adc     r11, r11
	nop
	sbb     ebp, ebp
	mov     rax, [rsi+r9*4+8]
	mul     rax
	add     ebx, ebx
	adc     r10, rax
	adc     r11, rdx
	mov     [rdi+r9*8+16], r10
.29:mov     [rdi+r9*8+24], r11
	mov     r10, [rdi+r9*8+32]
	adc     r10, r10
	mov     r11, [rdi+r9*8+40]
	adc     r11, r11
	sbb     ebx, ebx
	add     r9, 4
	js      .27
	mov     rax, [rsi]
	mul     rax
	add     ebp, ebp
	adc     r10, rax
	adc     r11, rdx
	mov     [rdi], r10
	mov     [rdi+8], r11
	mov     r10, [rdi+16]
	adc     r10, r10
	sbb     ebp, ebp
	neg     ebp
	mov     rax, [rsi+8]
	mul     rax
	add     ebx, ebx
	adc     r10, rax
	adc     rdx, rbp
	mov     [rdi+16], r10
	mov     [rdi+24], rdx
	WIN64_GCC_END

	end
