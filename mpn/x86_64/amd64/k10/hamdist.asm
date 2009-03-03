dnl  AMD64 mpn_hamdist

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
PROLOGUE(mpn_hamdist)
xor %eax,%eax
sub $4,%rdx
jc skiploop
ALIGN(16)
loop:
	mov 24(%rdi,%rdx,8),%r8
	mov 16(%rdi,%rdx,8),%r9
	xor 24(%rsi,%rdx,8),%r8
	popcnt %r8,%r8
	add %r8,%rax
	xor 16(%rsi,%rdx,8),%r9
	popcnt %r9,%r9
	add %r9,%rax
	mov 8(%rdi,%rdx,8),%r10
	mov (%rdi,%rdx,8),%r11
	xor 8(%rsi,%rdx,8),%r10
	popcnt %r10,%r10
	add %r10,%rax
	xor (%rsi,%rdx,8),%r11
	popcnt %r11,%r11
	add %r11,%rax
	sub $4,%rdx
	jnc loop
skiploop:
add $4,%rdx
jz end
mov -8(%rdi,%rdx,8),%r8
xor -8(%rsi,%rdx,8),%r8
popcnt %r8,%r8
add %r8,%rax
dec %rdx
jz end
mov -8(%rdi,%rdx,8),%r9
xor -8(%rsi,%rdx,8),%r9
popcnt %r9,%r9
add %r9,%rax
dec %rdx
jz end
mov -8(%rdi,%rdx,8),%r10
xor -8(%rsi,%rdx,8),%r10
popcnt %r10,%r10
add %r10,%rax
end:
ret
EPILOGUE()
