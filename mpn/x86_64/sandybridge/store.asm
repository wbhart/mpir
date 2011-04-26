dnl  mpn_store

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

define(`MOVQ',`movd')

C	mpn_store(mp_ptr,mp_size_t,mp_limb_t)
C	rax          rdi,   rsi,    rdx

ASM_START()
PROLOGUE(mpn_store)
lea -32(%rdi),%rdi
cmp $0,%rsi
jz case0
MOVQ %rdx,%xmm0
movddup %xmm0,%xmm0
test $0xF,%rdi
jz notodd
	mov %rdx,32(%rdi)
	lea 8(%rdi),%rdi
	sub $1,%rsi
notodd:
sub $4,%rsi
jc skiplp
ALIGN(16)
lp:
	lea 32(%rdi),%rdi
	sub $4,%rsi
	movdqa %xmm0,(%rdi)
	movdqa %xmm0,16(%rdi)
	jnc lp
skiplp:
cmp $-2,%rsi
ja case3
jz case2
jp case1
case0:
ret
case3:// rsi=-1
	movdqa %xmm0,32(%rdi)
case1:
	mov %rdx,56(%rdi,%rsi,8)
	ret
case2:// rsi=-2
	movdqa %xmm0,32(%rdi)
	ret
EPILOGUE()
