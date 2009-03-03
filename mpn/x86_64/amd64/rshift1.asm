dnl  AMD64 mpn_rshift1

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

C	(rdi,rdx)=(rsi,rdx)>>1
C	rax=carry

ASM_START()
PROLOGUE(mpn_rshift1)
xor	%rax,%rax
lea	-8(%rsi,%rdx,8),%rsi
lea	-8(%rdi,%rdx,8),%rdi
mov	%rdx,%r11
and	$7,%r11
inc	%r11
shr	$3,%rdx
C and clear carry flag
cmp	$0,%rdx	
jz	next
ALIGN(16)
loop:
	mov	(%rsi),%rcx
	mov	-8(%rsi),%r8
	mov	-16(%rsi),%r9
	mov	-24(%rsi),%r10
	rcr	$1,%rcx
	rcr	$1,%r8
	rcr	$1,%r9
	rcr	$1,%r10
	mov	%rcx,(%rdi)
	mov	%r8,-8(%rdi)
	mov	%r9,-16(%rdi)
	mov	%r10,-24(%rdi)
	mov	-32(%rsi),%rcx
	mov	-40(%rsi),%r8
	mov	-48(%rsi),%r9
	mov	-56(%rsi),%r10
	rcr	$1,%rcx
	rcr	$1,%r8
	rcr	$1,%r9
	rcr	$1,%r10
	mov	%rcx,-32(%rdi)
	mov	%r8,-40(%rdi)
	mov	%r9,-48(%rdi)
	mov	%r10,-56(%rdi)
	lea	-64(%rsi),%rsi
	dec	%rdx
	lea	-64(%rdi),%rdi
	jnz	loop
next:
dec	%r11
jz	end
C Could suffer cache-bank conflicts in this tail part
	mov	(%rsi),%rcx
	rcr	$1,%rcx
	mov	%rcx,(%rdi)
	dec	%r11
	jz end
	mov	-8(%rsi),%rcx
	rcr	$1,%rcx
	mov	%rcx,-8(%rdi)
	dec	%r11
	jz end
	mov	-16(%rsi),%rcx
	rcr	$1,%rcx
	mov	%rcx,-16(%rdi)
	dec	%r11
	jz end
	mov	-24(%rsi),%rcx
	rcr	$1,%rcx
	mov	%rcx,-24(%rdi)
	dec	%r11
	jz end
	mov	-32(%rsi),%rcx
	rcr	$1,%rcx
	mov	%rcx,-32(%rdi)
	dec	%r11
	jz end
	mov	-40(%rsi),%rcx
	rcr	$1,%rcx
	mov	%rcx,-40(%rdi)
	dec	%r11
	jz end
	mov	-48(%rsi),%rcx
	rcr	$1,%rcx
	mov	%rcx,-48(%rdi)
end:
rcr	$1,%rax
ret
EPILOGUE()
