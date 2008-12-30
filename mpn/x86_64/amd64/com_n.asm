dnl  AMD64 mpn_com_n

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

ASM_START()
PROLOGUE(mpn_com_n)
# Version 1.0.4
sub	$4,%rdx
jc	next
ALIGN(8)
loop:
	mov	24(%rsi,%rdx,8),%rax
	mov	16(%rsi,%rdx,8),%rcx
	not	%rax
	not	%rcx
	mov	%rax,24(%rdi,%rdx,8)
	mov	%rcx,16(%rdi,%rdx,8)
	mov	8(%rsi,%rdx,8),%rax
	mov	(%rsi,%rdx,8),%rcx
	not	%rax
	not	%rcx
	mov	%rax,8(%rdi,%rdx,8)
	mov	%rcx,(%rdi,%rdx,8)
	sub	$4,%rdx
	jae	loop		
next:
add	$4,%rdx
jz	end
# Could still have potential cache-bank conflicts in this tail part
mov	-8(%rsi,%rdx,8),%rax
not	%rax
mov	%rax,-8(%rdi,%rdx,8)
dec	%rdx
jz	end
mov	-8(%rsi,%rdx,8),%rax
not	%rax
mov	%rax,-8(%rdi,%rdx,8)
dec	%rdx
jz	end
mov	-8(%rsi,%rdx,8),%rax
not	%rax
mov	%rax,-8(%rdi,%rdx,8)
end:
ret
EPILOGUE()
