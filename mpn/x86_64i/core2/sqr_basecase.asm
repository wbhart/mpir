;  Copyright 2001, 2002 Free Software Foundation, Inc.
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
;
; CREDITS
;
; The code used here is derived from that provided by ct35z at:
;
;    http://www.geocities.jp/ct35z/gmp-core2-en.html
;
; This code is based largely on Pierrick Gaudry's excellent assembly
; support for the AMD64 architecture.  (Note that Intel64 and AMD64,
; while using the same instruction set, have very different
; microarchitectures.  So, this code performs very poorly on AMD64
; machines even though it is near-optimal on Intel64.)
;
; Roger Golliver works for Intel and provided insightful improvements
; particularly in using the "lea" instruction to perform additions
; and register-to-register moves.
;
; Jason Worth Martin's excellent assembly support for the Intel64
; architecture has been used where appropriate.
;
; Eric Bainville has a brilliant exposition of optimizing arithmetic for
; AMD64 (http://www.bealto.it).  I adapted many of the ideas he
; describes to Intel64.
;
; Agner Fog is a demigod in the x86 world.  If you are reading assembly
; code files and you haven't heard of Agner Fog, then take a minute to
; look over his software optimization manuals (http://www.agner.org/).
; They are superb.
;
; Adapted for use with VC++ and YASM using a special mode in which NASM
; preprocessing is used with AT&T assembler syntax. I am very grateful
; for the support that Peter Johnson (one of the authors of YASM) has
; provided in getting this special YASM mode working.  Without his
; support this port would have been a great deal more difficult.
;
; The principle issues that I have had to address is the difference
; between GCC and MSVC in their register saving and parameter passing
; conventions.  Registers that have to be preserved across function
; calls are:
;
; GCC:             rbx, rbp, r12..r15
; MSVC:  rsi, rdi, rbx, rbp, r12..r15 xmm6..xmm15
;
; Parameter passing conventions for non floating point parameters:
;
;   function(   GCC     MSVC
;       p1,     rdi      rcx
;       p2,     rsi      rdx
;       p3,     rdx       r8
;       p4,     rcx       r9
;       p5,      r8 [rsp+40]
;       p6,      r9 [rsp+48]
;
; Care must be taken with 32-bit values in 64-bit register or on the
; stack because the upper 32-bits of such parameters are undefined.
;
;       Brian Gladman
;
;
; Intel64 mpn_sqr_basecase -- square an mpn number.
;
;  void mpn_sqr_basecase(  MSVC  GCC
;     mp_ptr dst,           rcx  rdi
;     mp_srcptr src,        rdx  rsi
;     mp_size_t size         r8  rdx
;  )
;
; BPL is bytes per limb, which is 8 in the 64-bit code here

%define BPL                 8
%define UNROLL_EXPONENT     4
%define UNROLL_SIZE         (1 << UNROLL_EXPONENT)
%define UNROLL_MASK         (UNROLL_SIZE - 1)

; variables in main   prologue
%define s2limb  rcx ; rdi
%define s1p     rsi ;  r8
%define rp      rdi ; rcx
%define a_x      r8 ; rsi
%define a_y      r9
%define a_z     r10
%define b_x     r11
%define b_y     r12
%define b_z     r13
%define temp    r14
%define index   r15

%define PARAM_SIZE  %xmm2

    bits    64
    section .text
    align   16
    global  __gmpn_sqr_basecase

%ifdef DLL
    export  __gmpn_sqr_basecase
%endif

__gmpn_sqr_basecase:
    xor     rax, rax
    mov     eax, r8d
    mov     r8, rdx
    mov     rdx, rax
    mov     rax, [r8]
    cmp     rdx, 2
    je     .0
    ja     .1
    mul     rax         ; 1 limb
    mov     [rcx], rax
    mov     [rcx+8], rdx
    ret

.0: mul     rax         ; 2 limbs
    mov     [rcx], rax
    mov     rax, [r8+8]
    mov     a_y, rdx
    mul     rax
    mov     a_z, rax
    mov     rax, [r8]
    mov     b_x, rdx
    mul     qword [r8+8]
    add     a_y, rax
    adc     a_z, rdx
    adc     b_x, 0
    add     rax, a_y
    adc     rdx, a_z
    adc     b_x, 0
    mov     [rcx+8], rax
    mov     [rcx+24], b_x
    mov     [rcx+16], rdx
    ret

.1: cmp     rdx, 4
    jae     sqr_basecase
    mul     rax         ; 3 limbs
    mov     [rcx], rax
    mov     [rcx+8], rdx
    mov     rax, [r8+8]
    xor     b_x, b_x
    mul     rax
    mov     [rcx+16], rax
    mov     [rcx+24], rdx
    mov     rax, [r8+16]
    mul     rax
    mov     [rcx+32], rax
    mov     [rcx+40], rdx
    mov     rax, [r8]
    mul     qword [r8+8]
    mov     a_y, rax
    mov     a_z, rdx
    mov     rax, [r8]
    mul     qword [r8+16]
    add     a_z, rax
    mov     b_x, rdx
    adc     b_x, 0
    mov     rax, [r8+8]
    mul     qword [r8+16]
    xor     r8, r8
    add     b_x, rax
    adc     rdx, 0
    add     a_y, a_y
    adc     a_z, a_z
    adc     b_x, b_x
    adc     rdx, rdx
    mov     rax, [rcx+8]
    adc     r8, 0
    add     rax, a_y
    mov     [rcx+8], rax
    mov     rax, [rcx+16]
    adc     rax, a_z
    mov     a_y, [rcx+24]
    adc     a_y, b_x
    mov     a_z, [rcx+32]
    mov     [rcx+16], rax
    mov     [rcx+24], a_y
    adc     a_z, rdx
    mov     rax, [rcx+40]
    mov     [rcx+32], a_z
    adc     rax, r8
    mov     [rcx+40], rax
    ret

PROC_FRAME sqr_basecase
    push_reg    rsi
    push_reg    rdi
    push_reg    rbx
    push_reg    r12
    push_reg    r13
    push_reg    r14
    push_reg    r15
END_PROLOGUE
    mov     rdi, rcx
    mov     rsi, r8
    
    movq    xmm2, rdx
    add     s1p, 8
    add     rp, 8
    sub     rdx, 1
    mov     s2limb, rax
    lea     s1p, [s1p+rdx*8]
    lea     rp, [rp+rdx*8]
    xor     index, index
    sub     index, rdx
    cmp     rdx, 4
    jge     main_loop
    lea     rax, [rel jt1]
    add     rax, [rax+rdx*8]
    jmp     rax

    align   8
jt1:dq      .0 - jt1
    dq      .1 - jt1
    dq      .2 - jt1
    dq      .3 - jt1

.0: xor     rax, rax
    jmp     l1_exit

.1: mov     rax, [s1p+index*8]
    mul     s2limb
    mov     [rp+index*8], rax
    mov     rax, rdx
    mov     [rp+index*8+8], rax
    jmp     l1_exit

.2: mov     rax, [s1p+index*8]
    mul     s2limb
    mov     a_x, rax
    mov     a_y, rdx
    mov     rax, [s1p+index*8+8]
    mul     s2limb
    add     rax, a_y
    mov     [rp+index*8], a_x
    adc     rdx, 0
    mov     [rp+index*8+8], rax
    mov     rax, rdx
    mov     [rp+index*8+16], rax
    jmp     l1_exit

.3: mov     rax, [s1p+index*8]
    mul     s2limb
    mov     a_x, rax
    mov     a_y, rdx
    mov     rax, [s1p+index*8+8]
    mul     s2limb
    mov     b_x, rax
    mov     b_y, rdx
    mov     rax, [s1p+index*8+16]
    mul     s2limb
    add     b_x, a_y
    mov     [rp+index*8], a_x
    adc     b_y, 0
    add     rax, b_y
    mov     [rp+index*8+8], b_x
    adc     rdx, 0
    mov     [rp+index*8+16], rax
    mov     [rp+index*8+24], rdx
    jmp     l1_exit

main_loop:
    mov     temp, rdx
    test    rdx, 1
    jz      .1
    mov     rax, [s1p+index*8]
    mul     s2limb
    mov     a_x, rax
    mov     a_y, rdx
    mov     rax, [s1p+index*8+8]
    mul     s2limb
    mov     b_x, rax
    mov     b_y, rdx
    jmp     .2

.1: mov     rax, [s1p+index*8]
    mul     s2limb
    mov     b_x, rax
    mov     b_y, rdx
    mov     rax, [s1p+index*8+8]
    mul     s2limb
    mov     a_x, rax
    mov     a_y, rdx

.2: sub     temp, 4
    and     temp, UNROLL_MASK
    inc     temp
    mov     rax, (l1_end - l1_start) >> UNROLL_EXPONENT
    mul     temp
    lea     rdx, [rel l1_end]
    sub     rdx, rax
    mov     rax, [s1p+index*8+16]
    lea     index, [index+temp+3-UNROLL_SIZE]
    jmp     rdx

%macro seq_1 6
    mul     s2limb
    mov     %3, %1
    lea     %1, [rax]
    mov     rax, [byte s1p+index*8+8*%6]
    add     %4, %2
    mov     [byte rp+index*8+8*(%6-3)], %3
    lea     %2, [rdx]
    adc     %5, 0
%endmacro

    align   16
l1_start:
%assign i 0
%rep 16
%if (i & 1)
    seq_1 b_x, b_y, b_z, a_x, a_y, i
%else
    seq_1 a_x, a_y, a_z, b_x, b_y, i
%endif
%assign i i + 1
%endrep
l1_end:
    add     index, UNROLL_SIZE
    jnz     l1_start

l1_finish:
    mul     s2limb
    mov     [rp+index*8-24], a_x
    add     b_x, a_y
    adc     b_y, 0
    mov     [rp+index*8-16], b_x
    add     rax, b_y
    adc     rdx, 0
    mov     [rp+index*8-8], rax
    mov     [rp+index*8], rdx

l1_exit:
    movq     rbx, xmm2
    sub     rbx, 4
    jz      outer_end
    neg     rbx

    align   16
outer_top:
    mov     edx, 2
    sub     rdx, rbx
    mov     s2limb, [s1p+rbx*8-24]
    add     rp, 8
    xor     index, index
    sub     index, rdx
    cmp     rdx, 4
    jge     loop_2
    lea     rax, [rel .0]
    add     rax, [rax+rdx*8]
    jmp     rax

   align 8
.0: dq      .1 - .0
    dq      .2 - .0
    dq      .3 - .0
    dq      .4 - .0

.1: xor     rax, rax
    jmp     l2_exit

.2: mov     rax, [s1p+index*8]
    mul     s2limb
    mov     a_z, [rp+index*8]
    add     a_z, rax
    mov     rax, 0
    mov     [rp+index*8], a_z
    adc     rax, rdx
    mov     [rp+index*8+8], rax
    jmp     l2_exit

.3: mov     rax, [s1p+index*8]
    mul     s2limb
    mov     a_z, [rp+index*8]
    mov     a_x, rax
    mov     a_y, rdx
    mov     rax, [s1p+index*8+8]
    mul     s2limb
    mov     b_z, [rp+index*8+8]
    add     a_z, a_x
    adc     rax, a_y
    mov     [rp+index*8], a_z
    adc     rdx, 0
    add     b_z, rax
    mov     rax, 0
    mov     [rp+index*8+8], b_z
    adc     rax, rdx
    mov     [rp+index*8+16], rax
    jmp     l2_exit

.4: mov     rax, [s1p+index*8]
    mul     s2limb
    mov     a_z, [rp+index*8]
    mov     a_x, rax
    mov     a_y, rdx
    mov     rax, [s1p+index*8+8]
    mul     s2limb
    mov     b_z, [rp+index*8+8]
    mov     b_x, rax
    mov     b_y, rdx
    mov     rax, [s1p+index*8+16]
    mul     s2limb
    add     a_z, a_x
    adc     b_x, a_y
    mov     [rp+index*8], a_z
    mov     a_z, [rp+index*8+16]
    adc     b_y, 0
    add     b_z, b_x
    adc     rax, b_y
    mov     [rp+index*8+8], b_z
    adc     rdx, 0
    add     a_z, rax
    mov     rax, 0
    mov     [rp+index*8+16], a_z
    adc     rax, rdx
    mov     [rp+index*8+24], rax
    jmp     l2_exit

loop_2:
    mov     temp, rdx
    test    rdx, 1
    jz      .1
    mov     rax, [s1p+index*8]
    mul     s2limb
    mov     a_z, [rp+index*8]
    mov     a_x, rax
    mov     a_y, rdx
    mov     rax, [s1p+index*8+8]
    mul     s2limb
    mov     b_z, [rp+index*8+8]
    mov     b_x, rax
    mov     b_y, rdx
    jmp     .2

.1: mov     rax, [s1p+index*8]
    mul     s2limb
    mov     b_z, [rp+index*8]
    mov     b_x, rax
    mov     b_y, rdx
    mov     rax, [s1p+index*8+8]
    mul     s2limb
    mov     a_z, [rp+index*8+8]
    mov     a_x, rax
    mov     a_y, rdx

.2: sub     temp, 4
    and     temp, UNROLL_MASK
    inc     temp
    mov     rax, (l2_end - l2_start) >> UNROLL_EXPONENT
    mul     temp
    lea     rdx, [rel l2_end]
    sub     rdx, rax
    mov     rax, [s1p+index*8+16]
    lea     index, [index+temp+3-UNROLL_SIZE]
    jmp     rdx

%macro seq_2 6
    mul     s2limb
    add     %3, %1
    lea     %1, [rax]
    mov     rax, [byte s1p+index*8+8*%6]
    adc     %4, %2
    mov     [byte rp+index*8+8*(%6-3)], %3
    mov     %3, [byte rp+index*8+8*(%6-1)]
    lea     %2, [rdx]
    adc     %5, 0
%endmacro

    align   16
l2_start
%assign  i 0
%rep 16
%if (i & 1)
    seq_2 b_x, b_y, b_z, a_x, a_y, i
%else
    seq_2 a_x, a_y, a_z, b_x, b_y, i
%endif
%assign i i + 1
%endrep
l2_end:
    add     index, UNROLL_SIZE
    jnz     l2_start

    mul     s2limb
    add     a_z, a_x
    mov     [rp+index*8-24], a_z
    mov     a_z, [rp+index*8-8]
    adc     b_x, a_y
    adc     b_y, 0
    add     b_z, b_x
    mov     [rp+index*8-16], b_z
    adc     rax, b_y
    adc     rdx, 0
    add     a_z, rax
    mov     rax, 0
    mov     [rp+index*8-8], a_z
    adc     rax, rdx
    mov     [rp+index*8], rax

l2_exit:
    inc     rbx
    jnz     outer_top

outer_end:
    mov     rax, [s1p-24]
    mul     qword [s1p-16]
    add     [rp-8], rax
    mov     rax, [s1p-24]
    mov     rbx, 0
    adc     rbx, rdx
    mul     qword [s1p-8]
    add     rbx, rax
    mov     rax, [s1p-16]
    adc     rdx, 0
    add     [rp], rbx
    mov     rbx, 0
    adc     rbx, rdx
    mul     qword [s1p-8]
    movq     s2limb, xmm2
    add     rax, rbx
    adc     rdx, 0
    mov     [rp+8], rax
    mov     [rp+16], rdx
    lea     s2limb, [s2limb+s2limb-2]
    add     rp, 16
    neg     s2limb
    mov     a_x, s2limb
    xor     rax, rax

.0: rcl     qword [rp+s2limb*8+8], 1
    rcl     qword [rp+s2limb*8+16], 1
    lea     s2limb, [s2limb+2]
    jrcxz   .1
    jmp     .0
.1: adc     rax, rax
    mov     [rp+8], rax
    mov     rax, [s1p+a_x*4-8]
    mul     rax
    mov     [rp+a_x*8], rax

.2: mov     rax, [s1p+a_x*4]
    mov     rbx, rdx
    mul     rax
    add     [rp+a_x*8+8], rbx
    adc     [rp+a_x*8+16], rax
    adc     rdx, 0
    add     a_x, 2
    jnz     .2
    add     [rp+08], rdx

    add     rsp, 0
    pop     r15
    pop     r14
    pop     r13
    pop     r12
    pop     rbx
    pop     rdi
    pop     rsi
    ret
ENDPROC_FRAME

    end
