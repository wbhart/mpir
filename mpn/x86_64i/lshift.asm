;  Copyright 1999, 2000, 2001, 2002 Free Software Foundation, Inc.
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
;  not, write to the Free Software Foundation, Inc., 59 Temple Place -
;  Suite 330, Boston, MA 02111-1307, USA.
;
;  Adapted by Brian Gladman AMD64 using the Microsoft VC++ v8 64-bit
;  compiler and the YASM assembler.

;  AMD64 mpn_lshift -- mpn left shift
;
;  Calling interface:
;
; mp_limb_t mpn_lshift(
;     mp_ptr dst,       rcx
;     mp_srcptr src,    rdx
;     mp_size_t size,    r8
;     unsigned shift     r9
; )
;
;  This is an SEH Leaf Function (no unwind support needed)

%define s_len   r8
%define  r_tmp  r9
%define d_ptr  r10
%define s_ptr  r11

    bits    64
    section .text

    global  __gmpn_lshift

%ifdef DLL
    export  __gmpn_lshift
%endif

%if 1

__gmpn_lshift:
    movsxd  s_len,r8d
    or      s_len,s_len
    jz      .0
    mov     d_ptr,rcx
    mov     s_ptr,rdx
    mov     rcx,r9
    cmp     s_len,byte 2
    jge     .1
    or      s_len,s_len
    mov     rax,[s_ptr]
    mov     r_tmp,rax
    shl     r_tmp,cl
    neg     cl
    mov     [d_ptr],r_tmp
    shr     rax,cl
.0: ret
.1: dec     s_len
    mov     rdx,[s_ptr+s_len*8]
    movq    xmm0, rdx
.2: mov     rax,[s_ptr+s_len*8-8]
    mov     r_tmp,rax
    shl     rdx,cl
    neg     cl
    shr     r_tmp,cl
    neg     cl
    or      r_tmp,rdx
    mov     rdx,rax
    mov     [d_ptr+s_len*8],r_tmp
    dec     s_len
    jnz     .2
    shl     rax,cl
    mov     [d_ptr],rax
    neg     cl
    movq    rax, xmm0
    shr     rax,cl
    ret

%else

__gmpn_lshift:
    movsxd  r8, r8d
    movq    mm7, [rdx+r8*8-8]   ; rdx -> src
    movd    mm1, r9d            ; <<  -> mm1
    mov     eax, 64
    sub     eax, r9d
    movd    mm0, eax            ; >>  -> mm0
    movq    mm3, mm7
    psrlq   mm7, mm0
    movd    rax, mm7
    sub     r8, 2
    jl      .1

    align   4
.0: movq    mm6, [rdx+r8*8]
    movq    mm2, mm6
    psrlq   mm6, mm0
    psllq   mm3, mm1
    por     mm3, mm6
    movq    [rcx+r8*8+8], mm3
    je      .2
    movq    mm7, [rdx+r8*8-8]
    movq    mm3, mm7
    psrlq   mm7, mm0
    psllq   mm2, mm1
    por     mm2, mm7
    movq    [rcx+r8*8], mm2
    sub     r8, 2
    jge     .0
.1: movq    mm2, mm3
.2: psllq   mm2, mm1
    movq    [rcx], mm2
    emms
    ret

%endif

    end
