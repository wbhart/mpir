dnl  AMD64 mpn_sublsh 

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

C	    carry+(xp,n)= (yp,n) - (zp,n)<<c with carry in ci
C	mp_limb_t	mpn_sublsh_nc(mp_ptr xp, mp_srcptr yp,mp_srcptr zp,mp_size_t n,unsigned int c,mp_limb_t ci)
C	xp in rdi	yp in rsi	zp in rdx	n  in rcx	c  in r8	ci in r9

MULFUNC_PROLOGUE(mpn_sublsh_n mpn_sublsh_nc)

ASM_START()
PROLOGUE(mpn_sublsh_n)
xor %r9,%r9
EPILOGUE()
PROLOGUE(mpn_sublsh_nc)
mov	%rcx,%r10
lea	(%rdi,%r10,8),%rdi
lea	(%rsi,%r10,8),%rsi
lea	(%rdx,%r10,8),%rdx
mov	%r8,%rcx
neg	%rcx
shr	%cl,%r9
neg	%r10
xor	%rax,%rax
test	$3,%r10
jz	next
lp:
	mov	(%rdx,%r10,8),%r8
	mov	%r8,%r11
	neg	%rcx
	shl	%cl,%r8
	neg	%rcx
	shr	%cl,%r11
	or	%r9,%r8
	mov	%r11,%r9
	add	$1,%rax
	mov	(%rsi,%r10,8),%r11
	sbb	%r8,%r11
	sbb	%rax,%rax
	mov	%r11,(%rdi,%r10,8)
	inc	%r10
	test	$3,%r10
	jnz	lp
next:
cmp	$0,%r10
jz	end
push	%rbx
push	%rbp
push	%r12
push	%r13
push	%r14
push	%r15
ALIGN(16)
loop:
	mov	(%rdx,%r10,8),%r8
	mov	8(%rdx,%r10,8),%rbp
	mov	16(%rdx,%r10,8),%rbx
	mov	24(%rdx,%r10,8),%r12
	mov	%r8,%r11
	mov	%rbp,%r13
	mov	%rbx,%r14
	mov	%r12,%r15
	neg	%rcx
	shl	%cl,%r8
	shl	%cl,%rbp
	shl	%cl,%rbx
	shl	%cl,%r12
	neg	%rcx
	shr	%cl,%r11
	shr	%cl,%r13
	shr	%cl,%r14
	shr	%cl,%r15
	or	%r9,%r8
	or	%r11,%rbp
	or	%r13,%rbx
	or	%r14,%r12
	mov	%r15,%r9
	add	$1,%rax
	mov	(%rsi,%r10,8),%r11
	mov	8(%rsi,%r10,8),%r13
	mov	16(%rsi,%r10,8),%r14
	mov	24(%rsi,%r10,8),%r15
	sbb	%r8,%r11
	sbb	%rbp,%r13
	sbb	%rbx,%r14
	sbb	%r12,%r15
	sbb	%rax,%rax
	mov	%r11,(%rdi,%r10,8)
	mov	%r13,8(%rdi,%r10,8)
	mov	%r14,16(%rdi,%r10,8)
	mov	%r15,24(%rdi,%r10,8)
	add	$4,%r10
	jnz	loop
pop	%r15
pop	%r14
pop	%r13
pop	%r12
pop	%rbp
pop	%rbx
end:
neg	%rax
add	%r9,%rax
ret
EPILOGUE()
