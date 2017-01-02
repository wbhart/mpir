divert(-1)

dnl  m4 macros for x86-64 assembler.

dnl WARNING: only a few of the macros have been ported from x86 to x86-64

dnl  This file is just an adaptation of similar file in the x86 directory.
dnl  Adapted by P. Gaudry in April 2005.
dnl  Here is the copyright of the original x86 version:

dnl  Copyright 1999, 2000, 2001, 2002, 2003, 2004, 2005, 2008, 2009, 2011, 
dnl  2012, 2013 Free Software Foundation, Inc.
dnl 
dnl  This file is part of the GNU MP Library.
dnl
dnl  The GNU MP Library is free software; you can redistribute it and/or
dnl  modify it under the terms of the GNU Lesser General Public License as
dnl  published by the Free Software Foundation; either version 3 of the
dnl  License, or (at your option) any later version.
dnl
dnl  The GNU MP Library is distributed in the hope that it will be useful,
dnl  but WITHOUT ANY WARRANTY; without even the implied warranty of
dnl  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
dnl  Lesser General Public License for more details.
dnl
dnl  You should have received a copy of the GNU Lesser General Public License
dnl  along with the GNU MP Library.  If not, see http://www.gnu.org/licenses/.

dnl  Usage: ret_internal
dnl
dnl  A plain "ret", without any __cyg_profile_func_exit call.  This can be
dnl  used for a return which is internal to some function, such as when
dnl  getting %eip for PIC.

define(ret_internal,
m4_assert_numargs(-1)
``ret'')

dnl  Usage: CPUVEC_FUNCS_LIST
dnl
dnl  A list of the functions from gmp-impl.h x86 struct cpuvec_t, in the
dnl  order they appear in that structure.

define(CPUVEC_FUNCS_LIST,
``add_err1_n',
`add_err2_n',
`add_n',
`addmul_1',
`copyd',
`copyi',
`divexact_1',
`divexact_by3c',
`divexact_byfobm1',
`divrem_1',
`divrem_2',
`divrem_euclidean_qr_1',
`divrem_euclidean_qr_2',
`gcd_1',
`lshift',
`mod_1',
`mod_34lsub1',
`modexact_1c_odd',
`mul_1',
`mul_basecase',
`mulmid_basecase',
`preinv_divrem_1',
`preinv_mod_1',
`redc_1',
`rshift',
`sqr_basecase',
`sub_err1_n',
`sub_err2_n',
`sub_n',
`submul_1',
`sumdiff_n'')

dnl  Notes:
dnl
dnl  m4 isn't perfect for processing BSD style x86 assembler code, the main
dnl  problems are,
dnl
dnl  1. Doing define(foo,123) and then using foo in an addressing mode like
dnl     foo(%ebx) expands as a macro rather than a constant.  This is worked
dnl     around by using deflit() from asm-defs.m4, instead of define().
dnl
dnl  2. Immediates in macro definitions need a space or `' to stop the $
dnl     looking like a macro parameter.  For example,
dnl
dnl  	        define(foo, `mov $ 123, %eax')
dnl
dnl     This is only a problem in macro definitions, not in ordinary text,
dnl     and not in macro parameters like text passed to forloop() or ifdef().


deflit(BYTES_PER_MP_LIMB, 8)


dnl  Libtool gives -DPIC -DDLL_EXPORT to indicate a cygwin or mingw DLL.  We
dnl  undefine PIC since we don't need to be position independent in this
dnl  case and definitely don't want the ELF style _GLOBAL_OFFSET_TABLE_ etc.

