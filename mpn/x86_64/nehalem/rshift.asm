dnl  mpn_rshift

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

C	mpn_rshift(mp_ptr rdi,mp_ptr rsi,mp_size_t rdx,mp_limb_t rcx)
C	rax=carry

ASM_START()
PROLOGUE(mpn_rshift)
C // when n=1 mod4 seem to have different runtimes
push %rbx
mov $4,%rbx
lea -24(%rsi,%rdx,8),%rsi
lea -24(%rdi,%rdx,8),%rdi
sub %rdx,%rbx
xor %rax,%rax
mov -8(%rsi,%rbx,8),%rdx
shrd %cl,%rdx,%rax
cmp $0,%rbx
jge skiplp	
ALIGN(16)
lp:
	mov (%rsi,%rbx,8),%r8
	mov 24(%rsi,%rbx,8),%r11
	shrd %cl,%r8,%rdx
	mov 8(%rsi,%rbx,8),%r9
	shrd %cl,%r9,%r8
	mov %rdx,-8(%rdi,%rbx,8)
	mov %r8,(%rdi,%rbx,8)
	mov 16(%rsi,%rbx,8),%r10
	shrd %cl,%r10,%r9
	mov %r11,%rdx
	mov %r9,8(%rdi,%rbx,8)
	shrd %cl,%r11,%r10
	add $4,%rbx
	mov %r10,-16(%rdi,%rbx,8)
	jnc lp
skiplp:
cmp $2,%rbx
ja case0
je case1
jp case2
case3:
	mov (%rsi,%rbx,8),%r8
	shrd %cl,%r8,%rdx
	mov 8(%rsi,%rbx,8),%r9
	shrd %cl,%r9,%r8
	mov %rdx,-8(%rdi,%rbx,8)
	mov %r8,(%rdi,%rbx,8)
	mov 16(%rsi,%rbx,8),%r10
	shrd %cl,%r10,%r9
	mov %r9,8(%rdi,%rbx,8)
	shr %cl,%r10
	mov %r10,16(%rdi,%rbx,8)
	pop %rbx
	ret
ALIGN(16)
case2:
	mov (%rsi,%rbx,8),%r8
	shrd %cl,%r8,%rdx
	mov 8(%rsi,%rbx,8),%r9
	shrd %cl,%r9,%r8
	mov %rdx,-8(%rdi,%rbx,8)
	mov %r8,(%rdi,%rbx,8)
	shr %cl,%r9
	mov %r9,8(%rdi,%rbx,8)
	pop %rbx
	ret
ALIGN(16)
case1:
	mov (%rsi,%rbx,8),%r8
	shrd %cl,%r8,%rdx
	shr %cl,%r8
	mov %rdx,-8(%rdi,%rbx,8)
	mov %r8,(%rdi,%rbx,8)
	pop %rbx
	ret
ALIGN(16)
case0:
	shr %cl,%rdx
	mov %rdx,-8(%rdi,%rbx,8)
	pop %rbx
	ret
EPILOGUE()
