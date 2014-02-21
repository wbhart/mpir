dnl  x86-64 mpn_addmul_1 and mpn_submul_1, optimized for "Core 2".

dnl  Copyright 2003, 2004, 2005, 2007, 2008, 2009, 2011, 2012 Free Software
dnl  Foundation, Inc.

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

C	     cycles/limb
C AMD K8,K9	 4
C AMD K10	 4
C AMD bd1	 5.1
C AMD bobcat
C Intel P4	 ?
C Intel core2	 4.3-4.5 (fluctuating)
C Intel NHM	 5.0
C Intel SBR	 4.1
C Intel atom	 ?
C VIA nano	 5.25

C INPUT PARAMETERS
define(`rp',	`%rdi')
define(`up',	`%rsi')
define(`n',	`%rdx')
define(`v0',	`%rcx')
define(`carry_in', `%r8')

MULFUNC_PROLOGUE(mpn_addmul_1 mpn_addmul_1c)

ASM_START()
	TEXT
	ALIGN(16)
PROLOGUE(mpn_addmul_1c)
	push	%rbx
	push	%rbp
	lea	(%rdx), %rbx
	neg	%rbx

	mov	(up), %rax
	mov	(rp), %r10

	lea	-16(rp,%rdx,8), rp
	lea	(up,%rdx,8), up
	mul	%rcx
	add	carry_in, %rax
	adc	$0, %rdx
	jmp	start_nc
EPILOGUE()

	ALIGN(16)
PROLOGUE(mpn_addmul_1)
	push	%rbx
	push	%rbp
	lea	(%rdx), %rbx
	neg	%rbx

	mov	(up), %rax
	mov	(rp), %r10

	lea	-16(rp,%rdx,8), rp
	lea	(up,%rdx,8), up
	mul	%rcx

start_nc:
	bt	$0, %ebx
	jc	odd

	lea	(%rax), %r11
	mov	8(up,%rbx,8), %rax
	lea	(%rdx), %rbp
	mul	%rcx
	add	$2, %rbx
	jns	ln2

	lea	(%rax), %r8
	mov	(up,%rbx,8), %rax
	lea	(%rdx), %r9
	jmp	mid

odd:	add	$1, %rbx
	jns	ln1

	lea	(%rax), %r8
	mov	(up,%rbx,8), %rax
	lea	(%rdx), %r9
	mul	%rcx
	lea	(%rax), %r11
	mov	8(up,%rbx,8), %rax
	lea	(%rdx), %rbp
	jmp	le

	ALIGN(16)
top:	mul	%rcx
	add	%r8, %r10
	lea	(%rax), %r8
	mov	(up,%rbx,8), %rax
	adc	%r9, %r11
	mov	%r10, -8(rp,%rbx,8)
	mov	(rp,%rbx,8), %r10
	lea	(%rdx), %r9
	adc	$0, %rbp
mid:	mul	%rcx
	add	%r11, %r10
	lea	(%rax), %r11
	mov	8(up,%rbx,8), %rax
	adc	%rbp, %r8
	mov	%r10, (rp,%rbx,8)
	mov	8(rp,%rbx,8), %r10
	lea	(%rdx), %rbp
	adc	$0, %r9
le:	add	$2, %rbx
	js	top

	mul	%rcx
	add	%r8, %r10
	adc	%r9, %r11
	mov	%r10, -8(rp)
	adc	$0, %rbp
ln2:	mov	(rp), %r10
	add	%r11, %r10
	adc	%rbp, %rax
	mov	%r10, (rp)
	adc	$0, %rdx
ln1:	mov	8(rp), %r10
	add	%rax, %r10
	mov	%r10, 8(rp)
	mov    %ebx, %eax	C zero rax
	adc	%rdx, %rax
	pop	%rbp
	pop	%rbx
	ret
EPILOGUE()
