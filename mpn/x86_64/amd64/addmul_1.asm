dnl  AMD64 mpn_addmul_1

dnl  Copyright 2008 Jason Moxham

include(`../config.m4')

C	(rdi,rdx)=(rdi,rdx)+(rsi,rdx)*rcx
C	rax=carry

ASM_START()
PROLOGUE(mpn_addmul_1)
# Version 1.0.3
mov (%rsi),%rax
# this is neccessary as addmul loop will not handle 1 limb
cmp $1,%rdx
jnz notone		
	mul %rcx
	add %rax,(%rdi)
	adc $0,%rdx
	mov %rdx,%rax
	ret
notone:
mov $5,%r11
lea -40(%rsi,%rdx,8),%rsi
lea -40(%rdi,%rdx,8),%rdi
sub %rdx,%r11
mul %rcx
mov %rax,%r8
mov 8(%rsi,%r11,8),%rax
mov %rdx,%r9
cmp $0,%r11
# for windows can just move this push to the prologue
push %rbx
jge skiploop
ALIGN(16)
loop:
	mov $0,%r10d
	mul %rcx
	add %r8,(%rdi,%r11,8)
	adc %rax,%r9
	adc %rdx,%r10
	mov 16(%rsi,%r11,8),%rax
	mul %rcx
	add %r9,8(%rdi,%r11,8)
	adc %rax,%r10
	mov $0,%ebx
	adc %rdx,%rbx
	mov 24(%rsi,%r11,8),%rax
	mov $0,%r8d
	mov $0,%r9d
	mul %rcx
	add %r10,16(%rdi,%r11,8)
	adc %rax,%rbx
	adc %rdx,%r8
	mov 32(%rsi,%r11,8),%rax
 	mul %rcx
	add %rbx,24(%rdi,%r11,8)
	adc %rax,%r8
	adc %rdx,%r9
	add $4,%r11
	mov 8(%rsi,%r11,8),%rax
	jnc loop
#ALIGN(16)
skiploop:
jz next0
jp next3
cmp $1,%r11
je next1
#ALIGN(16)
next2:
#here is 1 load ie if r11=2
	mov $0,%r10d
	mul %rcx
	add %r8,(%rdi,%r11,8)
	adc %rax,%r9
	adc %rdx,%r10
	mov 16(%rsi,%r11,8),%rax
	mul %rcx
	add %r9,8(%rdi,%r11,8)
	adc %rax,%r10
	mov $0,%ebx
	adc %rdx,%rbx
	add %r10,16(%rdi,%r11,8)
	adc $0,%rbx
	mov %rbx,%rax
	pop %rbx
	ret
ALIGN(16)
next3:
#here is 0 loads ie if r11=3
	mov $0,%r10d
	mul %rcx
	add %r8,(%rdi,%r11,8)
	adc %rax,%r9
	adc %rdx,%r10
	add %r9,8(%rdi,%r11,8)
	adc $0,%r10
	mov %r10,%rax
	pop %rbx
	ret
ALIGN(16)
next1:
#here is 2 loads ie if r11=1
	mov $0,%r10d
	mul %rcx
	add %r8,(%rdi,%r11,8)
	adc %rax,%r9
	adc %rdx,%r10
	mov 16(%rsi,%r11,8),%rax
	mul %rcx
	add %r9,8(%rdi,%r11,8)
	adc %rax,%r10
	mov $0,%ebx
	adc %rdx,%rbx
	mov 24(%rsi,%r11,8),%rax
	mov $0,%r8d
	mul %rcx
	add %r10,16(%rdi,%r11,8)
	adc %rax,%rbx
	adc %rdx,%r8
	add %rbx,24(%rdi,%r11,8)
	adc $0,%r8
	mov %r8,%rax
	pop %rbx
	ret
ALIGN(16)
next0:
#here is 3 loads ie if r11=0
	mov $0,%r10d
	mul %rcx
	add %r8,(%rdi,%r11,8)
	adc %rax,%r9
	adc %rdx,%r10
	mov 16(%rsi,%r11,8),%rax
	mul %rcx
	add %r9,8(%rdi,%r11,8)
	adc %rax,%r10
	mov $0,%ebx
	adc %rdx,%rbx
	mov 24(%rsi,%r11,8),%rax
	mov $0,%r8d
	mov $0,%r9d
	mul %rcx
	add %r10,16(%rdi,%r11,8)
	adc %rax,%rbx
	adc %rdx,%r8
	mov 32(%rsi,%r11,8),%rax
	mul %rcx
	add %rbx,24(%rdi,%r11,8)
	adc %rax,%r8
	adc %rdx,%r9
	add %r8,32(%rdi,%r11,8)
	adc $0,%r9
	mov %r9,%rax
	pop %rbx
	ret
EPILOGUE()
