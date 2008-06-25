
# line 2 "calc.y"
/* A simple integer desk calculator using yacc and gmp.

Copyright 2000, 2001, 2002 Free Software Foundation, Inc.

This file is part of the GNU MP Library.

This program is free software; you can redistribute it and/or modify it under
the terms of the GNU General Public License as published by the Free Software
Foundation; either version 2 of the License, or (at your option) any later
version.

This program is distributed in the hope that it will be useful, but WITHOUT ANY
WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A
PARTICULAR PURPOSE.  See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with
this program; if not, write to the Free Software Foundation, Inc., 51 Franklin
Street, Fifth Floor, Boston, MA 02110-1301, USA. */


/* This is a simple program, meant only to show one way to use GMP for this
   sort of thing.  There's few features, and error checking is minimal.
   Standard input is read, calc_help() below shows the inputs accepted.

   Expressions are evaluated as they're read.  If user defined functions
   were wanted it'd be necessary to build a parse tree like pexpr.c does, or
   a list of operations for a stack based evaluator.  That would also make
   it possible to detect and optimize evaluations "mod m" like pexpr.c does.

   A stack is used for intermediate values in the expression evaluation,
   separate from the yacc parser stack.  This is simple, makes error
   recovery easy, minimizes the junk around mpz calls in the rules, and
   saves initializing or clearing "mpz_t"s during a calculation.  A
   disadvantage though is that variables must be copied to the stack to be
   worked on.  A more sophisticated calculator or language system might be
   able to avoid that when executing a compiled or semi-compiled form.

   Avoiding repeated initializing and clearing of "mpz_t"s is important.  In
   this program the time spent parsing is obviously much greater than any
   possible saving from this, but a proper calculator or language should
   take some trouble over it.  Don't be surprised if an init/clear takes 3
   or more times as long as a 10 limb addition, depending on the system (see
   the mpz_init_realloc_clear example in tune/README).  */


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "gmp.h"
#define NO_CALC_H /* because it conflicts with normal calc.c stuff */
#include "calc-common.h"


#define numberof(x)  (sizeof (x) / sizeof ((x)[0]))


void
calc_help (void)
{
  printf ("Examples:\n");
  printf ("    2+3*4        expressions are evaluated\n");
  printf ("    x=5^6        variables a to z can be set and used\n");
  printf ("Operators:\n");
  printf ("    + - *        arithmetic\n");
  printf ("    / %%          division and remainder (rounding towards negative infinity)\n");
  printf ("    ^            exponentiation\n");
  printf ("    !            factorial\n");
  printf ("    << >>        left and right shifts\n");
  printf ("    <= >= >      \\ comparisons, giving 1 if true, 0 if false\n");
  printf ("    == != <      /\n");
  printf ("    && ||        logical and/or, giving 1 if true, 0 if false\n");
  printf ("Functions:\n");
  printf ("    abs(n)       absolute value\n");
  printf ("    bin(n,m)     binomial coefficient\n");
  printf ("    fib(n)       fibonacci number\n");
  printf ("    gcd(a,b,..)  greatest common divisor\n");
  printf ("    kron(a,b)    kronecker symbol\n");
  printf ("    lcm(a,b,..)  least common multiple\n");
  printf ("    lucnum(n)    lucas number\n");
  printf ("    nextprime(n) next prime after n\n");
  printf ("    powm(b,e,m)  modulo powering, b^e%%m\n");
  printf ("    root(n,r)    r-th root\n");
  printf ("    sqrt(n)      square root\n");
  printf ("Other:\n");
  printf ("    hex          \\ set hex or decimal for input and output\n");
  printf ("    decimal      /   (\"0x\" can be used for hex too)\n");
  printf ("    quit         exit program (EOF works too)\n");
  printf ("    ;            statements are separated with a ; or newline\n");
  printf ("    \\            continue expressions with \\ before newline\n");
  printf ("    # xxx        comments are # though to newline\n");
  printf ("Hex numbers must be entered in upper case, to distinguish them from the\n");
  printf ("variables a to f (like in bc).\n");
}


int  ibase = 0;
int  obase = 10;


/* The stack is a fixed size, which means there's a limit on the nesting
   allowed in expressions.  A more sophisticated program could let it grow
   dynamically.  */

mpz_t    stack[100];
mpz_ptr  sp = stack[0];

#define CHECK_OVERFLOW()                                                  \
  if (sp >= stack[numberof(stack)])                                       \
    {                                                                     \
      fprintf (stderr,                                                    \
               "Value stack overflow, too much nesting in expression\n"); \
      YYERROR;                                                            \
    }

#define CHECK_EMPTY()                                                   \
  if (sp != stack[0])                                                   \
    {                                                                   \
      fprintf (stderr, "Oops, expected the value stack to be empty\n"); \
      sp = stack[0];                                                    \
    }


mpz_t  variable[26];

#define CHECK_VARIABLE(var)                                             \
  if ((var) < 0 || (var) >= numberof (variable))                        \
    {                                                                   \
      fprintf (stderr, "Oops, bad variable somehow: %d\n", var);        \
      YYERROR;                                                          \
    }


#define CHECK_UI(name,z)                        \
  if (! mpz_fits_ulong_p (z))                   \
    {                                           \
      fprintf (stderr, "%s too big\n", name);   \
      YYERROR;                                  \
    }


