
;  AMD64 mpn_add_n, mpn_add_nc
;  Copyright 2008, 2016 Jason Moxham and Alexander Kruppa
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

%include 'yasm_mac.inc'

%ifdef USE_WIN64
    %define SumP    rcx
    %define Inp1P   rdx
    %define Inp2P   r8
    %define Size    r9
    %define CarryIn
    %define LIMB1   rax
    %define LIMB2   r10
%else
    %define SumP    rdi
    %define Inp1P   rsi
    %define Inp2P   rdx
    %define Size    rcx
    %define CarryIn r8
    %define LIMB1   rax
    %define LIMB2   r8  ; may reuse CarryIn
    %define SizeRest r9
%endif

%define ADCSBB adc

    BITS    64

	align   16
   GLOBAL_FUNC mpn_add_nc
	mov     SizeRest, Size
	and     SizeRest, 7
	shr     Size, 3
        neg	CarryIn		; Set CF iff CarryIn != 0
	inc     Size
	dec     Size		; Set ZF without affecting CF
	jnz     loop1
        jmp     therest ;ajs:notshortform

	align   16
   GLOBAL_FUNC mpn_add_n
	mov     SizeRest, Size
	and     SizeRest, 7
	shr     Size, 3
	cmp     Size, 0
;	carry flag is clear here
	jnz     loop1
        jmp     therest ;ajs:notshortform

	align   16
loop1:
	mov     LIMB1, [Inp1P]
	mov     LIMB2, [Inp1P+8]
	ADCSBB  LIMB1, [Inp2P]
	mov     [SumP], LIMB1
	ADCSBB  LIMB2, [Inp2P+8]
	mov     LIMB1, [Inp1P+16]
	mov     [SumP+8], LIMB2
	ADCSBB  LIMB1, [Inp2P+16]
	mov     LIMB2, [Inp1P+24]
	mov     [SumP+16], LIMB1
	mov     LIMB1, [Inp1P+32]
	ADCSBB  LIMB2, [Inp2P+24]
	mov     [SumP+24], LIMB2
	ADCSBB  LIMB1, [Inp2P+32]
	mov     [SumP+32], LIMB1
	mov     LIMB2, [Inp1P+40]
	ADCSBB  LIMB2, [Inp2P+40]
	mov     [SumP+40], LIMB2
	mov     LIMB1, [Inp1P+48]
	mov     LIMB2, [Inp1P+56]
	lea     Inp1P, [Inp1P+64]
	ADCSBB  LIMB1, [Inp2P+48]
	ADCSBB  LIMB2, [Inp2P+56]
	lea     Inp2P, [Inp2P+64]
	mov     [SumP+48], LIMB1
	mov     [SumP+56], LIMB2
	lea     SumP, [SumP+64]
	dec     Size
	jnz     loop1
	inc     SizeRest
	dec     SizeRest
	jz      end
therest:
	mov     LIMB1, [Inp1P]
	ADCSBB  LIMB1, [Inp2P]
	mov     [SumP], LIMB1
	dec     SizeRest
	jz      end
	mov     LIMB1, [Inp1P+8]
	ADCSBB  LIMB1, [Inp2P+8]
	mov     [SumP+8], LIMB1
	dec     SizeRest
	jz      end
	mov     LIMB1, [Inp1P+16]
	ADCSBB  LIMB1, [Inp2P+16]
	mov     [SumP+16], LIMB1
	dec     SizeRest
        jz      end
	mov     LIMB1, [Inp1P+24]
	ADCSBB  LIMB1, [Inp2P+24]
	mov     [SumP+24], LIMB1
	dec     SizeRest
        jz      end
        lea     Inp1P, [Inp1P+32]
        lea     Inp2P, [Inp2P+32]
        lea     SumP, [SumP+32]
        jmp     therest
end:
	mov     eax, 0
	adc     eax, eax
	ret
