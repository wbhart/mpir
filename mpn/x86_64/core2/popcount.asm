dnl  mpn_popcount

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

C	ret mpn_popcount(mp_ptr,mp_size_t)
C	rax               rdi,   rsi

define(`MOVQ',`movd')

ASM_START()
PROLOGUE(mpn_popcount)
# could store these constants in mem and retune to get the same speed
mov $0x5555555555555555,%rax
MOVQ %rax,%xmm4
movddup %xmm4,%xmm4
mov $0x3333333333333333,%rax
MOVQ %rax,%xmm5
movddup %xmm5,%xmm5
mov $0x0f0f0f0f0f0f0f0f,%rax
MOVQ %rax,%xmm6
movddup %xmm6,%xmm6
pxor %xmm7,%xmm7
pxor %xmm11,%xmm11
pxor %xmm8,%xmm8
# this takes care of an odd address by padding with zeros
btr $3,%rdi		# rdi is even
sbb %rax,%rax		# rax =-1 if was odd
sub %rax,%rsi
MOVQ %rax,%xmm0
pandn (%rdi),%xmm0	# first load padded with zero
# this takes care of odd number of digits by padding with zeros
bt $0,%rsi
sbb %rcx,%rcx
sub %rcx,%rsi		# len is even
MOVQ %rcx,%xmm2
shufpd $1,%xmm2,%xmm2	# swap high/low halfs
pandn -16(%rdi,%rsi,8),%xmm2	# last load padded with zero
# so we have an even addr and an even number of digits 
# and we have loaded up first 2 and last 2 digits
# by chance the general code handles all cases correctly except for
# n=0,1, 2evenaddr
cmp $2,%rsi
jne big
	# so just pad out with zeros
	add $2,%rsi
	MOVQ %rax,%xmm1
	movddup %xmm1,%xmm1
	pand %xmm1,%xmm0
	pandn %xmm2,%xmm1
	movdqa %xmm1,%xmm2
big:
movdqa %xmm0,%xmm1
movdqa %xmm2,%xmm3
sub $8,%rsi
jc skiplp
ALIGN(16)
lp:
	psrlw $1,%xmm0
	pand %xmm4,%xmm0
	psubb %xmm0,%xmm1
		psrlw $1,%xmm2
	movdqa %xmm1,%xmm0
				paddq %xmm8,%xmm11
	psrlw $2,%xmm1
	pand %xmm5,%xmm0
	pand %xmm5,%xmm1
	paddb %xmm0,%xmm1
		pand %xmm4,%xmm2
				sub $4,%rsi
		psubb %xmm2,%xmm3
		movdqa %xmm3,%xmm2
		psrlw $2,%xmm3
		pand %xmm5,%xmm2
		pand %xmm5,%xmm3
		paddb %xmm2,%xmm3
				movdqa 32-32+64(%rdi,%rsi,8),%xmm0
	paddb %xmm1,%xmm3
	movdqa %xmm3,%xmm8
	psrlw $4,%xmm3
	pand %xmm6,%xmm3
				movdqa 32-48+64(%rdi,%rsi,8),%xmm2
	pand %xmm6,%xmm8
				movdqa 32-32+64(%rdi,%rsi,8),%xmm1
	paddb %xmm3,%xmm8
				movdqa 32-48+64(%rdi,%rsi,8),%xmm3
	psadbw %xmm7,%xmm8
	jnc lp
skiplp:
	psrlw $1,%xmm0
	pand %xmm4,%xmm0
	psubb %xmm0,%xmm1
		psrlw $1,%xmm2
	movdqa %xmm1,%xmm0
				paddq %xmm8,%xmm11
	psrlw $2,%xmm1
	pand %xmm5,%xmm0
	pand %xmm5,%xmm1
	paddb %xmm0,%xmm1
		pand %xmm4,%xmm2
		psubb %xmm2,%xmm3
		movdqa %xmm3,%xmm2
		psrlw $2,%xmm3
		pand %xmm5,%xmm2
		pand %xmm5,%xmm3
		paddb %xmm2,%xmm3
	paddb %xmm1,%xmm3
	movdqa %xmm3,%xmm8
	psrlw $4,%xmm3
	pand %xmm6,%xmm3
	pand %xmm6,%xmm8
	paddb %xmm3,%xmm8
	psadbw %xmm7,%xmm8	
cmp $-3,%rsi
jl nomore
onemore:
	movdqa -32+64(%rdi,%rsi,8),%xmm2
	movdqa %xmm2,%xmm3
		psrlw $1,%xmm2
				paddq %xmm8,%xmm11
		pand %xmm4,%xmm2
		psubb %xmm2,%xmm3
		movdqa %xmm3,%xmm2
		psrlw $2,%xmm3
		pand %xmm5,%xmm2
		pand %xmm5,%xmm3
		paddb %xmm2,%xmm3
	movdqa %xmm3,%xmm8
	psrlw $4,%xmm3
	pand %xmm6,%xmm3
	pand %xmm6,%xmm8
	paddb %xmm3,%xmm8
	psadbw %xmm7,%xmm8
nomore:
	paddq %xmm8,%xmm11
MOVQ %xmm11,%rax
shufpd $1,%xmm11,%xmm11
MOVQ %xmm11,%rcx
add %rcx,%rax
ret
EPILOGUE()
