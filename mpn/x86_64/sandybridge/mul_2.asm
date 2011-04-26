dnl  mpn_mul_2

dnl  Copyright 2010 The Code Cavern

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
PROLOGUE(mpn_mul_2)
push %rbx
mov 8(%rcx),%r8
mov (%rcx),%rcx
mov $3,%rbx
lea -24(%rdi,%rdx,8),%rdi
lea -24(%rsi,%rdx,8),%rsi
sub %rdx,%rbx
	#mul %r8
	#add %rax,%r9
	#adc %rdx,%r10
	#mov -16(%rsi,%rbx,8),%rax
		mov $0,%r11
	#mul %rcx
	#add %rax,%r9
	#adc %rdx,%r10
	#adc $0,%r11
	#mov -16(%rsi,%rbx,8),%rax
	#mul %r8
	#add %rax,%r10
	#mov -8(%rsi,%rbx,8),%rax
	#adc %rdx,%r11
	#mul %rcx
	#add %rax,%r10
	#mov %r9,-16(%rdi,%rbx,8)
	#adc %rdx,%r11
		mov $0,%r9
	#mov -8(%rsi,%rbx,8),%rax
	#adc $0,%r9
	#mul %r8
	#add %rax,%r11
	#mov %r10,-8(%rdi,%rbx,8)
	mov (%rsi,%rbx,8),%rax
	mov $0,%r10
	#adc %rdx,%r9
	mul %rcx
	add %rax,%r11
	mov (%rsi,%rbx,8),%rax
	mov %r11,(%rdi,%rbx,8)
	adc %rdx,%r9
	#adc $0,%r10
	#add $3,%rbx
cmp $0,%rbx
jge skiplp
ALIGN(16)
lp:	mul %r8
	add %rax,%r9
	adc %rdx,%r10
	mov 8(%rsi,%rbx,8),%rax
	mov $0,%r11
	mul %rcx
	add %rax,%r9
	adc %rdx,%r10
	adc $0,%r11
	mov 8(%rsi,%rbx,8),%rax
	mul %r8
	add %rax,%r10
	mov 16(%rsi,%rbx,8),%rax
	adc %rdx,%r11
	mul %rcx
	add %rax,%r10
	mov %r9,8(%rdi,%rbx,8)
	adc %rdx,%r11
	mov $0,%r9
	mov 16(%rsi,%rbx,8),%rax
	adc $0,%r9
	mul %r8
	add %rax,%r11
	mov %r10,16(%rdi,%rbx,8)
	mov 24(%rsi,%rbx,8),%rax
	mov $0,%r10
	adc %rdx,%r9
	mul %rcx
	add %rax,%r11
	mov 24(%rsi,%rbx,8),%rax
	mov %r11,24(%rdi,%rbx,8)
	adc %rdx,%r9
	adc $0,%r10
	add $3,%rbx
	jnc lp
skiplp:
cmp $1,%rbx
ja case0
je case1
case2:	mul %r8
	add %rax,%r9
	adc %rdx,%r10
	mov 8(%rsi,%rbx,8),%rax
	mov $0,%r11
	mul %rcx
	add %rax,%r9
	adc %rdx,%r10
	adc $0,%r11
	mov 8(%rsi,%rbx,8),%rax
	mul %r8
	add %rax,%r10
	mov 16(%rsi,%rbx,8),%rax
	adc %rdx,%r11
	mul %rcx
	add %rax,%r10
	mov %r9,8(%rdi,%rbx,8)
	adc %rdx,%r11
	mov $0,%r9
	mov 16(%rsi,%rbx,8),%rax
	adc $0,%r9
	mul %r8
	add %rax,%r11
	mov %r10,16(%rdi,%rbx,8)
	adc %rdx,%r9
	mov %r11,24(%rdi,%rbx,8)
	mov %r9,%rax
	pop %rbx
	ret
case1:	mul %r8
	add %rax,%r9
	adc %rdx,%r10
	mov 8(%rsi,%rbx,8),%rax
	mov $0,%r11
	mul %rcx
	add %rax,%r9
	adc %rdx,%r10
	adc $0,%r11
	mov 8(%rsi,%rbx,8),%rax
	mul %r8
	add %rax,%r10
	adc %rdx,%r11
	mov %r9,8(%rdi,%rbx,8)
	mov %r10,16(%rdi,%rbx,8)
	mov %r11,%rax
	pop %rbx
	ret
case0:	mul %r8
	add %rax,%r9
	adc %rdx,%r10
	mov %r9,8(%rdi,%rbx,8)
	mov %r10,%rax
	pop %rbx
	ret
EPILOGUE()



