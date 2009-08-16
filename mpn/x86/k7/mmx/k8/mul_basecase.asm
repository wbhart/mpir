dnl  mpn_mul_basecase

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

C	ret mpn_mul_basecase(mp_ptr dst  ,mp_ptr src1,mp_size_t size1,    mp_ptr src2,mp_size_t size2)

C	Caller sets up the stack like
C	size2		44
C	src2		40
C	size1		36
C	src1		32
C	dst		28
C	retaddr		24
C	MUL		20	we sub $8.esp for this
C	SIZE1T		16	and this , and remember to add 8 back when retting
C	SAVE_EBX	12	we push and pop this
C	SAVE_EBP	8	and this    .  we could do one big sub of esp
C	SAVE_EDI	4	and this    .  and just mov in and out
C	SAVE_ESI	0	and this    . this would save macro-ops

define(`SIZE2',`44(%esp)')
define(`SRC2',`40(%esp)')
define(`SIZE1',`36(%esp)')
define(`SRC1',`32(%esp)')
define(`DST',`28(%esp)')
define(`MUL',`20(%esp)')
define(`SIZE1T',`16(%esp)')
define(`SAVE_EBX',`12(%esp)')
define(`SAVE_EBP',`8(%esp)')
define(`SAVE_EDI',`4(%esp)')
define(`SAVE_ESI',`(%esp)')



define(`MULSTART',`
	mov SIZE1,%eax
	mov SRC2,%ecx
	mov %eax,SIZE1T
	mov SRC1,%esi
	mov (%ecx),%edx
	mov %edx,MUL
	mov (%esi),%eax
	mul %edx
	mov %eax,%ebx
	mov %edx,%ecx
	mov DST,%edi
	mov MUL,%edx
	mov `$'0,%ebp
	subl `$'4,SIZE1T
')

define(`MULLOOP',`
	mov 4(%esi),%eax
	mul %edx
	mov %ebx,(%edi)
	add %eax,%ecx
	adc %edx,%ebp	
	mov 8(%esi),%eax
	mull MUL
	mov `$'0,%ebx
	mov %ecx,4(%edi)
	add %eax,%ebp
	adc %edx,%ebx	
	mov 12(%esi),%eax
	mov MUL,%edx
	mov `$'0,%ecx
	mul %edx
	mov %ebp,8(%edi)
	add %eax,%ebx
	adc %edx,%ecx
	lea 12(%esi),%esi
	mov MUL,%edx
	subl `$'3,SIZE1T
	lea 12(%edi),%edi
	mov `$'0,%ebp
')

define(`MULEND2',`
	mov 4(%esi),%eax
	mul %edx
	mov %ebx,(%edi)
	add %eax,%ecx
	adc %edx,%ebp	
	mov 8(%esi),%eax
	mull MUL
	mov `$'0,%ebx
	mov %ecx,4(%edi)
	add %eax,%ebp
	adc %edx,%ebx
	mov %ebp,8(%edi)
	mov %ebx,12(%edi)
	decl SIZE2
')

define(`MULEND1',`
	mov 4(%esi),%eax
	mul %edx
	mov %ebx,(%edi)
	add %eax,%ecx
	adc %edx,%ebp
	mov %ecx,4(%edi)
	mov %ebp,8(%edi)
	mov DST,%edi
	decl SIZE2
')

define(`MULEND0',`
	mov %ebx,(%edi)
	mov %ecx,4(%edi)
	mov SRC2,%ebx
	decl SIZE2
')


define(`ADDMULLOOP',`
	mov 4(%esi),%eax
	mul %edx
	add %ebx,(%edi)
	adc %eax,%ecx
	adc %edx,%ebp	
	mov 8(%esi),%eax
	mull MUL
	mov `$'0,%ebx
	add %ecx,4(%edi)
	adc %eax,%ebp
	adc %edx,%ebx	
	mov 12(%esi),%eax
	mov MUL,%edx
	mov `$'0,%ecx
	mul %edx
	add %ebp,8(%edi)
	adc %eax,%ebx
	adc %edx,%ecx
	lea 12(%esi),%esi
	mov MUL,%edx
	subl `$'3,SIZE1T
	lea 12(%edi),%edi
	mov `$'0,%ebp
')

define(`ADDMULSTART2',`
	mov SIZE1,%eax
	mov SRC2,%ecx
	mov %eax,SIZE1T
	lea 4(%ecx),%ecx
	mov SRC1,%esi
	mov %ecx,SRC2
	mov (%ecx),%edx
	mov %edx,MUL
	mov (%esi),%eax
	mul %edx
	mov %eax,%ebx
	mov %edx,%ecx
	mov DST,%edi
	mov MUL,%edx
	mov `$'0,%ebp
	subl `$'4,SIZE1T
	lea 4(%edi),%edi
	mov %edi,DST
')

define(`ADDMULEND2',`
	mov 4(%esi),%eax
	mul %edx
	add %ebx,(%edi)
	adc %eax,%ecx
	adc %edx,%ebp	
	mov 8(%esi),%eax
	mull MUL
	mov `$'0,%ebx
	add %ecx,4(%edi)
	adc %eax,%ebp
	adc %edx,%ebx
	add %ebp,8(%edi)
	adc `$'0,%ebx
	mov %ebx,12(%edi)
	decl SIZE2
')

define(`ADDMULSTART1',`
	mov SRC2,%ebx
	mov SIZE1,%eax
	mov %eax,SIZE1T
	lea 4(%ebx),%ebx
	mov SRC1,%esi
	mov (%ebx),%edx
	mov %edx,MUL
	mov (%esi),%eax
	lea 4(%edi),%edi
	mov %ebx,SRC2
	mul %edx
	mov %eax,%ebx
	mov %edx,%ecx
	mov MUL,%edx
	mov `$'0,%ebp
	mov %edi,DST
	subl `$'4,SIZE1T
')


define(`ADDMULEND1',`
	mov 4(%esi),%eax
	mul %edx
	add %ebx,(%edi)
	adc %eax,%ecx
	adc %edx,%ebp
	add %ecx,4(%edi)
	adc `$'0,%ebp
	mov %ebp,8(%edi)
	mov DST,%edi
	decl SIZE2
')

define(`ADDMULSTART0',`
	mov SIZE1,%eax
	lea 4(%ebx),%ebx
	mov %ebx,SRC2
	mov %eax,SIZE1T
	mov SRC1,%esi
	mov DST,%edi
	mov (%ebx),%edx
	mov %edx,MUL
	mov (%esi),%eax
	lea 4(%edi),%edi
	mul %edx
	mov %eax,%ebx
	mov %edi,DST
	mov %edx,%ecx
	mov MUL,%edx
	mov `$'0,%ebp
	subl `$'4,SIZE1T
')

define(`ADDMULEND0',`
	add %ebx,(%edi)
	adc `$'0,%ecx
	mov %ecx,4(%edi)
	mov SRC2,%ebx
	decl SIZE2
')

ASM_START()
PROLOGUE(mpn_mul_basecase)
sub $24,%esp
mov %ebx,SAVE_EBX
mov %ebp,SAVE_EBP
mov %edi,SAVE_EDI
mov %esi,SAVE_ESI
#sub $8,%esp
#push %ebx
#push %ebp
#push %esi
#push %edi
MULSTART
jc skipmullp
ALIGN(16)
mullp:
	MULLOOP
	jnc mullp
skipmullp:
cmpl $-2,SIZE1T
jl case0
je case1
case2:
MULEND2
jz theend
ALIGN(16)
lp2:
ADDMULSTART2
jc skipaddmullp2
ALIGN(16)
addmullp2:
	ADDMULLOOP
	jnc addmullp2
skipaddmullp2:
ADDMULEND2
jnz lp2
theend:
mov SAVE_EBX,%ebx
mov SAVE_EBP,%ebp
mov SAVE_EDI,%edi
mov SAVE_ESI,%esi
add $24,%esp
#pop %edi
#pop %esi
#pop %ebp
#pop %ebx
#add $8,%esp
ret
ALIGN(16)
case1:
MULEND1
jz theend
ALIGN(16)
lp1:
ADDMULSTART1
jc skipaddmullp1
ALIGN(16)
addmullp1:
	ADDMULLOOP
	jnc addmullp1
skipaddmullp1:
ADDMULEND1
jnz lp1
mov SAVE_EBX,%ebx
mov SAVE_EBP,%ebp
mov SAVE_EDI,%edi
mov SAVE_ESI,%esi
add $24,%esp
#pop %edi
#pop %esi
#pop %ebp
#pop %ebx
#add $8,%esp
ret
ALIGN(16)
case0:
MULEND0
jz theend
ALIGN(16)
lp0:
ADDMULSTART0
jc skipaddmullp0
ALIGN(16)
addmullp0:
	ADDMULLOOP
	jnc addmullp0
skipaddmullp0:
ADDMULEND0
jnz lp0
mov SAVE_EBX,%ebx
mov SAVE_EBP,%ebp
mov SAVE_EDI,%edi
mov SAVE_ESI,%esi
add $24,%esp
#pop %edi
#pop %esi
#pop %ebp
#pop %ebx
#add $8,%esp
ret
EPILOGUE()
