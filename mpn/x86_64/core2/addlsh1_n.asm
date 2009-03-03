dnl  core2 mpn_addlsh1_n 

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

C	(rdi,rcx)=(rsi,rcx)+(rdx,rcx)<<1
C	rax=carry

ASM_START()
PROLOGUE(mpn_addlsh1_n)
lea	(%rsi,%rcx,8),%rsi
lea	(%rdx,%rcx,8),%rdx
lea	(%rdi,%rcx,8),%rdi
neg	%rcx
xor	%r9,%r9
xor	%rax,%rax
test	$3,%rcx
jz	next
lp1:
	mov	(%rdx,%rcx,8),%r10
	add	$1,%r9
	adc	%r10,%r10
	sbb	%r9,%r9
	add	$1,%rax
	adc	(%rsi,%rcx,8),%r10
	sbb	%rax,%rax
	mov	%r10,(%rdi,%rcx,8)
	add	$1,%rcx
	test	$3,%rcx
	jnz lp1
next:
cmp	$0,%rcx
jz	end
push %rbx
ALIGN(16)
lp:
	mov	(%rdx,%rcx,8),%r10
	mov	8(%rdx,%rcx,8),%rbx
	mov	16(%rdx,%rcx,8),%r11
	mov	24(%rdx,%rcx,8),%r8
	add	$1,%r9
	adc	%r10,%r10
	adc	%rbx,%rbx
	adc	%r11,%r11
	adc	%r8,%r8
	sbb	%r9,%r9
	add	$1,%rax
	adc	(%rsi,%rcx,8),%r10
	adc	8(%rsi,%rcx,8),%rbx
	adc	16(%rsi,%rcx,8),%r11
	adc	24(%rsi,%rcx,8),%r8
	sbb	%rax,%rax
	mov	%r10,(%rdi,%rcx,8)
	mov	%rbx,8(%rdi,%rcx,8)
	mov	%r11,16(%rdi,%rcx,8)
	mov	%r8,24(%rdi,%rcx,8)
	add	$4,%rcx
	jnz lp
pop	%rbx
end:
add	%r9,%rax
neg	%rax
ret
EPILOGUE()
