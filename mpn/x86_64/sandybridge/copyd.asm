dnl  mpn_copyd

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

C	ret mpn_copyd(mp_ptr,mp_ptr,mp_size_t)
C	rax             rdi,   rsi,      rdx

ASM_START()
PROLOGUE(mpn_copyd)
lea 16(%rsi),%rsi
lea 16(%rdi),%rdi
sub $4,%rdx
jc skiplp
ALIGN(16)
lp:
	movdqu (%rsi,%rdx,8),%xmm0
	movdqu -16(%rsi,%rdx,8),%xmm1
	sub $4,%rdx
	movdqu %xmm1,-16+32(%rdi,%rdx,8)
	movdqu %xmm0,32(%rdi,%rdx,8)
	jnc lp
skiplp:
cmp $-2,%rdx
jg case3
je case2
jnp case0
case1:	mov 8(%rsi,%rdx,8),%rax
	mov %rax,8(%rdi,%rdx,8)
case0:	ret
case3:	movdqu (%rsi,%rdx,8),%xmm0
	mov -8(%rsi,%rdx,8),%rax
	mov %rax,-8(%rdi,%rdx,8)
	movdqu %xmm0,(%rdi,%rdx,8)
	ret
case2:	movdqu (%rsi,%rdx,8),%xmm0
	movdqu %xmm0,(%rdi,%rdx,8)
	ret
EPILOGUE()
