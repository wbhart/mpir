dnl  mpn_hamdist

dnl  Copyright 2010 The Code Cavern

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

ASM_START()
PROLOGUE(mpn_hamdist)
xor %eax,%eax
lea -24(%rdi,%rdx,8),%rdi
lea -24(%rsi,%rdx,8),%rsi
mov $3,%rcx
sub %rdx,%rcx
jnc skiplp
ALIGN(16)
lp:	mov (%rdi,%rcx,8),%r8
	mov 8(%rdi,%rcx,8),%r9
	xor (%rsi,%rcx,8),%r8
	mov 16(%rdi,%rcx,8),%r10
	popcnt %r8,%r8
	xor 8(%rsi,%rcx,8),%r9
	xor 16(%rsi,%rcx,8),%r10
	popcnt %r9,%r9
	mov 24(%rdi,%rcx,8),%r11
	add %r8,%rax
	popcnt %r10,%r10
	xor 24(%rsi,%rcx,8),%r11
	add %r9,%rax
	popcnt %r11,%r11
	add %r10,%rax
	add %r11,%rax
	add $4,%rcx
	jnc lp
skiplp:
cmp $2,%rcx
ja case0
je case1
jp case2
case3:	mov (%rdi),%r8
	xor (%rsi),%r8
	popcnt %r8,%r8
	add %r8,%rax
case2:	mov 8(%rdi),%r8
	xor 8(%rsi),%r8
	popcnt %r8,%r8
	add %r8,%rax
case1:	mov 16(%rdi),%r8
	xor 16(%rsi),%r8
	popcnt %r8,%r8
	add %r8,%rax
case0:	ret
EPILOGUE()
