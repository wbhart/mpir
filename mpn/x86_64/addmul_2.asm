dnl  k8 mpn_addmul_2

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

ASM_START()
PROLOGUE(mpn_addmul_2)
#       (rdi,rdx+1) = (rdi,rdx) + (rsi,rdx)*(rcx,2) return carrylimb
push %rbx
push %r12
mov 8(%rcx),%r8
mov (%rcx),%rcx
mov $4,%rbx
sub %rdx,%rbx
lea -32(%rsi,%rdx,8),%rsi
lea -32(%rdi,%rdx,8),%rdi
mov $0,%r10
mov (%rsi,%rbx,8),%rax
mul %rcx
mov %rax,%r12
mov %rdx,%r9
cmp $0,%rbx
jge skiplp
ALIGN(16)
lp:
	mov (%rsi,%rbx,8),%rax
	mul %r8
	add %rax,%r9
	mov 8(%rsi,%rbx,8),%rax
	adc %rdx,%r10
	mov $0,%r11
	mul %rcx
	add %r12,(%rdi,%rbx,8)
	adc %rax,%r9
	mov $0,%r12
	adc %rdx,%r10
	mov 8(%rsi,%rbx,8),%rax
	adc $0,%r11
	mul %r8
	add %r9,8(%rdi,%rbx,8)
	adc %rax,%r10
	adc %rdx,%r11
	mov 16(%rsi,%rbx,8),%rax
	mul %rcx
	add %rax,%r10
	mov 16(%rsi,%rbx,8),%rax
	adc %rdx,%r11
	adc $0,%r12
	mul %r8
	add %r10,16(%rdi,%rbx,8)
	mov $0,%r9
	adc %rax,%r11
	mov $0,%r10
	mov 24(%rsi,%rbx,8),%rax
	adc %rdx,%r12
	mov %r15,%r15
	mul %rcx
	add %rax,%r11
	mov 24(%rsi,%rbx,8),%rax
	adc %rdx,%r12
	adc $0,%r9
	mul %r8
	add %r11,24(%rdi,%rbx,8)
	adc %rax,%r12
	adc %rdx,%r9
	mov 32(%rsi,%rbx,8),%rax
	mul %rcx
	add %rax,%r12
	adc %rdx,%r9
	adc $0,%r10
	add $4,%rbx
	jnc lp
skiplp:
mov (%rsi,%rbx,8),%rax
mul %r8
cmp $2,%rbx
ja case0
jz case1
jp case2
# case 3 3 more src so 24(rsi) is last
#ALIGN(16)
case3:
	add %rax,%r9
	mov 8(%rsi,%rbx,8),%rax
	adc %rdx,%r10
	mov $0,%r11
	mul %rcx
	add %r12,(%rdi,%rbx,8)
	adc %rax,%r9
	mov $0,%r12
	adc %rdx,%r10
	mov 8(%rsi,%rbx,8),%rax
	adc $0,%r11
	mul %r8
	add %r9,8(%rdi,%rbx,8)
	adc %rax,%r10
	adc %rdx,%r11
	mov 16(%rsi,%rbx,8),%rax
	mul %rcx
	add %rax,%r10
	mov 16(%rsi,%rbx,8),%rax
	adc %rdx,%r11
	adc $0,%r12
	mul %r8
	add %r10,16(%rdi,%rbx,8)
	mov $0,%r9
	adc %rax,%r11
	#padding
	mov $0,%r10
	mov 24(%rsi,%rbx,8),%rax
	adc %rdx,%r12
	#padding
	mov %r15,%r15
	mul %rcx
	add %rax,%r11
	mov 24(%rsi,%rbx,8),%rax
	adc %rdx,%r12
	adc $0,%r9
	mul %r8
	add %r11,24(%rdi,%rbx,8)
	adc %rax,%r12
	adc %rdx,%r9
	mov %r12,32(%rdi,%rbx,8)
	mov %r9,%rax
	pop %r12
	pop %rbx
	ret
# case 2 2 more src so 16(rsi) is last
ALIGN(16)
case2:
	add %rax,%r9
	mov 8(%rsi,%rbx,8),%rax
	adc %rdx,%r10
	mov $0,%r11
	mul %rcx
	add %r12,(%rdi,%rbx,8)
	adc %rax,%r9
	mov $0,%r12
	adc %rdx,%r10
	mov 8(%rsi,%rbx,8),%rax
	adc $0,%r11
	mul %r8
	add %r9,8(%rdi,%rbx,8)
	adc %rax,%r10
	adc %rdx,%r11
	mov 16(%rsi,%rbx,8),%rax
	mul %rcx
	add %rax,%r10
	mov 16(%rsi,%rbx,8),%rax
	adc %rdx,%r11
	adc $0,%r12
	mul %r8
	add %r10,16(%rdi,%rbx,8)
	adc %rax,%r11
	adc %rdx,%r12
	mov %r11,24(%rdi,%rbx,8)
	mov %r12,%rax
	pop %r12
	pop %rbx
	ret
# case 1 1 more src so 8(rsi) is last
ALIGN(16)
case1:
	add %rax,%r9
	mov 8(%rsi,%rbx,8),%rax
	adc %rdx,%r10
	mov $0,%r11
	mul %rcx
	add %r12,(%rdi,%rbx,8)
	adc %rax,%r9
	mov $0,%r12
	adc %rdx,%r10
	mov 8(%rsi,%rbx,8),%rax
	adc $0,%r11
	mul %r8
	add %r9,8(%rdi,%rbx,8)
	adc %rax,%r10
	adc %rdx,%r11
	mov %r10,16(%rdi,%rbx,8)
	mov %r11,%rax
	pop %r12
	pop %rbx
	ret
# case 0 no more src so (rsi) is last
ALIGN(16)
case0:
	add %r12,(%rdi,%rbx,8)
	adc %rax,%r9
	adc %rdx,%r10
	mov %r9,8(%rdi,%rbx,8)
	mov %r10,%rax
	pop %r12
	pop %rbx
	ret
EPILOGUE()
