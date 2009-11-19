dnl  mpn_com_n

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

C	ret mpn_com_n(mp_ptr,mp_ptr,mp_size_t)
C	rax             rdi,   rsi,      rdx

ASM_START()
PROLOGUE(mpn_com_n)
mov $3,%rcx
lea -24(%rsi,%rdx,8),%rsi
pcmpeqb %xmm2,%xmm2
sub %rdx,%rcx
jnc skiplp
ALIGN(16)
lp:
	movdqu (%rsi,%rcx,8),%xmm0
	movdqu 16(%rsi,%rcx,8),%xmm1
	pxor %xmm2,%xmm0
	add $4,%rcx
	pxor %xmm2,%xmm1
	movdqu %xmm0,(%rdi)
	movdqu %xmm1,16(%rdi)
	lea 32(%rdi),%rdi
	jnc lp
skiplp:
cmp $2,%rcx
ja case0
je case1
jp case2	
case3:	movdqu (%rsi,%rcx,8),%xmm0
	mov 16(%rsi,%rcx,8),%rax
	pxor %xmm2,%xmm0
	not %rax
	movdqu %xmm0,(%rdi)
	mov %rax,16(%rdi)
	ret
case2:	movdqu (%rsi,%rcx,8),%xmm0
	pxor %xmm2,%xmm0
	movdqu %xmm0,(%rdi)
	ret
case1:	mov (%rsi,%rcx,8),%rax
	not %rax
	mov %rax,(%rdi)
case0:	ret
EPILOGUE()
