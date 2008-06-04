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
;  Calling interface(WIN64):
;
; mp_limb_t mpn_lshift(
;     mp_ptr dst,       rcx
;     mp_srcptr src,    rdx
;     mp_size_t size,    r8
;     unsigned shift     r9
; )
;
;  Calling interface(linux):
;
; mp_limb_t mpn_lshift(
;     mp_ptr dst,       rdi
;     mp_srcptr src,    rsi
;     mp_size_t size,   rdx
;     unsigned shift    rcx
; )
;
;  This is an SEH Leaf Function (no unwind support needed)

%ifdef _WIN64_ABI
%define s_len   r8
%define s_lend r8d
%define r_tmp   r9
%define r_tmpd r9d
%define d_ptr  r10
%define s_ptr  r11
%define dst    rcx
%define src    rdx
%define s_tmp  rdx
%define shift   r9

%else
%if 0
%define s_len  rdx
%define s_lend edx
%define r_tmp  r10
%define r_tmpd r10d
%define d_ptr  rdi
%define s_ptr  rsi
%define s_tmp  r11
%else
%define src rsi
%define dst rdi
%define s_len rdx
%define r_tmpd ecx
%endif
%endif

    bits    64
    section .text

    global  __gmpn_lshift:function

%ifdef DLL
    export  __gmpn_lshift
%endif

%if 0

__gmpn_lshift:
    movsxd  s_len,s_lend
    or      s_len,s_len
    jz      .0
%ifdef _WIN64_ABI
    mov     d_ptr,dst
    mov     s_ptr,src
    mov     rcx,shift
%endif
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
    mov     s_tmp,[s_ptr+s_len*8] 
    movq    xmm0, s_tmp              ; save value in xmm0
.2: mov     rax,[s_ptr+s_len*8-8]
    mov     r_tmp,rax
    shl     s_tmp,cl
    neg     cl
    shr     r_tmp,cl
    neg     cl
    or      r_tmp,s_tmp
    mov     s_tmp,rax
    mov     [d_ptr+s_len*8],r_tmp
    dec     s_len
    jnz     .2
    shl     rax,cl
    mov     [d_ptr],rax
    neg     cl
    movq    rax,xmm0
    shr     rax,cl
    ret

%else

__gmpn_lshift:
    movq    mm7, [src+s_len*8-8]   ; rdx  -> src
    movd    mm1, r_tmpd            ; <<  -> mm1
    mov     eax, 64
    sub     eax, r_tmpd
    movd    mm0, eax            ; >>  -> mm0
    movq    mm3, mm7
    psrlq   mm7, mm0
    movd   rax, mm7
    sub     s_len, 2
    jl      .1

    align   4
.0: movq    mm6, [src+s_len*8]
    movq    mm2, mm6
    psrlq   mm6, mm0
    psllq   mm3, mm1
    por     mm3, mm6
    movq    [dst+s_len*8+8], mm3
    je      .2
    movq    mm7, [src+s_len*8-8]
    movq    mm3, mm7
    psrlq   mm7, mm0
    psllq   mm2, mm1
    por     mm2, mm7
    movq    [dst+s_len*8], mm2
    sub     s_len, 2
    jge     .0
.1: movq    mm2, mm3
.2: psllq   mm2, mm1
    movq    [dst], mm2
    emms
    ret

%endif

    end
