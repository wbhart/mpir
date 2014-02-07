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

C ret mpn_add_err1(mp_ptr rp,mp_ptr up,mp_ptr vp,mp_ptr ep,mp_ptr_t yp,mp_size_t n,mp_limb_t cy)
C rax                    rdi,      rsi,      rdx,      rcx,         r8           r9       8(rsp)=>r10

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
	mov	cy_param, %rax    C cy

	push	%rbx
	push	%rbp
	push	%r12

	xor el, el              C zero el, eh
	xor eh, eh
	lea	(rp,n,8), rp     C rp += n, up += n, vp += n
	lea	(up,n,8), up
	lea	(vp,n,8), vp

	test	$1, n            C if n is odd goto L(odd)
	jnz	L(odd)

L(even):	
	lea	-8(yp,n,8), yp   C yp += n - 1
	neg	n                C { n = -n }
	jmp	L(top)

	ALIGN(16)
L(odd):                           C n is odd, do extra iteration
	lea	-16(yp,n,8), yp     C yp += n - 2
	neg	n                   C { n = -n }
	shr	$1, %rax            C rp[0] = up[0] + vp[0] + (cy&1)
	mov	(up,n,8), w
	adc	(vp,n,8), w
	cmovc	8(yp), el           C if carry el = *yp
	mov	w, (rp,n,8)
	setc	%al                 C store carry
	inc	n                   C n++
	jz	L(end)              C goto end if we are done

	ALIGN(16)
L(top):
       mov     (up,n,8), w     C rp[n] = up[n] + vp[n] + carry
	shr     $1, %rax        C { restore carry }
	adc     (vp,n,8), w
	mov     $0, t1          C initialise t1
	mov     w, (rp,n,8)
	mov     $0, t0          C initialise t0
	mov     8(up,n,8), w    C rp[n+1] = up[n+1] + vp[n+1] + carry
	cmovc   (yp), t0        C if carry t0 = yp
	adc     8(vp,n,8), w
	cmovc   -8(yp), t1      C if next carry t1 = *(yp-1)
	setc    %al             C { save carry }
	add     t0, el          C (eh:el) += carry*yp limb
	adc     $0, eh
	add     t1, el          C (eh:el) += next carry*next yp limb
	mov     w, 8(rp,n,8)
	adc     $0, eh
	add     $2, n           C n += 2
	lea     -16(yp), yp     C yp -= 2
	jnz     L(top)          C if not done goto top

L(end):	
	mov	el, (ep)         C write out (eh:el)
	mov	eh, 8(ep)

	pop	%r12
	pop	%rbp
	pop	%rbx
	ret
EPILOGUE()
