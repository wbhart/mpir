;  AMD64 mpn_mul_basecase optimised for Intel Broadwell.
;
;  Copyright 2015 Free Software Foundation, Inc.
;
;  This file is part of the GNU MP Library.
;
;  The GNU MP Library is free software; you can redistribute it and/or modify
;  it under the terms of either:
;
;    * the GNU Lesser General Public License as published by the Free
;      Software Foundation; either version 3 of the License, or (at your
;      option) any later version.
;
;  or
;
;    * the GNU General Public License as published by the Free Software
;      Foundation; either version 2 of the License, or (at your option) any
;      later version.
;
;  or both in parallel, as here.
;
;  The GNU MP Library is distributed in the hope that it will be useful, but
;  WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
;  or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
;  for more details.
;
;  You should have received copies of the GNU General Public License and the
;  GNU Lesser General Public License along with the GNU MP Library.  If not,
;  see https://www.gnu.org/licenses/.
;
;  mp_limb_t mpn_mul_basecase(mp_ptr, mp_ptr, mp_size_t, mp_ptr, mp_size_t)
;  rax                           rdi     rsi        rdx     rcx         r8
;  rax                           rcx     rdx         r8      r9   [rsp+40]

%include 'yasm_mac.inc'

%define reg_save_list rsi, rdi, rbx, rbp, r12, r14

    BITS 64
	align	16

	LEAF_PROC mpn_mul_basecase
	cmp     r8, 2
	ja      .4
    mov     r8, rdx
	mov     rdx, [r9]
	mulx    r11, rax, [r8]
    mov     [rcx], rax
	je      .1
.0:	mov     [rcx+8], r11
	ret
.1:	cmp     qword [rsp+40], 2
	mulx    r10, rax, [r8+8]
	je      .3
.2:	add     r11, rax
	adc     r10, 0
	mov     [rcx+8], r11
	mov     [rcx+16], r10
	ret
.3:	add     r11, rax
	adc     r10, 0
	mov     rdx, [r9+8]
    mov     rax, r8
	mulx    r9, r8, [rax]
	mulx    rdx, rax, [rax+8]
	add     rax, r9
	adc     rdx, 0
	add     r11, r8
	adc     r10, rax
	adc     rdx, 0
	mov     [rcx+8], r11
	mov     [rcx+16], r10
	mov     [rcx+24], rdx
	ret

	align	16
.4:
    FRAME_PROC mpn_mul_bc, 0, reg_save_list
    mov     rdi, rcx
    mov     rsi, rdx
    mov     rdx, r8
    mov     rcx, r9
    mov     r8, [rsp+stack_use+40]

	mov     r14, rcx
	lea     rbx, [rdx+1]
	mov     rbp, rdx
	mov     eax, edx
	and     rbx, -8
	shr     rbp, 3
	neg     rbx
	and     eax, 7
	mov     rcx, rbp
	mov     rdx, [r14]
	lea     r14, [r14+8]
	lea     r10, [rel .41]
	movsxd  r11, dword [r10+rax*4]
	lea     r10, [r11+r10]
	jmp     r10
.5:	mulx    r11, r10, [rsi]
	lea     rsi, [rsi+56]
	lea     rdi, [rdi-8]
	jmp     .15
.6:	mulx    r9, r12, [rsi]
	lea     rsi, [rsi+16]
	lea     rdi, [rdi+16]
	inc     rcx
	jmp     .20
.7:	mulx    r11, r10, [rsi]
	lea     rsi, [rsi+24]
	lea     rdi, [rdi+24]
	inc     rcx
	jmp     .19
.8:	mulx    r9, r12, [rsi]
	lea     rsi, [rsi+32]
	lea     rdi, [rdi+32]
	inc     rcx
	jmp     .18
.9:	mulx    r11, r10, [rsi]
	lea     rsi, [rsi+40]
	lea     rdi, [rdi+40]
	inc     rcx
	jmp     .17
.10:mulx    r9, r12, [rsi]
	lea     rsi, [rsi+48]
	lea     rdi, [rdi+48]
	inc     rcx
	jmp     .16
.11:mulx    r9, r12, [rsi]
	jmp     .14
.12:mulx    r11, r10, [rsi]
	lea     rsi, [rsi+8]
	lea     rdi, [rdi+8]
	mulx    r9, r12, [rsi]
	
	align	16
.13:mov     [rdi-8], r10
	adc     r12, r11
.14:mulx    r11, r10, [rsi+8]
	adc     r10, r9
	lea     rsi, [rsi+64]
	mov     [rdi], r12
