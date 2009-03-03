dnl  AMD64 mpn_mul_1

dnl  Copyright 2008,2009 Jason Moxham

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

C	(rdi,rdx)=(rsi,rdx)*rcx
C	rax=carry

ASM_START()
PROLOGUE(mpn_mul_1)
C this is just an addmul , so we can get rid off stack use
C and simplifiy wind down , and perhaps re-do the OOO order 
mov (%rsi),%rax
cmp $1,%rdx
je one		
mov $5,%r11
lea -40(%rsi,%rdx,8),%rsi
lea -40(%rdi,%rdx,8),%rdi
sub %rdx,%r11
mul %rcx
.byte 0x26
mov %rax,%r8
.byte 0x26
mov 8(%rsi,%r11,8),%rax
.byte 0x26
mov %rdx,%r9
.byte 0x26
cmp $0,%r11
.byte 0x26
mov %rbx,-8(%rsp)
.byte 0x26
jge skiploop
ALIGN(16)
loop:
	mov $0,%r10
	mul %rcx
	mov %r8,(%rdi,%r11,8)
	add %rax,%r9
	.byte 0x26
	adc %rdx,%r10
	mov 16(%rsi,%r11,8),%rax
	mul %rcx
	mov %r9,8(%rdi,%r11,8)
	add %rax,%r10
	mov $0,%ebx
	adc %rdx,%rbx
	mov 24(%rsi,%r11,8),%rax
	mov $0,%r8
	mov $0,%r9
	mul %rcx
	mov %r10,16(%rdi,%r11,8)
	.byte 0x26
	add %rax,%rbx
	.byte 0x26
	adc %rdx,%r8
	mov 32(%rsi,%r11,8),%rax
 	mul %rcx
	mov %rbx,24(%rdi,%r11,8)
	.byte 0x26
	add %rax,%r8
	.byte 0x26
	adc %rdx,%r9
	add $4,%r11
	mov 8(%rsi,%r11,8),%rax
	jnc loop
ALIGN(16)
skiploop:
mov $0,%r10d
mul %rcx
mov %r8,(%rdi,%r11,8)
add %rax,%r9
adc %rdx,%r10
cmp $2,%r11
jz next2
ja next3
jp next1
next0:
	mov 16(%rsi,%r11,8),%rax
	mul %rcx
	mov %r9,8(%rdi,%r11,8)
	add %rax,%r10
	mov $0,%ebx
	adc %rdx,%rbx
	mov 24(%rsi,%r11,8),%rax
	mov $0,%r8d
	mul %rcx
	mov %r10,16(%rdi,%r11,8)
	add %rax,%rbx
	adc %rdx,%r8
	mov 32(%rsi,%r11,8),%rax
	mul %rcx
	mov %rbx,24(%rdi,%r11,8)
	mov -8(%rsp),%rbx
	add %rax,%r8
	adc $0,%rdx
	mov %r8,32(%rdi,%r11,8)
	mov %rdx,%rax
	ret
ALIGN(16)
next1:
	mov 16(%rsi,%r11,8),%rax
	mul %rcx
	mov %r9,8(%rdi,%r11,8)
	add %rax,%r10
	mov $0,%r8d
	adc %rdx,%r8
	mov 24(%rsi,%r11,8),%rax
	mul %rcx
	mov %r10,16(%rdi,%r11,8)
	add %rax,%r8
	adc $0,%rdx
	mov %r8,24(%rdi,%r11,8)
	mov -8(%rsp),%rbx
	mov %rdx,%rax
	ret
ALIGN(16)
one:
	mul %rcx
	mov %rax,(%rdi)
	mov %rdx,%rax
	ret
ALIGN(16)
next2:
	mov 16(%rsi,%r11,8),%rax
	mul %rcx
	mov %r9,8(%rdi,%r11,8)
	add %rax,%r10
	mov $0,%ebx
	adc %rdx,%rbx
	mov %r10,16(%rdi,%r11,8)
	mov %rbx,%rax
	mov -8(%rsp),%rbx
	ret
ALIGN(16)
next3:
	mov -8(%rsp),%rbx
	mov %r9,8(%rdi,%r11,8)
	mov %r10,%rax
	ret
EPILOGUE()
