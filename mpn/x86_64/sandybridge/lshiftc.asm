dnl  mpn_lshiftc

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

ASM_START()
PROLOGUE(mpn_lshiftc)
MOVQ %rcx,%mm0
mov $64,%rax
sub %rcx,%rax
pcmpeqb %mm6,%mm6
MOVQ %rax,%mm1
lea 8(%rsi),%rsi
lea 8(%rdi),%rdi
sub $5,%rdx
movq 24(%rsi,%rdx,8),%mm5
movq %mm5,%mm3
psrlq %mm1,%mm5
MOVQ %mm5,%rax
psllq %mm0,%mm3
jc L(skiplp)
ALIGN(16)
L(lp):
	movq 16(%rsi,%rdx,8),%mm2
	movq %mm2,%mm4
	psrlq %mm1,%mm2
	por %mm2,%mm3
	movq (%rsi,%rdx,8),%mm2
	pxor %mm6,%mm3
	movq %mm3,24(%rdi,%rdx,8)
	psllq %mm0,%mm4
	movq 8(%rsi,%rdx,8),%mm5
	movq %mm5,%mm3
	psrlq %mm1,%mm5
	por %mm5,%mm4
	pxor %mm6,%mm4
	movq %mm4,16(%rdi,%rdx,8)
	psllq %mm0,%mm3
	movq %mm2,%mm4
	movq -8(%rsi,%rdx,8),%mm5
	sub $4,%rdx
	psrlq %mm1,%mm2
	por %mm2,%mm3
	pxor %mm6,%mm3
	movq %mm3,40(%rdi,%rdx,8)
	psllq %mm0,%mm4
	movq %mm5,%mm3
	psrlq %mm1,%mm5
	por %mm5,%mm4
	pxor %mm6,%mm4
	movq %mm4,32(%rdi,%rdx,8)
	psllq %mm0,%mm3
	jnc L(lp)
L(skiplp):
cmp $-2,%rdx
jz L(case2)
jp L(case1)
js L(case0)
L(case3):
	movq 16(%rsi,%rdx,8),%mm2
	movq %mm2,%mm4
	psrlq %mm1,%mm2
	por %mm2,%mm3
	movq (%rsi,%rdx,8),%mm2
	pxor %mm6,%mm3
	movq %mm3,24(%rdi,%rdx,8)
	psllq %mm0,%mm4
	movq 8(%rsi,%rdx,8),%mm5
	movq %mm5,%mm3
	psrlq %mm1,%mm5
	por %mm5,%mm4
	pxor %mm6,%mm4
	movq %mm4,16(%rdi,%rdx,8)
	psllq %mm0,%mm3
	movq %mm2,%mm4
	psrlq %mm1,%mm2
	por %mm2,%mm3
	pxor %mm6,%mm3
	movq %mm3,8(%rdi,%rdx,8)
	psllq %mm0,%mm4
	pxor %mm6,%mm4
	movq %mm4,(%rdi,%rdx,8)
	emms
	ret
ALIGN(16)
L(case2):
	movq 16(%rsi,%rdx,8),%mm2
	movq %mm2,%mm4
	psrlq %mm1,%mm2
	por %mm2,%mm3
	pxor %mm6,%mm3
	movq %mm3,24(%rdi,%rdx,8)
	psllq %mm0,%mm4
	movq 8(%rsi,%rdx,8),%mm5
	movq %mm5,%mm3
	psrlq %mm1,%mm5
	por %mm5,%mm4
	pxor %mm6,%mm4
	movq %mm4,16(%rdi,%rdx,8)
	psllq %mm0,%mm3
	pxor %mm6,%mm3
	movq %mm3,8(%rdi,%rdx,8)
	emms
	ret
ALIGN(16)
L(case1):
	movq 16(%rsi,%rdx,8),%mm2
	movq %mm2,%mm4
	psrlq %mm1,%mm2
	por %mm2,%mm3
	pxor %mm6,%mm3
	movq %mm3,24(%rdi,%rdx,8)
	psllq %mm0,%mm4
	pxor %mm6,%mm4
	movq %mm4,16(%rdi,%rdx,8)
	emms
	ret
ALIGN(16)
L(case0):
	pxor %mm6,%mm3
	movq %mm3,24(%rdi,%rdx,8)
	emms
	ret
EPILOGUE()
