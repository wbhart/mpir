dnl  core2 mpn_diveby3

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

C	(rdi,rdx)=(rsi,rdx)  rcx=carry in
C	rax=carry out

C   NOTE could pass 55555...555 as next param so this would
C   be mpn_divexact_by_ff_over_c , and change imul at end , or
C   drop backwards compatibilty and just dump the two imuls

ASM_START()
PROLOGUE(mpn_divexact_by3c)
mov $3,%r9d
lea -24(%rsi,%rdx,8),%rsi
lea -24(%rdi,%rdx,8),%rdi
mov $0x5555555555555555,%r8
imul %r8,%rcx
sub %rdx,%r9
jnc skiploop
ALIGN(16)
loop:
	mov (%rsi,%r9,8),%rax
	mul %r8
	sub %rax,%rcx
	mov %rcx,(%rdi,%r9,8)
	sbb %rdx,%rcx
	mov 8(%rsi,%r9,8),%rax
	mul %r8
	sub %rax,%rcx
	mov %rcx,8(%rdi,%r9,8)
	sbb %rdx,%rcx
	mov 16(%rsi,%r9,8),%rax
	mul %r8
	sub %rax,%rcx
	mov %rcx,16(%rdi,%r9,8)
	sbb %rdx,%rcx
	mov 24(%rsi,%r9,8),%rax
	mul %r8
	sub %rax,%rcx
	mov %rcx,24(%rdi,%r9,8)
	sbb %rdx,%rcx
	add $4,%r9
	jnc loop
skiploop:
test $2,%r9
jnz skip
	mov (%rsi,%r9,8),%rax
	mul %r8
	sub %rax,%rcx
	mov %rcx,(%rdi,%r9,8)
	sbb %rdx,%rcx
	mov 8(%rsi,%r9,8),%rax
	mul %r8
	sub %rax,%rcx
	mov %rcx,8(%rdi,%r9,8)
	sbb %rdx,%rcx
	add $2,%r9
skip:
test $1,%r9
jnz end
	mov (%rsi,%r9,8),%rax
	mul %r8
	sub %rax,%rcx
	mov %rcx,(%rdi,%r9,8)
	sbb %rdx,%rcx
end:
C the lea below is instead of imul $-3,%rcx,%rax
lea (%rcx,%rcx,2),%rax
neg %rax
ret
EPILOGUE()
