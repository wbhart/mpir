dnl  mpn_mod_1_2

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

C	(rdi,2)= not fully reduced remainder of (rsi,rdx) / divisor , and top limb <d
C	where (rcx,3)  contains B^i % divisor


ASM_START()
PROLOGUE(mpn_mod_1_2)
C // require rdx >=4
push %r12
push %r13
push %r14
mov -8(%rsi,%rdx,8),%r14
mov -16(%rsi,%rdx,8),%r13
mov (%rcx),%r8
mov 8(%rcx),%r9
mov 16(%rcx),%r10
mov %rdx,%rcx
sub $6,%rcx
jc skiplp
ALIGN(16)
lp:
	mov 16(%rsi,%rcx,8),%r11
	mov 16+8(%rsi,%rcx,8),%rax
	mul %r8
	add %rax,%r11
	mov $0,%r12d
	adc %rdx,%r12
	lea (%r9),%rax
	mul %r13
	add %rax,%r11
	lea (%r8),%r8
	adc %rdx,%r12
	mov %r10,%rax
	lea (%r11),%r13
	mul %r14
	add %rax,%r13
	lea (%r12),%r14
	adc %rdx,%r14
	sub $2,%rcx
	jnc lp
skiplp:	#// Dont need this extra wind down code now as we are not pipelined anymore
	mov 16(%rsi,%rcx,8),%r11
	mov 16+8(%rsi,%rcx,8),%rax
	mul %r8
	mov $0,%r12d
	add %rax,%r11
	adc %rdx,%r12
	mov %r9,%rax
	mul %r13
	add %rax,%r11
	adc %rdx,%r12
	mov %r11,%r13
	mov %r10,%rax
	mul %r14
	add %rax,%r13
	mov %r12,%r14
	adc %rdx,%r14
cmp $-2,%rcx
je case0
case1:
	mov 8(%rsi,%rcx,8),%r11
	mov $0,%r12d
	mov %r8,%rax
	mul %r13
	add %rax,%r11
	adc %rdx,%r12
	mov %r11,%r13
	mov %r9,%rax
	mul %r14
	add %rax,%r13
	mov %r12,%r14
	adc %rdx,%r14
case0:	
mov %r8,%rax
mul %r14
add %rax,%r13
adc $0,%rdx
mov %r13,(%rdi)
mov %rdx,8(%rdi)
pop %r14
pop %r13
pop %r12
ret
EPILOGUE()
