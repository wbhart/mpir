dnl  AMD64 mpn_rshift

dnl Copyright 2009 Jason Moxham

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
# Version 1.0.4
cmp $2,%rdx
ja threeormore
jz two
one:
	mov (%rsi),%rdx
	mov %rdx,%rax
	shr %cl,%rdx
	neg %rcx
	shl %cl,%rax
	mov %rdx,(%rdi)
	ret
two:
	mov (%rsi),%r8
	mov 8(%rsi),%r9
	mov %r8,%rax
	mov %r9,%r11
	shr %cl,%r8
	shr %cl,%r9
	neg %rcx
	shl %cl,%r11
	shl %cl,%rax
	or  %r11,%r8
	mov %r8,(%rdi)
	mov %r9,8(%rdi)
	ret
threeormore:
mov $64,%eax
lea 8(%rsi),%r9
sub %rcx,%rax
and $-16,%r9
movq %rcx,%xmm0
movq %rax,%xmm1
movdqa (%r9),%xmm5
movdqa %xmm5,%xmm3
psllq %xmm1,%xmm5
movq %xmm5,%rax
cmp %r9,%rsi
lea -40(%rsi,%rdx,8),%rsi
je aligned
	movq -8(%r9),%xmm2
	movq %xmm2,%xmm4
	psllq %xmm1,%xmm2
	psrlq %xmm0,%xmm4
	por %xmm5,%xmm4
	movq %xmm4,(%rdi)
	lea 8(%rdi),%rdi	
	dec %rdx
	movq %xmm2,%rax
aligned:
lea -40(%rdi,%rdx,8),%rdi
psrlq %xmm0,%xmm3
mov $5,%r8d
sub %rdx,%r8
jnc skiploop
ALIGN(16)
loop:
	movdqa 16(%rsi,%r8,8),%xmm2
	movdqa %xmm2,%xmm4
	psllq %xmm1,%xmm2
	shufpd $1,%xmm2,%xmm5
	por %xmm5,%xmm3
	movq %xmm3,(%rdi,%r8,8)
	movhpd %xmm3,8(%rdi,%r8,8)
	psrlq %xmm0,%xmm4
	movdqa 32(%rsi,%r8,8),%xmm5
	movdqa %xmm5,%xmm3
	psllq %xmm1,%xmm5
	shufpd $1,%xmm5,%xmm2
	psrlq %xmm0,%xmm3
	por %xmm2,%xmm4
	movq %xmm4,16(%rdi,%r8,8)
	movhpd %xmm4,24(%rdi,%r8,8)
	add $4,%r8
	jnc loop
skiploop:
# have 3-r8 limbs left to do
cmp $2,%r8
ja left0
jz left1
jp left2
left3:
	movdqa 16(%rsi,%r8,8),%xmm2
	movdqa %xmm2,%xmm4
	psllq %xmm1,%xmm2
	shufpd $1,%xmm2,%xmm5
	por %xmm5,%xmm3
	movq %xmm3,(%rdi,%r8,8)
	movhpd %xmm3,8(%rdi,%r8,8)
	psrlq %xmm0,%xmm4
	movq 32(%rsi,%r8,8),%xmm5
	movq %xmm5,%xmm3
	psllq %xmm1,%xmm5
	shufpd $1,%xmm5,%xmm2
	psrlq %xmm0,%xmm3
	por %xmm2,%xmm4
	movq %xmm4,16(%rdi,%r8,8)
	movhpd %xmm4,24(%rdi,%r8,8)
	psrldq $8,%xmm5
	por %xmm5,%xmm3
	movq %xmm3,32(%rdi,%r8,8)
	ret
ALIGN(16)
left2:
	movdqa 16(%rsi,%r8,8),%xmm2
	movdqa %xmm2,%xmm4
	psllq %xmm1,%xmm2
	shufpd $1,%xmm2,%xmm5
	por %xmm5,%xmm3
	movq %xmm3,(%rdi,%r8,8)
	movhpd %xmm3,8(%rdi,%r8,8)
	psrlq %xmm0,%xmm4
	psrldq $8,%xmm2
	por %xmm2,%xmm4
	movq %xmm4,16(%rdi,%r8,8)
	movhpd %xmm4,24(%rdi,%r8,8)
	ret
ALIGN(16)
left1:
	movq 16(%rsi,%r8,8),%xmm2
	movq %xmm2,%xmm4
	psllq %xmm1,%xmm2
	shufpd $1,%xmm2,%xmm5
	por %xmm5,%xmm3
	movq %xmm3,(%rdi,%r8,8)
	movhpd %xmm3,8(%rdi,%r8,8)
	psrlq %xmm0,%xmm4
	psrldq $8,%xmm2
	por %xmm2,%xmm4
	movq %xmm4,16(%rdi,%r8,8)
	ret
ALIGN(16)
left0:
	psrldq $8,%xmm5
	por %xmm5,%xmm3
	movq %xmm3,(%rdi,%r8,8)
	movhpd %xmm3,8(%rdi,%r8,8)
	ret
EPILOGUE()
