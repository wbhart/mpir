
;  AMD64 mpn_add_n -- mpn add or subtract
;  Version 1.0.3.
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
;  Calling interface:
;
;  mp_limb_t __gmpn_<op>_n(    <op> = add OR sub
;     mp_ptr dst,              rcx
;     mp_srcptr src1,          rdx
;     mp_srcptr src2,           r8
;     mp_size_t  len            r9
;  )
;
;  mp_limb_t __gmpn_<op>_nc(   <op> = add OR sub
;     mp_ptr dst,              rcx
;     mp_srcptr src1,          rdx
;     mp_srcptr src2,           r8
;     mp_size_t len,            r9
;     mp_limb_t carry   [rsp+0x28]
;  )
;
;  Calculate src1[size] plus(minus) src2[size] and store the result in
;  dst[size].  The return value is the carry bit from the top of the result
;  (1 or 0).  The _nc version accepts 1 or 0 for an initial carry into the
;  low limb of the calculation.  Note values other than 1 or 0 here will
;  lead to garbage results.
;
;  This is an SEH leaf function (no unwind support needed)

%include "..\yasm_mac.inc"

    BITS 64

    alignb  8, nop
    LEAF_PROC mpn_add_nc
    mov     r10,[rsp+0x28]
    jmp     entry

    alignb  8, nop
    LEAF_PROC mpn_add_n
    xor     r10, r10
entry:
    mov     eax, r9d
    mov     r9, rax
    and     rax, 3
    shr     r9, 2
    lea     r9,[r10+r9*2]
    shr     r9, 1
    jnz     .2

    mov     r10, [rdx]
    adc     r10, [r8]
    mov     [rcx], r10
    dec     rax
    jz      .1
    mov     r10, [rdx+8]
    adc     r10, [r8+8]
    mov     [rcx+8], r10
    dec     rax
    jz      .1
    mov     r10, [rdx+16]
    adc     r10, [r8+16]
    mov     [rcx+16], r10
    dec     rax
.1: adc     rax, rax
    ret

    alignb  8, nop
.2: mov     r10, [rdx]
    mov     r11, [rdx+8]
    lea     rdx, [rdx+32]
    adc     r10, [r8]
    adc     r11, [r8+8]
    lea     r8, [r8+32]
    mov     [rcx], r10
    mov     [rcx+8], r11
    lea     rcx, [rcx+32]
    mov     r10, [rdx-16]
    mov     r11, [rdx-8]
    adc     r10, [r8-16]
    adc     r11, [r8-8]
    mov     [rcx-16], r10
    dec     r9
    mov     [rcx-8], r11
    jnz     .2

    inc     rax
    dec     rax
    jz      .3
    mov     r10, [rdx]
    adc     r10, [r8]
    mov     [rcx], r10
    dec     rax
    jz      .3
    mov     r10, [rdx+8]
    adc     r10, [r8+8]
    mov     [rcx+8], r10
    dec     rax
    jz      .3
    mov     r10, [rdx+16]
    adc     r10, [r8+16]
    mov     [rcx+16], r10
    dec     rax
.3: adc     rax, rax
    ret

    end
