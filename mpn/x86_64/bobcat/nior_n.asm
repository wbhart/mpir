dnl  mpn_nior_n

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

ASM_START()
PROLOGUE(mpn_nior_n)
lea (%rdx,%rcx,8),%rdx
lea (%rsi,%rcx,8),%rsi
lea (%rdi,%rcx,8),%rdi
neg %rcx
add $3,%rcx
jc skip
	mov -24(%rdx,%rcx,8),%r8
	mov -16(%rdx,%rcx,8),%r9
	mov -24(%rdx,%rcx,8),%r8
	mov -16(%rdx,%rcx,8),%r9
add $4,%rcx
	mov -40(%rdx,%rcx,8),%r10
	mov -32(%rdx,%rcx,8),%r11
jc skiplp
ALIGN(16)
lp:
	or -56(%rsi,%rcx,8),%r8
	not %r8
	or -48(%rsi,%rcx,8),%r9
	or -40(%rsi,%rcx,8),%r10
	or -32(%rsi,%rcx,8),%r11
	mov %r8,-56(%rdi,%rcx,8)
	not %r9
	not %r10
	mov %r9,-48(%rdi,%rcx,8)
	not %r11
	mov -24(%rdx,%rcx,8),%r8
	mov -16(%rdx,%rcx,8),%r9
	mov %r10,-40(%rdi,%rcx,8)
	mov %r11,-32(%rdi,%rcx,8)
	add $4,%rcx
	mov -40(%rdx,%rcx,8),%r10
	mov -32(%rdx,%rcx,8),%r11
	jnc lp
skiplp:
	or -56(%rsi,%rcx,8),%r8
	not %r8
	or -48(%rsi,%rcx,8),%r9
	or -40(%rsi,%rcx,8),%r10
	or -32(%rsi,%rcx,8),%r11
	mov %r8,-56(%rdi,%rcx,8)
	not %r9
	not %r10
	mov %r9,-48(%rdi,%rcx,8)
	not %r11
	mov %r10,-40(%rdi,%rcx,8)
	mov %r11,-32(%rdi,%rcx,8)
skip:
cmp $2,%rcx
jg case0
je case1
jp case2
case3:	mov -24(%rdx),%r8
	or -24(%rsi),%r8
	not %r8
	mov %r8,-24(%rdi)
case2:	mov -16(%rdx),%r8
	or -16(%rsi),%r8
	not %r8
	mov %r8,-16(%rdi)
case1:	mov -8(%rdx),%r8
	or -8(%rsi),%r8
	not %r8
	mov %r8,-8(%rdi)
case0:	ret
EPILOGUE()
