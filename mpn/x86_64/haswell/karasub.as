;  mpn_karasub

;  Copyright 2011,2012 The Code Cavern

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

;  void mpn_karasub(mp_ptr, mp_ptr, mp_size_t)
;  rax                 rdi     rsi        rdx
;  rax                 rcx     rdx         r8
;
;  Karasuba Multiplication - split x and y into two equal length halves so
;  that x = xh.B + xl and y = yh.B + yl. Then their product is:
;
;  x.y = xh.yh.B^2 + (xh.yl + xl.yh).B + xl.yl
;      = xh.yh.B^2 + (xh.yh + xl.yl - {xh - xl}.{yh - yl}).B + xl.yl
;
; If the length of the elements is m (about n / 2), the output length is 4 * m 
; as illustrated below.  The middle two blocks involve three additions and one 
; subtraction: 
; 
;       -------------------- rp
;       |                  |-->
;       |   A:xl.yl[lo]    |   |
;       |                  |   |      (xh - xl).(yh - yl)
;       --------------------   |      -------------------- tp
;  <--  |                  |<--<  <-- |                  |
; |     |   B:xl.yl[hi]    |   |      |     E:[lo]       |
; |     |                  |   |      |                  |
; |     --------------------   |      --------------------
; >-->  |                  |-->   <-- |                  |
; |\___ |   C:xh.yh[lo]    | ____/    |     F:[hi]       |
; |     |                  |          |                  |
; |     --------------------          --------------------
;  <--  |                  |   
;       |   D:xh.yh[hi]    |
;       |                  |
;       --------------------
;
; Let n2 = floor(n/2), n3 = n - n2, i.e., either n3 = n2 or n3 = n2 + 1.
; The sizes of the blocks are: {rp, 2*n2} = xl.yl, {rp + 2*n2, 2*n3} = xh.yh,
; {tp, 2*n3} = (xh - xl).(yh - yl).
; We arrange sizes so that A, B, C, and E are of length n2, and D and F
; are of length 2*n3-n2. I.e.,
; A = {rp, n2}, B = {rp + n2, n2}, C = {rp + 2*n2, n2}
; D = {rp + 3*n2, 2*n3-n2}, E = {tp, n2}, F = {tp + n2, 2*n3-n2}.
;
; To avoid overwriting B before it is used, we need to do two operations
; in parallel:
;
; (1)   B = B + C + A - E = (B + C) + A - E
; (2)   C = C + B + D - F = (B + C) + D - F
;
; The final carry from (1) has to be propagated into C and and, D the final
; carry from (2) has to be propagated into D. When the number of input limbs
; is odd, some extra operations have to be undertaken. 

%include 'yasm_mac.inc'

BITS 64

%define TP rsi
%define RP rdi

%define A_P rdi
%define B_P rbx
%define C_P rcx
%define D_P rdx
%define E_P rsi
%define F_P rbp