# line 143 "calc.y"
typedef union
#ifdef __cplusplus
	YYSTYPE
#endif
 {
  char  *str;
  int   var;
} YYSTYPE;
# define EOS 257
# define BAD 258
# define HELP 259
# define HEX 260
# define DECIMAL 261
# define QUIT 262
# define ABS 263
# define BIN 264
# define FIB 265
# define GCD 266
# define KRON 267
# define LCM 268
# define LUCNUM 269
# define NEXTPRIME 270
# define POWM 271
# define ROOT 272
# define SQRT 273
# define NUMBER 274
# define VARIABLE 275
# define LOR 276
# define LAND 277
# define EQ 278
# define NE 279
# define LE 280
# define GE 281
# define LSHIFT 282
# define RSHIFT 283
# define UMINUS 284

#include <inttypes.h>

#ifdef __STDC__
#include <stdlib.h>
#include <string.h>
#define	YYCONST	const
#else
#include <malloc.h>
#include <memory.h>
#define	YYCONST
#endif

#include <values.h>

#if defined(__cplusplus) || defined(__STDC__)

#if defined(__cplusplus) && defined(__EXTERN_C__)
extern "C" {
#endif
#ifndef yyerror
#if defined(__cplusplus)
	void yyerror(YYCONST char *);
#endif
#endif
#ifndef yylex
	int yylex(void);
#endif
	int yyparse(void);
#if defined(__cplusplus) && defined(__EXTERN_C__)
}
#endif

#endif

#define yyclearin yychar = -1
#define yyerrok yyerrflag = 0
extern int yychar;
extern int yyerrflag;
YYSTYPE yylval;
YYSTYPE yyval;
typedef int yytabelem;
#ifndef YYMAXDEPTH
#define YYMAXDEPTH 150
#endif
#if YYMAXDEPTH > 0
int yy_yys[YYMAXDEPTH], *yys = yy_yys;
YYSTYPE yy_yyv[YYMAXDEPTH], *yyv = yy_yyv;
#else	/* user does initial allocation */
int *yys;
YYSTYPE *yyv;
#endif
static int yymaxdepth = YYMAXDEPTH;
# define YYERRCODE 256

# line 265 "calc.y"


yyerror (char *s)
{
  fprintf (stderr, "%s\n", s);
}

int calc_option_readline = -1;

int
main (int argc, char *argv[])
{
  int  i;

  for (i = 1; i < argc; i++)
    {
      if (strcmp (argv[i], "--readline") == 0)
        calc_option_readline = 1;
      else if (strcmp (argv[i], "--noreadline") == 0)
        calc_option_readline = 0;
      else if (strcmp (argv[i], "--help") == 0)
        {
          printf ("Usage: calc [--option]...\n");
          printf ("  --readline    use readline\n");
          printf ("  --noreadline  don't use readline\n");
          printf ("  --help        this message\n");
          printf ("Readline is only available when compiled in,\n");
          printf ("and in that case it's the default on a tty.\n");
          exit (0);
        }
      else
        {
          fprintf (stderr, "Unrecognised option: %s\n", argv[i]);
          exit (1);
        }
    }

#if WITH_READLINE
  calc_init_readline ();
#else
  if (calc_option_readline == 1)
    {
      fprintf (stderr, "Readline support not available\n");
      exit (1);
    }
#endif

  for (i = 0; i < numberof (variable); i++)
    mpz_init (variable[i]);

  for (i = 0; i < numberof (stack); i++)
    mpz_init (stack[i]);

  return yyparse ();
}
static YYCONST yytabelem yyexca[] ={
-1, 0,
	0, 6,
	257, 6,
	-2, 0,
-1, 1,
	0, -1,
	-2, 0,
-1, 69,
	60, 0,
	62, 0,
	278, 0,
	279, 0,
	280, 0,
	281, 0,
	-2, 24,
-1, 70,
	60, 0,
	62, 0,
	278, 0,
	279, 0,
	280, 0,
	281, 0,
	-2, 25,
-1, 71,
	60, 0,
	62, 0,
	278, 0,
	279, 0,
	280, 0,
	281, 0,
	-2, 26,
-1, 72,
	60, 0,
	62, 0,
	278, 0,
	279, 0,
	280, 0,
	281, 0,
	-2, 27,
-1, 73,
	60, 0,
	62, 0,
	278, 0,
	279, 0,
	280, 0,
	281, 0,
	-2, 28,
-1, 74,
	60, 0,
	62, 0,
	278, 0,
	279, 0,
	280, 0,
	281, 0,
	-2, 29,
	};
