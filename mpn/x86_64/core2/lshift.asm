dnl  mpn_lshift

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

C	mpn_lshift(mp_ptr rdi,mp_ptr rsi,mp_size_t rdx,mp_limb_t rcx)
C	rax=carry

ASM_START()
PROLOGUE(mpn_lshift)
C	// odd and even n seem to have different runtimes
push %rbx
mov %rdx,%rbx
lea 24(%rsi),%rsi
lea 24(%rdi),%rdi
mov -32(%rsi,%rbx,8),%rdx
xor %rax,%rax
shld %cl,%rdx,%rax
sub $5,%rbx
js skiplp
ALIGN(16)
lp:
	mov (%rsi,%rbx,8),%r8
	mov -24(%rsi,%rbx,8),%r11
	mov -8(%rsi,%rbx,8),%r9
	shld %cl,%r8,%rdx
	mov %rdx,8(%rdi,%rbx,8)
	mov %r11,%rdx
	mov -16(%rsi,%rbx,8),%r10
	shld %cl,%r9,%r8
	shld %cl,%r10,%r9
	mov %r8,(%rdi,%rbx,8)
	mov %r9,-8(%rdi,%rbx,8)
	shld %cl,%r11,%r10
	sub $4,%rbx
	mov %r10,16(%rdi,%rbx,8)
	jns lp
skiplp:
cmp $-2,%rbx
ja case3
je case2
jp case1
case0:
	shl %cl,%rdx
	mov %rdx,8(%rdi,%rbx,8)
	pop %rbx
	ret
ALIGN(16)
case3:
	mov (%rsi,%rbx,8),%r8
	mov -8(%rsi,%rbx,8),%r9
	shld %cl,%r8,%rdx
	mov %rdx,8(%rdi,%rbx,8)
	mov -16(%rsi,%rbx,8),%r10
	shld %cl,%r9,%r8
	shld %cl,%r10,%r9
	mov %r8,(%rdi,%rbx,8)
	mov %r9,-8(%rdi,%rbx,8)
	shl %cl,%r10
	mov %r10,16-32(%rdi,%rbx,8)
	pop %rbx
	ret
ALIGN(16)
case2:
	mov (%rsi,%rbx,8),%r8
	mov -8(%rsi,%rbx,8),%r9
	shld %cl,%r8,%rdx
	mov %rdx,8(%rdi,%rbx,8)
	shld %cl,%r9,%r8
	shl %cl,%r9
	mov %r8,(%rdi,%rbx,8)
	mov %r9,-8(%rdi,%rbx,8)
	pop %rbx
	ret
ALIGN(16)
case1:
	mov (%rsi,%rbx,8),%r8
	shld %cl,%r8,%rdx
	mov %rdx,8(%rdi,%rbx,8)
	shl %cl,%r8
	mov %r8,(%rdi,%rbx,8)
	pop %rbx
	ret
EPILOGUE()
