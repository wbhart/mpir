dnl  mpn_copyi

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

C	ret mpn_copyi(mp_ptr,mp_ptr,mp_size_t)
C	rax             rdi,   rsi,      rdx

ASM_START()
PROLOGUE(mpn_copyi)
C // for <20 limbs this is slower than core2/copyi for rev 2257
C // probaly want to tweek it , that should do most of the work
C //below small loop is not much help
C //cmp $10,%rdx
C //jge large
C //ALIGN(16)
C //lp:	mov (%rsi),%rax
C //	mov %rax,(%rdi)
C //	lea 8(%rsi),%rsi
C //	lea 8(%rdi),%rdi
C //	sub $1,%rdx
C //	jnz lp
C //	ret
C // large:
cmp $0,%rdx
jz L(endfn)
mov %rdi,%rax
sub %rsi,%rax
test $0xF,%rax
jz L(aligned)
test $0xF,%rdi
jz L(srcisodd)
mov $5,%rcx
sub %rdx,%rcx
lea -40(%rsi,%rdx,8),%rsi
lea -40(%rdi,%rdx,8),%rdi
movapd (%rsi,%rcx,8),%xmm1
movq %xmm1,(%rdi,%rcx,8)
add $8,%rdi
cmp $1,%rdx
jz L(endfn)
cmp $0,%rcx
jge L(skiplpud)
ALIGN(16)
L(lpud):	movapd 16(%rsi,%rcx,8),%xmm0
	add $4,%rcx
	shufpd $1,%xmm0,%xmm1
	movapd %xmm1,-32(%rdi,%rcx,8)
	movapd 32-32(%rsi,%rcx,8),%xmm1
	shufpd $1,%xmm1,%xmm0
	movapd %xmm0,16-32(%rdi,%rcx,8)
	jnc L(lpud)
L(skiplpud):
cmp $2,%rcx
ja L(case0d)
jz L(case1d)
jp L(case2d)
ALIGN(16)
L(case3d):	movapd 16(%rsi,%rcx,8),%xmm0
	shufpd $1,%xmm0,%xmm1
	movapd %xmm1,(%rdi,%rcx,8)
	movapd 32(%rsi,%rcx,8),%xmm1  
	shufpd $1,%xmm1,%xmm0
	movapd %xmm0,16(%rdi,%rcx,8)
	ret
ALIGN(16)
L(case2d):	movapd 16(%rsi,%rcx,8),%xmm0
	shufpd $1,%xmm0,%xmm1
	movapd %xmm1,(%rdi,%rcx,8)
	movhpd %xmm0,16(%rdi,%rcx,8)
	ret
ALIGN(16)
L(case1d):	movapd 16(%rsi,%rcx,8),%xmm0
	shufpd $1,%xmm0,%xmm1
	movapd %xmm1,(%rdi,%rcx,8)
	ret
ALIGN(16)
L(case0d):	movhpd %xmm1,(%rdi,%rcx,8)
L(endfn):	ret
C //////////////////////////
L(srcisodd):
mov $4,%rcx
sub %rdx,%rcx
lea -32(%rsi,%rdx,8),%rsi
lea -32(%rdi,%rdx,8),%rdi
	movapd -8(%rsi,%rcx,8),%xmm1
	sub $8,%rsi
cmp $0,%rcx
jge L(skiplpus)
ALIGN(16)
L(lpus):	movapd 16(%rsi,%rcx,8),%xmm0
	add $4,%rcx
	shufpd $1,%xmm0,%xmm1
	movapd %xmm1,-32(%rdi,%rcx,8)
	movapd 32-32(%rsi,%rcx,8),%xmm1
	shufpd $1,%xmm1,%xmm0
	movapd %xmm0,16-32(%rdi,%rcx,8)
	jnc L(lpus)
L(skiplpus):
cmp $2,%rcx
ja L(case0s)
jz L(case1s)
jp L(case2s)
ALIGN(16)
L(case3s):	movapd 16(%rsi,%rcx,8),%xmm0
	shufpd $1,%xmm0,%xmm1
	movapd %xmm1,(%rdi,%rcx,8)
	movapd 32(%rsi,%rcx,8),%xmm1
	shufpd $1,%xmm1,%xmm0
	movapd %xmm0,16(%rdi,%rcx,8)
	ret
ALIGN(16)
L(case2s): movapd 16(%rsi,%rcx,8),%xmm0
	shufpd $1,%xmm0,%xmm1
	movapd %xmm1,(%rdi,%rcx,8)
	movhpd %xmm0,16(%rdi,%rcx,8)
	ret
ALIGN(16)
L(case1s):	movapd 16(%rsi,%rcx,8),%xmm0
	shufpd $1,%xmm0,%xmm1
	movapd %xmm1,(%rdi,%rcx,8)
	ret
ALIGN(16)
L(case0s):	movhpd %xmm1,(%rdi,%rcx,8)
	ret
C //////////////////////////
ALIGN(16)
L(aligned):
mov $3,%rcx
sub %rdx,%rcx
test $0xF,%rdi
lea -24(%rsi,%rdx,8),%rsi
lea -24(%rdi,%rdx,8),%rdi
jz L(notodda)
	mov (%rsi,%rcx,8),%rax
	mov %rax,(%rdi,%rcx,8)
	add $1,%rcx
L(notodda):
cmp $0,%rcx
jge L(skiplpa)
ALIGN(16)
L(lpa):	add $4,%rcx
	movapd -32(%rsi,%rcx,8),%xmm0
	movapd %xmm0,-32(%rdi,%rcx,8)
	movapd 16-32(%rsi,%rcx,8),%xmm1
	movapd %xmm1,16-32(%rdi,%rcx,8)
	jnc L(lpa)
L(skiplpa):
cmp $2,%rcx
ja L(casea0)
je L(casea1)
jp L(casea2)
L(casea3):	movapd (%rsi,%rcx,8),%xmm0
	movapd %xmm0,(%rdi,%rcx,8)
	mov 16(%rsi,%rcx,8),%rax
	mov %rax,16(%rdi,%rcx,8)
L(casea0):	ret
ALIGN(16)
L(casea2):	movapd (%rsi,%rcx,8),%xmm0
	movapd %xmm0,(%rdi,%rcx,8)
	ret
ALIGN(16)
L(casea1):	mov (%rsi,%rcx,8),%rax
	mov %rax,(%rdi,%rcx,8)
	ret
EPILOGUE()