# define YYNPROD 49
# define YYLAST 733
static YYCONST yytabelem yyact[]={

    35,    60,    45,    25,    31,    44,    59,    35,   116,    29,
    27,    35,    28,    98,    30,    31,    99,    58,    85,   114,
    29,    27,    35,    28,    57,    30,    31,    36,    56,    41,
    55,    29,    27,   113,    28,    54,    30,    53,    36,    95,
    41,    52,    96,    51,    35,    50,    49,    82,    31,    36,
     3,    41,   112,    29,    27,    35,    28,     1,    30,    31,
     0,    32,     0,   111,    29,    27,    35,    28,    32,    30,
    31,    36,    32,    41,   104,    29,    27,    35,    28,     0,
    30,    31,    36,    32,    41,     0,    29,    27,   103,    28,
     2,    30,     0,    36,    26,    41,     0,     0,     0,    35,
     0,     0,     0,    31,    36,    32,    41,     0,    29,    27,
   102,    28,     0,    30,     0,     0,    32,     0,     0,     0,
     0,    35,     0,     0,     0,    31,    36,    32,    41,   101,
    29,    27,    35,    28,     0,    30,    31,     0,    32,     0,
   100,    29,    27,    35,    28,     0,    30,    31,    36,     0,
    41,     0,    29,    27,    97,    28,     0,    30,     0,    36,
    32,    41,     0,     0,     0,    35,     0,     0,     0,    31,
    36,     0,    41,    94,    29,    27,    35,    28,     0,    30,
    31,     0,    32,     0,     0,    29,    27,    93,    28,     0,
    30,     0,    36,    32,    41,     0,     0,     0,     0,     0,
     0,     0,    35,    36,    32,    41,    31,     0,     0,     0,
    92,    29,    27,    35,    28,     0,    30,    31,     0,     0,
     0,    78,    29,    27,    35,    28,    32,    30,    31,    36,
     0,    41,     0,    29,    27,     0,    28,    32,    30,     0,
    36,     0,    41,    43,    42,    38,    39,    37,    40,    33,
    34,    36,     0,    41,    43,    42,    38,    39,    37,    40,
    33,    34,     0,    32,     0,    43,    42,    38,    39,    37,
    40,    33,    34,     0,    32,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,    32,     0,    43,    42,    38,
    39,    37,    40,    33,    34,     0,     0,     0,    43,    42,
    38,    39,    37,    40,    33,    34,     0,     0,     0,    43,
    42,    38,    39,    37,    40,    33,    34,     0,     0,     0,
    43,    42,    38,    39,    37,    40,    33,    34,    11,     0,
     0,     0,     0,    12,     0,     0,     0,     0,     0,     0,
     0,     0,    43,    42,    38,    39,    37,    40,    33,    34,
    11,     0,     0,     0,     0,    12,     0,     0,     0,     0,
     0,     0,     0,     0,    43,    42,    38,    39,    37,    40,
    33,    34,     0,     0,     0,    43,    42,    38,    39,    37,
    40,    33,    34,     0,     0,     0,    43,    42,    38,    39,
    37,    40,    33,    34,    11,     0,     0,     0,     0,    12,
     0,     0,     0,     0,     0,     0,     0,     0,    43,    42,
    38,    39,    37,    40,    33,    34,     0,     0,     0,    43,
    42,    38,    39,    37,    40,    33,    34,    35,     0,    35,
     0,    31,     0,    31,     0,     0,    29,    27,    29,    28,
     0,    30,     0,    30,     0,    43,    42,    38,    39,    37,
    40,    33,    34,     0,     0,     0,    43,    42,    38,    39,
    37,    40,    33,    34,     0,     0,     0,    43,    42,    38,
    39,    37,    40,    33,    34,    35,     0,     0,     0,    31,
     0,     0,    35,     0,    29,    27,    31,    28,    32,    30,
    32,    29,    27,     0,    28,     0,    30,     0,     0,     0,
     0,     0,    36,    35,    41,     0,     0,    31,     0,    36,
     0,    41,    29,    27,     0,    28,     0,    30,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,    32,     0,     0,     0,
     0,     0,     0,    32,    10,     0,     0,     6,     7,     8,
     9,    13,    14,    15,    16,    17,    18,    19,    20,    21,
    22,    23,    24,     5,    32,     0,     0,     0,     0,     6,
     7,     8,     9,    13,    14,    15,    16,    17,    18,    19,
    20,    21,    22,    23,    24,     5,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     4,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,    46,    48,     0,    13,    14,    15,
    16,    17,    18,    19,    20,    21,    22,    23,    24,    47,
    61,    62,    63,    64,    65,    66,    67,    68,     0,    69,
    70,    71,    72,    73,    74,    75,    76,    77,     0,     0,
     0,     0,    79,    80,    81,    83,    84,    86,    87,    88,
    89,    90,    91,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,    33,    34,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,   105,     0,     0,   106,
   107,     0,   108,     0,     0,   109,   110,     0,     0,     0,
     0,     0,     0,     0,     0,     0,   115,     0,     0,    42,
    38,    39,    37,    40,    33,    34,     0,    38,    39,    37,
    40,    33,    34 };
static YYCONST yytabelem yypact[]={

   288,-10000000,  -254,   310,   191,   -56,-10000000,-10000000,-10000000,-10000000,
  -255,   354,   354,     6,     5,     3,     1,    -3,    -5,   -10,
   -12,   -16,   -23,   -34,-10000000,-10000000,  -256,   354,   354,   354,
   354,   354,   354,   354,   354,-10000000,   354,   354,   354,   354,
   354,   354,   354,   354,   354,-10000000,   180,-10000000,   -26,   354,
   354,   354,   354,   354,   354,   354,   354,   354,   354,   354,
-10000000,   396,   396,   -26,   -26,   -26,   -26,   470,   470,   394,
   394,   394,   394,   394,   394,   449,   442,   191,-10000000,   169,
   143,   132,    -2,   191,   110,   -28,   191,    99,    88,    66,
    44,    33,-10000000,   354,-10000000,-10000000,   354,   354,-10000000,   354,
-10000000,-10000000,   354,   354,-10000000,    22,   191,    11,   191,   -11,
   -22,-10000000,-10000000,   354,-10000000,   -33,-10000000 };
