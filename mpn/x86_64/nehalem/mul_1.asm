dnl  mpn_mul_1

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
PROLOGUE(mpn_mul_1)
mov $3,%r8
lea -24(%rdi,%rdx,8),%rdi
lea -24(%rsi,%rdx,8),%rsi
sub %rdx,%r8
mov $0,%r9d
mov $0,%r11d
mov 24-24(%rsi,%r8,8),%rax
jnc skiplp
ALIGN(16)
lp:	mul %rcx
	add %rax,%r9
	mov %r9,(%rdi,%r8,8)
	mov $0,%r10d
	mov 8(%rsi,%r8,8),%rax
	adc %rdx,%r10
	mov $0,%r9d
	mul %rcx
	add %rax,%r10
	mov $0,%r11d
	mov 16(%rsi,%r8,8),%rax
	mov %r10,8(%rdi,%r8,8)
	adc %rdx,%r11
	mul %rcx
	add %rax,%r11
	mov 24(%rsi,%r8,8),%rax
	mov %r11,16(%rdi,%r8,8)
	adc %rdx,%r9
	add $3,%r8
	jnc lp
skiplp:
cmp $1,%r8
ja case0
je case1
case2:	mul %rcx
	add %rax,%r9
	mov %r9,(%rdi)
	mov $0,%r10d
	mov 8(%rsi),%rax
	adc %rdx,%r10
	mov $0,%r9d
	mul %rcx
	add %rax,%r10
	mov $0,%r11d
	mov 16(%rsi),%rax
	mov %r10,8(%rdi)
	adc %rdx,%r11
	mul %rcx
	add %rax,%r11
	mov %r11,16(%rdi)
	adc %rdx,%r9
	mov %r9,%rax
	ret
case1:	mul %rcx
	add %rax,%r9
	mov %r9,8(%rdi)
	mov $0,%r10d
	mov 16(%rsi),%rax
	adc %rdx,%r10
	mov $0,%r9d
	mul %rcx
	add %rax,%r10
	mov $0,%r11d
	mov %r10,16(%rdi)
	adc %rdx,%r11
	mov %r11,%rax
	ret
case0:	mul %rcx
	add %rax,%r9
	mov %r9,16(%rdi)
	mov $0,%r10d
	adc %rdx,%r10
	mov %r10,%rax
	ret	
EPILOGUE()
