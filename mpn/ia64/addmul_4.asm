include(`../config.m4')
C FILE: addmul_4_asm.s
C
C AUTHOR: Jason Worth Martin <jason.worth.martin@gmail.com>
C
C LICENSE: GNU LGPLv3+
C
C DESCRIPTION: uint64_t addmul_4(uint64_t *rp,
C                                uint64_t *up,
C                                uint64_t n,
C                                uint64_t *vp)
C
C Form the product of the n-limb number pointed to by up with the
C 4-limb number pointed to by vp.  Accumulate n limbs of the product
C to the n-limb number pointed to by rp.  Returns the sum of the
C most significant limb of the product with the last carry from the
C addition.
C
C WARNING: This code will only work correctly if n > 1.  This is okay since
C for n = 1 it isn't worth the overhead of starting up all the pipelines.
C So, mul_basecase needs to check for the size of n and use a smaller routine,
C such as addmul_2 or addmul_1 if n is small.
C
C HOW THIS CODE WORKS
C -------------------
C
C This code is highly pipelined.  There are four multiplication pipelines,
C two load pipelines, and four addition pipelines.  All the pipelines are run
C in parallel to hide the latency of the operations.  This means we have a
C large startup penalty, but once the pipelines are full we get four
C multiplications every six cycles for a throughput of 1.5 cycles/mul.
C
C The multiplication pipelines are m1, m2, m3, m4 with the mklo and mkhi
C for k=1,2,3,4 denoting the lo and hi words.
C
C The addition pipelines are a1, a2, a3, a4.  The addition pipelines have
C varying lengths to accomidate the staggered additions.  In other words,
C we have add lines that look like this:
C
C **** **** **** a1_1 a1_0
C **** **** a2_2 a2_1 a2_0
C **** a3_3 a3_2 a3_1 a3_0
C a4_4 a4_3 a4_2 a4_1 a4_0
C
C where the additions are performed in columns.  We put the results from mul1
C into a1, mul2 into a2, etc.
C
C Note that we use the add+add+cmp+cmp carry propigation method instead of
C the add+cmp+add method.  This is not a disadvantage because the main loop
C needs the ldf8, ld8, st8, and br instructions anyway, so we bury the extra
C compares in the bundles used by those instructions.  This means that we are
C propigatin single bit carries along each of these add lines separately instead
C of dealing with word-wise carries.  Using the word-wise carry propigation
C would reduce the number of instructions needed (add+cmp+add) but increases
C the latency.  At this point, we need at least 16 limbs in the input before
C we break even with two times the addmul_2 routine, so it isn't worth using
C the longer latency loops because somewhere between 50 and 60 limbs we
C transition into Karasuba multiplication anyway.
C
C The code uses the register rotation supported on Itanium.  On each iteration
C of the main loop, the value appears to move up a register.  We label the
C registers to take this into account so that, for eaxmple, the value in a1_1
C appears in a1_0 on the next loop cycle.
C
C We also use the rotating predicate registers to turn on and off the loads and
C stores.  This is because we need to wait until the first result has moved
C all the way through the pipelines before storing it.  Likewise, we stop the
C loads in the epilogue stages of the loop but continue the stores.  See the
C Itanium documentation for a full explaination of register rotation and the
C br.ctop instruction that supports it.
C


C
C General Register use:
C
C r0      - Always 0
C r1      - Global Pointer (preserve)
C r2      - stores ar.pfs (scratch)
C r3      - scratch
C r4-r7   - preserve
C r8      - return0 (structure/union return pointer on entry)
C r9      - lc store (return1)
C r10     - small loop trip count (return2)
C r11     - scratch (return3)
C r12     - stack pointer
C r13     - thread pointer (preserve)


C v1p = vp -- only used at beginning of routine.  Originally in r35
C v2p = vp + 1*8
C v3p = vp + 2*8
C v4p = vp + 3*8
C numlim = number of limbs
C res1 = (*rp) + a1_0
C res2 = res1  + a2_0
C res3 = res2  + a3_0
C res4 = res3  + a4_0
C sp = store pointer.  This is where we write the result.  It is originally rp
pr_store = r3
lc_store = r9
tripcnt_s = r10
rptr	= r14
uptr	= r15
v1p	= r35
v2p	= r16
v3p	= r17
v4p	= r18
tripcnt	= r19
res1	= r20
res2	= r21
res3	= r22
res4	= r8
sptr	= r23
m1hi_s	= r24
m2hi_s	= r25
m3hi_s	= r26
m4hi_s	= r27
	
C
C General Register Rotating Pipelines
C
C
C r32     - add line 1 stage 1 (rp on entry)
C r33     - add line 1 stage 0 (uptr on entry)
a1_1	= r32
a1_0	= r33

C r34     - add line 2 stage 2 (n on entry)
C r35     - add line 2 stage 1 (vp on entry)
C r36     - add line 2 stage 0 (vp on entry)
a2_2	= r34
a2_1	= r35
a2_0	= r36

C r37     - add line 3 stage 3
C r38     - add line 3 stage 2
C r39     - add line 3 stage 1
C r40     - add line 3 stage 0
a3_3	= r37
a3_2	= r38
a3_1	= r39
a3_0	= r40
	
C r41     - add line 4 stage 4
C r42     - add line 4 stage 3
C r43     - add line 4 stage 2
C r44     - add line 4 stage 1
C r45     - add line 4 stage 0
a4_4	= r41
a4_3	= r42
a4_2	= r43
a4_1	= r44
a4_0	= r45

C rp input pipeline
C rpin_c	= r46
rpin_c	= r47
rpin_3	= r48
rpin_2	= r49
rpin_1	= r50
rpin_0	= r51

C Remainder of rotating registers
toprot	= r55


C
C Floating Point Register Use
C
v1	= f6
v2	= f7
v3	= f8
v4	= f9

C
C Floating point rotating register pipelines:
C

C fp load pipeline
upld_c  = f127
upld_2	= f32
upld_1	= f33
upld_0	= f34

C mul_line_1:
m1lo_1	= f35
m1lo_0	= f36
m1hi_1	= f37
m1hi_0	= f38
	
C mul_line_2:
m2lo_1	= f39
m2lo_0	= f40
m2hi_1	= f41
m2hi_0	= f42
	
C mul_line_3:
m3lo_1	= f43
m3lo_0	= f44
m3hi_1	= f45
m3hi_0	= f46
	
C mul_line_4:
m4lo_1	= f47
m4lo_0	= f48
m4hi_1	= f49
m4hi_0	= f50



C
C Predicate Register Use
C
c1_1	= p47
c1_0	= p48
c1n_1	= p49
c1n_0	= p50

c2_1	= p51
c2_0	= p52
c2n_1	= p53
c2n_0	= p54

c3_1	= p55
c3_0	= p56
c3n_1	= p57
c3n_0	= p58

c4_1	= p59
c4_0	= p60
c4n_1	= p61
c4n_0	= p62



ASM_START()
PROLOGUE(mpn_addmul_4)
	.explicit
	.pred.rel "mutex", p14, p15
	.pred.rel "mutex", c1_0, c1n_0
	.pred.rel "mutex", c1_1, c1n_1
	.pred.rel "mutex", c2_0, c2n_0
	.pred.rel "mutex", c2_1, c2n_1
	.pred.rel "mutex", c3_0, c3n_0
	.pred.rel "mutex", c3_1, c3n_1
	.pred.rel "mutex", c4_0, c4n_0
	.pred.rel "mutex", c4_1, c4n_1
	C cycle 1
{
	.prologue
	.save	ar.pfs, r2
	alloc	r2 = ar.pfs,4,20,0,24
	.save	ar.lc, lc_store
	mov	lc_store = ar.lc
	.save	pr, pr_store
	mov	pr_store = pr
}
{
	.body
	mov	rptr = r32
	mov	sptr = r32
	mov	uptr = r33
}
	;; 
	C cycle 2
{
	ldf8	v1 = [v1p]	
	cmp.eq	p14,p15 = 1, r34
	add	v2p = 8, r35
}
{
	ldf8	upld_0 = [uptr], 8
	add	v3p = 16, r35
	add	v4p = 24, r35
}
	;;
	C cycle 3
{
	ldf8	v2 = [v2p]
(p15)	add	tripcnt = -2, r34
	mov	m1lo_0 = f0
}
{
	ldf8	v3 = [v3p]
(p14)	mov	tripcnt = 0
	mov	m1hi_0 = f0
}
	C cycle 4
{
	ldf8	v4 = [v4p]
	mov	m2lo_0 = f0
(p15)	mov	pr.rot = 0x0fff0000 C PR16 = 16-27 set to 1
}
{
	ld8	rpin_2 = [rptr], 8
	mov	m2hi_0 = f0
(p14)	mov	pr.rot = 0x0ffe0000 C Single Limb
}
	;;
	C cycle 5
{
	mov	rpin_3 = 0
	mov	rpin_1 = 0
	mov	m3lo_0 = f0
}
{
	mov	rpin_0 = 0
	cmp.ne	c3_0, c3n_0 = r0, r0
	mov	m3hi_0 = f0
}
	C cycle 6
{
	cmp.ne	c4_0, c4n_0 = r0, r0
	mov	a1_1 = 0
	mov	m4lo_0 = f0
}
{
	mov	a1_0 = 0
	mov	a2_2 = 0
	mov	m4hi_0 = f0
}
	C cycle 7
{
	mov	a2_1 = 0
	mov	a2_0 = 0
	mov	upld_1 = f0
}
{
	mov	a3_3 = 0
	mov	upld_2 = f0
(p14)	mov	ar.ec = 7
}
	;;
	C cycle 8
{
	mov	a3_2 = 0
	mov	upld_c = f0
(p15)	mov	ar.ec = 8
}
{
	cmp.ne	c1_0, c1n_0 = r0, r0
	mov	toprot = 0
	mov	ar.lc = tripcnt
}
	C cycle 9
{
	cmp.ne	c2_0, c2n_0 = r0, r0
	mov	a3_0 = 0
	mov	a3_1 = 0
}
{
	mov	a4_4 = 0
	mov	a4_3 = 0
	mov	a4_0 = 0
}
	C cycle 10
{
	mov	a4_1 = 0
	mov	a4_2 = 0
	nop	0
}
{
	nop	0
	nop	0
	nop	0
}
	;;	
addmul_4_main_loop:
	.pred.rel "mutex", c1_0, c1n_0
	.pred.rel "mutex", c1_1, c1n_1
	.pred.rel "mutex", c2_0, c2n_0
	.pred.rel "mutex", c2_1, c2n_1
	.pred.rel "mutex", c3_0, c3n_0
	.pred.rel "mutex", c3_1, c3n_1
	.pred.rel "mutex", c4_0, c4n_0
	.pred.rel "mutex", c4_1, c4n_1
	C loop cycle 1
{
	getf.sig	a1_1 = m1lo_0
(p16)	ldf8	upld_1 = [uptr], 8
	xma.lu	m1lo_1 = upld_0, v1, m1hi_0
}
{
	nop	0
	nop	0
	xma.hu	m1hi_1 = upld_0, v1, m1hi_0
}
	;; 
	C loop cycle 2
{
	getf.sig	a2_2 = m2lo_0
	xma.lu	m2lo_1 = upld_0, v2, m2hi_0
(c1_0)	add	res1 = rpin_0, a1_0, 1
}
{
(p30)	st8	[sptr] = res4, 8
(c1n_0)	add	res1 = rpin_0, a1_0
	xma.hu	m2hi_1 = upld_0, v2, m2hi_0
}
	;; 
	C loop cycle 3
{
	getf.sig	a3_3 = m3lo_0
(p16)	ld8	rpin_3 = [rptr], 8
	xma.lu	m3lo_1 = upld_0, v3, m3hi_0
}
{
(c2_0)	add	res2 = res1, a2_0, 1
(c2n_0)	add	res2 = res1, a2_0
	xma.hu	m3hi_1 = upld_0, v3, m3hi_0	
}
	;; 
	C loop cycle 4
{
	getf.sig	a4_4 = m4lo_0
	xma.lu	m4lo_1 = upld_0, v4, m4hi_0
(c1n_0)	cmp.ltu	c1_1, c1n_1 = res1, rpin_0
}
{
(c3_0)	add	res3 = res2, a3_0, 1
(c3n_0)	add	res3 = res2, a3_0
	xma.hu	m4hi_1 = upld_0, v4, m4hi_0	
}
	;; 
	C loop cycle 5
{
(c1_0)	cmp.leu	c1_1, c1n_1 = res1, rpin_0
(c4_0)	add	res4 = res3, a4_0, 1
(c4n_0)	add	res4 = res3, a4_0
}
{
(c2n_0)	cmp.ltu	c2_1, c2n_1 = res2, res1
(c2_0)	cmp.leu	c2_1, c2n_1 = res2, res1
(c3n_0)	cmp.ltu	c3_1, c3n_1 = res3, res2
}
	;;
	C loop cycle 6
{
(c3_0)	cmp.leu	c3_1, c3n_1 = res3, res2
	mov	upld_c = f0
	mov	rpin_c = 0
}
{
(c4_0)	cmp.leu	c4_1, c4n_1 = res4, res3
(c4n_0)	cmp.ltu	c4_1, c4n_1 = res4, res3
	br.ctop.sptk.many	addmul_4_main_loop
}
	;;
	
	.auto
	mov	pr = pr_store,-1
	mov	ar.lc = lc_store
	mov	ar.pfs = r2
	br.ret.sptk.many	b0
EPILOGUE()
ASM_END()

