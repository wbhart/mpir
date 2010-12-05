dnl  mpn_hamdist

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

C	ret mpn_hamdist(mp_ptr,mp_ptr,mp_size_t)
C	rax               rdi,   rsi,      rdx

ASM_START()
PROLOGUE(mpn_hamdist)
push %r12
push %r14
push %rbp
mov $0x5555555555555555,%r8
mov $0x3333333333333333,%r9
mov $0x0f0f0f0f0f0f0f0f,%r10
mov $0x0101010101010101,%r11
xor %eax,%eax
sub $3,%rdx
jc skip
	mov 16(%rdi,%rdx,8),%rcx
	xor 16(%rsi,%rdx,8),%rcx
	mov 8(%rdi,%rdx,8),%r12
	xor 8(%rsi,%rdx,8),%r12
	mov (%rdi,%rdx,8),%r14
	xor (%rsi,%rdx,8),%r14
sub $3,%rdx
jc skiplp
ALIGN(16)
lp:	mov %rcx,%rbp
	shr $1,%rcx
	and %r8,%rcx
	sub %rcx,%rbp
	mov %rbp,%rcx
	shr $2,%rbp
	and %r9,%rcx
	and %r9,%rbp
	add %rbp,%rcx

	mov %r12,%rbp
	shr $1,%r12
	and %r8,%r12
	sub %r12,%rbp
	mov %rbp,%r12
	shr $2,%rbp
	and %r9,%r12
	and %r9,%rbp
	add %r12,%rbp

	mov %r14,%r12
	shr $1,%r14
	and %r8,%r14
	sub %r14,%r12
	mov %r12,%r14
	shr $2,%r12
	and %r9,%r14
	and %r9,%r12
	add %r14,%r12

	add %rcx,%rbp
	add %r12,%rbp	
		mov 16(%rdi,%rdx,8),%rcx
	mov %rbp,%r14
	shr $4,%rbp
	and %r10,%r14
		xor 16(%rsi,%rdx,8),%rcx
		mov 8(%rdi,%rdx,8),%r12
		xor 8(%rsi,%rdx,8),%r12
	and %r10,%rbp
	add %rbp,%r14
	imul %r11,%r14
	shr $56,%r14
	add %r14,%rax
		mov (%rdi,%rdx,8),%r14
		xor (%rsi,%rdx,8),%r14
	sub $3,%rdx
	jnc lp
skiplp:
	mov %rcx,%rbp
	shr $1,%rcx
	and %r8,%rcx
	sub %rcx,%rbp
	mov %rbp,%rcx
	shr $2,%rbp
	and %r9,%rcx
	and %r9,%rbp
	add %rbp,%rcx
	
	mov %r12,%rbp
	shr $1,%r12
	and %r8,%r12
	sub %r12,%rbp
	mov %rbp,%r12
	shr $2,%rbp
	and %r9,%r12
	and %r9,%rbp
	add %r12,%rbp
	
	mov %r14,%r12
	shr $1,%r14
	and %r8,%r14
	sub %r14,%r12
	mov %r12,%r14
	shr $2,%r12
	and %r9,%r14
	and %r9,%r12
	add %r14,%r12
	
	add %rcx,%rbp
	add %r12,%rbp	
	mov %rbp,%r14
	shr $4,%rbp
	and %r10,%r14
	and %r10,%rbp
	add %rbp,%r14
	imul %r11,%r14
	shr $56,%r14
	add %r14,%rax
skip:
	cmp $-2,%rdx
	jl case0
	jz case1
case2:
	mov 16(%rdi,%rdx,8),%rcx
	xor 16(%rsi,%rdx,8),%rcx
	mov %rcx,%rbp
	shr $1,%rcx
	and %r8,%rcx
	sub %rcx,%rbp
	mov %rbp,%rcx
	shr $2,%rbp
	and %r9,%rcx
	and %r9,%rbp
	add %rbp,%rcx
	
	mov %rcx,%r14
	shr $4,%rcx
	and %r10,%r14
	and %r10,%rcx
	add %rcx,%r14
	imul %r11,%r14
	shr $56,%r14
	add %r14,%rax
	dec %rdx
case1:
	mov 16(%rdi,%rdx,8),%rcx
	xor 16(%rsi,%rdx,8),%rcx
	mov %rcx,%rbp
	shr $1,%rcx
	and %r8,%rcx
	sub %rcx,%rbp
	mov %rbp,%rcx
	shr $2,%rbp
	and %r9,%rcx
	and %r9,%rbp
	add %rbp,%rcx
	
	mov %rcx,%r14
	shr $4,%rcx
	and %r10,%r14
	and %r10,%rcx
	add %rcx,%r14
	imul %r11,%r14
	shr $56,%r14
	add %r14,%rax
case0:	pop %rbp
	pop %r14
	pop %r12
	ret
EPILOGUE()
