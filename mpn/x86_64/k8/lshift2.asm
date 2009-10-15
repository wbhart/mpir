dnl  mpn_lshift2

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

C	ret mpn_lshidft2(mp_ptr,mp_ptr,mp_size_t)
C	rax                 rdi,   rsi,      rdx

ASM_START()
PROLOGUE(mpn_lshift2)
mov $3,%ecx
sub %rdx,%rcx
lea -24(%rsi,%rdx,8),%rsi
lea -24(%rdi,%rdx,8),%rdi
xor %eax,%eax
xor %edx,%edx
cmp $0,%rcx
jge skiplp
ALIGN(16)
lp:
	mov (%rsi,%rcx,8),%r8
	mov 8(%rsi,%rcx,8),%r9
	mov 16(%rsi,%rcx,8),%r10
	mov 24(%rsi,%rcx,8),%r11
	add %rax,%rax
	adc %r8,%r8
	adc %r9,%r9
	adc %r10,%r10
	adc %r11,%r11
	sbb %rax,%rax
	add %rdx,%rdx
	adc %r8,%r8
	adc %r9,%r9
	adc %r10,%r10
	adc %r11,%r11
	mov %r11,24(%rdi,%rcx,8)
	sbb %rdx,%rdx
	mov %r8,(%rdi,%rcx,8)
	add $4,%rcx
	mov %r9,-24(%rdi,%rcx,8)
	mov %r10,-16(%rdi,%rcx,8)
	jnc lp
skiplp:
cmp $2,%rcx
ja case0
je case1
jp case2
case3:
	mov (%rsi,%rcx,8),%r8
	mov 8(%rsi,%rcx,8),%r9
	mov 16(%rsi,%rcx,8),%r10
	add %rax,%rax
	adc %r8,%r8
	adc %r9,%r9
	adc %r10,%r10
	sbb %rax,%rax
	add %rdx,%rdx
	adc %r8,%r8
	adc %r9,%r9
	adc %r10,%r10
	sbb %rdx,%rdx
	mov %r8,(%rdi,%rcx,8)
	mov %r9,8(%rdi,%rcx,8)
	mov %r10,16(%rdi,%rcx,8)
	lea (%rdx,%rax,2),%rax
	neg %rax
	ret
ALIGN(16)
case2:
	mov (%rsi,%rcx,8),%r8
	mov 8(%rsi,%rcx,8),%r9
	add %rax,%rax
	adc %r8,%r8
	adc %r9,%r9
	sbb %rax,%rax
	add %rdx,%rdx
	adc %r8,%r8
	adc %r9,%r9
	sbb %rdx,%rdx
	mov %r8,(%rdi,%rcx,8)
	mov %r9,8(%rdi,%rcx,8)
	lea (%rdx,%rax,2),%rax
	neg %rax
	ret
ALIGN(16)
case1:
	mov (%rsi,%rcx,8),%r8
	add %rax,%rax
	adc %r8,%r8
	sbb %rax,%rax
	add %rdx,%rdx
	adc %r8,%r8
	sbb %rdx,%rdx
	mov %r8,(%rdi,%rcx,8)
	lea (%rdx,%rax,2),%rax
	neg %rax
	ret
ALIGN(16)
case0:
	lea (%rdx,%rax,2),%rax
	neg %rax
	ret
EPILOGUE()
