; PROLOGUE(mpn_popcount)

;  mpn_popcount

;  Copyright 2009 Jason Moxham

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

;	mp_limb_t mpn_popcount(mp_ptr,mp_size_t)
;	rax                       rdi,      rsi
;	rax                       rcx,      rdx

%include "yasm_mac.inc"

    CPU  Core2
    BITS 64

    global  __gmpn_popcount
    
%ifdef DLL
    export  __gmpn_popcount
%endif

    PROC_FRAME __gmpn_popcount
    alloc_stack 0x48
    save_xmm128 xmm6, 0x00
    save_xmm128 xmm7, 0x10
    save_xmm128 xmm8, 0x20
    save_xmm128 xmm9, 0x30
    END_PROLOGUE

	mov     rax, 0x5555555555555555
	movq    xmm4, rax
	movddup xmm4, xmm4
	mov     rax, 0x3333333333333333
	movq    xmm5, rax
	movddup xmm5, xmm5
	mov     rax, 0x0f0f0f0f0f0f0f0f
	movq    xmm6, rax
	movddup xmm6, xmm6
	pxor    xmm7, xmm7
	pxor    xmm9, xmm9
	pxor    xmm8, xmm8
	btr     rcx, 3
	sbb     rax, rax
	sub     rdx, rax
	movq    xmm0, rax
	pandn   xmm0, [rcx]
	bt      rdx, 0
	sbb     r8, r8
	sub     rdx, r8
	movq    xmm2, r8
	shufpd  xmm2, xmm2, 1
	pandn   xmm2, [rcx+rdx*8-16]
	cmp     rdx, 2
	jne     .0
	add     rdx, 2
	movq    xmm1, rax
	movddup xmm1, xmm1
	pand    xmm0, xmm1
	pandn   xmm1, xmm2
	movdqa  xmm2, xmm1
.0: movdqa  xmm1, xmm0
	movdqa  xmm3, xmm2
	sub     rdx, 8
	jc      .2
	
    xalign  16
.1: psrlw   xmm0, 1
	pand    xmm0, xmm4
	psubb   xmm1, xmm0
	psrlw   xmm2, 1
	movdqa  xmm0, xmm1
	paddq   xmm9, xmm8
	psrlw   xmm1, 2
	pand    xmm0, xmm5
	pand    xmm1, xmm5
	paddb   xmm1, xmm0
	pand    xmm2, xmm4
	sub     rdx, 4
	psubb   xmm3, xmm2
	movdqa  xmm2, xmm3
	psrlw   xmm3, 2
	pand    xmm2, xmm5
	pand    xmm3, xmm5
	paddb   xmm3, xmm2
	movdqa  xmm0, [rcx+rdx*8+32-32+64]
	paddb   xmm3, xmm1
	movdqa  xmm8, xmm3
	psrlw   xmm3, 4
	pand    xmm3, xmm6
	movdqa  xmm2, [rcx+rdx*8+32-48+64]
	pand    xmm8, xmm6
	movdqa  xmm1, [rcx+rdx*8+32-32+64]
	paddb   xmm8, xmm3
	movdqa  xmm3, [rcx+rdx*8+32-48+64]
	psadbw  xmm8, xmm7
	jnc     .1
.2: psrlw   xmm0, 1
	pand    xmm0, xmm4
	psubb   xmm1, xmm0
	psrlw   xmm2, 1
	movdqa  xmm0, xmm1
	paddq   xmm9, xmm8
	psrlw   xmm1, 2
	pand    xmm0, xmm5
	pand    xmm1, xmm5
	paddb   xmm1, xmm0
	pand    xmm2, xmm4
	psubb   xmm3, xmm2
	movdqa  xmm2, xmm3
	psrlw   xmm3, 2
	pand    xmm2, xmm5
	pand    xmm3, xmm5
	paddb   xmm3, xmm2
	paddb   xmm3, xmm1
	movdqa  xmm8, xmm3
	psrlw   xmm3, 4
	pand    xmm3, xmm6
	pand    xmm8, xmm6
	paddb   xmm8, xmm3
	psadbw  xmm8, xmm7
	cmp     rdx, -3
	jl      .4
.3: movdqa  xmm2, [rcx+rdx*8-32+64]
	movdqa  xmm3, xmm2
	psrlw   xmm2, 1
	paddq   xmm9, xmm8
	pand    xmm2, xmm4
	psubb   xmm3, xmm2
	movdqa  xmm2, xmm3
	psrlw   xmm3, 2
	pand    xmm2, xmm5
	pand    xmm3, xmm5
	paddb   xmm3, xmm2
	movdqa  xmm8, xmm3
	psrlw   xmm3, 4
	pand    xmm3, xmm6
	pand    xmm8, xmm6
	paddb   xmm8, xmm3
	psadbw  xmm8, xmm7
.4:     paddq   xmm9, xmm8
	movq    rax, xmm9
	shufpd  xmm9, xmm9, 1
	movq    r8, xmm9
	add     rax, r8
.5: movdqa  xmm6, [rsp+0x00]
    movdqa  xmm7, [rsp+0x10]
    movdqa  xmm8, [rsp+0x20]
    movdqa  xmm9, [rsp+0x30]
    add     rsp, 0x48
    ret
    ENDPROC_FRAME

    end
