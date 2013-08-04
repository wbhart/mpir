; PROLOGUE(mpn_not)

;  Copyright 2011 The Code Cavern  
;
;  Windows Conversion Copyright 2011 Brian Gladman
;
;  This file is part of the MPIR Library.
;  The MPIR Library is free software; you can redistribute it and/or modify
;  it under the terms of the GNU Lesser General Public License as published
;  by the Free Software Foundation; either version 2.1 of the License, or (at
;  your option) any later version.
;  The MPIR Library is distributed in the hope that it will be useful, but
;  WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
;  or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public
;  License for more details.
;  You should have received a copy of the GNU Lesser General Public License
;  along with the MPIR Library; see the file COPYING.LIB.  If not, write
;  to the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
;  Boston, MA 02110-1301, USA.
;
;  void mpn_not(mp_ptr, mp_size_t)
;                  rdi        rsi
;                  rcx        rdx

%include "yasm_mac.inc"

        CPU  Athlon64
        BITS 64
        LEAF_PROC mpn_not
        mov     eax, 1
        lea     rcx, [rcx+rdx*8-8]
        sub     rax, rdx
        jnc     .2

        align   16
.1:     not     qword[rcx+rax*8]
        not     qword[rcx+rax*8+8]
        add     rax, 2
        jnc     .1
.2:     jnz     .4
.3:     not     qword[rcx+rax*8]
.4:     ret     

        end
