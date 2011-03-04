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
lea -8(%rsi,%rdx,8),%rsi
lea -8(%rdi,%rdx,8),%rdi
mov $1,%r8d
sub %rdx,%r8
mov (%rsi,%r8,8),%rax
mov $0,%r9d
jz skiplp
ALIGN(16)
lp:	mul %rcx
	add %r9,%rax
	mov %rax,(%rdi,%r8,8)
	mov 8(%rsi,%r8,8),%rax
	mov $0,%r9d
	adc %rdx,%r9
	add $1,%r8
	jnc lp
skiplp:
mul %rcx
add %r9,%rax
mov %rax,(%rdi,%r8,8)
mov $0,%eax
adc %rdx,%rax
ret
EPILOGUE()
