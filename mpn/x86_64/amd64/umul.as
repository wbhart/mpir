;  Copyright 1999, 2000, 2002 Free Software Foundation, Inc.
;
;  This file is part of the GNU MP Library.
;
;  The GNU MP Library is free software; you can redistribute it and/or
;  modify it under the terms of the GNU Lesser General Public License as
;  published by the Free Software Foundation; either version 2.1 of the
;  License, or (at your option) any later version.
;
;  The GNU MP Library is distributed in the hope that it will be useful,
;  but WITHOUT ANY WARRANTY; without even the implied warranty of
;  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
;  Lesser General Public License for more details.
;
;  You should have received a copy of the GNU Lesser General Public
;  License along with the GNU MP Library; see the file COPYING.LIB.  If
;  not, write to the Free Software Foundation, Inc., 51 Franklin Street,
;  Fifth Floor, Boston, MA 02110-1301, USA.

;  mpn_umul_ppmm -- 1x1->2 limb multiplication
;
;  Calling interface:
;
;  mp_limb_t mpn_umul_ppmm (
;  mp_limb_t *lowptr,       rdi
;  mp_limb_t m1,            rsi
;  mp_limb_t m2             rdx
;  );

%include '../yasm_mac.inc'

%define low   rdi
%define m1    rsi
%define m2    rdx

    BITS    64

GLOBAL_FUNC mpn_umul_ppmm
    mov     rax,m1
    mul     m2
    mov     [low],rax
    mov     rax,m1
    ret
