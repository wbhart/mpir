; PROLOGUE(mpn_rsh_divrem_hensel_qr_1_1)

;  Copyright 2009 Jason Moxham
;
;  Windows Conversion Copyright 2008 Brian Gladman
;
;  This file is part of the MPIR Library.
;
;  The MPIR Library is free software; you can redistribute it and/or modify
;  it under the terms of the GNU Lesser General Public License as published
;  by the Free Software Foundation; either version 2.1 of the License, or (at
;  your option) any later version.
;
;  The MPIR Library is distributed in the hope that it will be useful, but
;  WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
;  or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public
;  License for more details.
;
;  You should have received a copy of the GNU Lesser General Public License
;  along with the MPIR Library; see the file COPYING.LIB.  If not, write
;  to the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
;  Boston, MA 02110-1301, USA.
;
;  mp_limb_t  mpn_rsh_divrem_hensel_qr_1_1(mp_ptr, mp_ptr, mp_size_t, mp_limb_t,  mp_int, mp_limb_t)
;  rax                                        rdi     rsi        rdx        rcx       r8         r9
;  rax                                        rcx     rdx         r8         r9 [rsp+40]   [rsp+48]

%include "yasm_mac.inc"

    CPU  Athlon64
    BITS 64

%define reg_save_list rsi, rdi

	FRAME_PROC mpn_rsh_divrem_hensel_qr_1_1, 0, reg_save_list
    mov     rax, r8
	lea     rdi, [rcx+rax*8]
	lea     rsi, [rdx+rax*8]
    mov     rcx, r9
	mov     rdx, r9
	mov     r9, 1
	sub     r9, rax
    movsxd  r8, dword [rsp+stack_use+40]
    mov     r10, qword [rsp+stack_use+48]
    
	mov     rax, rdx
	imul    edx, ecx
	mov     r11, 2
	sub     r11, rdx
	imul    r11d, eax

	mov     rax, r11
	imul    r11d, ecx
	mov     rdx, 2
	sub     rdx, r11
	imul    edx, eax

	mov     rax, rdx
	imul    edx, ecx
	mov     r11, 2
	sub     r11, rdx
	imul    r11d, eax

	mov     rax, r11
	imul    r11d, ecx
	mov     rdx, 2
	sub     rdx, r11
	imul    edx, eax

	mov     rax, rdx
	imul    rdx, rcx
	mov     r11, 2
	sub     r11, rdx
	imul    r11, rax

	mov     rax, 64
	sub     rax, r8
	movq    mm0, r8
	movq    mm1, rax
	mov     rax, [rsi+r9*8-8]
	sub     rax, r10
	sbb     r8, r8
	imul    rax, r11
	movq    mm4, rax
	movq    mm5, mm4
	psrlq   mm4, mm0
	psllq   mm5, mm1
	psrlq   mm5, mm1
	mul     rcx
	cmp     r9, 0
	je      .3
	add     r8, r8
	
	xalign  16
.1:	movq    mm2, mm4
	mov     rax, [rsi+r9*8]
	sbb     rax, rdx
	sbb     r8, r8
	imul    rax, r11
	movq    mm3, rax
	movq    mm4, mm3
	psllq   mm3, mm1
	psrlq   mm4, mm0
	por     mm2, mm3
	movq    [rdi+r9*8-8], mm2
	mul     rcx
	add     r8, r8
	inc     r9
	jnz     .1
.2:	movq    [rdi+r9*8-8], mm4
	mov     rax, 0
	adc     rax, rdx
	emms
	EXIT_PROC reg_save_list

.3:	movq    [rdi+r9*8-8], mm4
	add     r8, r8
	mov     rax, 0
	adc     rax, rdx
.4:	emms
	END_PROC reg_save_list

    end
