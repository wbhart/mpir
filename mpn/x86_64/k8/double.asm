dnl  mpn_double

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
PROLOGUE(mpn_double)
mov %rsi,%rax
shr $2,%rsi
and $3,%eax
jz t1
shlq $1,(%rdi)
lea 8(%rdi),%rdi
dec %rax
jz t1
rclq $1,(%rdi)
lea 8(%rdi),%rdi
dec %rax
jz t1
rclq $1,(%rdi)
lea 8(%rdi),%rdi
dec %rax
t1:
sbb %rdx,%rdx
cmp $0,%rsi
jz skiplp
add %rdx,%rdx
.align 16
lp:
  rclq $1,(%rdi)
  nop
  rclq $1,8(%rdi)
  rclq $1,16(%rdi)
  rclq $1,24(%rdi)
  nop
  dec %rsi
  lea 32(%rdi),%rdi
  jnz lp
sbb %rdx,%rdx
skiplp:
sub %rdx,%rax
ret
EPILOGUE()