.15:mov     [rdi+8], r10
	mulx    r9, r12, [rsi-48]
	lea     rdi, [rdi+64]
	adc     r12, r11
.16:mulx    r11, r10, [rsi-40]
	mov     [rdi-48], r12
	adc     r10, r9
.17:mov     [rdi-40], r10
	mulx    r9, r12, [rsi-32]
	adc     r12, r11
.18:mulx    r11, r10, [rsi-24]
	mov     [rdi-32], r12
	adc     r10, r9
.19:mulx    r9, r12, [rsi-16]
	mov     [rdi-24], r10
	adc     r12, r11
.20:mulx    r11, r10, [rsi-8]
	adc     r10, r9
	mov     [rdi-16], r12
	dec     rcx
	mulx    r9, r12, [rsi]
	jnz     .13
.21:mov     [rdi-8], r10
	adc     r12, r11
	mov     [rdi], r12
	adc     r9, rcx
	mov     [rdi+8], r9
	dec     r8
	jz      .31
	lea     r10, [rel .42]
	movsxd  rax, dword [r10+rax*4]
	lea     rax, [rax+r10]
.22:lea     rsi, [rsi+rbx*8]
	mov     rcx, rbp
	mov     rdx, [r14]
	lea     r14, [r14+8]
	jmp     rax
.23:mulx    r11, r10, [rsi+8]
	lea     rdi, [rdi+rbx*8+8]
	lea     rcx, [rcx-1]
	jmp     .35
.24:mulx    r9, r12, [rsi-16]
	lea     rdi, [rdi+rbx*8-56]
	jmp     .40
.25:mulx    r11, r10, [rsi-24]
	lea     rdi, [rdi+rbx*8-56]
	jmp     .39
.26:mulx    r9, r12, [rsi-32]
	lea     rdi, [rdi+rbx*8-56]
	jmp     .38
.27:mulx    r11, r10, [rsi-40]
	lea     rdi, [rdi+rbx*8-56]
	jmp     .37
.28:mulx    r9, r12, [rsi+16]
	lea     rdi, [rdi+rbx*8+8]
	jmp     .36
.29:mulx    r9, r12, [rsi]
	lea     rdi, [rdi+rbx*8+8]
	jmp     .34
.30:adox    r12, [rdi]
	adox    r9, rcx
	mov     [rdi], r12
	adc     r9, rcx
	mov     [rdi+8], r9
	dec     r8
	jnz     .22
.31:
    END_PROC reg_save_list

.32:mulx    r11, r10, [rsi-8]
	lea     rdi, [rdi+rbx*8+8]
	mulx    r9, r12, [rsi]

	align	16
.33:adox    r10, [rdi-8]
	adcx    r12, r11
	mov     [rdi-8], r10
	jrcxz   .30
.34:mulx    r11, r10, [rsi+8]
	adox    r12, [rdi]
	lea     rcx, [rcx-1]
	mov     [rdi], r12
	adcx    r10, r9
.35:mulx    r9, r12, [rsi+16]
	adcx    r12, r11
	adox    r10, [rdi+8]
	mov     [rdi+8], r10
.36:mulx    r11, r10, [rsi+24]
	lea     rsi, [rsi+64]
	adcx    r10, r9
	adox    r12, [rdi+16]
	mov     [rdi+16], r12
.37:mulx    r9, r12, [rsi-32]
	adox    r10, [rdi+24]
	adcx    r12, r11
	mov     [rdi+24], r10
.38:mulx    r11, r10, [rsi-24]
	adcx    r10, r9
	adox    r12, [rdi+32]
	mov     [rdi+32], r12
.39:mulx    r9, r12, [rsi-16]
	adox    r10, [rdi+40]
	adcx    r12, r11
	mov     [rdi+40], r10
.40:adox    r12, [rdi+48]
	mulx    r11, r10, [rsi-8]
	mov     [rdi+48], r12
	lea     rdi, [rdi+64]
	adcx    r10, r9
	mulx    r9, r12, [rsi]
	jmp     .33

	align	8
.41:
	dd      .5 - .41
	dd     .11 - .41
	dd     .12 - .41
	dd      .6 - .41
	dd      .7 - .41
	dd      .8 - .41
	dd      .9 - .41
	dd     .10 - .41
.42:
	dd     .23 - .42
	dd     .29 - .42
	dd     .32 - .42
	dd     .24 - .42
	dd     .25 - .42
	dd     .26 - .42
	dd     .27 - .42
	dd     .28 - .42