ifdef(`DLL_EXPORT',`undefine(`PIC')')


dnl  Called: PROLOGUE_cpu(GSYM_PREFIX`'foo)
dnl
dnl  In the x86 code we use explicit TEXT and ALIGN() calls in the code,
dnl  since different alignments are wanted in various circumstances.  So for
dnl  instance,
dnl
dnl                  TEXT
dnl                  ALIGN(16)
dnl          PROLOGUE(mpn_add_n)
dnl          ...
dnl          EPILOGUE()

define(`PROLOGUE_cpu',
m4_assert_numargs(1)
	`GLOBL	$1
	TYPE($1,`function')
$1:
ifelse(WANT_PROFILING,`no',,`call_mcount
')')

define(`JUMPTABSECT', `.section .data.rel.ro.local,"aw",@progbits')


dnl  Usage: JMPENT(targlabel,tablabel)

define(`JMPENT',`dnl
ifdef(`PIC',
    `.long  $1-$2'
,
    `.quad  $1'
)')

dnl  Usage: call_mcount
dnl
dnl  For `gprof' style profiling, %ebp is setup as a frame pointer.  None of
dnl  the assembler routines use %ebp this way, so it's done only for the
dnl  benefit of mcount.  glibc sysdeps/i386/i386-mcount.S shows how mcount
dnl  gets the current function from (%esp) and the parent from 4(%ebp).
dnl
dnl  For `prof' style profiling gcc generates mcount calls without setting
dnl  up %ebp, and the same is done here.

define(`call_mcount',
m4_assert_numargs(-1)
m4_assert_defined(`WANT_PROFILING')
m4_assert_defined(`MCOUNT_PIC_REG')
m4_assert_defined(`MCOUNT_NONPIC_REG')
m4_assert_defined(`MCOUNT_PIC_CALL')
m4_assert_defined(`MCOUNT_NONPIC_CALL')
`ifelse(ifdef(`PIC',`MCOUNT_PIC_REG',`MCOUNT_NONPIC_REG'),,,
`	DATA
	ALIGN(4)
L(mcount_data_`'mcount_data_counter):
	W32	0
	TEXT
')dnl
ifelse(WANT_PROFILING,`gprof',
`	pushl	%ebp
	movl	%esp, %ebp
')dnl
ifdef(`PIC',
`	pushl	%ebx
	mcount_movl_GOT_ebx
ifelse(MCOUNT_PIC_REG,,,
`	leal	L(mcount_data_`'mcount_data_counter)@GOTOFF(%ebx), MCOUNT_PIC_REG')
MCOUNT_PIC_CALL
	popl	%ebx
',`dnl non-PIC
ifelse(MCOUNT_NONPIC_REG,,,
`	movl	`$'L(mcount_data_`'mcount_data_counter), MCOUNT_NONPIC_REG
')dnl
MCOUNT_NONPIC_CALL
')dnl
ifelse(WANT_PROFILING,`gprof',
`	popl	%ebp
')
define(`mcount_data_counter',eval(mcount_data_counter+1))')

define(mcount_data_counter,1)

dnl  Called: mcount_movl_GOT_ebx
dnl  Label H is "here", the %eip obtained from the call.  C is the called
dnl  subroutine.  J is the jump across that subroutine.  A fetch and "ret"
dnl  is always done so calls and returns are balanced for the benefit of the
dnl  various x86s that have return stack branch prediction.
define(mcount_movl_GOT_ebx,
m4_assert_numargs(-1)
`	call	L(mcount_movl_GOT_ebx_C`'mcount_movl_GOT_ebx_counter)
L(mcount_movl_GOT_ebx_H`'mcount_movl_GOT_ebx_counter):
	jmp	L(mcount_movl_GOT_ebx_J`'mcount_movl_GOT_ebx_counter)
L(mcount_movl_GOT_ebx_C`'mcount_movl_GOT_ebx_counter):
	movl	(%esp), %ebx
	ret
L(mcount_movl_GOT_ebx_J`'mcount_movl_GOT_ebx_counter):
	addl	$_GLOBAL_OFFSET_TABLE_+[.-L(mcount_movl_GOT_ebx_H`'mcount_movl_GOT_ebx_counter)], %ebx
define(`mcount_movl_GOT_ebx_counter',incr(mcount_movl_GOT_ebx_counter))')

define(mcount_movl_GOT_ebx_counter,1)


dnl  --------------------------------------------------------------------------
dnl  Various x86 macros.
dnl


dnl  Usage: ALIGN_OFFSET(bytes,offset)
dnl
dnl  Align to `offset' away from a multiple of `bytes'.
dnl
dnl  This is useful for testing, for example align to something very strict
dnl  and see what effect offsets from it have, "ALIGN_OFFSET(256,32)".
dnl
dnl  Generally you wouldn't execute across the padding, but it's done with
dnl  nop's so it'll work.

define(ALIGN_OFFSET,
m4_assert_numargs(2)
`ALIGN($1)
forloop(`i',1,$2,`	nop
')')


dnl  Usage: defframe(name,offset)
dnl
dnl  Make a definition like the following with which to access a parameter
dnl  or variable on the stack.
dnl
dnl         define(name,`FRAME+offset(%esp)')
dnl
dnl  Actually m4_empty_if_zero(FRAME+offset) is used, which will save one
dnl  byte if FRAME+offset is zero, by putting (%esp) rather than 0(%esp).
dnl  Use define(`defframe_empty_if_zero_disabled',1) if for some reason the
dnl  zero offset is wanted.
dnl
dnl  The new macro also gets a check that when it's used FRAME is actually
dnl  defined, and that the final %esp offset isn't negative, which would
dnl  mean an attempt to access something below the current %esp.
dnl
dnl  deflit() is used rather than a plain define(), so the new macro won't
dnl  delete any following parenthesized expression.  name(%edi) will come
dnl  out say as 16(%esp)(%edi).  This isn't valid assembler and should
dnl  provoke an error, which is better than silently giving just 16(%esp).
dnl
dnl  See README for more on the suggested way to access the stack frame.

define(defframe,
m4_assert_numargs(2)
`deflit(`$1',
m4_assert_defined(`FRAME')
`defframe_check_notbelow(`$1',$2,FRAME)dnl
defframe_empty_if_zero(FRAME+($2))(%rsp)')')

dnl  Called: defframe_empty_if_zero(expression)
define(defframe_empty_if_zero,
m4_assert_numargs(1)
`ifelse(defframe_empty_if_zero_disabled,1,
`eval($1)',
`m4_empty_if_zero($1)')')

dnl  Called: defframe_check_notbelow(`name',offset,FRAME)
define(defframe_check_notbelow,
m4_assert_numargs(3)
`ifelse(eval(($3)+($2)<0),1,
`m4_error(`$1 at frame offset $2 used when FRAME is only $3 bytes
')')')


dnl  Usage: FRAME_pushl()
dnl         FRAME_popl()
dnl         FRAME_addl_esp(n)
dnl         FRAME_subl_esp(n)
dnl
dnl  Adjust FRAME appropriately for a pushl or popl, or for an addl or subl
dnl  %esp of n bytes.
dnl
dnl  Using these macros is completely optional.  Sometimes it makes more
dnl  sense to put explicit deflit(`FRAME',N) forms, especially when there's
dnl  jumps and different sequences of FRAME values need to be used in
dnl  different places.

define(FRAME_pushq,
m4_assert_numargs(0)
m4_assert_defined(`FRAME')
`deflit(`FRAME',eval(FRAME+8))')

define(FRAME_popq,
m4_assert_numargs(0)
m4_assert_defined(`FRAME')
`deflit(`FRAME',eval(FRAME-8))')

define(FRAME_addq_rsp,
m4_assert_numargs(1)
m4_assert_defined(`FRAME')
`deflit(`FRAME',eval(FRAME-($1)))')

define(FRAME_subq_rsp,
m4_assert_numargs(1)
m4_assert_defined(`FRAME')
`deflit(`FRAME',eval(FRAME+($1)))')


dnl  Usage: defframe_pushl(name)
dnl
dnl  Do a combination FRAME_pushl() and a defframe() to name the stack
dnl  location just pushed.  This should come after a pushl instruction.
dnl  Putting it on the same line works and avoids lengthening the code.  For
dnl  example,
dnl
dnl         pushl   %eax     defframe_pushl(VAR_COUNTER)
dnl
dnl  Notice the defframe() is done with an unquoted -FRAME thus giving its
dnl  current value without tracking future changes.

define(defframe_pushq,
m4_assert_numargs(1)
`FRAME_pushl()defframe(`$1',-FRAME)')


dnl  --------------------------------------------------------------------------
dnl  Assembler instruction macros.
dnl


dnl  Usage: x86_lookup(target, key,value, key,value, ...)
dnl         x86_lookup_p(target, key,value, key,value, ...)
dnl
dnl  Look for `target' among the `key' parameters.
dnl
dnl  x86_lookup expands to the corresponding `value', or generates an error
dnl  if `target' isn't found.
dnl
dnl  x86_lookup_p expands to 1 if `target' is found, or 0 if not.

define(x86_lookup,
m4_assert_numargs_range(1,999)
`ifelse(eval($#<3),1,
`m4_error(`unrecognised part of x86 instruction: $1
')',
`ifelse(`$1',`$2', `$3',
`x86_lookup(`$1',shift(shift(shift($@))))')')')

define(x86_lookup_p,
m4_assert_numargs_range(1,999)
`ifelse(eval($#<3),1, `0',
`ifelse(`$1',`$2',    `1',
`x86_lookup_p(`$1',shift(shift(shift($@))))')')')


dnl  Usage: x86_opcode_reg32(reg)
dnl         x86_opcode_reg32_p(reg)
dnl
dnl  x86_opcode_reg32 expands to the standard 3 bit encoding for the given
dnl  32-bit register, eg. `%ebp' turns into 5.
dnl
dnl  x86_opcode_reg32_p expands to 1 if reg is a valid 32-bit register, or 0
dnl  if not.

define(x86_opcode_reg32,
m4_assert_numargs(1)
`x86_lookup(`$1',x86_opcode_reg32_list)')

define(x86_opcode_reg32_p,
m4_assert_onearg()
`x86_lookup_p(`$1',x86_opcode_reg32_list)')

define(x86_opcode_reg32_list,
``%eax',0,
`%ecx',1,
`%edx',2,
`%ebx',3,
`%esp',4,
`%ebp',5,
`%esi',6,
`%edi',7')


dnl  Usage: x86_opcode_tttn(cond)
dnl
dnl  Expand to the 4-bit "tttn" field value for the given x86 branch
dnl  condition (like `c', `ae', etc).

define(x86_opcode_tttn,
m4_assert_numargs(1)
`x86_lookup(`$1',x86_opcode_ttn_list)')

define(x86_opcode_tttn_list,
``o',  0,
`no',  1,
`b',   2, `c',  2, `nae',2,
`nb',  3, `nc', 3, `ae', 3,
`e',   4, `z',  4,
`ne',  5, `nz', 5,
`be',  6, `na', 6,
`nbe', 7, `a',  7,
`s',   8,
`ns',  9,
`p',  10, `pe', 10, `npo',10,
`np', 11, `npe',11, `po', 11,
`l',  12, `nge',12,
`nl', 13, `ge', 13,
`le', 14, `ng', 14,
`nle',15, `g',  15')


dnl  Usage: x86_opcode_regmmx(reg)
dnl
dnl  Validate the given mmx register, and return its number, 0 to 7.

define(x86_opcode_regmmx,
m4_assert_numargs(1)
`x86_lookup(`$1',x86_opcode_regmmx_list)')

define(x86_opcode_regmmx_list,
``%mm0',0,
`%mm1',1,
`%mm2',2,
`%mm3',3,
`%mm4',4,
`%mm5',5,
`%mm6',6,
`%mm7',7')

dnl  Usage: Zdisp(inst,op,op,op)
dnl
dnl  Generate explicit .byte sequences if necessary to force a byte-sized
dnl  zero displacement on an instruction.  For example,
dnl
dnl         Zdisp(  movl,   0,(%esi), %eax)
dnl
dnl  expands to
dnl
dnl                 .byte   139,70,0  C movl 0(%esi), %eax
dnl
dnl  If the displacement given isn't 0, then normal assembler code is
dnl  generated.  For example,
dnl
dnl         Zdisp(  movl,   4,(%esi), %eax)
dnl
dnl  expands to
dnl
dnl                 movl    4(%esi), %eax
dnl
dnl  This means a single Zdisp() form can be used with an expression for the
dnl  displacement, and .byte will be used only if necessary.  The
dnl  displacement argument is eval()ed.
dnl
dnl  Because there aren't many places a 0(reg) form is wanted, Zdisp is
dnl  implemented with a table of instructions and encodings.  A new entry is
dnl  needed for any different operation or registers.  The table is split
dnl  into separate macros to avoid overflowing BSD m4 macro expansion space.

define(Zdisp,
m4_assert_numargs(4)
`define(`Zdisp_found',0)dnl
Zdisp_1($@)dnl
Zdisp_2($@)dnl
Zdisp_3($@)dnl
Zdisp_4($@)dnl
Zdisp_5($@)dnl
ifelse(Zdisp_found,0,
`m4_error(`unrecognised instruction in Zdisp: $1 $2 $3 $4
')')')

define(Zdisp_1,`dnl
Zdisp_match( adcl, 0,(%edx), %eax,        `0x13,0x42,0x00',           $@)`'dnl
Zdisp_match( adcl, 0,(%edx), %ebx,        `0x13,0x5a,0x00',           $@)`'dnl
Zdisp_match( adcl, 0,(%edx), %esi,        `0x13,0x72,0x00',           $@)`'dnl
Zdisp_match( addl, %ebx, 0,(%edi),        `0x01,0x5f,0x00',           $@)`'dnl
Zdisp_match( addl, %ecx, 0,(%edi),        `0x01,0x4f,0x00',           $@)`'dnl
Zdisp_match( addl, %esi, 0,(%edi),        `0x01,0x77,0x00',           $@)`'dnl
Zdisp_match( sbbl, 0,(%edx), %eax,        `0x1b,0x42,0x00',           $@)`'dnl
Zdisp_match( sbbl, 0,(%edx), %esi,        `0x1b,0x72,0x00',           $@)`'dnl
Zdisp_match( subl, %ecx, 0,(%edi),        `0x29,0x4f,0x00',           $@)`'dnl
Zdisp_match( movzbl, 0,(%eax,%ebp), %eax, `0x0f,0xb6,0x44,0x28,0x00', $@)`'dnl
Zdisp_match( movzbl, 0,(%ecx,%edi), %edi, `0x0f,0xb6,0x7c,0x39,0x00', $@)`'dnl
')
define(Zdisp_2,`dnl
Zdisp_match( movl, %eax, 0,(%edi),        `0x89,0x47,0x00',           $@)`'dnl
Zdisp_match( movl, %ebx, 0,(%edi),        `0x89,0x5f,0x00',           $@)`'dnl
Zdisp_match( movl, %esi, 0,(%edi),        `0x89,0x77,0x00',           $@)`'dnl
Zdisp_match( movl, 0,(%ebx), %eax,        `0x8b,0x43,0x00',           $@)`'dnl
Zdisp_match( movl, 0,(%ebx), %esi,        `0x8b,0x73,0x00',           $@)`'dnl
Zdisp_match( movl, 0,(%edx), %eax,        `0x8b,0x42,0x00',           $@)`'dnl
Zdisp_match( movl, 0,(%esi), %eax,        `0x8b,0x46,0x00',           $@)`'dnl
Zdisp_match( movl, 0,(%esi,%ecx,4), %eax, `0x8b,0x44,0x8e,0x00',      $@)`'dnl
')
define(Zdisp_3,`dnl
Zdisp_match( movq, 0,(%eax,%ecx,8), %mm0, `0x0f,0x6f,0x44,0xc8,0x00', $@)`'dnl
Zdisp_match( movq, 0,(%ebx,%eax,4), %mm0, `0x0f,0x6f,0x44,0x83,0x00', $@)`'dnl
Zdisp_match( movq, 0,(%ebx,%eax,4), %mm2, `0x0f,0x6f,0x54,0x83,0x00', $@)`'dnl
Zdisp_match( movq, 0,(%ebx,%ecx,4), %mm0, `0x0f,0x6f,0x44,0x8b,0x00', $@)`'dnl
Zdisp_match( movq, 0,(%edx), %mm0,        `0x0f,0x6f,0x42,0x00',      $@)`'dnl
Zdisp_match( movq, 0,(%esi), %mm0,        `0x0f,0x6f,0x46,0x00',      $@)`'dnl
Zdisp_match( movq, %mm0, 0,(%edi),        `0x0f,0x7f,0x47,0x00',      $@)`'dnl
Zdisp_match( movq, %mm2, 0,(%ecx,%eax,4), `0x0f,0x7f,0x54,0x81,0x00', $@)`'dnl
Zdisp_match( movq, %mm2, 0,(%edx,%eax,4), `0x0f,0x7f,0x54,0x82,0x00', $@)`'dnl
Zdisp_match( movq, %mm0, 0,(%edx,%ecx,8), `0x0f,0x7f,0x44,0xca,0x00', $@)`'dnl
')
define(Zdisp_4,`dnl
Zdisp_match( movd, 0,(%eax,%ecx,4), %mm0, `0x0f,0x6e,0x44,0x88,0x00', $@)`'dnl
Zdisp_match( movd, 0,(%eax,%ecx,8), %mm1, `0x0f,0x6e,0x4c,0xc8,0x00', $@)`'dnl
Zdisp_match( movd, 0,(%edx,%ecx,8), %mm0, `0x0f,0x6e,0x44,0xca,0x00', $@)`'dnl
Zdisp_match( movd, %mm0, 0,(%eax,%ecx,4), `0x0f,0x7e,0x44,0x88,0x00', $@)`'dnl
Zdisp_match( movd, %mm0, 0,(%ecx,%eax,4), `0x0f,0x7e,0x44,0x81,0x00', $@)`'dnl
Zdisp_match( movd, %mm2, 0,(%ecx,%eax,4), `0x0f,0x7e,0x54,0x81,0x00', $@)`'dnl
Zdisp_match( movd, %mm0, 0,(%edx,%ecx,4), `0x0f,0x7e,0x44,0x8a,0x00', $@)`'dnl
')
define(Zdisp_5,`dnl
Zdisp_match( movq, 0,(%rsi), %rax, `0x48,0x8b,0x46,0x00', $@)`'dnl
Zdisp_match( addq, %rcx, 0, (%rdi), `0x48,0x01,0x4f,0x00', $@)`'dnl
Zdisp_match( addq, %rbx, 0, (%rdi), `0x48,0x01,0x5f,0x00', $@)`'dnl
Zdisp_match( subq, %rcx, 0, (%rdi), `0x48,0x29,0x4f,0x00', $@)`'dnl
Zdisp_match( movq, 0,(%rbx), %rsi, `0x48,0x8b,0x73,0x00', $@)`'dnl
Zdisp_match( movq, %rsi, 0,(%rdi), `0x48,0x89,0x77,0x00', $@)`'dnl
Zdisp_match( adcq, 0, (%rdx), %rsi, `0x48,0x13,0x72,0x00', $@)`'dnl
Zdisp_match( sbbq, 0, (%rdx), %rsi, `0x48,0x1b,0x72,0x00', $@)`'dnl
')


define(Zdisp_match,
m4_assert_numargs(9)
`ifelse(eval(m4_stringequal_p(`$1',`$6')
	&& m4_stringequal_p(`$2',0)
	&& m4_stringequal_p(`$3',`$8')
	&& m4_stringequal_p(`$4',`$9')),1,
`define(`Zdisp_found',1)dnl
ifelse(eval(`$7'),0,
`	.byte	$5  C `$1 0$3, $4'',
`	$6	$7$8, $9')',

`ifelse(eval(m4_stringequal_p(`$1',`$6')
	&& m4_stringequal_p(`$2',`$7')
	&& m4_stringequal_p(`$3',0)
	&& m4_stringequal_p(`$4',`$9')),1,
`define(`Zdisp_found',1)dnl
ifelse(eval(`$8'),0,
`	.byte	$5  C `$1 $2, 0$4'',
`	$6	$7, $8$9')')')')


dnl  Usage: shldl(count,src,dst)
dnl         shrdl(count,src,dst)
dnl         shldw(count,src,dst)
dnl         shrdw(count,src,dst)
dnl
dnl  Generate a double-shift instruction, possibly omitting a %cl count
dnl  parameter if that's what the assembler requires, as indicated by
dnl  WANT_SHLDL_CL in config.m4.  For example,
dnl
dnl         shldl(  %cl, %eax, %ebx)
dnl
dnl  turns into either
dnl
dnl         shldl   %cl, %eax, %ebx
dnl  or
dnl         shldl   %eax, %ebx
dnl
dnl  Immediate counts are always passed through unchanged.  For example,
dnl
dnl         shrdl(  $2, %esi, %edi)
dnl  becomes
dnl         shrdl   $2, %esi, %edi
dnl
dnl
dnl  If you forget to use the macro form "shldl( ...)" and instead write
dnl  just a plain "shldl ...", an error results.  This ensures the necessary
dnl  variant treatment of %cl isn't accidentally bypassed.

define(define_shd_instruction,
m4_assert_numargs(1)
`define($1,
m4_instruction_wrapper()
m4_assert_numargs(3)
`shd_instruction'(m4_doublequote($`'0),m4_doublequote($`'1),dnl
m4_doublequote($`'2),m4_doublequote($`'3)))')

dnl  Effectively: define(shldl,`shd_instruction(`$0',`$1',`$2',`$3')') etc
define_shd_instruction(shldl)
define_shd_instruction(shrdl)
define_shd_instruction(shldw)
define_shd_instruction(shrdw)

dnl  Called: shd_instruction(op,count,src,dst)
define(shd_instruction,
m4_assert_numargs(4)
m4_assert_defined(`WANT_SHLDL_CL')
`ifelse(eval(m4_stringequal_p(`$2',`%cl') && !WANT_SHLDL_CL),1,
``$1'	`$3', `$4'',
``$1'	`$2', `$3', `$4'')')


dnl  Usage: ASSERT([cond][,instructions])
dnl
dnl  If WANT_ASSERT is 1, output the given instructions and expect the given
dnl  flags condition to then be satisfied.  For example,
dnl
dnl         ASSERT(ne, `cmpl %eax, %ebx')
dnl
dnl  The instructions can be omitted to just assert a flags condition with
dnl  no extra calculation.  For example,
dnl
dnl         ASSERT(nc)
dnl
dnl  When `instructions' is not empty, a pushf/popf is added to preserve the
dnl  flags, but the instructions themselves must preserve any registers that
dnl  matter.  FRAME is adjusted for the push and pop, so the instructions
dnl  given can use defframe() stack variables.
dnl
dnl  The condition can be omitted to just output the given instructions when
dnl  assertion checking is wanted.  In this case the pushf/popf is omitted.
dnl  For example,
dnl
dnl         ASSERT(, `movl %eax, VAR_KEEPVAL')

define(ASSERT,
m4_assert_numargs_range(1,2)
`ifelse(WANT_ASSERT,1,
`ifelse(`$1',,
	`$2',
	`C ASSERT
ifelse(`$2',,,`	pushf	ifdef(`FRAME',`FRAME_pushl()')')
	$2
	j`$1'	L(ASSERT_ok`'ASSERT_counter)
	ud2	C assertion failed
L(ASSERT_ok`'ASSERT_counter):
ifelse(`$2',,,`	popf	ifdef(`FRAME',`FRAME_popl()')')
define(`ASSERT_counter',incr(ASSERT_counter))')')')

define(ASSERT_counter,1)

define(`R32',
        `ifelse($1,`%rax',`%eax',
                $1,`%rbx',`%ebx',
                $1,`%rcx',`%ecx',
                $1,`%rdx',`%edx',
                $1,`%rsi',`%esi',
                $1,`%rdi',`%edi',
                $1,`%rbp',`%ebp',
                $1,`%r8',`%r8d',
                $1,`%r9',`%r9d',
                $1,`%r10',`%r10d',
                $1,`%r11',`%r11d',
                $1,`%r12',`%r12d',
                $1,`%r13',`%r13d',
                $1,`%r14',`%r14d',
                $1,`%r15',`%r15d')')

define(`R8',
        `ifelse($1,`%rax',`%al',
                $1,`%rbx',`%bl',
                $1,`%rcx',`%cl',
                $1,`%rdx',`%dl',
                $1,`%rsi',`%sil',
                $1,`%rdi',`%dil',
                $1,`%rbp',`%bpl',
                $1,`%r8',`%r8b',
                $1,`%r9',`%r9b',
                $1,`%r10',`%r10b',
                $1,`%r11',`%r11b',
                $1,`%r12',`%r12b',
                $1,`%r13',`%r13b',
                $1,`%r14',`%r14b',
                $1,`%r15',`%r15b')')

define(`JUMPTABSECT', `.section .data.rel.ro.local,"aw",@progbits')


dnl  Usage: JMPENT(targlabel,tablabel)

define(`JMPENT',`dnl
ifdef(`PIC',
        `.long  $1-$2'
,
        `.quad  $1'
)')

dnl  Usage: movl_text_address(label,register)
dnl
dnl  Get the address of a text segment label, using either a plain movl or a
dnl  position-independent calculation, as necessary.  For example,
dnl
dnl         movl_code_address(L(foo),%eax)
dnl
dnl  This macro is only meant for use in ASSERT()s or when testing, since
dnl  the PIC sequence it generates will want to be done with a ret balancing
dnl  the call on CPUs with return address branch predition.
dnl
dnl  The addl generated here has a backward reference to the label, and so
dnl  won't suffer from the two forwards references bug in old gas (described
dnl  in mpn/x86/README).

define(movl_text_address,
m4_assert_numargs(2)
`ifdef(`PIC',
	`call	L(movl_text_address_`'movl_text_address_counter)
L(movl_text_address_`'movl_text_address_counter):
	popl	$2	C %eip
	addl	`$'$1-L(movl_text_address_`'movl_text_address_counter), $2
define(`movl_text_address_counter',incr(movl_text_address_counter))',
	`movl	`$'$1, $2')')

define(movl_text_address_counter,1)


dnl  Usage: notl_or_xorl_GMP_NUMB_MASK(reg)
dnl
dnl  Expand to either "notl `reg'" or "xorl $GMP_NUMB_BITS,`reg'" as
dnl  appropriate for nails in use or not.

define(notl_or_xorl_GMP_NUMB_MASK,
m4_assert_numargs(1)
`ifelse(GMP_NAIL_BITS,0,
`notl	`$1'',
`xorl	$GMP_NUMB_MASK, `$1'')')

define(`mulx', `ifelse(`$#',3,``mulx' $1,$2,$3',`$#',4,``mulx' $1$2,$3,$4',``mulx'')')
define(`adcx', `ifelse(`$#',2,``adcx' $1,$2',`$#',3,``adcx' $1$2,$3',``adcx'')')
define(`adox', `ifelse(`$#',2,``adox' $1,$2',`$#',3,``adox' $1$2,$3',``adox'')')
define(`ABI_SUPPORT', `')
define(`FUNC_ENTRY', `')
define(`FUNC_EXIT', `')
define(`IFDOS', `')

divert`'dnl
