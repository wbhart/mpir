; PROLOGUE(mpn_nior_n)
;
;  Copyright 2008 Jason Moxham
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
;  void mpn_nior_n(mp_ptr, mp_srcptr, mp_srcptr, mp_size_t)
;                     rdi        rsi        rdx        rcx
;                     rcx        rdx         r8         r9

%include "yasm_mac.inc"

    CPU  Athlon64
    BITS 64

 %define reg_save_list rbx, rsi, rdi

    FRAME_PROC mpn_nior_n, 0, reg_save_list
	lea     rdi, [rcx+r9*8]
	lea     rsi, [rdx+r9*8]
	lea     rdx, [r8+r9*8]
    mov     rcx, r9
	neg     rcx
	add     rcx, 3
	jc      .2
	mov     r8, [rdx+rcx*8-24]
	mov     r9, [rdx+rcx*8-16]
	mov     r8, [rdx+rcx*8-24]
	mov     r9, [rdx+rcx*8-16]
	add     rcx, 4
	mov     r10, [rdx+rcx*8-40]
	mov     r11, [rdx+rcx*8-32]
	jc      .1
	xalign  16
.0:
	or      r8, [rsi+rcx*8-56]
	not     r8
	or      r9, [rsi+rcx*8-48]
	or      r10, [rsi+rcx*8-40]
	or      r11, [rsi+rcx*8-32]
	mov     [rdi+rcx*8-56], r8
	not     r9
	not     r10
	mov     [rdi+rcx*8-48], r9
	not     r11
	mov     r8, [rdx+rcx*8-24]
	mov     r9, [rdx+rcx*8-16]
	mov     [rdi+rcx*8-40], r10
	mov     [rdi+rcx*8-32], r11
	add     rcx, 4
	mov     r10, [rdx+rcx*8-40]
	mov     r11, [rdx+rcx*8-32]
	jnc     .0
.1:
	or      r8, [rsi+rcx*8-56]
	not     r8
	or      r9, [rsi+rcx*8-48]
	or      r10, [rsi+rcx*8-40]
	or      r11, [rsi+rcx*8-32]
	mov     [rdi+rcx*8-56], r8
	not     r9
	not     r10
	mov     [rdi+rcx*8-48], r9
	not     r11
	mov     [rdi+rcx*8-40], r10
	mov     [rdi+rcx*8-32], r11
.2:
	cmp     rcx, 2
	jg      .6
	je      .5
	jp      .4
.3:	mov     r8, [rdx-24]
	or      r8, [rsi-24]
	not     r8
	mov     [rdi-24], r8
.4:	mov     r8, [rdx-16]
	or      r8, [rsi-16]
	not     r8
	mov     [rdi-16], r8
.5:	mov     r8, [rdx-8]
	or      r8, [rsi-8]
	not     r8
	mov     [rdi-8], r8
.6:	END_PROC reg_save_list

    end
