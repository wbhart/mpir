dnl  mpn_karasub

dnl  Copyright 2011 The Code Cavern

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

ASM_START()
PROLOGUE(mpn_karasub)
push %rbx
push %rbp
push %r12
push %r13
push %r14
push %r15
#rp is rdi
#tp is rsi
#n is rdx and put it on the stack
push %rdx
shr $1,%rdx
#n2 is rdx
lea (%rdx,%rdx,1),%rcx
# 2*n2 is rcx
# L is rdi
# H is rbp
# tp is rsi
lea (%rdi,%rcx,8),%rbp
xor %rax,%rax
xor %rbx,%rbx
# rax rbx are the carrys
lea -24(%rdi,%rdx,8),%rdi
lea -24(%rsi,%rdx,8),%rsi
lea -24(%rbp,%rdx,8),%rbp
mov $3,%rcx
neg %rdx
add $3,%rdx
# assume n>8 or some such
.align 16
lp:	bt $2,%rbx
	mov (%rdi,%rcx,8),%r8
	adc (%rbp,%rdx,8),%r8
	mov %r8,%r12
	mov 8(%rdi,%rcx,8),%r9
	adc 8(%rbp,%rdx,8),%r9
	mov 16(%rdi,%rcx,8),%r10
	adc 16(%rbp,%rdx,8),%r10
	mov 24(%rdi,%rcx,8),%r11
	adc 24(%rbp,%rdx,8),%r11
	rcl $1,%rbx
	bt $1,%rax
	mov %r11,%r15
	adc (%rdi,%rdx,8),%r8
	mov %r9,%r13
	adc 8(%rdi,%rdx,8),%r9
	mov %r10,%r14
	adc 16(%rdi,%rdx,8),%r10
	adc 24(%rdi,%rdx,8),%r11
	rcl $1,%rax
	bt $2,%rbx
	adc (%rbp,%rcx,8),%r12
	adc 8(%rbp,%rcx,8),%r13
	adc 16(%rbp,%rcx,8),%r14
	adc 24(%rbp,%rcx,8),%r15
	rcl $1,%rbx
	bt $1,%rax
	sbb (%rsi,%rdx,8),%r8
	sbb 8(%rsi,%rdx,8),%r9
	sbb 16(%rsi,%rdx,8),%r10
	sbb 24(%rsi,%rdx,8),%r11
	mov %r10,16(%rdi,%rcx,8)
	mov %r11,24(%rdi,%rcx,8)
	rcl $1,%rax
	bt $2,%rbx
	mov %r8,(%rdi,%rcx,8)
	mov %r9,8(%rdi,%rcx,8)
	sbb (%rsi,%rcx,8),%r12
	sbb 8(%rsi,%rcx,8),%r13
	sbb 16(%rsi,%rcx,8),%r14
	sbb 24(%rsi,%rcx,8),%r15
	rcl $1,%rbx
	add $4,%rcx
	mov %r12,(%rbp,%rdx,8)
	mov %r13,8(%rbp,%rdx,8)
	mov %r14,16(%rbp,%rdx,8)
	mov %r15,24(%rbp,%rdx,8)
	add $4,%rdx
	jnc lp
cmp $2,%rdx
jg	case0
jz	case1
jp	case2
case3:	bt $2,%rbx
	mov (%rdi,%rcx,8),%r8
	adc (%rbp,%rdx,8),%r8
	mov %r8,%r12
	mov 8(%rdi,%rcx,8),%r9
	adc 8(%rbp,%rdx,8),%r9
	mov 16(%rdi,%rcx,8),%r10
	adc 16(%rbp,%rdx,8),%r10
	rcl $1,%rbx
	bt $1,%rax
	adc (%rdi,%rdx,8),%r8
	mov %r9,%r13
	adc 8(%rdi,%rdx,8),%r9
	mov %r10,%r14
	adc 16(%rdi,%rdx,8),%r10
	rcl $1,%rax
	bt $2,%rbx
	adc (%rbp,%rcx,8),%r12
	adc 8(%rbp,%rcx,8),%r13
	adc 16(%rbp,%rcx,8),%r14
	rcl $1,%rbx
	bt $1,%rax
	sbb (%rsi,%rdx,8),%r8
	sbb 8(%rsi,%rdx,8),%r9
	sbb 16(%rsi,%rdx,8),%r10
	mov %r10,16(%rdi,%rcx,8)
	rcl $1,%rax
	bt $2,%rbx
	mov %r8,(%rdi,%rcx,8)
	mov %r9,8(%rdi,%rcx,8)
	sbb (%rsi,%rcx,8),%r12
	sbb 8(%rsi,%rcx,8),%r13
	sbb 16(%rsi,%rcx,8),%r14
	rcl $1,%rbx
	add $3,%rcx
	mov %r12,(%rbp,%rdx,8)
	mov %r13,8(%rbp,%rdx,8)
	mov %r14,16(%rbp,%rdx,8)
	add $3,%rdx
	jmp fin
