dnl  mpn_mod_1_3

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

C	(rdi,2)= not fully reduced remainder of (rsi,rdx) / divisor , and top limb <d
C	where (rcx,4)  contains B^i % divisor

ASM_START()
PROLOGUE(mpn_mod_1_3)
C // require rdx >=5
push %r12
push %r13
push %r14
push %r15
mov -8(%rsi,%rdx,8),%r15
mov -16(%rsi,%rdx,8),%r14
mov -32(%rsi,%rdx,8),%rax
mov -40(%rsi,%rdx,8),%r12
mov (%rcx),%r8
mov 8(%rcx),%r9
mov 16(%rcx),%r10
mov 24(%rcx),%r11
mov %rdx,%rcx
sub $8,%rcx
jc skiplp
ALIGN(16)
C // r15 r14 -8() -16()=rax -24()=r12
lp:
	mul %r8
	add %rax,%r12
	mov 40(%rsi,%rcx,8),%rax
	mov $0,%r13
	adc %rdx,%r13
	mul %r9
	add %rax,%r12
	nop
	adc %rdx,%r13
	mov %r10,%rax
	mul %r14	
	add %rax,%r12
	adc %rdx,%r13
	mov %r12,%r14
	mov %r11,%rax
	mul %r15
	add %rax,%r14
	mov 0(%rsi,%rcx,8),%r12
	mov %r13,%r15
	mov 8(%rsi,%rcx,8),%rax
	adc %rdx,%r15
	sub $3,%rcx
	jnc lp
skiplp:
C // we have loaded up the next two limbs
C // but because they are out of order we can have to do 3 limbs min
cmp $-2,%rcx
jl case1
je case2
case3:
	C //two more limbs is 4 limbs
	C // r15 r14 40() 8+24()=rax 0+24()=r12
	mul %r8
	add %rax,%r12
	mov 40(%rsi,%rcx,8),%rax
	mov $0,%r13
	adc %rdx,%r13
	mul %r9
	add %rax,%r12
	nop
	adc %rdx,%r13
	mov %r10,%rax
	mul %r14	
	add %rax,%r12
	adc %rdx,%r13
	mov %r12,%r14
	mov %r11,%rax
	mul %r15
	add %rax,%r14
	mov 8(%rsi,%rcx,8),%r12
	mov %r13,%r15
	mov 16(%rsi,%rcx,8),%rax
	adc %rdx,%r15
	C // r15 r14 rax r12
	mov $0,%r13
	mul %r8
	add %rax,%r12
	adc %rdx,%r13
	mov %r9,%rax
	mul %r14
	add %rax,%r12
	adc %rdx,%r13
	mov %r10,%rax
	mul %r15
	add %rax,%r12
	adc %rdx,%r13
	C // r13 r12
	mov %r8,%rax
	mul %r13
	add %rax,%r12
	adc $0,%rdx
	mov %r12,(%rdi)
	mov %rdx,8(%rdi)
	pop %r15
	pop %r14
	pop %r13
	pop %r12
	ret
ALIGN(16)
case2:
	C //two more limbs is 4 limbs
	C // r15 r14 40() 8+24()=rax 0+24()=r12
	mul %r8
	add %rax,%r12
	mov 40(%rsi,%rcx,8),%rax
	mov $0,%r13
	adc %rdx,%r13
	mul %r9
	add %rax,%r12
	nop
	adc %rdx,%r13
	mov %r10,%rax
	mul %r14	
	add %rax,%r12
	adc %rdx,%r13
	mov %r12,%r14
	mov %r11,%rax
	mul %r15
	add %rax,%r14
	mov 16(%rsi,%rcx,8),%r12
	mov %r13,%r15
	adc %rdx,%r15
	C // r15 r14 r12
	mov $0,%r13
	mov %r8,%rax
	mul %r14
	add %rax,%r12
	adc %rdx,%r13
	mov %r9,%rax
	mul %r15
	add %rax,%r12
	adc %rdx,%r13
	C // r13 r12
	mov %r8,%rax
	mul %r13
	add %rax,%r12
	adc $0,%rdx
	mov %r12,(%rdi)
	mov %rdx,8(%rdi)
	pop %r15
	pop %r14
	pop %r13
	pop %r12
	ret
ALIGN(16)
case1:
	C // one more is 3 limbs
	C // r15 r14 40() 8+24()=rax 0+24()=r12 
	mul %r8
	add %rax,%r12
	mov 40(%rsi,%rcx,8),%rax
	mov $0,%r13
	adc %rdx,%r13
	mul %r9
	add %rax,%r12
	nop
	adc %rdx,%r13
	mov %r10,%rax
	mul %r14	
	add %rax,%r12
	adc %rdx,%r13
	mov %r12,%r14
	mov %r11,%rax
	mul %r15
	add %rax,%r14
	mov %r13,%r15
	adc %rdx,%r15
	mov %r8,%rax
	mul %r15
	add %rax,%r14
	adc $0,%rdx
	mov %r14,(%rdi)
	mov %rdx,8(%rdi)
	pop %r15
	pop %r14
	pop %r13
	pop %r12
	ret
EPILOGUE()
