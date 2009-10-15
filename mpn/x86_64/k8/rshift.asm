dnl  mpn_rshift

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

C	ret mpn_rshift(mp_ptr,mp_ptr,mp_size_t,ul)
C	rax                 rdi,   rsi,      rdx

ASM_START()
PROLOGUE(mpn_rshift)
C below really a movq
movd %rcx,%mm0

mov $64,%rax
sub %rcx,%rax

C below really a movq
movd %rax,%mm1

mov $4,%rcx
lea -32(%rsi,%rdx,8),%rsi
lea -32(%rdi,%rdx,8),%rdi
sub %rdx,%rcx
movq (%rsi,%rcx,8),%mm5
movq %mm5,%mm3
psllq %mm1,%mm5

C below really a movq
movd %mm5,%rax

psrlq %mm0,%mm3
jge skiplp
ALIGN(16)
lp:
	movq 8(%rsi,%rcx,8),%mm2
	movq %mm2,%mm4
	psllq %mm1,%mm2
	por %mm2,%mm3
	movq %mm3,(%rdi,%rcx,8)
	psrlq %mm0,%mm4
	movq 16(%rsi,%rcx,8),%mm5
	movq %mm5,%mm3
	psllq %mm1,%mm5
	por %mm5,%mm4
	movq %mm4,8(%rdi,%rcx,8)
	psrlq %mm0,%mm3
	movq 24(%rsi,%rcx,8),%mm2
	movq %mm2,%mm4
	psllq %mm1,%mm2
	por %mm2,%mm3
	movq 32(%rsi,%rcx,8),%mm5
	movq %mm3,16(%rdi,%rcx,8)
	psrlq %mm0,%mm4
	movq %mm5,%mm3
	psllq %mm1,%mm5
	por %mm5,%mm4
	movq %mm4,24(%rdi,%rcx,8)
	psrlq %mm0,%mm3
	add $4,%rcx
	jnc lp
skiplp:
cmp $2,%rcx
ja case0
jz case1
jp case2
case3:
	movq 8(%rsi,%rcx,8),%mm2
	movq %mm2,%mm4
	psllq %mm1,%mm2
	por %mm2,%mm3
	movq %mm3,(%rdi,%rcx,8)
	psrlq %mm0,%mm4
	movq 16(%rsi,%rcx,8),%mm5
	movq %mm5,%mm3
	psllq %mm1,%mm5
	por %mm5,%mm4
	movq %mm4,8(%rdi,%rcx,8)
	psrlq %mm0,%mm3
	movq 24(%rsi,%rcx,8),%mm2
	movq %mm2,%mm4
	psllq %mm1,%mm2
	por %mm2,%mm3
	movq %mm3,16(%rdi,%rcx,8)
	psrlq %mm0,%mm4
	movq %mm4,24(%rdi,%rcx,8)
	emms
	ret
ALIGN(16)
case2:
	movq 8(%rsi,%rcx,8),%mm2
	movq %mm2,%mm4
	psllq %mm1,%mm2
	por %mm2,%mm3
	movq %mm3,(%rdi,%rcx,8)
	psrlq %mm0,%mm4
	movq 16(%rsi,%rcx,8),%mm5
	movq %mm5,%mm3
	psllq %mm1,%mm5
	por %mm5,%mm4
	movq %mm4,8(%rdi,%rcx,8)
	psrlq %mm0,%mm3
	movq %mm3,16(%rdi,%rcx,8)
	emms
	ret
ALIGN(16)
case1:
	movq 8(%rsi,%rcx,8),%mm2
	movq %mm2,%mm4
	psllq %mm1,%mm2
	por %mm2,%mm3
	movq %mm3,(%rdi,%rcx,8)
	psrlq %mm0,%mm4
	movq %mm4,8(%rdi,%rcx,8)
	emms
	ret
ALIGN(16)
case0:
	movq %mm3,(%rdi,%rcx,8)
	emms
	ret
EPILOGUE()
