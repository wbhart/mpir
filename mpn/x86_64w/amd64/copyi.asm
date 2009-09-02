
;  Copyright 2009 Jason Moxham
;
;  Windows Conversion Copyright 2008 Brian Gladman
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
;  void mpn_copyi(mp_ptr, mp_ptr, mp_size_t)
;                    rdi     rsi        rdx
;                    rcx     rdx        r8d 

%include "..\yasm_mac.inc"

    CPU  Athlon64
    BITS 64

;	mpn_copyi(mp_ptr rcx ,mp_ptr rdx ,mp_size_t r8)
;	(rcx,r8)=(rcx,r8)

	LEAF_PROC mpn_copyi
	movsxd  r8, r8d
	sub     r8, 4
	jl      L_skiplp
	
	xalign  16
L_lp:
	mov     rax, [rdx]
	mov     r9, [rdx+8]
	sub     r8, 4
	mov     r10, [rdx+16]
	mov     r11, [rdx+24]
	lea     rcx, [rcx+32]
	mov     [rcx-32], rax
	mov     [rcx-24], r9
	mov     [rcx-16], r10
	lea     rdx, [rdx+32]
	mov     [rcx-8], r11
	jns     L_lp
L_skiplp:
	add     r8, 2
	jz      L_case2
	jns     L_case3
	jp      L_case1
L_case0:
	ret
	
	xalign  16
L_case1:
	mov     rax, [rdx]
	mov     [rcx], rax
	ret
	
	xalign  16
L_case2:
	mov     rax, [rdx]
	mov     r9, [rdx+8]
	mov     [rcx], rax
	mov     [rcx+8], r9
	ret
	
	xalign  16
L_case3:
	mov     rax, [rdx]
	mov     r9, [rdx+8]
	mov     r10, [rdx+16]
	mov     [rcx], rax
	mov     [rcx+8], r9
	mov     [rcx+16], r10
	ret
	end
