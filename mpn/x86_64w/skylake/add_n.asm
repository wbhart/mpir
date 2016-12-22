
;  AMD64 mpn_add_n
;  Copyright 2016 Alexander Kruppa
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

;	(rdi,rcx) = (rsi,rcx)+(rdx,rcx)
;	rax = carry

%define USE_WIN64

%include 'yasm_mac.inc'

%ifdef USE_WIN64
    %define SumP    rcx
    %define Inp1P   rdx
    %define Inp2P   r8
    %define Size    r9
    %define SizeRest r11
    %define LIMB1   rax
    %define LIMB2   r10
%else
    %define SumP    rdi
    %define Inp1P   rsi
    %define Inp2P   rdx
    %define Size    rcx
    %define SizeRest r11
    %define LIMB1   rax
    %define LIMB2   r8
%endif

%define ADDSUB add
%define ADCSBB adc

; Skylake has problems sustaining 2 read and 1 write per clock cycle.
; It sometimes gets into a "mode" (for the lack of a better word) where
; it does not fully utilize port 7, causing store uops to compete with
; the reads for ports 2,3. We try to alleviate the problem by turning
; some of the 64-bit writes into 128-bit writes, reducing the number of
; write instructions. Unfortunately, SSE2/AVX2 do not have particularly
; good instructions for assembling an SSE2 128-bit word from two GPR
; 64-bit words, so the instruction count is greatly inflated.

%macro  STORE 1
	mov	[SumP %1], LIMB1
	mov	[SumP %1 + 8], LIMB2
%endmacro

%macro  SSESTORE 1
	movq	xmm0, LIMB1
	movq	xmm1, LIMB2
	vpermilpd xmm1, xmm1, 0
	pblendw xmm0, xmm1, 0xf0
	movaps	[SumP %1], xmm0
%endmacro


    BITS    64

    LEAF_PROC mpn_add_n
; Make dest 16-bytes aligned
	test	SumP, 8
	jz	.aligned
	dec	Size
	mov	SizeRest, Size
	and	SizeRest, 7
	shr	Size, 3
; Unaligned and Size > 8: do one limb separately, then the normal loop
	jnz	.unaligned
; Unaligned and Size <= 8: do all with .rest loop
	inc	SizeRest
	clc
	jmp	.rest ;ajs:notshortform

.aligned:
	mov	SizeRest, Size
	and	SizeRest, 7
	shr	Size, 3
	clc
	jz	.rest ;ajs:notshortform
	jmp	.loop1

.unaligned:
	mov	LIMB1, [Inp1P]
	ADDSUB	LIMB1, [Inp2P]
	mov	[SumP], LIMB1
	lea	Inp1P, [Inp1P+8]
	lea	Inp2P, [Inp2P+8]
	lea	SumP, [SumP+8]

	align   16
.loop1:
	mov	LIMB1, [Inp1P]
	mov	LIMB2, [Inp1P+8]
	ADCSBB	LIMB1, [Inp2P]
	ADCSBB	LIMB2, [Inp2P+8]
	SSESTORE +0
	mov	LIMB1, [Inp1P+16]
	mov	LIMB2, [Inp1P+24]
	ADCSBB	LIMB1, [Inp2P+16]
	ADCSBB	LIMB2, [Inp2P+24]
	STORE +16
	mov	LIMB1, [Inp1P+32]
	mov	LIMB2, [Inp1P+40]
	ADCSBB	LIMB1, [Inp2P+32]
	ADCSBB	LIMB2, [Inp2P+40]
	STORE +32
	mov	LIMB1, [Inp1P+48]
	mov	LIMB2, [Inp1P+56]
	ADCSBB	LIMB1, [Inp2P+48]
	ADCSBB	LIMB2, [Inp2P+56]
	STORE +48
	lea	Inp1P, [Inp1P+64]
	lea	Inp2P, [Inp2P+64]
	lea	SumP, [SumP+64]
	dec	Size
	jnz	.loop1
	inc	SizeRest
	dec	SizeRest
	jz	.end
.rest:
	mov	LIMB1, [Inp1P]
	ADCSBB	LIMB1, [Inp2P]
	mov	[SumP], LIMB1
	dec	SizeRest
	jz	.end
	mov	LIMB1, [Inp1P+8]
	ADCSBB	LIMB1, [Inp2P+8]
	mov	[SumP+8], LIMB1
	dec	SizeRest
	jz	.end
	mov	LIMB1, [Inp1P+16]
	ADCSBB	LIMB1, [Inp2P+16]
	mov	[SumP+16], LIMB1
	dec	SizeRest
	jz	.end
	mov	LIMB1, [Inp1P+24]
	ADCSBB	LIMB1, [Inp2P+24]
	mov	[SumP+24], LIMB1
	dec	SizeRest
	jz	.end
	lea	Inp1P, [Inp1P+32]
	lea	Inp2P, [Inp2P+32]
	lea	SumP, [SumP+32]
	jmp	.rest
.end:
	mov	eax, 0
	adc	eax, eax
	ret
