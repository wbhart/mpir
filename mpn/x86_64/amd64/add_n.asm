dnl  AMD64 mpn_add_n

dnl  Copyright 2008 Jason Moxham

include(`../config.m4')

C	(rdi,rcx)=(rsi,rcx)+(rdx,rcx)
C	rax=carry

ASM_START()
PROLOGUE(mpn_add_n)
# Version 1.0.3
mov	%rcx,%rax
and	$3,%rax
shr	$2,%rcx
cmp	$0,%rcx	
# carry flag is clear here
jnz	loop
mov	(%rsi),%r11
add	(%rdx),%r11
mov	%r11,(%rdi)
dec	%rax
jz	end1
mov	8(%rsi),%r11
adc	8(%rdx),%r11
mov	%r11,8(%rdi)
dec	%rax
jz	end1
mov	16(%rsi),%r11
adc	16(%rdx),%r11
mov	%r11,16(%rdi)
dec	%rax
end1:
adc	%rax,%rax
ret
ALIGN(8)
loop:
	mov	(%rsi),%r11
	mov	8(%rsi),%r8
	lea	32(%rsi),%rsi
	adc	(%rdx),%r11
	adc	8(%rdx),%r8
	lea	32(%rdx),%rdx
	mov	%r11,(%rdi)
	mov	%r8,8(%rdi)
	lea	32(%rdi),%rdi
	mov	-16(%rsi),%r9
	mov	-8(%rsi),%r10
	adc	-16(%rdx),%r9
	adc	-8(%rdx),%r10
	mov	%r9,-16(%rdi)
	dec	%rcx
	mov	%r10,-8(%rdi)
	jnz	loop
inc	%rax
dec	%rax
jz	end
mov	(%rsi),%r11
adc	(%rdx),%r11
mov	%r11,(%rdi)
dec	%rax
jz	end
mov	8(%rsi),%r11
adc	8(%rdx),%r11
mov	%r11,8(%rdi)
dec	%rax
jz	end
mov	16(%rsi),%r11
adc	16(%rdx),%r11
mov	%r11,16(%rdi)
dec	%rax
end:
adc	%rax,%rax
ret
EPILOGUE()