static YYCONST yytabelem yypgo[]={

     0,    57,    90,    50,   602,    47,    18 };
static YYCONST yytabelem yyr1[]={

     0,     1,     1,     3,     3,     3,     2,     2,     2,     2,
     2,     2,     2,     4,     4,     4,     4,     4,     4,     4,
     4,     4,     4,     4,     4,     4,     4,     4,     4,     4,
     4,     4,     4,     4,     4,     4,     4,     4,     4,     4,
     4,     4,     4,     4,     4,     5,     5,     6,     6 };
static YYCONST yytabelem yyr2[]={

     0,     2,     4,     4,     6,     5,     0,     3,     7,     3,
     3,     3,     3,     6,     7,     7,     7,     7,     7,     7,
     7,     7,     5,     5,     7,     7,     7,     7,     7,     7,
     7,     7,     9,    13,     9,     8,    13,     8,     9,     9,
    17,    13,     9,     3,     3,     2,     7,     2,     7 };
static YYCONST yytabelem yychk[]={

-10000000,    -1,    -2,    -3,    -4,   275,   259,   260,   261,   262,
   256,    40,    45,   263,   264,   265,   266,   267,   268,   269,
   270,   271,   272,   273,   274,   257,    -2,    43,    45,    42,
    47,    37,    94,   282,   283,    33,    60,   280,   278,   279,
   281,    62,   277,   276,    61,   257,    -4,   275,    -4,    40,
    40,    40,    40,    40,    40,    40,    40,    40,    40,    40,
   257,    -4,    -4,    -4,    -4,    -4,    -4,    -4,    -4,    -4,
    -4,    -4,    -4,    -4,    -4,    -4,    -4,    -4,    41,    -4,
    -4,    -4,    -5,    -4,    -4,    -6,    -4,    -4,    -4,    -4,
    -4,    -4,    41,    44,    41,    41,    44,    44,    41,    44,
    41,    41,    44,    44,    41,    -4,    -4,    -4,    -4,    -4,
    -4,    41,    41,    44,    41,    -4,    41 };
static YYCONST yytabelem yydef[]={

    -2,    -2,     1,     6,     7,    43,     9,    10,    11,    12,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,    44,     3,     2,     0,     0,     0,
     0,     0,     0,     0,     0,    22,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     5,     0,    43,    23,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     4,    14,    15,    16,    17,    18,    19,    20,    21,    -2,
    -2,    -2,    -2,    -2,    -2,    30,    31,     8,    13,     0,
     0,     0,     0,    45,     0,     0,    47,     0,     0,     0,
     0,     0,    32,     0,    34,    35,     0,     0,    37,     0,
    38,    39,     0,     0,    42,     0,    46,     0,    48,     0,
     0,    33,    36,     0,    41,     0,    40 };
typedef struct
#ifdef __cplusplus
	yytoktype
#endif
{
#ifdef __cplusplus
const
#endif
char *t_name; int t_val; } yytoktype;
#ifndef YYDEBUG
#	define YYDEBUG	0	/* don't allow debugging */
#endif

#if YYDEBUG

yytoktype yytoks[] =
{
	"EOS",	257,
	"BAD",	258,
	"HELP",	259,
	"HEX",	260,
	"DECIMAL",	261,
	"QUIT",	262,
	"ABS",	263,
	"BIN",	264,
	"FIB",	265,
	"GCD",	266,
	"KRON",	267,
	"LCM",	268,
	"LUCNUM",	269,
	"NEXTPRIME",	270,
	"POWM",	271,
	"ROOT",	272,
	"SQRT",	273,
	"NUMBER",	274,
	"VARIABLE",	275,
	"LOR",	276,
	"LAND",	277,
	"<",	60,
	">",	62,
	"EQ",	278,
	"NE",	279,
	"LE",	280,
	"GE",	281,
	"LSHIFT",	282,
	"RSHIFT",	283,
	"+",	43,
	"-",	45,
	"*",	42,
	"/",	47,
	"%",	37,
	"UMINUS",	284,
	"^",	94,
	"!",	33,
	"-unknown-",	-1	/* ends search */
};

#ifdef __cplusplus
const
#endif
char * yyreds[] =
{
	"-no such reduction-",
	"top : statement",
	"top : statements statement",
	"statements : statement EOS",
	"statements : statements statement EOS",
	"statements : error EOS",
	"statement : /* empty */",
	"statement : e",
	"statement : VARIABLE '=' e",
	"statement : HELP",
	"statement : HEX",
	"statement : DECIMAL",
	"statement : QUIT",
	"e : '(' e ')'",
	"e : e '+' e",
	"e : e '-' e",
	"e : e '*' e",
	"e : e '/' e",
	"e : e '%' e",
	"e : e '^' e",
	"e : e LSHIFT e",
	"e : e RSHIFT e",
	"e : e '!'",
	"e : '-' e",
	"e : e '<' e",
	"e : e LE e",
	"e : e EQ e",
	"e : e NE e",
	"e : e GE e",
	"e : e '>' e",
	"e : e LAND e",
	"e : e LOR e",
	"e : ABS '(' e ')'",
	"e : BIN '(' e ',' e ')'",
	"e : FIB '(' e ')'",
	"e : GCD '(' gcdlist ')'",
	"e : KRON '(' e ',' e ')'",
	"e : LCM '(' lcmlist ')'",
	"e : LUCNUM '(' e ')'",
	"e : NEXTPRIME '(' e ')'",
	"e : POWM '(' e ',' e ',' e ')'",
	"e : ROOT '(' e ',' e ')'",
	"e : SQRT '(' e ')'",
	"e : VARIABLE",
	"e : NUMBER",
	"gcdlist : e",
	"gcdlist : gcdlist ',' e",
	"lcmlist : e",
	"lcmlist : lcmlist ',' e",
};
#endif /* YYDEBUG */
# line	1 "/usr/ccs/bin/yaccpar"
/*
 * Copyright (c) 1993 by Sun Microsystems, Inc.
 */

