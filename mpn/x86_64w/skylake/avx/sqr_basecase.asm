;  AMD64 mpn_sqr_basecase optimised for Intel Broadwell.

;  Copyright 2015 Free Software Foundation, Inc.

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
; void mpn_sqr_basecase(mp_ptr, mp_srcptr, mp_size_t)
; Linux                    rdi        rsi        rdx
; Win64                    rcx        rdx         r8

%include 'yasm_mac.inc'

%define reg_save_list rsi, rdi, rbx

	TEXT
	align	16
    LEAF_PROC mpn_sqr_basecase
	cmp     r8, 2
	jae     .1
	mov     rdx, [rdx]
	mulx    rdx, rax, rdx
	mov     [rcx], rax
	mov     [rcx+8], rdx
	ret
.1:	jne     .2
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
.2:	
    FRAME_PROC ?mpn_sqb, 0, reg_save_list
    mov     rdi, rcx
    mov     rsi, rdx
    mov     rdx, r8

    cmp     rdx, 4
	jae     .3
	mov     rdx, [rsi]
	mulx    r11, r10, [rsi+8]
	mulx    r9, r8, [rsi+16]
	add     r8, r11
	mov     rdx, [rsi+8]
	mulx    r11, rax, [rsi+16]
	adc     r9, rax
	adc     r11, 0
	test    ebx, ebx
	mov     rdx, [rsi]
	mulx    rcx, rbx, rdx
	mov     [rdi], rbx
	mov     rdx, [rsi+8]
	mulx    rbx, rax, rdx
	mov     rdx, [rsi+16]
	mulx    rdx, rsi, rdx
	adcx    r10, r10
	adcx    r8, r8
	adcx    r9, r9
	adcx    r11, r11
	adox    rcx, r10
	adox    rax, r8
	adox    rbx, r9
	adox    rsi, r11
	mov     r8d, 0
	adox    rdx, r8
	adcx    rdx, r8
	mov     [rdi+8], rcx
	mov     [rdi+16], rax
	mov     [rdi+24], rbx
	mov     [rdi+32], rsi
	mov     [rdi+40], rdx
	EXIT_PROC reg_save_list

.3:	mov     [rsp+stack_use+8], rdi
    mov     [rsp+stack_use+16], rsi
    mov     [rsp+stack_use+24], rdx
	lea     ebx, [rdx-3]
	lea     rcx, [rdx+5]
	mov     eax, edx
	and     ebx, -8
	shr     ecx, 3
	neg     rbx
	and     eax, 7
	mov     rdx, [rsi]
	lea     r10, [rel .58]
	movsxd  r8, dword [r10+rax*4]
	lea     r10, [r8+r10]
	jmp     r10
.4:	mulx    r11, r10, [rsi+8]
	lea     rsi, [rsi+64]
	jmp     .14
.5:	mulx    r9, r8, [rsi+8]
	lea     rsi, [rsi+24]
	lea     rdi, [rdi+24]
	jmp     .19
.6:	mulx    r11, r10, [rsi+8]
	lea     rsi, [rsi+32]
	lea     rdi, [rdi+32]
	jmp     .18
.7:	mulx    r9, r8, [rsi+8]
	lea     rsi, [rsi+40]
	lea     rdi, [rdi+40]
	jmp     .17
.8:	mulx    r11, r10, [rsi+8]
	lea     rsi, [rsi+48]
	lea     rdi, [rdi+48]
	jmp     .16
.9:	mulx    r9, r8, [rsi+8]
	lea     rsi, [rsi+56]
	lea     rdi, [rdi+56]
	jmp     .15
.10:mulx    r9, r8, [rsi+8]
	lea     rsi, [rsi+8]
	lea     rdi, [rdi+8]
	jmp     .13
.11:mulx    r11, r10, [rsi+8]
	lea     rsi, [rsi+16]
	lea     rdi, [rdi+16]
	dec     ecx
	mulx    r9, r8, [rsi]
	align	16
.12:mov     [rdi-8], r10
	adc     r8, r11
.13:mulx    r11, r10, [rsi+8]
	adc     r10, r9
	lea     rsi, [rsi+64]
	mov     [rdi], r8
.14:mov     [rdi+8], r10
	mulx    r9, r8, [rsi-48]
	lea     rdi, [rdi+64]
	adc     r8, r11
.15:mulx    r11, r10, [rsi-40]
	mov     [rdi-48], r8
	adc     r10, r9
