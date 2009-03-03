dnl  core2 mpn_xor_n

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
PROLOGUE(mpn_xor_n)
sub $4,%rcx
jb skiploop
ALIGN(16)
loop:
	mov 24(%rdx,%rcx,8),%r8
	mov 16(%rdx,%rcx,8),%r9
	or %r8,%r8
	or %r9,%r9
	xor 24(%rsi,%rcx,8),%r8
	xor 16(%rsi,%rcx,8),%r9
	mov %r8,24(%rdi,%rcx,8)
	mov %r9,16(%rdi,%rcx,8)
	mov 8(%rdx,%rcx,8),%r10
	mov (%rdx,%rcx,8),%r11
	or %r10,%r10
	or %r11,%r11
	xor 8(%rsi,%rcx,8),%r10
	xor (%rsi,%rcx,8),%r11
	mov %r10,8(%rdi,%rcx,8)
	mov %r11,(%rdi,%rcx,8)
	sub $4,%rcx
	jnc loop
skiploop:
add $4,%rcx
jz end
mov -8(%rdx,%rcx,8),%r8
or %r8,%r8
xor -8(%rsi,%rcx,8),%r8
mov %r8,-8(%rdi,%rcx,8)
sub $1,%rcx
jz end
mov -8(%rdx,%rcx,8),%r8
or %r8,%r8
xor -8(%rsi,%rcx,8),%r8
mov %r8,-8(%rdi,%rcx,8)
sub $1,%rcx
jz end
mov -8(%rdx,%rcx,8),%r8
or %r8,%r8
xor -8(%rsi,%rcx,8),%r8
mov %r8,-8(%rdi,%rcx,8)
end:
ret
EPILOGUE()
