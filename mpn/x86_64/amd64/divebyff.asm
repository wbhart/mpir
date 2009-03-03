dnl  AMD64 mpn_divexact_byff

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

C	(rdi,rdx)=(rsi,rdx)/0xFFFFFFFFFFFFFFFF
C	rax="remainder"

C	where (rsi,rdx) = (rdi,rdx)*(B-1) -rax*B^rdx    and 0 <= rax < B-1      B=0xFFFFFFFFFFFFFFFF

ASM_START()
PROLOGUE(mpn_divexact_byff)
C this is good but suffers from alignment slowdown
C we dont seem to have much freedom to re-arrange the instructions to avoid
C it , I suppose we could detect alignment at the start and have different
C routines for different alignments
xor %eax,%eax
mov %rdx,%rcx
and $3,%rcx
shr $2,%rdx
cmp $0,%rdx
C carry flag is clear here
jnz loop
sbb (%rsi),%rax
mov %rax,(%rdi)
dec %rcx
jz  end1
sbb 8(%rsi),%rax
mov %rax,8(%rdi)
dec %rcx
jz  end1
sbb 16(%rsi),%rax
mov %rax,16(%rdi)
dec %rcx
end1:
sbb $0,%rax
ret
ALIGN(16)
loop:
	sbb (%rsi),%rax
	mov %rax,(%rdi)
	sbb 8(%rsi),%rax
	mov %rax,8(%rdi)
	sbb 16(%rsi),%rax
	mov %rax,16(%rdi)
	sbb 24(%rsi),%rax
	mov %rax,24(%rdi)
	lea 32(%rsi),%rsi
	dec %rdx
	lea 32(%rdi),%rdi
	jnz loop
inc %rcx
dec %rcx
jz  end
sbb (%rsi),%rax
mov %rax,(%rdi)
dec %rcx
jz  end
sbb 8(%rsi),%rax
mov %rax,8(%rdi)
dec %rcx
jz  end
sbb 16(%rsi),%rax
mov %rax,16(%rdi)
dec %rcx
end:
sbb $0,%rax
ret
EPILOGUE()
