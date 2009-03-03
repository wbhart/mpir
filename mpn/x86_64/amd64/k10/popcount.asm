dnl  AMD64 mpn_popcount

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
PROLOGUE(mpn_popcount)
xor %eax,%eax
sub $4,%rsi
jc skiploop
ALIGN(16)
loop:
	popcnt 24(%rdi,%rsi,8),%r8
	add %r8,%rax
	popcnt 16(%rdi,%rsi,8),%r9
	add %r9,%rax
	popcnt 8(%rdi,%rsi,8),%r10
	add %r10,%rax
	popcnt (%rdi,%rsi,8),%r11
	add %r11,%rax
	sub $4,%rsi
	jnc loop
skiploop:
add $4,%rsi
jz end
popcnt -8(%rdi,%rsi,8),%r8
add %r8,%rax
dec %rsi
jz end
popcnt -8(%rdi,%rsi,8),%r9
add %r9,%rax
dec %rsi
jz end
popcnt -8(%rdi,%rsi,8),%r10
add %r10,%rax
end:
ret
EPILOGUE()
