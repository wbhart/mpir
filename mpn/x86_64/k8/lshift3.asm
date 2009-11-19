dnl  mpn_lshift3

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

C	ret mpn_lshift3(mp_ptr,mp_ptr,mp_size_t)
C	rax                 rdi,   rsi,      rdx

ASM_START()
PROLOGUE(mpn_lshift3)
mov $3,%rcx
lea -24(%rsi,%rdx,8),%rsi
lea -24(%rdi,%rdx,8),%rdi
mov $0,%r8
sub %rdx,%rcx
jnc skiplp
ALIGN(16)
lp:
	mov (%rsi,%rcx,8),%r9
	lea (%r8,%r9,8),%r8
	shr $61,%r9
	mov 8(%rsi,%rcx,8),%r10
	lea (%r9,%r10,8),%r9
	shr $61,%r10
	mov 16(%rsi,%rcx,8),%r11
	mov %r8,(%rdi,%rcx,8)
	lea (%r10,%r11,8),%r10
	mov %r10,16(%rdi,%rcx,8)
	shr $61,%r11
	mov 24(%rsi,%rcx,8),%r8
	lea (%r11,%r8,8),%r11
	mov %r11,24(%rdi,%rcx,8)
	shr $61,%r8
	add $4,%rcx
	mov %r9,8-32(%rdi,%rcx,8)
	jnc lp
skiplp:
cmp $2,%rcx
ja case0
je case1
jp case2
case3:	mov (%rsi,%rcx,8),%r9
	lea (%r8,%r9,8),%r8
	shr $61,%r9
	mov 8(%rsi,%rcx,8),%r10
	lea (%r9,%r10,8),%r9
	shr $61,%r10
	mov 16(%rsi,%rcx,8),%r11
	mov %r8,(%rdi,%rcx,8)
	lea (%r10,%r11,8),%r10
	mov %r10,16(%rdi,%rcx,8)
	shr $61,%r11
	mov %r11,%rax
	mov %r9,8(%rdi,%rcx,8)
	ret
ALIGN(16)
case2:	mov (%rsi,%rcx,8),%r9
	lea (%r8,%r9,8),%r8
	shr $61,%r9
	mov 8(%rsi,%rcx,8),%r10
	lea (%r9,%r10,8),%r9
	shr $61,%r10
	mov %r8,(%rdi,%rcx,8)
	mov %r10,%rax
	mov %r9,8(%rdi,%rcx,8)
	ret
ALIGN(16)
case1:	mov (%rsi,%rcx,8),%r9
	lea (%r8,%r9,8),%r8
	shr $61,%r9
	mov %r8,(%rdi,%rcx,8)
	mov %r9,%rax
	ret
ALIGN(16)
case0:	mov %r8,%rax
	ret
EPILOGUE()
