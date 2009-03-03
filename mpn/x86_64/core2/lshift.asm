dnl  core2 mpn_lshift

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

C	(rdi,rdx)=(rsi,rdx)<<rcx
C	rax=carry

C	decent assmeblers understand what movq means ,except
C	microsofts/apple masm (what a suprise there) so for the broken old masm
C	assembler.  Needed for movq reg64,mediareg and movq mediareg,reg64
C	only , where mediareg is xmm or mm
define(`MOVQ',`movd')

ASM_START()
PROLOGUE(mpn_lshift)
cmp $2,%rdx
ja threeormore
jz two
one:
	mov (%rsi),%rdx
	mov %rdx,%rax
	shl %cl,%rdx
	neg %rcx
	shr %cl,%rax
	mov %rdx,(%rdi)
	ret
two:
	mov (%rsi),%r8
	mov 8(%rsi),%r9
	mov %r8,%r11
	mov %r9,%rax
	shl %cl,%r8
	shl %cl,%r9
	neg %rcx
	shr %cl,%r11
	shr %cl,%rax
	or  %r11,%r9
	mov %r8,(%rdi)
	mov %r9,8(%rdi)
	ret
threeormore:
mov $64,%eax
sub %rcx,%rax
MOVQ %rcx,%xmm0
MOVQ %rax,%xmm1
mov %rdx,%r8
lea -16(%rsi,%r8,8),%r9
mov %r9,%r10
and $-16,%r9
movdqa (%r9),%xmm3
movdqa %xmm3,%xmm5
psrlq %xmm1,%xmm3
pshufd $0x4E,%xmm3,%xmm3
MOVQ %xmm3,%rax
cmp %r9,%r10
je aligned 
	movq -8(%rsi,%r8,8),%xmm2
	movq %xmm2,%xmm4
	psrlq %xmm1,%xmm2
	MOVQ %xmm2,%rax
	psllq %xmm0,%xmm4
	por %xmm3,%xmm4
	movq %xmm4,-8(%rdi,%r8,8)
	sub $1,%r8
aligned:
sub $5,%r8
jle skiploop
ALIGN(16)
loop:
	movdqa 8(%rsi,%r8,8),%xmm2
	movdqa %xmm2,%xmm4
	psllq %xmm0,%xmm5
	psrlq %xmm1,%xmm2
	movhlps %xmm2,%xmm3
	por %xmm3,%xmm5
	movq %xmm5,24(%rdi,%r8,8)
	pshufd $0x4E,%xmm2,%xmm2
	movhpd %xmm5,32(%rdi,%r8,8)
	movdqa -8(%rsi,%r8,8),%xmm3
	movdqa %xmm3,%xmm5
	psrlq %xmm1,%xmm3
	movhlps %xmm3,%xmm2
	psllq %xmm0,%xmm4
	pshufd $0x4E,%xmm3,%xmm3
	por %xmm2,%xmm4
	movq %xmm4,8(%rdi,%r8,8)
	movhpd %xmm4,16(%rdi,%r8,8)
	sub $4,%r8
	jg loop
skiploop:
cmp $-1,%r8
je left2
jg left3
jp left1
left0:
C may be easier to bswap xmm5 first , same with other cases
	pxor %xmm2,%xmm2 
	psllq %xmm0,%xmm5
	movhlps %xmm2,%xmm3
	por %xmm3,%xmm5
	movq %xmm5,24(%rdi,%r8,8)
	movhpd %xmm5,32(%rdi,%r8,8)
	ret
ALIGN(16)
left3:
	movdqa 8(%rsi,%r8,8),%xmm2
	movdqa %xmm2,%xmm4
	psllq %xmm0,%xmm5
	psrlq %xmm1,%xmm2
	movhlps %xmm2,%xmm3
	por %xmm3,%xmm5
	movq %xmm5,24(%rdi,%r8,8)
	pshufd $0x4E,%xmm2,%xmm2
	movhpd %xmm5,32(%rdi,%r8,8)
	movq (%rsi,%r8,8),%xmm3
	pshufd $0x4E,%xmm3,%xmm3
	movdqa %xmm3,%xmm5
	psrlq %xmm1,%xmm3
	movhlps %xmm3,%xmm2
	psllq %xmm0,%xmm4
	por %xmm2,%xmm4
	movq %xmm4,8(%rdi,%r8,8)
	movhpd %xmm4,16(%rdi,%r8,8)
	psllq %xmm0,%xmm5
	movhpd %xmm5,(%rdi,%r8,8)
	ret
ALIGN(16)
left2:
	movdqa 8(%rsi,%r8,8),%xmm2
	movdqa %xmm2,%xmm4
	psllq %xmm0,%xmm5
	psrlq %xmm1,%xmm2
	movhlps %xmm2,%xmm3
	por %xmm3,%xmm5
	movq %xmm5,24(%rdi,%r8,8)
	pshufd $0x4E,%xmm2,%xmm2
	movhpd %xmm5,32(%rdi,%r8,8)
	pxor %xmm3,%xmm3
	movhlps %xmm3,%xmm2
	psllq %xmm0,%xmm4
	por %xmm2,%xmm4
	movq %xmm4,8(%rdi,%r8,8)
	movhpd %xmm4,16(%rdi,%r8,8)
	ret
ALIGN(16)
left1:
	movq 16(%rsi,%r8,8),%xmm2
	pshufd $0x4E,%xmm2,%xmm2
	movdqa %xmm2,%xmm4
	psllq %xmm0,%xmm5
	psrlq %xmm1,%xmm2
	movhlps %xmm2,%xmm3
	por %xmm3,%xmm5
	movq %xmm5,24(%rdi,%r8,8)
	movhpd %xmm5,32(%rdi,%r8,8)
	psllq %xmm0,%xmm4
	movhpd %xmm4,16(%rdi,%r8,8)
	ret
EPILOGUE()
