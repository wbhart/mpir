dnl  mpn_lshift4

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

C	ret mpn_lshift4(mp_ptr,mp_ptr,mp_size_t)
C	rax                 rdi,   rsi,      rdx

ASM_START()
PROLOGUE(mpn_lshift4)
mov $3,%rcx
lea -24(%rsi,%rdx,8),%rsi
lea -24(%rdi,%rdx,8),%rdi
mov $0,%r8
mov $0,%rax
sub %rdx,%rcx
jnc skiplp
ALIGN(16)
lp:
	mov (%rsi,%rcx,8),%r9
	lea (%rax,%r9,2),%rdx
	lea (%r8,%rdx,8),%r8
	shr $60,%r9
	mov 8(%rsi,%rcx,8),%r10
	lea (%rax,%r10,2),%rdx
	lea (%r9,%rdx,8),%r9
	shr $60,%r10
	mov 16(%rsi,%rcx,8),%r11
	lea (%rax,%r11,2),%rdx
	lea (%r10,%rdx,8),%r10
	shr $60,%r11
	mov %r10,16(%rdi,%rcx,8)
	mov %r8,(%rdi,%rcx,8)
	mov 24(%rsi,%rcx,8),%r8
	lea (%rax,%r8,2),%rdx
	lea (%r11,%rdx,8),%r11
	shr $60,%r8
	mov %r11,24(%rdi,%rcx,8)
	add $4,%rcx
	mov %r9,8-32(%rdi,%rcx,8)
	jnc lp
skiplp:
cmp $2,%rcx
ja case0
je case1
jp case2
case3:
	mov (%rsi,%rcx,8),%r9
	lea (%rax,%r9,2),%rdx
	lea (%r8,%rdx,8),%r8
	shr $60,%r9
	mov 8(%rsi,%rcx,8),%r10
	lea (%rax,%r10,2),%rdx
	lea (%r9,%rdx,8),%r9
	shr $60,%r10
	mov 16(%rsi,%rcx,8),%r11
	lea (%rax,%r11,2),%rdx
	lea (%r10,%rdx,8),%r10
	shr $60,%r11
	mov %r10,16(%rdi,%rcx,8)
	mov %r8,(%rdi,%rcx,8)
	mov %r11,%rax
	mov %r9,8(%rdi,%rcx,8)
	ret
ALIGN(16)
case2:
	mov (%rsi,%rcx,8),%r9
	lea (%rax,%r9,2),%rdx
	lea (%r8,%rdx,8),%r8
	shr $60,%r9
	mov 8(%rsi,%rcx,8),%r10
	lea (%rax,%r10,2),%rdx
	lea (%r9,%rdx,8),%r9
	shr $60,%r10
	mov %r10,%rax
	mov %r8,(%rdi,%rcx,8)
	mov %r9,8(%rdi,%rcx,8)
	ret
ALIGN(16)
case1:
	mov (%rsi,%rcx,8),%r9
	lea (%rax,%r9,2),%rdx
	lea (%r8,%rdx,8),%r8
	shr $60,%r9
	mov %r8,(%rdi,%rcx,8)
	mov %r9,%rax
	ret
ALIGN(16)
case0:	mov %r8,%rax
	ret
EPILOGUE()
