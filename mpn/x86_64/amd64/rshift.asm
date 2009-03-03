dnl  AMD64 mpn_rshift

dnl Copyright 2008 Jason Moxham

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

C	(rdi,rdx)=(rsi,rdx)>>rcx
C	rax=carry

ASM_START()
PROLOGUE(mpn_rshift)
mov $64,%eax
lea -32(%rsi,%rdx,8),%rsi
lea -32(%rdi,%rdx,8),%rdi
sub %rcx,%rax
movq %rcx,%mm0
mov $4,%r8d
sub %rdx,%r8
movq %rax,%mm1
movq (%rsi,%r8,8),%mm5
movq %mm5,%mm3
psllq %mm1,%mm5
movq %mm5,%rax
psrlq %mm0,%mm3
jnc skiploop
ALIGN(16)
loop:
	movq 8(%rsi,%r8,8),%mm2
	movq %mm2,%mm4
	psllq %mm1,%mm2
	por %mm2,%mm3
	movq %mm3,(%rdi,%r8,8)
	psrlq %mm0,%mm4

	movq 16(%rsi,%r8,8),%mm5
	movq %mm5,%mm3
	psllq %mm1,%mm5
	por %mm5,%mm4
	movq %mm4,8(%rdi,%r8,8)
	psrlq %mm0,%mm3

C got room here for another jump out , if we can arrange our r8 to be
C slightly different , so we can use a jz or jp here
	movq 24(%rsi,%r8,8),%mm2
	movq %mm2,%mm4
	psllq %mm1,%mm2
	por %mm2,%mm3
	movq %mm3,16(%rdi,%r8,8)
	psrlq %mm0,%mm4

	movq 32(%rsi,%r8,8),%mm5
	movq %mm5,%mm3
	psllq %mm1,%mm5
	por %mm5,%mm4
	movq %mm4,24(%rdi,%r8,8)
	psrlq %mm0,%mm3

	add $4,%r8
	jnc loop
skiploop:
test $2,%r8
jnz next
	movq 8(%rsi,%r8,8),%mm2
	movq %mm2,%mm4
	psllq %mm1,%mm2
	por %mm2,%mm3
	movq %mm3,(%rdi,%r8,8)
	psrlq %mm0,%mm4
	movq 16(%rsi,%r8,8),%mm5
	movq %mm5,%mm3
	psllq %mm1,%mm5
	por %mm5,%mm4
	movq %mm4,8(%rdi,%r8,8)
	psrlq %mm0,%mm3
	add $2,%r8
next:
test $1,%r8
jnz end
	movq 8(%rsi,%r8,8),%mm2
	movq %mm2,%mm4
	psllq %mm1,%mm2
	por %mm2,%mm3
	movq %mm3,(%rdi,%r8,8)
	psrlq %mm0,%mm4
	movq %mm4,8(%rdi,%r8,8)
	emms
	ret
end:
movq %mm3,(%rdi,%r8,8)
emms
ret
EPILOGUE()
