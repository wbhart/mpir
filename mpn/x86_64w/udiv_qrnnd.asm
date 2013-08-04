; PROLOGUE(mpn_udiv_qrnnd)

;  Copyright 2008 Brian Gladman
;
;  This file is part of the MPIR Library.
;
;  The MPIR Library is free software; you can redistribute it and/or
;  modify it under the terms of the GNU Lesser General Public License as
;  published by the Free Software Foundation; either version 2.1 of the
;  License, or (at your option) any later version.
;
;  The MPIR Library is distributed in the hope that it will be useful,
;  but WITHOUT ANY WARRANTY; without even the implied warranty of
;  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
;  Lesser General Public License for more details.
;
;  You should have received a copy of the GNU Lesser General Public
;  License along with the MPIR Library; see the file COPYING.LIB.  If
;  not, write to the Free Software Foundation, Inc., 51 Franklin Street,
;  Fifth Floor, Boston, MA 02110-1301, USA.
;
;  mp_limb_t mpn_udiv_qrnnd(mp_ptr, mp_limb_t, mp_limb_t, mp_limb_t)
;  rax                         rdi        rsi        rdx        rcx
;  rax                         rcx        rdx         r8         r9

%include "yasm_mac.inc"

    BITS 64

    LEAF_PROC mpn_udiv_qrnnd
    mov     rax,r8
    div     r9
    mov     [rcx],rdx
    ret

    end
