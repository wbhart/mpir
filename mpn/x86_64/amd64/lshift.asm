dnl  AMD64 mpn_lshift

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

C	(rdi,rdx)=(rsi,rdx)<<rcx
C	rax=carry

ASM_START()
PROLOGUE(mpn_lshift)
mov $64,%eax
sub %rcx,%rax
movq %rcx,%mm0
sub $4,%rdx
movq %rax,%mm1
movq 24(%rsi,%rdx,8),%mm5
movq %mm5,%mm3
psrlq %mm1,%mm5
movq %mm5,%rax
psllq %mm0,%mm3
jbe skiploop
ALIGN(16)
loop:
	movq 16(%rsi,%rdx,8),%mm2
	movq %mm2,%mm4
	psrlq %mm1,%mm2
	por %mm2,%mm3
	movq %mm3,24(%rdi,%rdx,8)
	psllq %mm0,%mm4
	movq 8(%rsi,%rdx,8),%mm5
	movq %mm5,%mm3
	psrlq %mm1,%mm5
	por %mm5,%mm4
	movq %mm4,16(%rdi,%rdx,8)
	psllq %mm0,%mm3
	movq (%rsi,%rdx,8),%mm2
	movq %mm2,%mm4
	psrlq %mm1,%mm2
	por %mm2,%mm3
	movq %mm3,8(%rdi,%rdx,8)
	psllq %mm0,%mm4
	movq -8(%rsi,%rdx,8),%mm5
	movq %mm5,%mm3
	psrlq %mm1,%mm5
	por %mm5,%mm4
	movq %mm4,(%rdi,%rdx,8)
	psllq %mm0,%mm3
	sub $4,%rdx
	ja loop
skiploop:
cmp $-1,%rdx
jl next
	movq 16(%rsi,%rdx,8),%mm2
	movq %mm2,%mm4
	psrlq %mm1,%mm2
	por %mm2,%mm3
	movq %mm3,24(%rdi,%rdx,8)
	psllq %mm0,%mm4
	movq 8(%rsi,%rdx,8),%mm5
	movq %mm5,%mm3
	psrlq %mm1,%mm5
	por %mm5,%mm4
	movq %mm4,16(%rdi,%rdx,8)
	psllq %mm0,%mm3
	sub $2,%rdx
next:
test $1,%rdx
jnz end
	movq 16(%rsi,%rdx,8),%mm2
	movq %mm2,%mm4
	psrlq %mm1,%mm2
	por %mm2,%mm3
	movq %mm3,24(%rdi,%rdx,8)
	psllq %mm0,%mm4
	movq %mm4,16(%rdi,%rdx,8)
	emms
	ret
end:
movq %mm3,24(%rdi,%rdx,8)
emms
ret
EPILOGUE()
