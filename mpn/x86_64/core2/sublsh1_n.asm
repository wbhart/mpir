dnl  core2 mpn_sublsh1_n 

dnl  Copyright 2008 Jason Moxham

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

C	(rdi,rcx)=(rsi,rcx)-(rdx,rcx)<<1
C	rax=borrow

ASM_START()
PROLOGUE(mpn_sublsh1_n)
lea	(%rsi,%rcx,8),%rsi
lea	(%rdx,%rcx,8),%rdx
lea	(%rdi,%rcx,8),%rdi
neg	%rcx
xor	%r9,%r9
xor	%rax,%rax
test	$3,%rcx
jz	next
lp1:
	mov	(%rsi,%rcx,8),%r10
	add	$1,%r9
	sbb	(%rdx,%rcx,8),%r10
	sbb	%r9,%r9
	add	$1,%rax
	sbb	(%rdx,%rcx,8),%r10
	sbb	%rax,%rax
	mov	%r10,(%rdi,%rcx,8)
	add	$1,%rcx
	test	$3,%rcx
	jnz lp1
next:
cmp	$0,%rcx
jz	end
push %r15
push %r14
push %r13
push %r12
push %rbx
ALIGN(16)
lp:
	mov	(%rsi,%rcx,8),%r10
	mov	8(%rsi,%rcx,8),%rbx
	mov	16(%rsi,%rcx,8),%r11
	mov	24(%rsi,%rcx,8),%r8
	mov	(%rdx,%rcx,8),%r12
	mov	8(%rdx,%rcx,8),%r13
	mov	16(%rdx,%rcx,8),%r14
	mov	24(%rdx,%rcx,8),%r15
	add	$1,%r9
	sbb	%r12,%r10
	sbb	%r13,%rbx
	sbb	%r14,%r11
	sbb	%r15,%r8
	sbb	%r9,%r9
	add	$1,%rax
	sbb	%r12,%r10
	sbb	%r13,%rbx
	sbb	%r14,%r11
	sbb	%r15,%r8
	sbb	%rax,%rax
	mov	%r10,(%rdi,%rcx,8)
	mov	%rbx,8(%rdi,%rcx,8)
	mov	%r11,16(%rdi,%rcx,8)
	mov	%r8,24(%rdi,%rcx,8)
	add	$4,%rcx
	jnz lp
pop	%rbx
pop	%r12
pop	%r13
pop	%r14
pop	%r15
end:
add	%r9,%rax
neg	%rax
ret
EPILOGUE()
