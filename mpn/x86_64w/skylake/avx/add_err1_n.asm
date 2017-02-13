;  AMD64 mpn_add_err1_n
;  Copyright 2017 Alexander Kruppa
;  This file is part of the MPIR Library.
;  The MPIR Library is free software; you can redistribute it and/or modify
;  it under the terms of the GNU Lesser General Public License as published
;  by the Free Software Foundation; either version 2.1 of the License, or (at
;  your option) any later version.
;  The MPIR Library is distributed in the hope that it will be useful, but
;  WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
;  or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public
;  License for more details.
;  You should have received a copy of the GNU Lesser General Public License
;  along with the MPIR Library; see the file COPYING.LIB.  If not, write
;  to the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
;  Boston, MA 02110-1301, USA.

;	(rdi,rcx) = (rsi,rcx)+(rdx,rcx)+CyIn
;	rax = carry
;	(rcx,2) = rev(r8,rcx) \dot (carry,rcx) where carry is the sequence
;       of carries from the addition of (rsi,rcx)+(rdx,rcx)

;  mp_limb_t mpn_add_err1_n(mp_ptr, mp_ptr, mp_ptr, mp_ptr, mp_ptr, mp_size_t, mp_limb_t)
;  mp_limb_t mpn_sub_err1_n(mp_ptr, mp_ptr, mp_ptr, mp_ptr, mp_ptr, mp_size_t, mp_limb_t)
;  rax                         rdi     rsi     rdx     rcx       r8        r9     8(rsp)
;  rax                         rcx     rdx      r8      r9 [rsp+40]  [rsp+48]   [rsp+56]

%include 'yasm_mac.inc'

%define SumP    rdi
%define Inp1P   rsi
%define Inp2P   rdx
%define EP      r11
%define SizeRest rcx
%define YP      r8
%define Size    r9
%define CyIn    [rsp+8]
%define LIMB0   rax
%define E0      r12
%define E1      r13
%define Zero    r14
%define Dummy   rbx

%define reg_save_list rsi, rdi, rbx, r12, r13, r14

	align   32
	BITS    64

%macro  DO_LIMB 1
	mov     LIMB0, [Inp1P + %1*8]
	adc     LIMB0, [Inp2P + %1*8]
	mov     [SumP + %1*8], LIMB0
	mov	    LIMB0, [YP - %1*8]
	cmovnc	LIMB0, Zero
	inc	    Dummy			; OF = 0
	adox	E0, LIMB0
	adox	E1, Zero
%endmacro

FRAME_PROC mpn_add_err1_n, 0, reg_save_list
    mov     rdi, rcx
    mov     rsi, rdx
    mov     rdx, r8
    mov     r11, r9
    mov     r8, [rsp + stack_use + 40]
    mov     r9, [rsp + stack_use + 48]
    mov     LIMB0, [rsp + stack_use + 56]

	mov     SizeRest, Size
	lea	YP, [YP + Size*8 - 8]
	and	SizeRest, 7
	xor	Zero, Zero
	mov	E0, Zero
	mov	E1, Zero
	shr     Size, 3
	bt	LIMB0, 0
	jz      .testrest

	align   16
.loop:
	DO_LIMB 0
	DO_LIMB 1
	DO_LIMB 2
	DO_LIMB 3
	DO_LIMB 4
	DO_LIMB 5
	DO_LIMB 6
	DO_LIMB 7

	lea     Inp1P, [Inp1P+64]
	lea     Inp2P, [Inp2P+64]
	lea     SumP, [SumP+64]
	lea     YP, [YP-64]

	dec     Size
	jne     .loop

.testrest:
	inc	SizeRest
	dec	SizeRest
	jz	.exit

.rest:
	DO_LIMB 0
	dec	SizeRest
	jz	.exit
	DO_LIMB 1
	dec	SizeRest
	jz	.exit
	DO_LIMB 2
	dec	SizeRest
	jz	.exit
	DO_LIMB 3
	dec	SizeRest
	jz	.exit
	lea	Inp1P, [Inp1P+32]
	lea	Inp2P, [Inp2P+32]
	lea	SumP, [SumP+32]
	lea	YP, [YP-32]
	jmp	.rest

.exit:
	mov rax, Zero
	setc al
	mov	[EP], E0
	mov	[EP+8], E1
    END_PROC reg_save_list
