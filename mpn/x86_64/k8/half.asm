dnl  mpn_half

dnl  Copyright 2011 The Code Cavern

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
PROLOGUE(mpn_half)
mov %rsi,%rax
lea -8(%rdi,%rsi,8),%rdi
shr $2,%rsi
and $3,%eax
jz t1
shrq $1,(%rdi)
lea -8(%rdi),%rdi
dec %rax
jz t1
rcrq $1,(%rdi)
lea -8(%rdi),%rdi
dec %rax
jz t1
rcrq $1,(%rdi)
lea -8(%rdi),%rdi
dec %rax
t1:
sbb %rdx,%rdx
cmp $0,%rsi
jz skiplp
add %rdx,%rdx
.align 16
lp:
  rcrq $1,(%rdi)
  nop
  rcrq $1,-8(%rdi)
  rcrq $1,-16(%rdi)
  rcrq $1,-24(%rdi)
  nop
  dec %rsi
  lea -32(%rdi),%rdi
  jnz lp
sbb %rdx,%rdx
skiplp:
sub %rdx,%rax
shl $63,%rax
ret
EPILOGUE()