.16:mov     [rdi-40], r10
	mulx    r9, r8, [rsi-32]
	adc     r8, r11
.17:mulx    r11, r10, [rsi-24]
	mov     [rdi-32], r8
	adc     r10, r9
.18:mulx    r9, r8, [rsi-16]
	mov     [rdi-24], r10
	adc     r8, r11
.19:mulx    r11, r10, [rsi-8]
	adc     r10, r9
	mov     [rdi-16], r8
	dec     ecx
	mulx    r9, r8, [rsi]
	jnz     .12
.20:mov     [rdi-8], r10
	adc     r8, r11
	mov     [rdi], r8
	adc     r9, rcx
	mov     [rdi+8], r9
	lea     r10, [rel .59]
	movsxd  r11, dword [r10+rax*4]
	lea     r11, [r11+r10]
	jmp     r11
.21:adox    r8, [rdi]
	adox    r9, rcx
	mov     [rdi], r8
	adc     r9, rcx
	mov     [rdi+8], r9
.22:lea     rsi, [rsi+rbx*8-64]
	or      ecx, ebx
	mov     rdx, [rsi+8]
	mulx    r9, r8, [rsi+16]
	lea     rdi, [rdi+rbx*8-56]
	jmp     .51
	align	16
.23:adox    r10, [rdi-8]
	adcx    r8, r11
	mov     [rdi-8], r10
	jrcxz   .21
	mulx    r11, r10, [rsi+8]
	adox    r8, [rdi]
	lea     ecx, [rcx+8]
	mov     [rdi], r8
	adcx    r10, r9
.24:mulx    r9, r8, [rsi+16]
	adcx    r8, r11
	adox    r10, [rdi+8]
	mov     [rdi+8], r10
	mulx    r11, r10, [rsi+24]
	lea     rsi, [rsi+64]
	adcx    r10, r9
	adox    r8, [rdi+16]
	mov     [rdi+16], r8
	mulx    r9, r8, [rsi-32]
	adox    r10, [rdi+24]
	adcx    r8, r11
	mov     [rdi+24], r10
	mulx    r11, r10, [rsi-24]
	adcx    r10, r9
	adox    r8, [rdi+32]
	mov     [rdi+32], r8
	mulx    r9, r8, [rsi-16]
	adox    r10, [rdi+40]
	adcx    r8, r11
	mov     [rdi+40], r10
	adox    r8, [rdi+48]
	mulx    r11, r10, [rsi-8]
	mov     [rdi+48], r8
	lea     rdi, [rdi+64]
	adcx    r10, r9
	mulx    r9, r8, [rsi]
	jmp     .23
.25:adox    r8, [rdi]
	adox    r9, rcx
	mov     [rdi], r8
	adc     r9, rcx
	mov     [rdi+8], r9
.26:lea     rsi, [rsi+rbx*8-64]
	or      ecx, ebx
	mov     rdx, [rsi]
	mulx    r11, r10, [rsi+8]
	lea     rdi, [rdi+rbx*8-56]
	jmp     .24
	align	16
.27:adox    r10, [rdi-8]
	adcx    r8, r11
	mov     [rdi-8], r10
	jrcxz   .25
.28:mulx    r11, r10, [rsi+8]
	adox    r8, [rdi]
	lea     ecx, [rcx+8]
	mov     [rdi], r8
	adcx    r10, r9
	mulx    r9, r8, [rsi+16]
	adcx    r8, r11
	adox    r10, [rdi+8]
	mov     [rdi+8], r10
	mulx    r11, r10, [rsi+24]
	lea     rsi, [rsi+64]
	adcx    r10, r9
	adox    r8, [rdi+16]
	mov     [rdi+16], r8
	mulx    r9, r8, [rsi-32]
	adox    r10, [rdi+24]
	adcx    r8, r11
	mov     [rdi+24], r10
	mulx    r11, r10, [rsi-24]
	adcx    r10, r9
	adox    r8, [rdi+32]
	mov     [rdi+32], r8
	mulx    r9, r8, [rsi-16]
	adox    r10, [rdi+40]
	adcx    r8, r11
	mov     [rdi+40], r10
	adox    r8, [rdi+48]
	mulx    r11, r10, [rsi-8]
	mov     [rdi+48], r8
	lea     rdi, [rdi+64]
	adcx    r10, r9
	mulx    r9, r8, [rsi]
	jmp     .27
