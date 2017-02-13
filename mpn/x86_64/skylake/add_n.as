;  AMD64 mpn_add_n
;  Copyright 2016 Jens Nurmann and Alexander Kruppa
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
    %define LIMB0   r10
%else
    %define SumP    rdi
    %define Inp1P   rsi
    %define Inp2P   rdx
    %define Size    rcx
    %define SizeRest r11
    %define LIMB0   rax
%endif

%define ADDSUB add
%define ADCSBB adc

	align   32
	BITS    64

GLOBAL_FUNC mpn_add_n

	mov     SizeRest, Size
	and	SizeRest, 7
	shr     Size, 3
	clc
	jz      .testrest

	align   16
.loop:
	mov     LIMB0, [Inp1P]        ;1	1	p23	2	0.5
	ADCSBB  LIMB0, [Inp2P]        ;2	2	p06 p23		1
	mov     [SumP], LIMB0         ;1	2	p237 p4	3	1
	vpblendd YMM0, YMM0, YMM0, 0  ; This one is black magic. Beware.
	mov     LIMB0, [Inp1P+8]      ;1	1	p23	2	0.5
	ADCSBB  LIMB0, [Inp2P+8]      ;2	2	p06 p23		1
	mov     [SumP+8], LIMB0       ;1	2	p237 p4	3	1
	mov     LIMB0, [Inp1P+16]     ;1	1	p23	2	0.5
	ADCSBB  LIMB0, [Inp2P+16]     ;2	2	p06 p23		1
	mov     [SumP+16], LIMB0      ;1	2	p237 p4	3	1
	mov     LIMB0, [Inp1P+24]     ;1	1	p23	2	0.5
	ADCSBB  LIMB0, [Inp2P+24]     ;2	2	p06 p23		1
	mov     [SumP+24], LIMB0      ;1	2	p237 p4	3	1

	mov     LIMB0, [Inp1P+32]     ;1	1	p23	2	0.5
	ADCSBB  LIMB0, [Inp2P+32]     ;2	2	p06 p23		1
	mov     [SumP+32], LIMB0      ;1	2	p237 p4	3	1
	mov     LIMB0, [Inp1P+40]     ;1	1	p23	2	0.5
	ADCSBB  LIMB0, [Inp2P+40]     ;2	2	p06 p23		1
	mov     [SumP+40], LIMB0      ;1	2	p237 p4	3	1
	mov     LIMB0, [Inp1P+48]     ;1	1	p23	2	0.5
	ADCSBB  LIMB0, [Inp2P+48]     ;2	2	p06 p23		1
	mov     [SumP+48], LIMB0      ;1	2	p237 p4	3	1
	mov     LIMB0, [Inp1P+56]     ;1	1	p23	2	0.5
	ADCSBB  LIMB0, [Inp2P+56]     ;2	2	p06 p23		1
	mov     [SumP+56], LIMB0      ;1	2	p237 p4	3	1

	lea     Inp1P, [Inp1P+64]     ;1	1	p15	1	0.5
	lea     Inp2P, [Inp2P+64]     ;1	1	p15	1	0.5
	lea     SumP, [SumP+64]       ;1	1	p15	1	0.5

	dec     Size
	jne     .loop

.testrest:
	inc	SizeRest
	dec	SizeRest
	jz	.exit

.rest:
	mov	LIMB0, [Inp1P]
	ADCSBB	LIMB0, [Inp2P]
	mov	[SumP], LIMB0
	dec	SizeRest
	jz	.exit
	mov	LIMB0, [Inp1P+8]
	ADCSBB	LIMB0, [Inp2P+8]
	mov	[SumP+8], LIMB0
	dec	SizeRest
	jz	.exit
	mov	LIMB0, [Inp1P+16]
	ADCSBB	LIMB0, [Inp2P+16]
	mov	[SumP+16], LIMB0
	dec	SizeRest
	jz	.exit
	mov	LIMB0, [Inp1P+24]
	ADCSBB	LIMB0, [Inp2P+24]
	mov	[SumP+24], LIMB0
	dec	SizeRest
	jz	.exit
	lea	Inp1P, [Inp1P+32]
	lea	Inp2P, [Inp2P+32]
	lea	SumP, [SumP+32]
	jmp	.rest

.exit:
	mov rax, 0
	setc al
	ret