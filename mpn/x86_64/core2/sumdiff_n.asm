dnl  core2 mpn_sumdiff_n

dnl  Copyright 2009 Jason Moxham

dnl  This file is part of the MPIR Library.

dnl  The MPIR Library is free software; you can redistribute it and/or modify
dnl  it under the terms of the GNU Lesser General Public License as published
dnl  by the Free Software Foundation; either version 2.1 of the License, or (at
dnl  your option) any later version.

dnl  The MPIR Library is distributed in the hope that it will be useful, but
dnl  WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
dnl  or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public
dnl  License for more details.

dnl  You should have received a copy of the GNU Lesser General Public License
dnl  along with the MPIR Library; see the file COPYING.LIB.  If not, write
dnl  to the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
dnl  Boston, MA 02110-1301, USA.

include(`../config.m4')

C	(rdi,r8)=(rdx,r8)+(rcx,r8)  (rsi,r8)=(rdx,r8)-(rcx,r8)
C	return 2*add_carry+sub_borrow

ASM_START()
PROLOGUE(mpn_sumdiff_n)
lea	(%rsi,%r8,8),%rsi
lea	(%rdx,%r8,8),%rdx
lea	(%rdi,%r8,8),%rdi
lea	(%rcx,%r8,8),%rcx
neg	%r8
xor	%r9,%r9
xor	%r10,%r10
test	$3,%r8
jz	next
lp1:
	mov	(%rdx,%r8,8),%rax
	mov	%rax,%r11
	add	$1,%r9
	adc	(%rcx,%r8,8),%rax
	sbb	%r9,%r9
	add	$1,%r10
	sbb	(%rcx,%r8,8),%r11
	sbb	%r10,%r10
	mov	%rax,(%rdi,%r8,8)
	mov	%r11,(%rsi,%r8,8)
	add	$1,%r8
	test	$3,%r8
	jnz lp1
next:
cmp	$0,%r8
jz	skiploop
mov %rbx,-8(%rsp)
mov %rbp,-16(%rsp)
mov %r12,-24(%rsp)
mov %r13,-32(%rsp)
mov %r14,-40(%rsp)
mov %r15,-48(%rsp)
ALIGN(16)
loop:
	mov	(%rdx,%r8,8),%rax
	mov	8(%rdx,%r8,8),%rbx
	mov	16(%rdx,%r8,8),%rbp
	mov	24(%rdx,%r8,8),%r12
	mov	%rax,%r11
	mov	%rbx,%r13
	mov	%rbp,%r14
	mov	%r12,%r15
	add	$1,%r9
	adc	(%rcx,%r8,8),%rax
	adc	8(%rcx,%r8,8),%rbx
	adc	16(%rcx,%r8,8),%rbp
	adc	24(%rcx,%r8,8),%r12
	sbb	%r9,%r9
	add	$1,%r10
	sbb	(%rcx,%r8,8),%r11
	sbb	8(%rcx,%r8,8),%r13
	sbb	16(%rcx,%r8,8),%r14
	sbb	24(%rcx,%r8,8),%r15
	sbb	%r10,%r10
	mov	%rax,(%rdi,%r8,8)
	mov	%rbx,8(%rdi,%r8,8)
	mov	%rbp,16(%rdi,%r8,8)
	mov	%r12,24(%rdi,%r8,8)
	mov	%r11,(%rsi,%r8,8)
	mov	%r13,8(%rsi,%r8,8)
	mov	%r14,16(%rsi,%r8,8)
	mov	%r15,24(%rsi,%r8,8)
	add	$4,%r8
	jnz loop
mov -8(%rsp),%rbx
mov -16(%rsp),%rbp
mov -24(%rsp),%r12
mov -32(%rsp),%r13
mov -40(%rsp),%r14
mov -48(%rsp),%r15
skiploop:
lea (%r10,%r9,2),%rax
neg	%rax
ret
EPILOGUE()
