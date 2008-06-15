;  Copyright 2000, 2001, 2002, 2003, 2004, 2005, 2006 Free Software
;  Foundation, Inc.
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
;               cycles/limb
; Hammer:         10
; Prescott/Nocona:   33

; mp_limb_t mpn_modexact_1_odd (mp_srcptr src, mp_size_t size,
;                               mp_limb_t divisor);
; mp_limb_t mpn_modexact_1c_odd (mp_srcptr src, mp_size_t size,
;                                mp_limb_t divisor, mp_limb_t carry);
;
; The dependent chain in the main loop is
;
;                            cycles
; subq   %r8, %rax   1
; imulq  %r9, %rax   4
; mulq   %rsi      5
;              ----
; total            10
;
; The movq load from src seems to need to be scheduled back before the jz to
; achieve this speed, out-of-order execution apparently can't completely
; hide the latency otherwise.
;
; The l=src[i]-cbit step is rotated back too, since that allows us to avoid
; it for the first iteration (where there's no cbit).
;
; The code alignment used (32-byte) for the loop also seems necessary.
; Without that the non-PIC case has adcq crossing the 0x60 offset,
; apparently making it run at 11 cycles instead of 10.
;
; Not done:
;
; divq for size==1 was measured at about 79 cycles, compared to the inverse
; at about 25 cycles (both including function call overheads), so that's not
; used.
;
; Enhancements:
;
; For PIC, we shouldn't really need the GOT fetch for modlimb_invert_table,
; it'll be in rodata or text in libgmp.so and can be accessed directly %rip
; relative.  This would be for small model only (something we don't
; presently detect, but which is all that gcc 3.3.3 supports), since 8-byte
; PC-relative relocations are apparently not available.  Some rough
; experiments with binutils 2.13 looked worrylingly like it might come out
; with an unwanted text segment relocation though, even with ".protected".

;  AMD64 mpn_modexact_1_odd -- exact division style remainder.
;
; mp_limb_t mpn_modexact_1_odd (
;  mp_srcptr src,           rdi
;  mp_size_t size,          rsi
;  mp_limb_t divisor        rdx
; );
; mp_limb_t mpn_modexact_1c_odd (
;  mp_srcptr src,           rdi
;  mp_size_t size,          rsi
;  mp_limb_t divisor,       rdx
;  mp_limb_t carry          rcx
; );
;

%include '../yasm_mac.inc'

    bits    64
    section .text
    align   32

    G_EXPORT __gmpn_modexact_1_odd
    G_EXPORT __gmpn_modexact_1c_odd
    G_EXTERN __gmp_modlimb_invert_table

G_LABEL __gmpn_modexact_1_odd
    mov      ecx, 0       ; carry

G_LABEL __gmpn_modexact_1c_odd
    mov      r8, rdx
    shr      edx, 1

%ifdef PIC
    mov      r9, [__gmp_modlimb_invert_table wrt rip wrt ..gotpcrel]
%else
    lea      r9, [__gmp_modlimb_invert_table wrt rip]
%endif

    and      edx, 127
    mov      r10, rcx

    movzx    edx, byte [rdx+r9]
    
    mov      rax, [rdi]
    lea      r11, [rdi+rsi*8]
    mov      rdi, r8

    lea      ecx, [rdx+rdx] 
    imul     rdx, rdx

    neg      rsi
 
    imul     edx, edi

    sub      ecx, edx
    
    lea      edx, [rcx+rcx]
    imul     ecx, ecx

    imul     ecx, edi

    sub      edx, ecx
    xor      ecx, ecx

    lea      r9, [rdx+rdx]
    imul     rdx, rdx

    imul     rdx, r8

    sub      r9, rdx
    mov      rdx, r10

 ; According to Brian Gladman, the
 ; following three lines are the
 ; remnant of a "dead" assert
 ; and so can be omitted 
 
    ;mov      r10, r8
    ;imul     r10, r9
    ;cmp      r10, 1

    inc      rsi
    jz       .1
    
    align    16
.0: 
    sub      rax, rdx
    
    adc      rcx, 0
    imul     rax, r9

    mul      r8

    mov      rax, [r11+rsi*8]
    sub      rax, rcx
    setc     cl

    inc      rsi
    jnz      .0

.1:
    sub      rax, rdx
   
    adc      rcx, 0
    imul     rax, r9

    mul      r8
    lea      rax, [rcx+rdx]
    ret

    end
