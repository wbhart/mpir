dnl  mpn_ior_n

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

C	ret mpn_ior_n(mp_ptr,mp_ptr,mp_ptr,mp_size_t)
C	rax              rdi,   rsi,    rdx,  rcx

ASM_START()
PROLOGUE(mpn_ior_n)
sub $4,%rcx
jc skiplp
ALIGN(16)
lp:
	mov (%rdx),%r8
	mov 8(%rdx),%r9
	or (%rsi),%r8
	or 8(%rsi),%r9
	lea 32(%rsi),%rsi
	mov 16(%rdx),%r10
	mov 24(%rdx),%r11
	lea 32(%rdi),%rdi
	or 16-32(%rsi),%r10
	or 24-32(%rsi),%r11
	sub $4,%rcx
	mov %r8,-32(%rdi)
	mov %r9,8-32(%rdi)
	mov %r10,16-32(%rdi)
	lea 32(%rdx),%rdx
	mov %r11,24-32(%rdi)
	jnc lp
skiplp:
cmp $-2,%rcx
je case2
jp case1
jl case0
case3:	mov 8(%rdx,%rcx,8),%rax
	or 8(%rsi,%rcx,8),%rax
	mov %rax,8(%rdi,%rcx,8)
case2:	mov 16(%rdx,%rcx,8),%rax
	or 16(%rsi,%rcx,8),%rax
	mov %rax,16(%rdi,%rcx,8)
case1:	mov 24(%rdx,%rcx,8),%rax
	or 24(%rsi,%rcx,8),%rax
	mov %rax,24(%rdi,%rcx,8)
case0:	ret
EPILOGUE()
