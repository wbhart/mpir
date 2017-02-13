;  AMD64 mpn_lshift optimised for CPUs with fast SSE including fast movdqu.

;  Contributed to the GNU project by Torbjorn Granlund.

;  Copyright 2010-2012 Free Software Foundation, Inc.

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

;	     cycles/limb     cycles/limb     cycles/limb    good
;              aligned	      unaligned	      best seen	   for cpu?
; AMD K8,K9	 3		 3		 2.35	  no, use shl/shr
; AMD K10	 1.5-1.8	 1.5-1.8	 1.33	  yes
; AMD bd1	 1.7-1.9	 1.7-1.9	 1.33	  yes
; AMD bobcat	 3.17		 3.17			  yes, bad for n < 20
; Intel P4	 4.67		 4.67		 2.7	  no, slow movdqu
; Intel core2	 2.15		 2.15		 1.25	  no, use shld/shrd
; Intel NHM	 1.66		 1.66		 1.25	  no, use shld/shrd
; Intel SBR	 1.3		 1.3		 1.25	  yes, bad for n = 4-6
; Intel atom	11.7		11.7		 4.5	  no
; VIA nano	 5.7		 5.95		 2.0	  no, slow movdqu

; We try to do as many aligned 16-byte operations as possible.  The top-most
; and bottom-most writes might need 8-byte operations.
;
; This variant rely on fast load movdqu, and uses it even for aligned operands,
; in order to avoid the need for two separate loops.
;
; TODO
;  * Could 2-limb wind-down code be simplified?
;  * Improve basecase code, using shld/shrd for SBR, discrete integer shifts
;    for other affected CPUs.

;  mp_limb_t mpn_lshift(mp_ptr, mp_ptr, mp_size_t, mp_uint)
;  rax                     rdi     rsi        rdx      rcx
;  rax                     rcx     rdx         r8      r9d

%include 'yasm_mac.inc'

	TEXT
	align	64
	LEAF_PROC mpn_lshift
    mov     r10, rcx
    mov     r11, rdx
    mov     rdx, r8
    mov     ecx, r9d

	movd    xmm4, ecx
	mov     eax, 64
	sub     eax, ecx
	movd    xmm5, eax
	neg     ecx
	mov     rax, [r11+rdx*8-8]
	shr     rax, cl
	cmp     rdx, 3
	jle     .10
	lea     ecx, [r10+rdx*8]
	test    cl, 8
	jz      .0
	movq    xmm0, [r11+rdx*8-8]
	movq    xmm1, [r11+rdx*8-16]
	psllq   xmm0, xmm4
	psrlq   xmm1, xmm5
	por     xmm0, xmm1
	movq    [r10+rdx*8-8], xmm0
	dec     rdx
.0: lea     r8d, [rdx+1]
	and     r8d, 6
	jz      .2
	cmp     r8d, 4
	jz      .3
	jc      .4
.1:	add     rdx, -4
	jmp     .7
.2:	add     rdx, -6
	jmp     .6
.3:	add     rdx, -2
	jmp     .8
.4:	add     rdx, -8
	jle     .9
	align	16
.5:	movdqu  xmm1, [r11+rdx*8+40]
	movdqu  xmm0, [r11+rdx*8+48]
	psllq   xmm0, xmm4
	psrlq   xmm1, xmm5
	por     xmm0, xmm1
	movdqa  [r10+rdx*8+48], xmm0
.6: movdqu  xmm1, [r11+rdx*8+24]
	movdqu  xmm0, [r11+rdx*8+32]
	psllq   xmm0, xmm4
	psrlq   xmm1, xmm5
	por     xmm0, xmm1
	movdqa  [r10+rdx*8+32], xmm0
.7: movdqu  xmm1, [r11+rdx*8+8]
	movdqu  xmm0, [r11+rdx*8+16]
	psllq   xmm0, xmm4
	psrlq   xmm1, xmm5
	por     xmm0, xmm1
	movdqa  [r10+rdx*8+16], xmm0
.8: movdqu  xmm1, [r11+rdx*8-8]
	movdqu  xmm0, [r11+rdx*8]
	psllq   xmm0, xmm4
	psrlq   xmm1, xmm5
	por     xmm0, xmm1
	movdqa  [r10+rdx*8], xmm0
	sub     rdx, 8
	jg      .5
.9:	test    dl, 1
	jnz     .11
	movdqu  xmm1, [r11]
	pxor    xmm0, xmm0
	punpcklqdq xmm0, xmm1
	psllq   xmm1, xmm4
	psrlq   xmm0, xmm5
	por     xmm0, xmm1
	movdqa  [r10], xmm0
	ret
	align	16
.10:dec     edx
	jz      .11
	movq    xmm1, [r11+rdx*8]
	movq    xmm0, [r11+rdx*8-8]
	psllq   xmm1, xmm4
	psrlq   xmm0, xmm5
	por     xmm0, xmm1
	movq    [r10+rdx*8], xmm0
	sub     edx, 2
	jl      .11
	movq    xmm1, [r11+8]
	movq    xmm0, [r11]
	psllq   xmm1, xmm4
	psrlq   xmm0, xmm5
	por     xmm0, xmm1
	movq    [r10+8], xmm0
.11:movq    xmm0, [r11]
	psllq   xmm0, xmm4
	movq    [r10], xmm0
	ret
