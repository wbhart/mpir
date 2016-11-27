
;  AMD64 mpn_add_n
;  Copyright 2008 Jason Moxham
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
    %define LIMB1   r10
    %define LIMB2   r11
%else
    %define SumP    rdi
    %define Inp1P   rsi
    %define Inp2P   rdx
    %define Size    rcx
    %define LIMB1   r9
    %define LIMB2   r10
%endif

    BITS    64

   GLOBAL_FUNC mpn_add_n
	mov     rax, Size
	and     rax, 3
	shr     Size, 2
	cmp     Size, 0
;	carry flag is clear here
	jnz     loop1
	mov     LIMB1, [Inp1P]
	add     LIMB1, [Inp2P]
	mov     [SumP], LIMB1
	dec     rax
	jz      end1
	mov     LIMB1, [Inp1P+8]
	adc     LIMB1, [Inp2P+8]
	mov     [SumP+8], LIMB1
	dec     rax
	jz      end1
	mov     LIMB1, [Inp1P+16]
	adc     LIMB1, [Inp2P+16]
	mov     [SumP+16], LIMB1
	dec     rax
end1:
	adc     rax, rax
	ret
	align   8
loop1:
	mov     LIMB2, [Inp1P]
	mov     LIMB1, [Inp1P+8]
	adc     LIMB2, [Inp2P]
	lea     Inp1P, [Inp1P+32]
	adc     LIMB1, [Inp2P+8]
	lea     Inp2P, [Inp2P+32]
	mov     [SumP+8], LIMB1
	mov     [SumP], LIMB2
	mov     LIMB1, [Inp1P-16]
	mov     LIMB2, [Inp1P-8]
	adc     LIMB1, [Inp2P-16]
	lea     SumP, [SumP+32]
	adc     LIMB2, [Inp2P-8]
	mov     [SumP-8], LIMB2
	mov     [SumP-16], LIMB1
	dec     Size
	jnz     loop1
	inc     rax
	dec     rax
	jz      end
	mov     LIMB1, [Inp1P]
	adc     LIMB1, [Inp2P]
	mov     [SumP], LIMB1
	dec     rax
	jz      end
	mov     LIMB1, [Inp1P+8]
	adc     LIMB1, [Inp2P+8]
	mov     [SumP+8], LIMB1
	dec     rax
	jz      end
	mov     LIMB1, [Inp1P+16]
	adc     LIMB1, [Inp2P+16]
	mov     [SumP+16], LIMB1
	dec     rax
end:
	adc     rax, rax
	ret