case2:	bt $2,%rbx
	mov (%rdi,%rcx,8),%r8
	adc (%rbp,%rdx,8),%r8
	mov %r8,%r12
	mov 8(%rdi,%rcx,8),%r9
	adc 8(%rbp,%rdx,8),%r9
	rcl $1,%rbx
	bt $1,%rax
	adc (%rdi,%rdx,8),%r8
	mov %r9,%r13
	adc 8(%rdi,%rdx,8),%r9
	rcl $1,%rax
	bt $2,%rbx
	adc (%rbp,%rcx,8),%r12
	adc 8(%rbp,%rcx,8),%r13
	rcl $1,%rbx
	bt $1,%rax
	sbb (%rsi,%rdx,8),%r8
	sbb 8(%rsi,%rdx,8),%r9
	rcl $1,%rax
	bt $2,%rbx
	mov %r8,(%rdi,%rcx,8)
	mov %r9,8(%rdi,%rcx,8)
	sbb (%rsi,%rcx,8),%r12
	sbb 8(%rsi,%rcx,8),%r13
	rcl $1,%rbx
	add $2,%rcx
	mov %r12,(%rbp,%rdx,8)
	mov %r13,8(%rbp,%rdx,8)
	add $2,%rdx
	jmp fin
case1:	bt $2,%rbx
	mov (%rdi,%rcx,8),%r8
	adc (%rbp,%rdx,8),%r8
	mov %r8,%r12
	rcl $1,%rbx
	bt $1,%rax
	adc (%rdi,%rdx,8),%r8
	rcl $1,%rax
	bt $2,%rbx
	adc (%rbp,%rcx,8),%r12
	rcl $1,%rbx
	bt $1,%rax
	sbb (%rsi,%rdx,8),%r8
	rcl $1,%rax
	bt $2,%rbx
	mov %r8,(%rdi,%rcx,8)
	sbb (%rsi,%rcx,8),%r12
	rcl $1,%rbx
	inc %rcx
	mov %r12,(%rbp,%rdx,8)
	inc %rdx
case0:
fin:	# if odd the do next two
	pop %r8
	bt $0,%r8
	jnc notodd
	xor %r10,%r10
	mov (%rbp,%rcx,8),%r8
	mov 8(%rbp,%rcx,8),%r9
	sub (%rsi,%rcx,8),%r8
	sbb 8(%rsi,%rcx,8),%r9
	rcl $1,%r10
	add %r8,(%rbp,%rdx,8)
	adc %r9,8(%rbp,%rdx,8)
	mov %rdx,%rsi
l7:	adcq $0,16(%rbp,%rdx,8)
	inc %rdx
	jc l7
	mov %rsi,%rdx
	bt $0,%r10
l8:	sbbq $0,16(%rbp,%rdx,8)
	inc %rdx
	jc l8
	mov %rsi,%rdx
	# add in all carrys
	# should we do the borrows last as it may be possible to underflow
	# could use popcount
notodd:	mov %rcx,%rsi
	bt $0,%rax
l1:	sbbq $0,(%rdi,%rcx,8)
	inc %rcx
	jc l1
	mov %rsi,%rcx
	bt $1,%rax
l2:	adcq $0,(%rdi,%rcx,8)
	inc %rcx
	jc l2
	mov %rsi,%rcx
	bt $2,%rbx
l3:	adcq $0,(%rdi,%rcx,8)
	inc %rcx
	jc l3
	mov %rdx,%rsi
	bt $0,%rbx
l4:	sbbq $0,(%rbp,%rdx,8)
	inc %rdx
	jc l4
	mov %rsi,%rdx
	bt $1,%rbx
l5:	adcq $0,(%rbp,%rdx,8)
	inc %rdx
	jc l5
	mov %rsi,%rdx
	bt $2,%rbx
l6:	adcq $0,(%rbp,%rdx,8)
	inc %rdx
	jc l6

pop %r15
pop %r14
pop %r13
pop %r12
pop %rbp
pop %rbx
ret
EPILOGUE()