GLOBAL_FUNC mpn_karasub
; requires n>=8
push rbp
push rbx
push r12
push r13
push r14
push r15
push rdx
; n is rdx and put it on the stack
and rdx, -2			; rdx = 2*n2
shl rdx, 2			; rdx = 8*n2
lea B_P, [RP + rdx]
lea C_P, [RP + rdx*2]
lea F_P, [E_P + rdx]
lea rdx, [rdx*2 + rdx]
lea D_P, [RP + rdx]
mov rax, B_P
sub rax, 3*8
mov [rsp-8], rax			; for testing end of main loop
; eax contains the carrys
xor eax, eax
mov r11, rax
align 16
.Lp:	bt r11, 3
	mov r8, [B_P]		; r8 = B[i]
	adc r8, [C_P]		; r8 = B[i] + C[i]
	mov r9, [B_P + 8]	; r9 = B[i+1]
	adc r9, [C_P + 8]	; r9 = B[i+1] + C[i+1]
	mov r10, [B_P + 16]	; r10 = B[i+2]
	adc r10, [C_P + 16]	; r10 = B[i+2] + C[i+2]
	mov r11, [B_P + 24]	; r11 = B[i+3]
	adc r11, [C_P + 24]	; r11 = B[i+3] + C[i+3]
	mov r12, rax
	adc eax, eax

	bt r12, 3
	mov r12, r8		; r12 = B[i] + C[i]
	mov r13, r9		; r13 = B[i+1] + C[i+1]
	mov r14, r10		; r14 = B[i+2] + C[i+2]
	mov r15, r11		; r15 = B[i+3] + C[i+3]
	adc r8, [A_P]		; r8 = B[i] + C[i] + A[i]
	adc r9, [A_P + 8]	; r9 = B[i+1] + C[i+1] + A[i+1]
	adc r10, [A_P + 16]	; r10 = B[i+2] + C[i+2] + A[i+2]
	adc r11, [A_P + 24]	; r11 = B[i+3] + C[i+3] + A[i+3]
	adc eax, eax

	bt eax, 4	; FIXME: can we break the dependency chain here?
	sbb r8, [E_P]		; r8 = B[i] + C[i] + A[i] - E[i]
	sbb r9, [E_P + 8]	; r9 = B[i+1] + C[i+1] + A[i+1] - E[i+1]
	sbb r10, [E_P + 16]	; r10 = B[i+2] + C[i+2] + A[i+2] - E[i+2]
	sbb r11, [E_P + 24]	; r11 = B[i+3] + C[i+3] + A[i+3] - E[i+3]
	mov [B_P], r8		; B[i] = B[i] + C[i] + A[i] - E[i]
	mov [B_P + 8], r9	; B[i+1] = B[i+1] + C[i+1] + A[i+1] - E[i+1]
	mov [B_P + 16], r10	; B[i+2] = B[i+2] + C[i+2] + A[i+2] - E[i+2]
	mov [B_P + 24], r11	; B[i+3] = B[i+3] + C[i+3] + A[i+3] - E[i+3]
	mov r11, rax
	adc eax, eax

	bt r11, 3
	adc r12, [D_P]		; r12 = B[i] + C[i] + D[i]
	adc r13, [D_P + 8]	; r13 = B[i+1] + C[i+1] + D[i+1]
	adc r14, [D_P + 16]	; r14 = B[i+2] + C[i+2] + D[i+2]
	adc r15, [D_P + 24]	; r15 = B[i+3] + C[i+3] + D[i+3]
	mov r11, rax
	adc eax, eax

	bt r11, 3
	sbb r12, [F_P]		; r12 = B[i] + C[i] + D[i] - F[i]
	sbb r13, [F_P + 8]	; r13 = B[i+1] + C[i+1] + D[i+1] - F[i+1]
	sbb r14, [F_P + 16]	; r14 = B[i+2] + C[i+2] + D[i+2] - F[i+2]
	sbb r15, [F_P + 24]	; r15 = B[i+3] + C[i+3] + D[i+3] - F[i+3]
	mov r11, rax
	adc eax, eax

	mov [C_P], r12		; C[i] = B[i] + C[i] + D[i] - F[i]
	mov [C_P + 8], r13	; C[i+1] = B[i+1] + C[i+1] + D[i+1] - F[i+1]
	mov [C_P + 16], r14	; C[i+2] = B[i+2] + C[i+2] + D[i+2] - F[i+2]
	mov [C_P + 24], r15	; C[i+3] = B[i+3] + C[i+3] + D[i+3] - F[i+3]
	lea A_P, [A_P + 4*8]
	lea B_P, [B_P + 4*8]
	lea C_P, [C_P + 4*8]
	lea D_P, [D_P + 4*8]
	lea E_P, [E_P + 4*8]
	lea F_P, [F_P + 4*8]
	mov r9, A_P
	sub r9, [rsp-8]		; r9 = A_P - (orig_B_P - 3*8) = A_P - orig_B_P + 24
	jc .Lp			; If A_P < orig_B_P - 3*8, then do another 4 words

; Bits of eax contain carries of:
; 0		1		2	3	4
; (B+C+D)-F	(B+C)+D	(B+C+A)-E	(B+C)+A	B+C

	jz .Lcase3		; If A_P = orig_B_P - 3*8, then do remaining 3 words
				; Difference is 8, 16, or 24, corresponding to 2, 1, or 0 words left to do, resp.
	cmp r9, 16
	jb	.Lcase2		; r9 = 8 : 2 words
	je	.Lcase1		; r9 = 16 : 1 word
	jmp	.Lcase0		; r9 = 24 : 0 words
.Lcase3:
	bt r11, 3
	mov r8, [B_P]
	adc r8, [C_P]
	mov r9, [B_P + 8]
	adc r9, [C_P + 8]
	mov r10, [B_P + 16]
	adc r10, [C_P + 16]
	mov r11, rax
	adc eax, eax
	bt r11, 3
	mov r12, r8
	mov r13, r9
	mov r14, r10
	adc r8, [A_P]
	adc r9, [A_P + 8]
	adc r10, [A_P + 16]
	mov r11, rax
	adc eax, eax
	bt r11, 3
	sbb r8, [E_P]
	sbb r9, [E_P + 8]
	sbb r10, [E_P + 16]
	mov [B_P], r8
	mov [B_P + 8], r9
	mov [B_P + 16], r10
	mov r11, rax
	adc eax, eax
	bt r11, 3
	adc r12, [D_P]
	adc r13, [D_P + 8]
	adc r14, [D_P + 16]
	mov r11, rax
	adc eax, eax
	bt r11, 3
	sbb r12, [F_P]
	sbb r13, [F_P + 8]
	sbb r14, [F_P + 16]
	adc eax, eax
	mov [C_P], r12
	mov [C_P + 8], r13
	mov [C_P + 16], r14
	lea A_P, [A_P + 3*8]
	lea B_P, [B_P + 3*8]
	lea C_P, [C_P + 3*8]
	lea D_P, [D_P + 3*8]
	lea E_P, [E_P + 3*8]
	lea F_P, [F_P + 3*8]
	jmp .Lfin