.29:adox    r8, [rdi]
	adox    r9, rcx
	mov     [rdi], r8
	adc     r9, rcx
	mov     [rdi+8], r9
.30:lea     rsi, [rsi+rbx*8]
	or      ecx, ebx
	lea     rbx, [rbx+8]
	mov     rdx, [rsi-8]
	mulx    r9, r8, [rsi]
	lea     rdi, [rdi+rbx*8-56]
	jmp     .28
	align	16
.31:adox    r10, [rdi-8]
	adcx    r8, r11
	mov     [rdi-8], r10
	jrcxz   .29
	mulx    r11, r10, [rsi+8]
	adox    r8, [rdi]
	lea     ecx, [rcx+8]
	mov     [rdi], r8
	adcx    r10, r9
	mulx    r9, r8, [rsi+16]
	adcx    r8, r11
	adox    r10, [rdi+8]
	mov     [rdi+8], r10
	mulx    r11, r10, [rsi+24]
	lea     rsi, [rsi+64]
	adcx    r10, r9
	adox    r8, [rdi+16]
	mov     [rdi+16], r8
	mulx    r9, r8, [rsi-32]
	adox    r10, [rdi+24]
	adcx    r8, r11
	mov     [rdi+24], r10
	mulx    r11, r10, [rsi-24]
	adcx    r10, r9
	adox    r8, [rdi+32]
	mov     [rdi+32], r8
	mulx    r9, r8, [rsi-16]
	adox    r10, [rdi+40]
	adcx    r8, r11
	mov     [rdi+40], r10
	adox    r8, [rdi+48]
	mulx    r11, r10, [rsi-8]
	mov     [rdi+48], r8
	lea     rdi, [rdi+64]
	adcx    r10, r9
	mulx    r9, r8, [rsi]
	jmp     .31
.32:adox    r8, [rdi]
	adox    r9, rcx
	mov     [rdi], r8
	adc     r9, rcx
	mov     [rdi+8], r9
.33:lea     rsi, [rsi+rbx*8]
	or      ecx, ebx
	jz      .53
	mov     rdx, [rsi-16]
	mulx    r11, r10, [rsi-8]
	lea     rdi, [rdi+rbx*8+8]
	mulx    r9, r8, [rsi]
	jmp     .31
	align	16
.34:adox    r10, [rdi-8]
	adcx    r8, r11
	mov     [rdi-8], r10
	jrcxz   .32
	mulx    r11, r10, [rsi+8]
	adox    r8, [rdi]
	lea     ecx, [rcx+8]
	mov     [rdi], r8
	adcx    r10, r9
	mulx    r9, r8, [rsi+16]
	adcx    r8, r11
	adox    r10, [rdi+8]
	mov     [rdi+8], r10
	mulx    r11, r10, [rsi+24]
	lea     rsi, [rsi+64]
	adcx    r10, r9
	adox    r8, [rdi+16]
	mov     [rdi+16], r8
	mulx    r9, r8, [rsi-32]
	adox    r10, [rdi+24]
	adcx    r8, r11
	mov     [rdi+24], r10
	mulx    r11, r10, [rsi-24]
	adcx    r10, r9
	adox    r8, [rdi+32]
	mov     [rdi+32], r8
	mulx    r9, r8, [rsi-16]
	adox    r10, [rdi+40]
	adcx    r8, r11
	mov     [rdi+40], r10
.35:adox    r8, [rdi+48]
	mulx    r11, r10, [rsi-8]
	mov     [rdi+48], r8
	lea     rdi, [rdi+64]
	adcx    r10, r9
	mulx    r9, r8, [rsi]
	jmp     .34
.36:adox    r8, [rdi]
	adox    r9, rcx
	mov     [rdi], r8
	adc     r9, rcx
	mov     [rdi+8], r9
.37:lea     rsi, [rsi+rbx*8]
	or      ecx, ebx
	jz      .52
	mov     rdx, [rsi-24]
	mulx    r9, r8, [rsi-16]
	lea     rdi, [rdi+rbx*8-56]
	jmp     .35
	align	16
