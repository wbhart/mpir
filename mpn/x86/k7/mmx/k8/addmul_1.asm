dnl  mpn_addmul_1

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

C	ret mpn_addmul_1(mp_ptr,mp_ptr,mp_ptr,mp_size_t)
C	rex                20,   24,    28,     32
ASM_START()
PROLOGUE(mpn_addmul_1)
push %ebx
push %ebp
push %esi
push %edi
mov 20(%esp),%edi
mov 24(%esp),%esi
	mov (%esi),%eax
	mov 32(%esp),%edx
	mul %edx
	mov %eax,%ebx
	mov %edx,%ecx
	mov 32(%esp),%edx
	mov $0,%ebp
subl $4,28(%esp)
jc skiplp
ALIGN(16)
lp:
	mov 4(%esi),%eax
	mul %edx
	add %ebx,(%edi)
	adc %eax,%ecx
	adc %edx,%ebp	
	mov 8(%esi),%eax
	mull 32(%esp)
	mov $0,%ebx
	add %ecx,4(%edi)
	adc %eax,%ebp
	adc %edx,%ebx	
	mov 12(%esi),%eax
	mov 32(%esp),%edx
	mov $0,%ecx
	mul %edx
	add %ebp,8(%edi)
	adc %eax,%ebx
	adc %edx,%ecx
	lea 12(%esi),%esi
	mov 32(%esp),%edx
	subl $3,28(%esp)
	lea 12(%edi),%edi
	mov $0,%ebp
	jnc lp
skiplp:
cmpl $-2,28(%esp)
jl case0
je case1
case2:
	mov 4(%esi),%eax
	mul %edx
	add %ebx,(%edi)
	adc %eax,%ecx
	adc %edx,%ebp	
	mov 8(%esi),%eax
	mull 32(%esp)
	mov $0,%ebx
	add %ecx,4(%edi)
	adc %eax,%ebp
	adc %edx,%ebx
	add %ebp,8(%edi)
	adc $0,%ebx
	mov %ebx,%eax
	pop %edi
	pop %esi
	pop %ebp
	pop %ebx
	ret
ALIGN(16)
case1:
	mov 4(%esi),%eax
	mul %edx
	add %ebx,(%edi)
	adc %eax,%ecx
	adc %edx,%ebp
	mov $0,%eax
	add %ecx,4(%edi)
	adc %ebp,%eax
	pop %edi
	pop %esi
	pop %ebp
	pop %ebx
	ret
ALIGN(16)
case0:
	add %ebx,(%edi)
	adc $0,%ecx
	mov %ecx,%eax
	pop %edi
	pop %esi
	pop %ebp
	pop %ebx
	ret
EPILOGUE()
