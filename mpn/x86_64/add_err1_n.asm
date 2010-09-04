dnl  AMD64 mpn_add_err1_n

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
C K8,K9:	 3.166
C K10:		 ?
C P4:		 ?
C P6-15 (Core2): ?
C P6-28 (Atom):	 ?

C INPUT PARAMETERS
define(`rp',	`%rdi')
define(`up',	`%rsi')
define(`vp',	`%rdx')
define(`ep',	`%rcx')
define(`yp',	`%r8')
define(`n',	`%r9')
define(`cy_param',	`8(%rsp)')
define(`el',	`%rbx')
define(`eh',	`%rbp')
define(`t0',	`%r10')
define(`t1',	`%r11')
define(`w',	`%r12')


ASM_START()
	TEXT
	ALIGN(16)
PROLOGUE(mpn_add_err1_n)
	mov	cy_param, %rax

	push	%rbx
	push	%rbp
	push	%r12

	xor el, el
	xor eh, eh
	lea	(rp,n,8), rp
	lea	(up,n,8), up
	lea	(vp,n,8), vp

	test	$1, n
	jnz	L(odd)

L(even):	
	lea	-8(yp,n,8), yp
	neg	n
	jmp	L(top)

	ALIGN(16)
L(odd):
	lea	-16(yp,n,8), yp
	neg	n
	shr	$1, %rax
	mov	(up,n,8), w
	adc	(vp,n,8), w
	cmovc	8(yp), el
	mov	w, (rp,n,8)
	setc	%al
	inc	n
	jz	L(end)

	ALIGN(16)
L(top):
        mov     (up,n,8), w
	shr     $1, %rax        C restore carry
	adc  (vp,n,8), w
	mov     $0, t1
	mov     w, (rp,n,8)
	mov     $0, t0
	mov     8(up,n,8), w
	cmovc   (yp), t0
	adc  8(vp,n,8), w
	cmovc   -8(yp), t1
	setc    %al             C save carry
	add     t0, el
	adc     $0, eh
	add     t1, el
	mov     w, 8(rp,n,8)
	adc     $0, eh
	add     $2, n
	lea     -16(yp), yp
	jnz     L(top)

L(end):	
	mov	el, (ep)
	mov	eh, 8(ep)

	pop	%r12
	pop	%rbp
	pop	%rbx
	ret
EPILOGUE()