#pragma ident	"@(#)yaccpar	6.16	99/01/20 SMI"

/*
** Skeleton parser driver for yacc output
*/

/*
** yacc user known macros and defines
*/
#define YYERROR		goto yyerrlab
#define YYACCEPT	return(0)
#define YYABORT		return(1)
#define YYBACKUP( newtoken, newvalue )\
{\
	if ( yychar >= 0 || ( yyr2[ yytmp ] >> 1 ) != 1 )\
	{\
		yyerror( "syntax error - cannot backup" );\
		goto yyerrlab;\
	}\
	yychar = newtoken;\
	yystate = *yyps;\
	yylval = newvalue;\
	goto yynewstate;\
}
#define YYRECOVERING()	(!!yyerrflag)
#define YYNEW(type)	malloc(sizeof(type) * yynewmax)
#define YYCOPY(to, from, type) \
	(type *) memcpy(to, (char *) from, yymaxdepth * sizeof (type))
#define YYENLARGE( from, type) \
	(type *) realloc((char *) from, yynewmax * sizeof(type))
#ifndef YYDEBUG
#	define YYDEBUG	1	/* make debugging available */
#endif

/*
** user known globals
*/
int yydebug;			/* set to 1 to get debugging */

/*
** driver internal defines
*/
#define YYFLAG		(-10000000)

/*
** global variables used by the parser
*/
YYSTYPE *yypv;			/* top of value stack */
int *yyps;			/* top of state stack */

int yystate;			/* current state */
int yytmp;			/* extra var (lasts between blocks) */

int yynerrs;			/* number of errors */
int yyerrflag;			/* error recovery flag */
int yychar;			/* current input token number */



#ifdef YYNMBCHARS
#define YYLEX()		yycvtok(yylex())
/*
** yycvtok - return a token if i is a wchar_t value that exceeds 255.
**	If i<255, i itself is the token.  If i>255 but the neither 
**	of the 30th or 31st bit is on, i is already a token.
*/
#if defined(__STDC__) || defined(__cplusplus)
int yycvtok(int i)
#else
int yycvtok(i) int i;
#endif
{
	int first = 0;
	int last = YYNMBCHARS - 1;
	int mid;
	wchar_t j;

	if(i&0x60000000){/*Must convert to a token. */
		if( yymbchars[last].character < i ){
			return i;/*Giving up*/
		}
		while ((last>=first)&&(first>=0)) {/*Binary search loop*/
			mid = (first+last)/2;
			j = yymbchars[mid].character;
			if( j==i ){/*Found*/ 
				return yymbchars[mid].tvalue;
			}else if( j<i ){
				first = mid + 1;
			}else{
				last = mid -1;
			}
		}
		/*No entry in the table.*/
		return i;/* Giving up.*/
	}else{/* i is already a token. */
		return i;
	}
}
#else/*!YYNMBCHARS*/
#define YYLEX()		yylex()
#endif/*!YYNMBCHARS*/

