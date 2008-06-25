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

;  x86 mpn_udiv_qrnnd -- 2 by 1 limb division
;
;  Calling interface (WIN64):
;
;  mp_limb_t mpn_udiv_qrnnd (
;  mp_limb_t *remptr,           rcx
;  mp_limb_t high,              rdx
;  mp_limb_t low,                r8
;  mp_limb_t divisor             r9
;  );
;
;  Calling interface (linux):
;
;  mp_limb_t mpn_udiv_qrnnd (
;  mp_limb_t *remptr,           rdi
;  mp_limb_t high,              rsi
;  mp_limb_t low,               rdx
;  mp_limb_t divisor            rcx
;  );

%include '../yasm_mac.inc'

%ifdef _WIN64_ABI
%define rem   rcx
%define high  rdx
%define low    r8
%define dv     r9

%else
%define rem   rdi
%define high  rsi
%define low   rdx
%define dv    rcx
%endif  

    bits    64
    section .text

    G_EXPORT __gmpn_udiv_qrnnd

%ifdef DLL
    export  __gmpn_udiv_qrnnd
%endif

G_LABEL __gmpn_udiv_qrnnd
    mov     rax,low
    div     dv
    mov     [rem],high
    ret

    end

