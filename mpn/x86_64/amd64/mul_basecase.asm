dnl  AMD64 mpn_mul_basecase

dnl  Copyright 2008 Jason Moxham

include(`../config.m4')

C	(rdi,rdx+r8)=(rsi,rdx)*(rcx,r8)
# Version 1.0.3

# The M4 use here is just for clarity , think of them as functions which are
# getting inlined , with the param to differentiate the lables

# changes from standard mpn_mul_1
# change lables
# change r8 to r11
# write top limb ax straight to mem dont return
# change lea offsets to match addmul
# reorder mod4 tail parts so have "only one ret"
# remove ret
# todo --------  could ignore small values ie mulskiploop for dx=1,2,3 we save one branch
# todo ------------- alignment of jumps etc
define(`MPN_MUL_1_INT',`
mov `$'3,%r10d
sub %rdx,%r10
mov `$'0,%r11d
jnc mulskiploop
ALIGN(16)
mulloop:
	mov 16(%rsi,%r10,8),%rax
	mov `$'0,%r9d
	mul %r13
	add %rax,%r11
	mov %r11,16(%rdi,%r10,8)
	mov 24(%rsi,%r10,8),%rax
	adc %rdx,%r9
	mul %r13
	mov `$'0,%r11d
	add %rax,%r9
	mov 32(%rsi,%r10,8),%rax
	adc %rdx,%r11
	mul %r13
	mov %r9,24(%rdi,%r10,8)
	add %rax,%r11
	mov `$'0,%r9d
	mov %r11,32(%rdi,%r10,8)
	mov 40(%rsi,%r10,8),%rax
	mov `$'0,%r11d
	adc %rdx,%r9
	mul %r13
	add %rax,%r9
	mov %r9,40(%rdi,%r10,8)
	adc %rdx,%r11
	add `$'4,%r10
	jnc mulloop
mulskiploop:
test `$'2,%r10
jnz mulskip
	mov 16(%rsi,%r10,8),%rax
	mov `$'0,%r9d
	mul %r13
	add %rax,%r11
	mov %r11,16(%rdi,%r10,8)
	mov 24(%rsi,%r10,8),%rax
	adc %rdx,%r9
	mul %r13
	mov `$'0,%r11d
	add %rax,%r9
	adc %rdx,%r11
	add `$'2,%r10
	mov %r9,8(%rdi,%r10,8)
mulskip:
test `$'1,%r10
jnz mulend
	mov 16(%rsi,%r10,8),%rax
	mov `$'0,%r9d
	mul %r13
	add %rax,%r11
	adc %rdx,%r9
	mov %r9,24(%rdi,%r10,8)
mulend:
	mov %r11,16(%rdi,%r10,8)
')


# changes from standard mpn_addmul_1 internal loop
# change lables
# change r8 to r12   , rcx to r13
define(`ADDMULLOOP',`
ALIGN(16)
addmulloop$1:
	mov `$'0,%r10d
	mul %r13
	add %r12,(%rdi,%r11,8)
	adc %rax,%r9
	adc %rdx,%r10
	mov 16(%rsi,%r11,8),%rax
	mul %r13
	add %r9,8(%rdi,%r11,8)
	adc %rax,%r10
	mov `$'0,%ebx
	adc %rdx,%rbx
	mov 24(%rsi,%r11,8),%rax
	mov `$'0,%r12d
	mov `$'0,%r9d
	mul %r13
	add %r10,16(%rdi,%r11,8)
	adc %rax,%rbx
	adc %rdx,%r12
	mov 32(%rsi,%r11,8),%rax
 	mul %r13
	add %rbx,24(%rdi,%r11,8)
	adc %rax,%r12
	adc %rdx,%r9
	add `$'4,%r11
	mov 8(%rsi,%r11,8),%rax
	jnc addmulloop$1
')

define(`ADDMULNEXT0',`
#here is 3 loads ie if r11=0
	mov `$'0,%r10d
	mul %r13
	add %r12,(%rdi,%r11,8)
	adc %rax,%r9
	adc %rdx,%r10
	mov 16(%rsi,%r11,8),%rax
	mul %r13
	add %r9,8(%rdi,%r11,8)
	adc %rax,%r10
	mov `$'0,%ebx
	adc %rdx,%rbx
	mov 24(%rsi,%r11,8),%rax
	mov `$'0,%r12d
	mov `$'0,%r9d
	mul %r13
	add %r10,16(%rdi,%r11,8)
	adc %rax,%rbx
	adc %rdx,%r12
	mov 32(%rsi,%r11,8),%rax
	mul %r13
	add %rbx,24(%rdi,%r11,8)
	adc %rax,%r12
	adc %rdx,%r9
	add %r12,32(%rdi,%r11,8)
	adc `$'0,%r9
	mov %r9,40(%rdi,%r11,8)
')

define(`ADDMULNEXT1',`
#here is 2 loads ie if r11=1
	mov `$'0,%r10d
	mul %r13
	add %r12,(%rdi,%r11,8)
	adc %rax,%r9
	adc %rdx,%r10
	mov 16(%rsi,%r11,8),%rax
	mul %r13
	add %r9,8(%rdi,%r11,8)
	adc %rax,%r10
	mov `$'0,%ebx
	adc %rdx,%rbx
	mov 24(%rsi,%r11,8),%rax
	mov `$'0,%r12d
	mul %r13
	add %r10,16(%rdi,%r11,8)
	adc %rax,%rbx
	adc %rdx,%r12
	add %rbx,24(%rdi,%r11,8)
	adc `$'0,%r12
	mov %r12,32(%rdi,%r11,8)
')

define(`ADDMULNEXT2',`
#here is 1 load ie if r11=2
	mov `$'0,%r10d
	mul %r13
	add %r12,(%rdi,%r11,8)
	adc %rax,%r9
	adc %rdx,%r10
	mov 16(%rsi,%r11,8),%rax
	mul %r13
	add %r9,8(%rdi,%r11,8)
	adc %rax,%r10
	mov `$'0,%ebx
	adc %rdx,%rbx
	add %r10,16(%rdi,%r11,8)
	adc `$'0,%rbx
	mov %rbx,24(%rdi,%r11,8)
')

define(`ADDMULNEXT3',`
#here is 0 loads ie if r11=3
	mov `$'0,%r10d
	mul %r13
	add %r12,(%rdi,%r11,8)
	adc %rax,%r9
	adc %rdx,%r10
	add %r9,8(%rdi,%r11,8)
	adc `$'0,%r10
	mov %r10,16(%rdi,%r11,8)
')

# changes from standard addmul_1
# change lables
# change r8 to r12
# write top limb ax straight to mem dont return  (NOTE we WRITE NOT ADD)
# remove one limb special case
# remove push/pop , basecase must save rbx
# split into mod4 types and remove rets
# surround with outer loop and pop ret
# todo ----------- can ignore small values
# this addmul MUST have a param whic is 0123 which is our r11
define(`MPN_ADDMUL_1_INT',`
loopaddmul$1:
mov (%rsi,%r14,8),%rax
mov (%rcx),%r13
mov %r14,%r11	
mul %r13
mov %rax,%r12
mov 8(%rsi,%r14,8),%rax
mov %rdx,%r9
cmp `$'0,%r14
jge addmulskiploop$1
ADDMULLOOP($1)
addmulskiploop$1:
ADDMULNEXT$1
lea 8(%rcx),%rcx
dec %r8
lea 8(%rdi),%rdi
jnz loopaddmul$1
pop %r12
pop %rbx
pop %r14
pop %r13
ret
')

ASM_START()
PROLOGUE(mpn_mul_basecase)
# these small special cases are not neccessary , the current mul will handle
# them , although I may change it later
cmp $2,%rdx
ja threeormore
jz two
	# one by one
	mov (%rsi),%rax
	mulq (%rcx)
	mov %rax,(%rdi)
	mov %rdx,8(%rdi)
	ret
two:
	mov (%rcx),%r10
	mov (%rsi),%rax
	mul %r10
	mov %rax,(%rdi)
	mov 8(%rsi),%rax
	mov %rdx,%r9
	mul %r10
	add %rax,%r9
	adc $0,%rdx
	mov %r9,8(%rdi)
	mov %rdx,16(%rdi)
	cmp $2,%r8
	jz twobytwo
	ret
twobytwo:
	mov $0,%r9d
	mov 8(%rcx),%r10
	mov (%rsi),%rax
	mul %r10
	add %rax,8(%rdi)
	mov 8(%rsi),%rax
	adc %rdx,%r9
	mul %r10
	add %rax,%r9
	adc $0,%rdx
	add %r9,16(%rdi)
	adc $0,%rdx
	mov %rdx,24(%rdi)
	ret	
threeormore:
# all the push/pop can be moved to pro/epilog
push %r13
mov (%rcx),%r13
push %r14
mov $5,%r14
sub %rdx,%r14
lea -40(%rdi,%rdx,8),%rdi
lea -40(%rsi,%rdx,8),%rsi
MPN_MUL_1_INT()
dec %r8
lea 8(%rdi),%rdi
lea 8(%rcx),%rcx
jnz next4
pop %r14
pop %r13
ret
next4:
push %rbx
push %r12
mov %r14,%rax
and $3,%rax
je case0
jp case3
cmp $1,%rax
je case1
ALIGN(16)
case2:
MPN_ADDMUL_1_INT(2)
ALIGN(16)
case0:
MPN_ADDMUL_1_INT(0)
ALIGN(16)
case1:
MPN_ADDMUL_1_INT(1)
ALIGN(16)
case3:
MPN_ADDMUL_1_INT(3)
EPILOGUE()