/*
** yyparse - return 0 if worked, 1 if syntax error not recovered from
*/
#if defined(__STDC__) || defined(__cplusplus)
int yyparse(void)
#else
int yyparse()
#endif
{
	register YYSTYPE *yypvt = 0;	/* top of value stack for $vars */

#if defined(__cplusplus) || defined(lint)
/*
	hacks to please C++ and lint - goto's inside
	switch should never be executed
*/
	static int __yaccpar_lint_hack__ = 0;
	switch (__yaccpar_lint_hack__)
	{
		case 1: goto yyerrlab;
		case 2: goto yynewstate;
	}
#endif

	/*
	** Initialize externals - yyparse may be called more than once
	*/
	yypv = &yyv[-1];
	yyps = &yys[-1];
	yystate = 0;
	yytmp = 0;
	yynerrs = 0;
	yyerrflag = 0;
	yychar = -1;

#if YYMAXDEPTH <= 0
	if (yymaxdepth <= 0)
	{
		if ((yymaxdepth = YYEXPAND(0)) <= 0)
		{
			yyerror("yacc initialization error");
			YYABORT;
		}
	}
#endif

	{
		register YYSTYPE *yy_pv;	/* top of value stack */
		register int *yy_ps;		/* top of state stack */
		register int yy_state;		/* current state */
		register int  yy_n;		/* internal state number info */
	goto yystack;	/* moved from 6 lines above to here to please C++ */

		/*
		** get globals into registers.
		** branch to here only if YYBACKUP was called.
		*/
	yynewstate:
		yy_pv = yypv;
		yy_ps = yyps;
		yy_state = yystate;
		goto yy_newstate;

		/*
		** get globals into registers.
		** either we just started, or we just finished a reduction
		*/
	yystack:
		yy_pv = yypv;
		yy_ps = yyps;
		yy_state = yystate;

		/*
		** top of for (;;) loop while no reductions done
		*/
	yy_stack:
		/*
		** put a state and value onto the stacks
		*/
#if YYDEBUG
		/*
		** if debugging, look up token value in list of value vs.
		** name pairs.  0 and negative (-1) are special values.
		** Note: linear search is used since time is not a real
		** consideration while debugging.
		*/
		if ( yydebug )
		{
			register int yy_i;

			printf( "State %d, token ", yy_state );
			if ( yychar == 0 )
				printf( "end-of-file\n" );
			else if ( yychar < 0 )
				printf( "-none-\n" );
			else
			{
				for ( yy_i = 0; yytoks[yy_i].t_val >= 0;
					yy_i++ )
				{
					if ( yytoks[yy_i].t_val == yychar )
						break;
				}
				printf( "%s\n", yytoks[yy_i].t_name );
			}
		}
#endif /* YYDEBUG */
		if ( ++yy_ps >= &yys[ yymaxdepth ] )	/* room on stack? */
		{
			/*
			** reallocate and recover.  Note that pointers
			** have to be reset, or bad things will happen
			*/
			long yyps_index = (yy_ps - yys);
			long yypv_index = (yy_pv - yyv);
			long yypvt_index = (yypvt - yyv);
			int yynewmax;
#ifdef YYEXPAND
			yynewmax = YYEXPAND(yymaxdepth);
#else
			yynewmax = 2 * yymaxdepth;	/* double table size */
			if (yymaxdepth == YYMAXDEPTH)	/* first time growth */
			{
				char *newyys = (char *)YYNEW(int);
				char *newyyv = (char *)YYNEW(YYSTYPE);
				if (newyys != 0 && newyyv != 0)
				{
					yys = YYCOPY(newyys, yys, int);
					yyv = YYCOPY(newyyv, yyv, YYSTYPE);
				}
				else
					yynewmax = 0;	/* failed */
			}
			else				/* not first time */
			{
				yys = YYENLARGE(yys, int);
				yyv = YYENLARGE(yyv, YYSTYPE);
				if (yys == 0 || yyv == 0)
					yynewmax = 0;	/* failed */
			}
#endif
			if (yynewmax <= yymaxdepth)	/* tables not expanded */
			{
				yyerror( "yacc stack overflow" );
				YYABORT;
			}
			yymaxdepth = yynewmax;

			yy_ps = yys + yyps_index;
			yy_pv = yyv + yypv_index;
			yypvt = yyv + yypvt_index;
		}
		*yy_ps = yy_state;
		*++yy_pv = yyval;

		/*
		** we have a new state - find out what to do
		*/
	yy_newstate:
		if ( ( yy_n = yypact[ yy_state ] ) <= YYFLAG )
			goto yydefault;		/* simple state */
#if YYDEBUG
		/*
		** if debugging, need to mark whether new token grabbed
		*/
		yytmp = yychar < 0;
#endif
		if ( ( yychar < 0 ) && ( ( yychar = YYLEX() ) < 0 ) )
			yychar = 0;		/* reached EOF */
#if YYDEBUG
		if ( yydebug && yytmp )
		{
			register int yy_i;

			printf( "Received token " );
			if ( yychar == 0 )
				printf( "end-of-file\n" );
			else if ( yychar < 0 )
				printf( "-none-\n" );
			else
			{
				for ( yy_i = 0; yytoks[yy_i].t_val >= 0;
					yy_i++ )
				{
					if ( yytoks[yy_i].t_val == yychar )
						break;
				}
				printf( "%s\n", yytoks[yy_i].t_name );
			}
		}
#endif /* YYDEBUG */
		if ( ( ( yy_n += yychar ) < 0 ) || ( yy_n >= YYLAST ) )
			goto yydefault;
		if ( yychk[ yy_n = yyact[ yy_n ] ] == yychar )	/*valid shift*/
		{
			yychar = -1;
			yyval = yylval;
			yy_state = yy_n;
			if ( yyerrflag > 0 )
				yyerrflag--;
			goto yy_stack;
		}

	yydefault:
		if ( ( yy_n = yydef[ yy_state ] ) == -2 )
		{
#if YYDEBUG
			yytmp = yychar < 0;
#endif
			if ( ( yychar < 0 ) && ( ( yychar = YYLEX() ) < 0 ) )
				yychar = 0;		/* reached EOF */
#if YYDEBUG
			if ( yydebug && yytmp )
			{
				register int yy_i;

				printf( "Received token " );
				if ( yychar == 0 )
					printf( "end-of-file\n" );
				else if ( yychar < 0 )
					printf( "-none-\n" );
				else
				{
					for ( yy_i = 0;
						yytoks[yy_i].t_val >= 0;
						yy_i++ )
					{
						if ( yytoks[yy_i].t_val
							== yychar )
						{
							break;
						}
					}
					printf( "%s\n", yytoks[yy_i].t_name );
				}
			}
#endif /* YYDEBUG */
			/*
			** look through exception table
			*/
			{
				register YYCONST int *yyxi = yyexca;

				while ( ( *yyxi != -1 ) ||
					( yyxi[1] != yy_state ) )
				{
					yyxi += 2;
				}
				while ( ( *(yyxi += 2) >= 0 ) &&
					( *yyxi != yychar ) )
					;
				if ( ( yy_n = yyxi[1] ) < 0 )
					YYACCEPT;
			}
		}

		/*
		** check for syntax error
		*/
		if ( yy_n == 0 )	/* have an error */
		{
			/* no worry about speed here! */
			switch ( yyerrflag )
			{
			case 0:		/* new error */
				yyerror( "syntax error" );
				goto skip_init;
			yyerrlab:
				/*
				** get globals into registers.
				** we have a user generated syntax type error
				*/
				yy_pv = yypv;
				yy_ps = yyps;
				yy_state = yystate;
			skip_init:
				yynerrs++;
				/* FALLTHRU */
			case 1:
			case 2:		/* incompletely recovered error */
					/* try again... */
				yyerrflag = 3;
				/*
				** find state where "error" is a legal
				** shift action
				*/
				while ( yy_ps >= yys )
				{
					yy_n = yypact[ *yy_ps ] + YYERRCODE;
					if ( yy_n >= 0 && yy_n < YYLAST &&
						yychk[yyact[yy_n]] == YYERRCODE)					{
						/*
						** simulate shift of "error"
						*/
						yy_state = yyact[ yy_n ];
						goto yy_stack;
					}
					/*
					** current state has no shift on
					** "error", pop stack
					*/
#if YYDEBUG
#	define _POP_ "Error recovery pops state %d, uncovers state %d\n"
					if ( yydebug )
						printf( _POP_, *yy_ps,
							yy_ps[-1] );
#	undef _POP_
#endif
					yy_ps--;
					yy_pv--;
				}
				/*
				** there is no state on stack with "error" as
				** a valid shift.  give up.
				*/
				YYABORT;
			case 3:		/* no shift yet; eat a token */
#if YYDEBUG
				/*
				** if debugging, look up token in list of
				** pairs.  0 and negative shouldn't occur,
				** but since timing doesn't matter when
				** debugging, it doesn't hurt to leave the
				** tests here.
				*/
				if ( yydebug )
				{
					register int yy_i;

					printf( "Error recovery discards " );
					if ( yychar == 0 )
						printf( "token end-of-file\n" );
					else if ( yychar < 0 )
						printf( "token -none-\n" );
					else
					{
						for ( yy_i = 0;
							yytoks[yy_i].t_val >= 0;
							yy_i++ )
						{
							if ( yytoks[yy_i].t_val
								== yychar )
							{
								break;
							}
						}
						printf( "token %s\n",
							yytoks[yy_i].t_name );
					}
				}
#endif /* YYDEBUG */
				if ( yychar == 0 )	/* reached EOF. quit */
					YYABORT;
				yychar = -1;
				goto yy_newstate;
			}
		}/* end if ( yy_n == 0 ) */
		/*
		** reduction by production yy_n
		** put stack tops, etc. so things right after switch
		*/
#if YYDEBUG
		/*
		** if debugging, print the string that is the user's
		** specification of the reduction which is just about
		** to be done.
		*/
		if ( yydebug )
			printf( "Reduce by (%d) \"%s\"\n",
				yy_n, yyreds[ yy_n ] );
#endif
		yytmp = yy_n;			/* value to switch over */
		yypvt = yy_pv;			/* $vars top of value stack */
		/*
		** Look in goto table for next state
		** Sorry about using yy_state here as temporary
		** register variable, but why not, if it works...
		** If yyr2[ yy_n ] doesn't have the low order bit
		** set, then there is no action to be done for
		** this reduction.  So, no saving & unsaving of
		** registers done.  The only difference between the
		** code just after the if and the body of the if is
		** the goto yy_stack in the body.  This way the test
		** can be made before the choice of what to do is needed.
		*/
		{
			/* length of production doubled with extra bit */
			register int yy_len = yyr2[ yy_n ];

			if ( !( yy_len & 01 ) )
			{
				yy_len >>= 1;
				yyval = ( yy_pv -= yy_len )[1];	/* $$ = $1 */
				yy_state = yypgo[ yy_n = yyr1[ yy_n ] ] +
					*( yy_ps -= yy_len ) + 1;
				if ( yy_state >= YYLAST ||
					yychk[ yy_state =
					yyact[ yy_state ] ] != -yy_n )
				{
					yy_state = yyact[ yypgo[ yy_n ] ];
				}
				goto yy_stack;
			}
			yy_len >>= 1;
			yyval = ( yy_pv -= yy_len )[1];	/* $$ = $1 */
			yy_state = yypgo[ yy_n = yyr1[ yy_n ] ] +
				*( yy_ps -= yy_len ) + 1;
			if ( yy_state >= YYLAST ||
				yychk[ yy_state = yyact[ yy_state ] ] != -yy_n )
			{
				yy_state = yyact[ yypgo[ yy_n ] ];
			}
		}
					/* save until reenter driver code */
		yystate = yy_state;
		yyps = yy_ps;
		yypv = yy_pv;
	}
	/*
	** code supplied by user is placed in this switch
	*/
	switch( yytmp )
	{
		
case 5:
# line 174 "calc.y"
{ sp = stack[0]; yyerrok; } break;
case 7:
# line 178 "calc.y"
{
      mpz_out_str (stdout, obase, sp); putchar ('\n');
      sp--;
      CHECK_EMPTY ();
    } break;
case 8:
# line 183 "calc.y"
{
      CHECK_VARIABLE (yypvt[-2].var);
      mpz_swap (variable[yypvt[-2].var], sp);
      sp--;
      CHECK_EMPTY ();
    } break;
case 9:
# line 189 "calc.y"
{ calc_help (); } break;
case 10:
# line 190 "calc.y"
{ ibase = 16; obase = -16; } break;
case 11:
# line 191 "calc.y"
{ ibase = 0;  obase = 10; } break;
case 12:
# line 192 "calc.y"
{ exit (0); } break;
case 14:
# line 199 "calc.y"
{ sp--; mpz_add    (sp, sp, sp+1); } break;
case 15:
# line 200 "calc.y"
{ sp--; mpz_sub    (sp, sp, sp+1); } break;
case 16:
# line 201 "calc.y"
{ sp--; mpz_mul    (sp, sp, sp+1); } break;
case 17:
# line 202 "calc.y"
{ sp--; mpz_fdiv_q (sp, sp, sp+1); } break;
case 18:
# line 203 "calc.y"
{ sp--; mpz_fdiv_r (sp, sp, sp+1); } break;
case 19:
# line 204 "calc.y"
{ CHECK_UI ("Exponent", sp);
                    sp--; mpz_pow_ui (sp, sp, mpz_get_ui (sp+1)); } break;
case 20:
# line 206 "calc.y"
{ CHECK_UI ("Shift count", sp);
                    sp--; mpz_mul_2exp (sp, sp, mpz_get_ui (sp+1)); } break;
case 21:
# line 208 "calc.y"
{ CHECK_UI ("Shift count", sp);
                    sp--; mpz_fdiv_q_2exp (sp, sp, mpz_get_ui (sp+1)); } break;
case 22:
# line 210 "calc.y"
{ CHECK_UI ("Factorial", sp);
                    mpz_fac_ui (sp, mpz_get_ui (sp)); } break;
case 23:
# line 212 "calc.y"
{ mpz_neg (sp, sp); } break;
case 24:
# line 214 "calc.y"
{ sp--; mpz_set_ui (sp, mpz_cmp (sp, sp+1) <  0); } break;
case 25:
# line 215 "calc.y"
{ sp--; mpz_set_ui (sp, mpz_cmp (sp, sp+1) <= 0); } break;
case 26:
# line 216 "calc.y"
{ sp--; mpz_set_ui (sp, mpz_cmp (sp, sp+1) == 0); } break;
case 27:
# line 217 "calc.y"
{ sp--; mpz_set_ui (sp, mpz_cmp (sp, sp+1) != 0); } break;
case 28:
# line 218 "calc.y"
{ sp--; mpz_set_ui (sp, mpz_cmp (sp, sp+1) >= 0); } break;
case 29:
# line 219 "calc.y"
{ sp--; mpz_set_ui (sp, mpz_cmp (sp, sp+1) >  0); } break;
case 30:
# line 221 "calc.y"
{ sp--; mpz_set_ui (sp, mpz_sgn (sp) && mpz_sgn (sp+1)); } break;
case 31:
# line 222 "calc.y"
{ sp--; mpz_set_ui (sp, mpz_sgn (sp) || mpz_sgn (sp+1)); } break;
case 32:
# line 224 "calc.y"
{ mpz_abs (sp, sp); } break;
case 33:
# line 225 "calc.y"
{ sp--; CHECK_UI ("Binomial base", sp+1);
                                   mpz_bin_ui (sp, sp, mpz_get_ui (sp+1)); } break;
case 34:
# line 227 "calc.y"
{ CHECK_UI ("Fibonacci", sp);
                                   mpz_fib_ui (sp, mpz_get_ui (sp)); } break;
case 36:
# line 230 "calc.y"
{ sp--; mpz_set_si (sp,
                                         mpz_kronecker (sp, sp+1)); } break;
case 38:
# line 233 "calc.y"
{ CHECK_UI ("Lucas number", sp);
                                   mpz_lucnum_ui (sp, mpz_get_ui (sp)); } break;
case 39:
# line 235 "calc.y"
{ mpz_nextprime (sp, sp); } break;
case 40:
# line 236 "calc.y"
{ sp -= 2; mpz_powm (sp, sp, sp+1, sp+2); } break;
case 41:
# line 237 "calc.y"
{ sp--; CHECK_UI ("Nth-root", sp+1);
                                   mpz_root (sp, sp, mpz_get_ui (sp+1)); } break;
case 42:
# line 239 "calc.y"
{ mpz_sqrt (sp, sp); } break;
case 43:
# line 241 "calc.y"
{
        sp++;
        CHECK_OVERFLOW ();
        CHECK_VARIABLE (yypvt[-0].var);
        mpz_set (sp, variable[yypvt[-0].var]);
      } break;
case 44:
# line 247 "calc.y"
{
        sp++;
        CHECK_OVERFLOW ();
        if (mpz_set_str (sp, yypvt[-0].str, ibase) != 0)
          {
            fprintf (stderr, "Invalid number: %s\n", yypvt[-0].str);
            YYERROR;
          }
      } break;
case 46:
# line 259 "calc.y"
{ sp--; mpz_gcd (sp, sp, sp+1); } break;
case 48:
# line 263 "calc.y"
{ sp--; mpz_lcm (sp, sp, sp+1); } break;
# line	531 "/usr/ccs/bin/yaccpar"
	}
	goto yystack;		/* reset registers in driver code */
}

