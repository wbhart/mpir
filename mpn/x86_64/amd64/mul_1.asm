dnl  AMD64 mpn_mul_1

dnl  Copyright 2008 Jason Moxham

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
# Version 1.0.3
mov $3,%r10d
lea -24(%rsi,%rdx,8),%rsi
sub %rdx,%r10
lea -24(%rdi,%rdx,8),%rdi
# r8 is our carry in 
mov $0,%r8d
jnc skiploop
ALIGN(16)
loop:
	mov (%rsi,%r10,8),%rax
	mov $0,%r9d
	mul %rcx
	add %rax,%r8
	mov %r8,(%rdi,%r10,8)
	mov 8(%rsi,%r10,8),%rax
	adc %rdx,%r9
	mul %rcx
	mov $0,%r8d
	add %rax,%r9
	mov 16(%rsi,%r10,8),%rax
	adc %rdx,%r8
	mul %rcx
	mov %r9,8(%rdi,%r10,8)
	add %rax,%r8
	mov $0,%r9d
	mov %r8,16(%rdi,%r10,8)
	mov 24(%rsi,%r10,8),%rax
	mov $0,%r8d
	adc %rdx,%r9
	mul %rcx
	add %rax,%r9
	mov %r9,24(%rdi,%r10,8)
	adc %rdx,%r8
	add $4,%r10
	jnc loop
#ALIGN(8)
skiploop:
# here r10=3,2,1,0  so have 3-r10 limbs left to do , with r8 as our carry in
# could copy r10 to r11 at fn entry , and test that here instead of r10 , as
# its fixed
test $2,%r10
jnz skip
	mov (%rsi,%r10,8),%rax
	mov $0,%r9d
	mul %rcx
	add %rax,%r8
	mov %r8,(%rdi,%r10,8)
	mov 8(%rsi,%r10,8),%rax
	adc %rdx,%r9
	mul %rcx
	mov $0,%r8d
	add %rax,%r9
	adc %rdx,%r8
	add $2,%r10
	mov %r9,-8(%rdi,%r10,8)
skip:
test $1,%r10
jnz end
	mov (%rsi,%r10,8),%rax
	mov $0,%r9d
	mul %rcx
	add %rax,%r8
	mov %r8,(%rdi,%r10,8)
	adc %rdx,%r9
	mov %r9,%rax
	ret
end:
mov %r8,%rax
ret
EPILOGUE()
# see if we can improve the alignment of all these jumps
