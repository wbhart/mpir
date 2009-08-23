dnl  mpn_addlsh_n

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

C	ret mpn_addlsh_n(mp_ptr,mp_ptr,mp_ptr,mp_size_t,shift)
C	rax                 rdi,   rsi,   rdx,      rcx   r8

ASM_START()
PROLOGUE(mpn_addlsh_n)
lea -32(%rdi,%rcx,8),%rdi
lea -32(%rsi,%rcx,8),%rsi
lea -32(%rdx,%rcx,8),%rdx
push %r12
push %rbx
mov $4,%rbx
sub %rcx,%rbx
mov $64,%rcx
sub %r8,%rcx
mov $0,%r12
mov $0,%rax
mov (%rdx,%rbx,8),%r8
cmp $0,%rbx
jge skiplp
ALIGN(16)
lp:
	mov 8(%rdx,%rbx,8),%r9
	mov 16(%rdx,%rbx,8),%r10
	mov 24(%rdx,%rbx,8),%r11
	shrd %cl,%r8,%r12
	shrd %cl,%r9,%r8
	shrd %cl,%r10,%r9
	shrd %cl,%r11,%r10
	sahf
	adc (%rsi,%rbx,8),%r12
	mov %r12,(%rdi,%rbx,8)
	adc 8(%rsi,%rbx,8),%r8
	mov %r11,%r12
	mov %r8,8(%rdi,%rbx,8)
	adc 16(%rsi,%rbx,8),%r9
	adc 24(%rsi,%rbx,8),%r10
	mov %r10,24(%rdi,%rbx,8)
	mov %r9,16(%rdi,%rbx,8)
	lahf
	mov 32(%rdx,%rbx,8),%r8
	add $4,%rbx
	jnc lp
ALIGN(16)
skiplp:
cmp $2,%rbx
ja case0
je case1
jp case2
case3:
	shrd %cl,%r8,%r12
	mov 8(%rdx,%rbx,8),%r9
	mov 16(%rdx,%rbx,8),%r10
	mov 24(%rdx,%rbx,8),%r11
	shrd %cl,%r9,%r8
	shrd %cl,%r10,%r9
	shrd %cl,%r11,%r10
	sahf
	adc (%rsi,%rbx,8),%r12
	mov %r12,(%rdi,%rbx,8)
	adc 8(%rsi,%rbx,8),%r8
	mov %r11,%r12
	mov %r8,8(%rdi,%rbx,8)
	adc 16(%rsi,%rbx,8),%r9
	adc 24(%rsi,%rbx,8),%r10
	mov %r10,24(%rdi,%rbx,8)
	mov %r9,16(%rdi,%rbx,8)
	lahf
	shr %cl,%r12
	sahf
	adc $0,%r12
	mov %r12,%rax
	pop %rbx
	pop %r12
	ret
case2:
	shrd %cl,%r8,%r12
	mov 8(%rdx,%rbx,8),%r9
	shrd %cl,%r9,%r8
	mov 16(%rdx,%rbx,8),%r10
	shrd %cl,%r10,%r9
	shr %cl,%r10
	sahf
	adc (%rsi,%rbx,8),%r12
	mov %r12,(%rdi,%rbx,8)
	adc 8(%rsi,%rbx,8),%r8
	mov $0,%rax
	mov %r8,8(%rdi,%rbx,8)
	adc 16(%rsi,%rbx,8),%r9
	adc %r10,%rax
	mov %r9,16(%rdi,%rbx,8)
	pop %rbx
	pop %r12
	ret
case1:
	shrd %cl,%r8,%r12
	mov 8(%rdx,%rbx,8),%r9
	shrd %cl,%r9,%r8
	shr %cl,%r9
	sahf
	adc (%rsi,%rbx,8),%r12
	mov %r12,(%rdi,%rbx,8)
	adc 8(%rsi,%rbx,8),%r8
	mov $0,%rax
	mov %r8,8(%rdi,%rbx,8)
	adc %r9,%rax
	pop %rbx
	pop %r12
	ret
case0:
	shrd %cl,%r8,%r12
	shr %cl,%r8
	sahf
	adc (%rsi,%rbx,8),%r12
	mov %r12,(%rdi,%rbx,8)
	adc $0,%r8
	mov %r8,%rax
	pop %rbx
	pop %r12
	ret
EPILOGUE()
