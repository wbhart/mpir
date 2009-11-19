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
cmp $0,%rdx		#needed for case n=0
jz endfn		#needed for case n=0
mov %rdi,%rax
sub %rsi,%rax
test $0xF,%rax
jz aligned
test $0xF,%rdi
jz srcisodd
mov $5,%rcx
sub %rdx,%rcx
lea -40(%rsi,%rdx,8),%rsi
lea -40(%rdi,%rdx,8),%rdi
movapd (%rsi,%rcx,8),%xmm1
movq %xmm1,(%rdi,%rcx,8)
add $8,%rdi
cmp $1,%rdx		#needed for case n=1
jz endfn		#needed for case n=1
cmp $0,%rcx
jge skiplpud
ALIGN(16)
lpud:	movapd 16(%rsi,%rcx,8),%xmm0
	add $4,%rcx
	shufpd $1,%xmm0,%xmm1
	movapd %xmm1,-32(%rdi,%rcx,8)
	movapd 32-32(%rsi,%rcx,8),%xmm1
	shufpd $1,%xmm1,%xmm0
	movapd %xmm0,16-32(%rdi,%rcx,8)
	jnc lpud
skiplpud:
cmp $2,%rcx
ja case0d
jz case1d
jp case2d
ALIGN(16)
case3d:	movapd 16(%rsi,%rcx,8),%xmm0
	shufpd $1,%xmm0,%xmm1
	movapd %xmm1,(%rdi,%rcx,8)
	movapd 32(%rsi,%rcx,8),%xmm1  	# top is read past
	shufpd $1,%xmm1,%xmm0
	movapd %xmm0,16(%rdi,%rcx,8)
	ret
ALIGN(16)
case2d:	movapd 16(%rsi,%rcx,8),%xmm0
	shufpd $1,%xmm0,%xmm1
	movapd %xmm1,(%rdi,%rcx,8)
	movhpd %xmm0,16(%rdi,%rcx,8)
	ret
ALIGN(16)
case1d:	movapd 16(%rsi,%rcx,8),%xmm0	# top read past
	shufpd $1,%xmm0,%xmm1
	movapd %xmm1,(%rdi,%rcx,8)
	ret
ALIGN(16)
case0d:	movhpd %xmm1,(%rdi,%rcx,8)
endfn:	ret



srcisodd:
mov $4,%rcx
sub %rdx,%rcx
lea -32(%rsi,%rdx,8),%rsi
lea -32(%rdi,%rdx,8),%rdi
	movapd -8(%rsi,%rcx,8),%xmm1
	sub $8,%rsi
cmp $0,%rcx
jge skiplpus	
ALIGN(16)
lpus:	movapd 16(%rsi,%rcx,8),%xmm0
	add $4,%rcx
	shufpd $1,%xmm0,%xmm1
	movapd %xmm1,-32(%rdi,%rcx,8)
	movapd 32-32(%rsi,%rcx,8),%xmm1
	shufpd $1,%xmm1,%xmm0
	movapd %xmm0,16-32(%rdi,%rcx,8)
	jnc lpus
skiplpus:
cmp $2,%rcx
ja case0s
jz case1s
jp case2s
ALIGN(16)
case3s:	movapd 16(%rsi,%rcx,8),%xmm0
	shufpd $1,%xmm0,%xmm1
	movapd %xmm1,(%rdi,%rcx,8)
	movapd 32(%rsi,%rcx,8),%xmm1  	# read past
	shufpd $1,%xmm1,%xmm0
	movapd %xmm0,16(%rdi,%rcx,8)
	ret
ALIGN(16)
case2s: movapd 16(%rsi,%rcx,8),%xmm0
	shufpd $1,%xmm0,%xmm1
	movapd %xmm1,(%rdi,%rcx,8)
	movhpd %xmm0,16(%rdi,%rcx,8)
	ret
ALIGN(16)
case1s:	movapd 16(%rsi,%rcx,8),%xmm0	# read past
	shufpd $1,%xmm0,%xmm1
	movapd %xmm1,(%rdi,%rcx,8)
	ret
ALIGN(16)
case0s:	movhpd %xmm1,(%rdi,%rcx,8)
	ret


ALIGN(16)
aligned:
sub $4,%rdx
test $0xF,%rdi
jz notodda
	mov (%rsi),%rax
	mov %rax,(%rdi)
	sub $1,%rdx
	lea 8(%rsi),%rsi
	lea 8(%rdi),%rdi
notodda:
cmp $0,%rdx
jl skiplpa
ALIGN(16)
lpa:	movdqa (%rsi),%xmm0
	sub $4,%rdx
	movdqa 16(%rsi),%xmm1
	lea 32(%rsi),%rsi
	movdqa %xmm0,(%rdi)
	lea 32(%rdi),%rdi
	movdqa %xmm1,16-32(%rdi)
	jnc lpa
skiplpa:
cmp $-2,%rdx
jg casea3
je casea2
jnp casea0
casea1:	mov (%rsi),%rax
	mov %rax,(%rdi)
	ret
casea3:	movdqa (%rsi),%xmm0
	mov 16(%rsi),%rax
	movdqa %xmm0,(%rdi)
	mov %rax,16(%rdi)
casea0:	ret
casea2:	movdqa (%rsi),%xmm0
	movdqa %xmm0,(%rdi)
	ret
EPILOGUE()