.Lcase2:
	bt r11, 3
	mov r8, [B_P]
	adc r8, [C_P]
	mov r9, [B_P + 8]
	adc r9, [C_P + 8]
	mov r11, rax
	adc eax, eax
	bt r11, 3
	mov r12, r8
	mov r13, r9
	adc r8, [A_P]
	adc r9, [A_P + 8]
	mov r11, rax
	adc eax, eax
	bt r11, 3
	sbb r8, [E_P]
	sbb r9, [E_P + 8]
	mov r11, rax
	adc eax, eax
	bt r11, 3
	adc r12, [D_P]
	adc r13, [D_P + 8]
	mov r11, rax
	adc eax, eax
	bt r11, 3
	mov [B_P], r8
	mov [B_P + 8], r9
	sbb r12, [F_P]
	sbb r13, [F_P + 8]
	adc eax, eax
	mov [C_P], r12
	mov [C_P + 8], r13
	lea A_P, [A_P + 2*8]
	lea B_P, [B_P + 2*8]
	lea C_P, [C_P + 2*8]
	lea D_P, [D_P + 2*8]
	lea E_P, [E_P + 2*8]
	lea F_P, [F_P + 2*8]
	jmp .Lfin
.Lcase1:
	bt r11, 3
	mov r8, [B_P]
	adc r8, [C_P]
	mov r11, rax
	adc eax, eax
	bt r11, 3
	mov r12, r8
	adc r8, [A_P]
	mov r11, rax
	adc eax, eax
	bt r11, 3
	sbb r8, [E_P]
	mov r11, rax
	adc eax, eax
	bt r11, 3
	adc r12, [D_P]
	mov r11, rax
	adc eax, eax
	bt r11, 3
	mov [B_P], r8
	sbb r12, [F_P]
	adc eax, eax
	mov [C_P], r12
	lea A_P, [A_P + 1*8]
	lea B_P, [B_P + 1*8]
	lea C_P, [C_P + 1*8]
	lea D_P, [D_P + 1*8]
	lea E_P, [E_P + 1*8]
	lea F_P, [F_P + 1*8]
.Lfin:
	; Now A_P, ..., F_P point at A[n2]=B[0], B[n2]=C[0], C[n2]=D[0],
        ; D[n2], E[n2]=F[0], F[n2], resp.
.Lcase0:
	; store top two words of D as carrys could change them
	pop r15
	bt r15, 0
	jnc .Lskipload
	mov r12, [D_P]		; load D[n2]
        mov r13, [D_P + 8]	; load D[n2 + 1]
	; the two carrys from 2nd to 3rd
.Lskipload:
	xor r8, r8
	mov r11, r8		; r11 is constant 0 now
	bt eax, 4		; carry of B+C
	adc r8, r8
	mov r9, r8
	bt eax, 3		; carry of (B+C)+A
	lea r10, [B_P + 8]
	adc [B_P], r8		; B_P points at B[n2] = C[0]
.L2:	adc qword [r10], r11
	lea r10, [r10 + 8]
	jc .L2
	; the two carrys from 3rd to 4th
	lea r10, [C_P + 8]
	bt eax, 1		; carry of (B+C)+D
	adc [C_P], r9
.L3:	adc qword [r10], r11
	lea r10, [r10 + 8]
	jc .L3
	; now the borrow from 2nd to 3rd
	mov r10, B_P
	bt eax, 2		; borrow of (B+C+A)-E
.L1:	sbb qword [r10], r11
	lea r10, [r10 + 8]
	jc .L1
	; borrow from 3rd to 4th
	bt eax, 0		; borrow of (B+C+D)-F
	mov r10, C_P
.L4:	sbb qword [r10], r11
	lea r10, [r10 + 8]
	jc .L4

	; if odd then do next two
	bt r15, 0
	jnc .Lnotodd

	sub r12, [F_P]		; r12 contains D[n2]
	sbb r13, [F_P + 8]	; r13 contains D[n2+1]
	sbb rax, rax
	add [C_P], r12
	adc [C_P + 8], r13
	adc rax, 0		; rax is -1, 0, or 1
.L7:	add [C_P + 16], rax
	adc rax, 0
	lea C_P, [C_P + 8]
	sar rax, 1
	jnz .L7
.Lnotodd:
pop r15
pop r14
pop r13
pop r12
pop rbx
pop rbp
ret
