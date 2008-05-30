dnl  AMD64 mpn_rshift

dnl  Copyright 2004, 2006 Free Software Foundation, Inc.

dnl  This file is part of the GNU MP Library.

dnl  The GNU MP Library is free software; you can redistribute it and/or modify
dnl  it under the terms of the GNU Lesser General Public License as published
dnl  by the Free Software Foundation; either version 3 of the License, or (at
dnl  your option) any later version.

dnl  The GNU MP Library is distributed in the hope that it will be useful, but
dnl  WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
dnl  or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public
dnl  License for more details.

dnl  You should have received a copy of the GNU Lesser General Public License
dnl  along with the GNU MP Library.  If not, see http://www.gnu.org/licenses/.

include(`../config.m4')


C		    cycles/limb
C AMD K8:		 2.5
C Intel P4:		 4
C Intel Core 2:		 2.4


C INPUT PARAMETERS
C rp	rdi
C up	rsi
C n	rdx
C cnt	rcx

ASM_START()
PROLOGUE(mpn_rshift)
	movq	(%rsi), %mm7
	movd	%ecx, %mm1
	movl	$64, %eax
	subl	%ecx, %eax
	movd	%eax, %mm0
	movq	%mm7, %mm3
	psllq	%mm0, %mm7
	.byte	0x48, 0x0f, 0x7e, 0xf8	C movd	%mm7, %rax
	leaq	(%rsi,%rdx,8), %rsi
	leaq	(%rdi,%rdx,8), %rdi
	negq	%rdx
	addq	$2, %rdx
	jg	L(endo)

	ALIGN(8)			C minimal alignment for claimed speed
L(loop):
	movq	-8(%rsi,%rdx,8), %mm6
	movq	%mm6, %mm2
	psllq	%mm0, %mm6
	psrlq	%mm1, %mm3
	por	%mm6, %mm3
	movq	%mm3, -16(%rdi,%rdx,8)
	je	L(ende)
	movq	(%rsi,%rdx,8), %mm7
	movq	%mm7, %mm3
	psllq	%mm0, %mm7
	psrlq	%mm1, %mm2
	por	%mm7, %mm2
	movq	%mm2, -8(%rdi,%rdx,8)
	addq	$2, %rdx
	jle	L(loop)

L(endo):
	movq	%mm3, %mm2
L(ende):
	psrlq	%mm1, %mm2
	movq	%mm2, -8(%rdi)
	emms
	ret
EPILOGUE()