.38:adox    r10, [rdi-8]
	adcx    r8, r11
	mov     [rdi-8], r10
	jrcxz   .36
	mulx    r11, r10, [rsi+8]
	adox    r8, [rdi]
	lea     ecx, [rcx+8]
	mov     [rdi], r8
	adcx    r10, r9
	mulx    r9, r8, [rsi+16]
	adcx    r8, r11
	adox    r10, [rdi+8]
	mov     [rdi+8], r10
	mulx    r11, r10, [rsi+24]
	lea     rsi, [rsi+64]
	adcx    r10, r9
	adox    r8, [rdi+16]
	mov     [rdi+16], r8
	mulx    r9, r8, [rsi-32]
	adox    r10, [rdi+24]
	adcx    r8, r11
	mov     [rdi+24], r10
	mulx    r11, r10, [rsi-24]
	adcx    r10, r9
	adox    r8, [rdi+32]
	mov     [rdi+32], r8
.39:mulx    r9, r8, [rsi-16]
	adox    r10, [rdi+40]
	adcx    r8, r11
	mov     [rdi+40], r10
	adox    r8, [rdi+48]
	mulx    r11, r10, [rsi-8]
	mov     [rdi+48], r8
	lea     rdi, [rdi+64]
	adcx    r10, r9
	mulx    r9, r8, [rsi]
	jmp     .38
.40:adox    r8, [rdi]
	adox    r9, rcx
	mov     [rdi], r8
	adc     r9, rcx
	mov     [rdi+8], r9
.41:lea     rsi, [rsi+rbx*8]
	or      ecx, ebx
	mov     rdx, [rsi-32]
	mulx    r11, r10, [rsi-24]
	lea     rdi, [rdi+rbx*8-56]
	jmp     .39
	align	16
.42:adox    r10, [rdi-8]
	adcx    r8, r11
	mov     [rdi-8], r10
	jrcxz   .40
	mulx    r11, r10, [rsi+8]
	adox    r8, [rdi]
	lea     ecx, [rcx+8]
	mov     [rdi], r8
	adcx    r10, r9
	mulx    r9, r8, [rsi+16]
	adcx    r8, r11
	adox    r10, [rdi+8]
	mov     [rdi+8], r10
	mulx    r11, r10, [rsi+24]
	lea     rsi, [rsi+64]
	adcx    r10, r9
	adox    r8, [rdi+16]
	mov     [rdi+16], r8
	mulx    r9, r8, [rsi-32]
	adox    r10, [rdi+24]
	adcx    r8, r11
	mov     [rdi+24], r10
.43:mulx    r11, r10, [rsi-24]
	adcx    r10, r9
	adox    r8, [rdi+32]
	mov     [rdi+32], r8
	mulx    r9, r8, [rsi-16]
	adox    r10, [rdi+40]
	adcx    r8, r11
	mov     [rdi+40], r10
	adox    r8, [rdi+48]
	mulx    r11, r10, [rsi-8]
	mov     [rdi+48], r8
	lea     rdi, [rdi+64]
	adcx    r10, r9
	mulx    r9, r8, [rsi]
	jmp     .42
.44:adox    r8, [rdi]
	adox    r9, rcx
	mov     [rdi], r8
	adc     r9, rcx
	mov     [rdi+8], r9
.45:lea     rsi, [rsi+rbx*8]
	or      ecx, ebx
	mov     rdx, [rsi-40]
	mulx    r9, r8, [rsi-32]
	lea     rdi, [rdi+rbx*8-56]
	jmp     .43
	align	16
.46:adox    r10, [rdi-8]
	adcx    r8, r11
	mov     [rdi-8], r10
	jrcxz   .44
	mulx    r11, r10, [rsi+8]
	adox    r8, [rdi]
	lea     ecx, [rcx+8]
	mov     [rdi], r8
	adcx    r10, r9
	mulx    r9, r8, [rsi+16]
	adcx    r8, r11
	adox    r10, [rdi+8]
	mov     [rdi+8], r10
	mulx    r11, r10, [rsi+24]
	lea     rsi, [rsi+64]
	adcx    r10, r9
	adox    r8, [rdi+16]
	mov     [rdi+16], r8
.47:mulx    r9, r8, [rsi-32]
	adox    r10, [rdi+24]
	adcx    r8, r11
	mov     [rdi+24], r10
	mulx    r11, r10, [rsi-24]
	adcx    r10, r9
	adox    r8, [rdi+32]
	mov     [rdi+32], r8
	mulx    r9, r8, [rsi-16]
	adox    r10, [rdi+40]
	adcx    r8, r11
	mov     [rdi+40], r10
	adox    r8, [rdi+48]
	mulx    r11, r10, [rsi-8]
	mov     [rdi+48], r8
	lea     rdi, [rdi+64]
	adcx    r10, r9
	mulx    r9, r8, [rsi]
	jmp     .46
