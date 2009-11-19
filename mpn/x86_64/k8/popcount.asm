dnl  mpn_popcount

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

C	ret mpn_popcount(mp_ptr,mp_size_t)
C	rax               rdi,   rsi

ASM_START()
PROLOGUE(mpn_popcount)
push %r12
push %rbp
push %rbx
mov $0x5555555555555555,%r8
mov $0x3333333333333333,%r9
mov $0x0f0f0f0f0f0f0f0f,%r10
mov $0x0101010101010101,%r11
mov $0,%rax
sub $2,%rsi
jc skip
	mov 8(%rdi,%rsi,8),%rcx
	or 8(%rdi,%rsi,8),%rcx
	mov (%rdi,%rsi,8),%r12
	or (%rdi,%rsi,8),%r12
sub $2,%rsi
jc skiplp
ALIGN(16)
lp:
	mov %rcx,%rbp
	shr $1,%rcx
	and %r8,%rcx
	sub %rcx,%rbp
	mov %rbp,%rcx
	shr $2,%rbp
	and %r9,%rcx
	and %r9,%rbp
	add %rcx,%rbp
	
	mov %r12,%rbx
	shr $1,%r12
	and %r8,%r12
	sub %r12,%rbx
		mov 8(%rdi,%rsi,8),%rcx
	mov %rbx,%r12
	shr $2,%rbx
	and %r9,%r12
		or 8(%rdi,%rsi,8),%rcx
	and %r9,%rbx
	add %r12,%rbx
	
	add %rbp,%rbx
	mov %rbx,%rdx
		mov (%rdi,%rsi,8),%r12
		or (%rdi,%rsi,8),%r12
	shr $4,%rbx
	and %r10,%rdx
	and %r10,%rbx
	add %rbx,%rdx
	imul %r11,%rdx
	shr $56,%rdx
	add %rdx,%rax
	sub $2,%rsi
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
	add %rcx,%rbp
	
	mov %r12,%rbx
	shr $1,%r12
	and %r8,%r12
	sub %r12,%rbx
	mov %rbx,%r12
	shr $2,%rbx
	and %r9,%r12
	and %r9,%rbx
	add %r12,%rbx
	
	add %rbp,%rbx
	mov %rbx,%rdx
	shr $4,%rbx
	and %r10,%rdx
	and %r10,%rbx
	add %rbx,%rdx
	imul %r11,%rdx
	shr $56,%rdx
	add %rdx,%rax
skip:	cmp $-2,%rsi
	jz  case0
case1:	mov 8(%rdi,%rsi,8),%rcx
	or 8(%rdi,%rsi,8),%rcx
	mov %rcx,%rbp
	shr $1,%rcx
	and %r8,%rcx
	sub %rcx,%rbp
	mov %rbp,%rcx
	shr $2,%rbp
	and %r9,%rcx
	and %r9,%rbp
	add %rcx,%rbp
	mov %rbp,%rdx
	shr $4,%rbp
	add %rbp,%rdx
	and %r10,%rdx
	imul %r11,%rdx
	shr $56,%rdx
	add %rdx,%rax
case0:	pop %rbx
	pop %rbp
	pop %r12
	ret
EPILOGUE()
