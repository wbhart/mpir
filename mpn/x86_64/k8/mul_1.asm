dnl  mpn_mul_1

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
PROLOGUE(mpn_mul_1)
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
mov %r12,-8(%rsp)
.byte 0x26
jge skiplp
lp:	xor %r10,%r10
	mul %rcx
	mov %r8,(%rdi,%r11,8)
	add %rax,%r9
	adc %rdx,%r10
	mov 16(%rsi,%r11,8),%rax
	mul %rcx
	mov %r9,8(%rdi,%r11,8)
	add %rax,%r10
	mov $0,%r12d
	adc %rdx,%r12
	mov 24(%rsi,%r11,8),%rax
	xor %r8,%r8
	xor %r9,%r9
	mul %rcx
	mov %r10,16(%rdi,%r11,8)
	add %rax,%r12
	adc %rdx,%r8
	mov 32(%rsi,%r11,8),%rax
 	mul %rcx
	mov %r12,24(%rdi,%r11,8)
	add %rax,%r8
	adc %rdx,%r9
	add $4,%r11
	mov 8(%rsi,%r11,8),%rax
	jnc lp
skiplp:
xor %r10,%r10
mul %rcx
mov %r8,(%rdi,%r11,8)
add %rax,%r9
adc %rdx,%r10
cmp $2,%r11
ja case0
jz case1
jp case2
case3:	mov 16(%rsi),%rax
	mul %rcx
	mov %r9,8(%rdi)
	add %rax,%r10
	mov $0,%r12d
	adc %rdx,%r12
	mov 24(%rsi),%rax
	xor %r8,%r8
	xor %r9,%r9
	mul %rcx
	mov %r10,16(%rdi)
	add %rax,%r12
	adc %rdx,%r8
	mov 32(%rsi),%rax
 	mul %rcx
	mov %r12,24(%rdi)
	add %rax,%r8
	adc %rdx,%r9
	mov %r8,32(%rdi)
	#add $0,%r9
	mov -8(%rsp),%r12
	mov %r9,%rax
	ret
case2:	mov 24(%rsi),%rax
	mul %rcx
	mov %r9,16(%rdi)
	add %rax,%r10
	mov $0,%r12d
	adc %rdx,%r12
	mov 32(%rsi),%rax
	xor %r8,%r8
	#xor %r9,%r9
	mul %rcx
	mov %r10,24(%rdi)
	add %rax,%r12
	adc %rdx,%r8
	mov %r12,32(%rdi)
	#add $0,%r8
	mov -8(%rsp),%r12
	mov %r8,%rax
	ret
ALIGN(16)
case1:	mov 32(%rsi),%rax
	mul %rcx
	mov %r9,24(%rdi)
	add %rax,%r10
	mov $0,%r12d
	adc %rdx,%r12
	mov %r10,32(%rdi)
	#add $0,%r12
	mov %r12,%rax
	mov -8(%rsp),%r12
	ret
one:	mul %rcx
	mov %rax,(%rdi)
	#add $0,%rdx
	mov %rdx,%rax
	ret
case0:	mov %r9,32(%rdi)
	#add $0,%r10
	mov -8(%rsp),%r12
	mov %r10,%rax
	ret
EPILOGUE()
