dnl  AMD64 mpn_add_err2_n

dnl  Copyright (C) 2009, David Harvey

dnl  All rights reserved.

dnl  Redistribution and use in source and binary forms, with or without
dnl  modification, are permitted provided that the following conditions are
dnl  met:

dnl  1. Redistributions of source code must retain the above copyright notice,
dnl  this list of conditions and the following disclaimer.

dnl  2. Redistributions in binary form must reproduce the above copyright
dnl  notice, this list of conditions and the following disclaimer in the
dnl  documentation and/or other materials provided with the distribution.

dnl  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
dnl  ``AS IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
dnl  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A
dnl  PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
dnl  HOLDERS OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
dnl  SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED
dnl  TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
dnl  PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
dnl  LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
dnl  NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
dnl  SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.


include(`../config.m4')

C	     cycles/limb
C K8,K9:	 4.5
C K10:		 ?
C P4:		 ?
C P6-15 (Core2): ?
C P6-28 (Atom):	 ?

C INPUT PARAMETERS
define(`rp',	`%rdi')
define(`up',	`%rsi')
define(`vp',	`%rdx')
define(`ep',	`%rcx')
define(`yp1',	`%r8')
define(`yp2',   `%r9')
define(`n_param',     `8(%rsp)')
define(`cy_param',    `16(%rsp)')

define(`cy1',   `%r14')
define(`cy2',   `%rax')
	
define(`n',     `%r10')

define(`w',     `%rbx')
define(`e1l',	`%rbp')
define(`e1h',	`%r11')
define(`e2l',	`%r12')
define(`e2h',	`%r13')

ASM_START()
	TEXT
	ALIGN(16)
PROLOGUE(mpn_add_err2_n)
	mov	cy_param, cy2
	mov	n_param, n

	push	%rbx
	push	%rbp
	push	%r12
	push	%r13
	push	%r14

	xor e1l, e1l
	xor e1h, e1h
	xor e2l, e2l
	xor e2h, e2h

	sub	yp1, yp2

	lea	(rp,n,8), rp
	lea	(up,n,8), up
	lea	(vp,n,8), vp

	test	$1, n
	jnz	L(odd)

	lea	-8(yp1,n,8), yp1
	neg	n
	jmp	L(top)

	ALIGN(16)
L(odd):
	lea	-16(yp1,n,8), yp1
	neg	n
	shr	$1, cy2
	mov	(up,n,8), w
	adc	(vp,n,8), w
	cmovc	8(yp1), e1l
	cmovc	8(yp1,yp2), e2l
	mov	w, (rp,n,8)
	sbb	cy2, cy2
	inc	n
	jz	L(end)
	
	ALIGN(16)
L(top):
        mov     (up,n,8), w
	shr     $1, cy2         C restore carry
	adc  (vp,n,8), w
	mov     w, (rp,n,8)
	sbb     cy1, cy1        C generate mask, preserve CF

	mov     8(up,n,8), w
	adc  8(vp,n,8), w
	mov     w, 8(rp,n,8)
	sbb     cy2, cy2        C generate mask, preserve CF

	mov     (yp1), w	C (e1h:e1l) += cy1 * yp1 limb
	and     cy1, w
	add     w, e1l
	adc     $0, e1h

	and     (yp1,yp2), cy1	C (e2h:e2l) += cy1 * yp2 limb
	add     cy1, e2l
	adc     $0, e2h

	mov     -8(yp1), w	C (e1h:e1l) += cy2 * next yp1 limb
	and     cy2, w
	add     w, e1l
	adc     $0, e1h

	mov     -8(yp1,yp2), w	C (e2h:e2l) += cy2 * next yp2 limb
	and     cy2, w
	add     w, e2l
	adc     $0, e2h

	add     $2, n
	lea     -16(yp1), yp1
	jnz     L(top)
L(end):

	mov	e1l, (ep)
	mov	e1h, 8(ep)
	mov	e2l, 16(ep)
	mov	e2h, 24(ep)

	and	$1, %eax	C return carry

	pop	%r14
	pop	%r13
	pop	%r12
	pop	%rbp
	pop	%rbx
	ret
EPILOGUE()