.48:adox    r8, [rdi]
	adox    r9, rcx
	mov     [rdi], r8
	adc     r9, rcx
	mov     [rdi+8], r9
.49:lea     rsi, [rsi+rbx*8]
	or      ecx, ebx
	mov     rdx, [rsi-48]
	mulx    r11, r10, [rsi-40]
	lea     rdi, [rdi+rbx*8-56]
	jmp     .47
	align	16
.50:adox    r10, [rdi-8]
	adcx    r8, r11
	mov     [rdi-8], r10
	jrcxz   .48
	mulx    r11, r10, [rsi+8]
	adox    r8, [rdi]
	lea     ecx, [rcx+8]
	mov     [rdi], r8
	adcx    r10, r9
	mulx    r9, r8, [rsi+16]
	adcx    r8, r11
	adox    r10, [rdi+8]
	mov     [rdi+8], r10
.51:mulx    r11, r10, [rsi+24]
	lea     rsi, [rsi+64]
	adcx    r10, r9
	adox    r8, [rdi+16]
	mov     [rdi+16], r8
	mulx    r9, r8, [rsi-32]
	adox    r10, [rdi+24]
	adcx    r8, r11
	mov     [rdi+24], r10
	mulx    r11, r10, [rsi-24]
	adcx    r10, r9
	adox    r8, [rdi+32]
	mov     [rdi+32], r8
	mulx    r9, r8, [rsi-16]
	adox    r10, [rdi+40]
	adcx    r8, r11
	mov     [rdi+40], r10
	adox    r8, [rdi+48]
	mulx    r11, r10, [rsi-8]
	mov     [rdi+48], r8
	lea     rdi, [rdi+64]
	adcx    r10, r9
	mulx    r9, r8, [rsi]
	jmp     .50
.52:
	mov     rdx, [rsi-24]
	mulx    r9, r8, [rsi-16]
	adox    r8, [rdi-8]
	mulx    r11, r10, [rsi-8]
	mov     [rdi-8], r8
	lea     rdi, [rdi+8]
	adcx    r10, r9
	mulx    r9, r8, [rsi]
	adox    r10, [rdi-8]
	adcx    r8, r11
	mov     [rdi-8], r10
	adox    r8, [rdi]
	adox    r9, rcx
	adcx    r9, rcx
.53:
	mov     rdx, [rsi-16]
	mulx    r11, r10, [rsi-8]
	mulx    rbx, rax, [rsi]
	adox    r10, r8
	adcx    rax, r11
	mov     [rdi], r10
	adox    rax, r9
	adox    rbx, rcx
	mov     [rdi+8], rax
	adc     rbx, rcx
	mov     rdx, [rsi-8]
	mulx    rdx, rax, [rsi]
	add     rax, rbx
	mov     [rdi+16], rax
	adc     rdx, rcx
	mov     [rdi+24], rdx
.54:
    mov     rdi, [rsp+stack_use+8]
    mov     rsi, [rsp+stack_use+16]
    mov     rcx, [rsp+stack_use+24]
	dec     ecx
	mov     rdx, [rsi]
	xor     ebx, ebx
	mulx    r10, rax, rdx
	mov     [rdi], rax
	mov     r8, [rdi+8]
	mov     r9, [rdi+16]
	jmp     .56
	align	16
.55:mov     r8, [rdi+24]
	mov     r9, [rdi+32]
	lea     rdi, [rdi+16]
	lea     r10, [rdx+rbx]
.56:adc     r8, r8
	adc     r9, r9
	setc    bl
	mov     rdx, [rsi+8]
	lea     rsi, [rsi+8]
	mulx    rdx, rax, rdx
	add     r8, r10
	adc     r9, rax
	mov     [rdi+8], r8
	mov     [rdi+16], r9
	dec     ecx
	jnz     .55
.57:adc     rdx, rbx
	mov     [rdi+24], rdx
    END_PROC reg_save_list

	align	8
.58:
	dd     .9 - .58
	dd     .4 - .58
	dd     .10 - .58
	dd     .11 - .58
	dd     .5 - .58
	dd     .6 - .58
	dd     .7 - .58
	dd     .8 - .58
.59:
	dd     .49 - .59
	dd     .22 - .59
	dd     .26 - .59
	dd     .30 - .59
	dd     .33 - .59
	dd     .37 - .59
	dd     .41 - .59
	dd     .45 - .59

    end
