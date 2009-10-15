dnl  mpn_sub_err1_n

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

C ret mpn_sub_err1(mp_ptr,mp_ptr,mp_ptr,mp_ptr,mp_ptr_t,mp_size_t,mp_limb_t)
C rax                 rdi,   rsi,   rdx,   rcx,      r8        r9    8(rsp)

ASM_START()
PROLOGUE(mpn_sub_err1_n)
C // if we rearrange the params we could save some moves
C // (rdi,r9)=(rsi,r9)+(rdx,r9)  sum=carry*(r8)
mov 8(%rsp),%r10
mov %rbp,-16(%rsp)
lea -24(%rdi,%r9,8),%rdi
mov %r12,-24(%rsp)
mov %r13,-32(%rsp)
lea -24(%rsi,%r9,8),%rsi
mov %r14,-40(%rsp)
mov %r15,-48(%rsp)
lea -24(%rdx,%r9,8),%rdx
mov %rcx,-56(%rsp)	
mov %rbx,-8(%rsp)
mov $3,%r11
shl $63,%r10
lea (%r8,%r9,8),%r8
sub %r9,%r11	
mov $0,%r9
mov $0,%rax
mov $0,%rbx
jnc skiplp
ALIGN(16)
lp:
	mov (%rsi,%r11,8),%r12
	mov 8(%rsi,%r11,8),%r13
	mov 16(%rsi,%r11,8),%r14
	mov 24(%rsi,%r11,8),%r15
	mov $0,%rbp
	shl $1,%r10
	sbb (%rdx,%r11,8),%r12
	cmovc -8(%r8),%rax
	sbb 8(%rdx,%r11,8),%r13
	cmovc -16(%r8),%rbx
	mov $0,%rcx
	sbb 16(%rdx,%r11,8),%r14
	cmovc -24(%r8),%rcx
	sbb 24(%rdx,%r11,8),%r15
	cmovc -32(%r8),%rbp
	rcr $1,%r10
	add %rax,%r9
	adc $0,%r10
	add %rbx,%r9
	adc $0,%r10
	add %rcx,%r9
	mov $0,%rax
	adc $0,%r10
	lea -32(%r8),%r8
	add %rbp,%r9
	adc $0,%r10
	mov %r12,(%rdi,%r11,8)
	mov %r13,8(%rdi,%r11,8)
	mov %r14,16(%rdi,%r11,8)
	mov %r15,24(%rdi,%r11,8)
	mov $0,%rbx
	add $4,%r11
	jnc  lp
skiplp:
cmp $2,%r11
mov -16(%rsp),%rbp
mov -48(%rsp),%r15
ja case0
je case1
jp case2
case3:
	mov (%rsi,%r11,8),%r12
	mov 8(%rsi,%r11,8),%r13
	mov 16(%rsi,%r11,8),%r14
	shl $1,%r10
	sbb (%rdx,%r11,8),%r12
	cmovc -8(%r8),%rax
	sbb 8(%rdx,%r11,8),%r13
	cmovc -16(%r8),%rbx
	mov $0,%rcx
	sbb 16(%rdx,%r11,8),%r14
	cmovc -24(%r8),%rcx
	rcr $1,%r10
	add %rax,%r9
	adc $0,%r10
	add %rbx,%r9
	adc $0,%r10
	add %rcx,%r9
	adc $0,%r10
	mov %r12,(%rdi,%r11,8)
	mov %r13,8(%rdi,%r11,8)
	mov %r14,16(%rdi,%r11,8)
	mov -56(%rsp),%rcx
	mov %r9,(%rcx)
	btr $63,%r10
	mov %r10,8(%rcx)
	mov -40(%rsp),%r14
	mov $0,%rax
	mov -32(%rsp),%r13
	adc $0,%rax
	mov -24(%rsp),%r12
	mov -8(%rsp),%rbx
	ret
ALIGN(16)
case2:
	mov (%rsi,%r11,8),%r12
	mov 8(%rsi,%r11,8),%r13
	shl $1,%r10
	sbb (%rdx,%r11,8),%r12
	cmovc -8(%r8),%rax
	sbb 8(%rdx,%r11,8),%r13
	cmovc -16(%r8),%rbx
	rcr $1,%r10
	add %rax,%r9
	adc $0,%r10
	add %rbx,%r9
	adc $0,%r10
	mov %r12,(%rdi,%r11,8)
	mov %r13,8(%rdi,%r11,8)
	mov -56(%rsp),%rcx
	mov %r9,(%rcx)
	btr $63,%r10
	mov %r10,8(%rcx)
	mov -40(%rsp),%r14
	mov $0,%rax
	mov -32(%rsp),%r13
	adc $0,%rax
	mov -24(%rsp),%r12
	mov -8(%rsp),%rbx
	ret
ALIGN(16)
case1:
	mov (%rsi,%r11,8),%r12
	shl $1,%r10
	sbb (%rdx,%r11,8),%r12
	cmovc -8(%r8),%rax
	rcr $1,%r10
	add %rax,%r9
	adc $0,%r10
	mov %r12,(%rdi,%r11,8)
case0:	mov -56(%rsp),%rcx
	mov %r9,(%rcx)
	btr $63,%r10
	mov %r10,8(%rcx)
	mov -40(%rsp),%r14
	mov $0,%rax
	mov -32(%rsp),%r13
	adc $0,%rax
	mov -24(%rsp),%r12
	mov -8(%rsp),%rbx
	ret
EPILOGUE()
