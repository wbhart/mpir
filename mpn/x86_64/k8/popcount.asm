dnl  mpn_popcount

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

C	ret mpn_popcount(mp_ptr,mp_size_t)
C	rax               rdi,   rsi

ASM_START()
PROLOGUE(mpn_popcount)
push %r12
push %r14
push %r15
mov $0x5555555555555555,%r8
mov $0x3333333333333333,%r9
mov $0x0f0f0f0f0f0f0f0f,%r10
mov $0x0101010101010101,%r11
xor %rax,%rax
sub $3,%rsi
jc skip
	mov 16(%rdi,%rsi,8),%rcx
	mov 8(%rdi,%rsi,8),%r12
	mov (%rdi,%rsi,8),%r14
sub $3,%rsi
jc skiplp
ALIGN(16)
lp:
	mov %rcx,%rdx
	shr $1,%rcx
	and %r8,%rcx
	sub %rcx,%rdx
	mov %rdx,%rcx
	shr $2,%rdx
	and %r9,%rcx
	and %r9,%rdx
	add %rcx,%rdx
	
	mov %r12,%rcx
	shr $1,%r12
	and %r8,%r12
	sub %r12,%rcx
	mov %rcx,%r12
	shr $2,%rcx
	and %r9,%r12
	and %r9,%rcx
	add %rcx,%r12
	
	mov %r14,%r15
	shr $1,%r14
	and %r8,%r14
		mov 16(%rdi,%rsi,8),%rcx
	sub %r14,%r15
	mov %r15,%r14
	shr $2,%r15
	and %r9,%r14
	and %r9,%r15
	add %r14,%r15
	
	add %rdx,%r12
	add %r15,%r12
	
	mov %r12,%r14
	shr $4,%r12
	and %r10,%r14
	and %r10,%r12
	add %r12,%r14
	imul %r11,%r14
	shr $56,%r14
	add %r14,%rax
		mov 8(%rdi,%rsi,8),%r12
	sub $3,%rsi
		mov 24-0(%rdi,%rsi,8),%r14
	jnc lp
skiplp:
	mov %rcx,%rdx
	shr $1,%rcx
	and %r8,%rcx
	sub %rcx,%rdx
	mov %rdx,%rcx
	shr $2,%rdx
	and %r9,%rcx
	and %r9,%rdx
	add %rcx,%rdx
	
	mov %r12,%rcx
	shr $1,%r12
	and %r8,%r12
	sub %r12,%rcx
	mov %rcx,%r12
	shr $2,%rcx
	and %r9,%r12
	and %r9,%rcx
	add %rcx,%r12
	
	mov %r14,%r15
	shr $1,%r14
	and %r8,%r14
	sub %r14,%r15
	mov %r15,%r14
	shr $2,%r15
	and %r9,%r14
	and %r9,%r15
	add %r14,%r15
	
	add %rdx,%r12
	add %r15,%r12
	
	mov %r12,%r14
	shr $4,%r12
	and %r10,%r14
	and %r10,%r12
	add %r12,%r14
	imul %r11,%r14
	shr $56,%r14
	add %r14,%rax
skip:	cmp $-2,%rsi
	jl case0
	jz case1
case2:
	mov 16(%rdi,%rsi,8),%rcx

	mov %rcx,%rdx
	shr $1,%rcx
	and %r8,%rcx
	sub %rcx,%rdx
	mov %rdx,%rcx
	shr $2,%rdx
	and %r9,%rcx
	and %r9,%rdx
	add %rcx,%rdx
	
	mov %rdx,%r14
	shr $4,%rdx
	and %r10,%r14
	and %r10,%rdx
	add %rdx,%r14
	imul %r11,%r14
	shr $56,%r14
	add %r14,%rax
	dec %rsi
case1:	mov 16(%rdi,%rsi,8),%rcx

	mov %rcx,%rdx
	shr $1,%rcx
	and %r8,%rcx
	sub %rcx,%rdx
	mov %rdx,%rcx
	shr $2,%rdx
	and %r9,%rcx
	and %r9,%rdx
	add %rcx,%rdx
	
	mov %rdx,%r14
	shr $4,%rdx
	and %r10,%r14
	and %r10,%rdx
	add %rdx,%r14
	imul %r11,%r14
	shr $56,%r14
	add %r14,%rax
case0:	pop %r15
	pop %r14
	pop %r12
	ret
EPILOGUE()
