dnl  core2 mpn_addadd_n

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

C	(rdi,r8)=(rsi,r8)+(rdx,r8)+(rcx,r8)
C	rax=carry

ASM_START()
PROLOGUE(mpn_addadd_n)
lea	(%rsi,%r8,8),%rsi
lea	(%rdx,%r8,8),%rdx
lea	(%rdi,%r8,8),%rdi
lea	(%rcx,%r8,8),%rcx
neg	%r8
xor	%rax,%rax
xor	%r10,%r10
test	$3,%r8
jz	next
lp1:
	mov	(%rdx,%r8,8),%r9
	add	$1,%rax
	adc	(%rcx,%r8,8),%r9
	sbb	%rax,%rax
	add	$1,%r10
	adc	(%rsi,%r8,8),%r9
	sbb	%r10,%r10
	mov	%r9,(%rdi,%r8,8)
	add	$1,%r8
	test	$3,%r8
	jnz lp1
next:
cmp	$0,%r8
jz	end
push %rbx
push %rbp
ALIGN(16)
lp:
	add	$1,%rax
	mov	(%rdx,%r8,8),%r9
	mov	8(%rdx,%r8,8),%rbx
	mov	16(%rdx,%r8,8),%rbp
	mov	24(%rdx,%r8,8),%r11
	adc	(%rcx,%r8,8),%r9
	adc	8(%rcx,%r8,8),%rbx
	adc	16(%rcx,%r8,8),%rbp
	adc	24(%rcx,%r8,8),%r11
	sbb	%rax,%rax
	add	$1,%r10
	adc	(%rsi,%r8,8),%r9
	adc	8(%rsi,%r8,8),%rbx
	adc	16(%rsi,%r8,8),%rbp
	adc	24(%rsi,%r8,8),%r11
	mov	%r9,(%rdi,%r8,8)
	mov	%rbx,8(%rdi,%r8,8)
	mov	%rbp,16(%rdi,%r8,8)
	mov	%r11,24(%rdi,%r8,8)
	sbb	%r10,%r10
	add	$4,%r8
	jnz lp
pop	%rbp
pop	%rbx
end:
add	%r10,%rax
neg	%rax
ret
EPILOGUE()
