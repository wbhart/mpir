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
;  Calling interface (WIN64):
;
;  mp_limb_t mpn_umul_ppmm (
;  mp_limb_t *lowptr,       rcx
;  mp_limb_t m1,            rdx
;  mp_limb_t m2              r8
;  );
;
;  Calling interface (linux):
;
;  mp_limb_t mpn_umul_ppmm (
;  mp_limb_t *lowptr,       rdi
;  mp_limb_t m1,            rsi
;  mp_limb_t m2             rdx
;  );

%include '../yasm_mac.inc'

%ifdef _WIN64_ABI
%define low   rcx
%define m1    rdx
%define m2     r8

%else
%define low   rdi
%define m1    rsi
%define m2   rdx
%endif  

    bits    64
    section .text

    G_EXPORT __gmpn_umul_ppmm

%ifdef DLL
    export  __gmpn_umul_ppmm
%endif

G_LABEL __gmpn_umul_ppmm
    mov     rax,m1
    mul     m2
    mov     [low],rax
    mov     rax,m1
    ret

    end
