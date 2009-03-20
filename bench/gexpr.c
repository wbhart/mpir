/* Expression evaluation using plain floating-point arithmetic.
   Copyright 1999, 2000, 2001, 2002, 2003, 2006 Free Software Foundation, Inc.

This program is free software; you can redistribute it and/or modify it under
the terms of the GNU General Public License as published by the Free Software
Foundation; either version 2, or (at your option) any later version.

This program is distributed in the hope that it will be useful, but WITHOUT ANY
WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A
PARTICULAR PURPOSE.  See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with
this program; see the file COPYING.  If not, write to the Free Software
Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301, USA.  */

#include <math.h>
#include <ctype.h>
#include <setjmp.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

jmp_buf top;

static char *bp, *expr_start_p, *expr_end_p;
static int ch;
static int previous_result_valid_flag;
double previous_result;

double term (void), expo (void), factor (void), number (void);

#define next skip ()

void
skip (void)
{
  do
    ch = *bp++;
  while (ch == ' ' || ch == '\n');
}

void
error (void)
{
  fprintf (stderr, "%s\n", expr_start_p);
  fprintf (stderr, "%*s^ syntax error\n", (int) (bp - expr_start_p) - 1, "");
  longjmp (top, 1);
}

double
expr (void)
{
  double e;
  if (ch == '+')
    {
      next;
      e = term ();
    }
  else if (ch == '-')
    {
      next;
      e = - term ();
    }
  else
    e = term ();
  while (ch == '+' || ch == '-')
    { char op;
      op = ch;
      next;
      if (op == '-')
	e -= term ();
      else
	e += term ();
    }
  return e;
}

double
term (void)
{
  double t;
  t = expo ();
  for (;;)
    switch (ch)
      {
      case '*':
	next;
	t *= expo ();
	break;
      case '/':
	next;
	t /= expo ();
	break;
      case '%':
	next;
	t = fmod (t, expo ());
	break;
      default:
	return t;
      }
}

double
expo (void)
{
  double e;
  e = factor ();
  if (ch == '^')
    {
      next;
      e = pow (e, expo ());
    }
  return e;
}

struct functions
{
  char *spelling;
  double (* evalfn) ();
};

double
my_log2 (double x)
{
  return log (x) * 1.4426950408889634073599246810019;
}

struct functions fns[] =
{
  {"log2", my_log2},
  {"log10", log10},
  {"log", log},
  {"exp", exp},
  {"sqrt", sqrt},
  {"floor", floor},
  {"ceil", ceil},
  {"sin", sin},
  {"cos", cos},
  {"tan", tan},
  {"asin", asin},
  {"acos", acos},
  {"atan", atan},
  {"sinh", sinh},
  {"cosh", cosh},
  {"tanh", tanh},
  {"asinh", asinh},
  {"acosh", acosh},
  {"atanh", atanh},
  {0, 0}
};


double
factor (void)
{
  double f;
  int i;

  for (i = 0; fns[i].spelling != 0; i++)
    {
      char *spelling = fns[i].spelling;
      int len = strlen (spelling);
      if (strncmp (spelling, bp - 1, len) == 0 && ! isalnum (bp[-1 + len]))
	{
	  bp += len - 1;
	  next;
	  if (ch != '(')
	    error ();
	  next;
	  f = expr ();
	  if (ch != ')')
	    error ();
	  next;
	  return (fns[i].evalfn) (f);
	}
    }

  if (ch == '(')
    {
      next;
      f = expr ();
      if (ch == ')')
	next;
      else
	error ();
    }
  else
    f = number ();
  if (ch == '!')
    {
      unsigned long n;
      if (floor (f) != f)
	error ();
      for (n = f, f = 1; n > 1; n--)
	f *= n;
      next;
    }
  return f;
}

double
number (void)
{
  double n;
  char *endp;

  if (strncmp ("pi", bp - 1, 2) == 0 && ! isalnum (bp[1]))
    {
      bp += 2 - 1;
      next;
      return 3.1415926535897932384626433832795;
    }
  if (ch == '$')
    {
      if (! previous_result_valid_flag)
	error ();
      next;
      return previous_result;
    }
  if (ch != '.' && (ch < '0' || ch > '9'))
    error ();
  n = strtod (bp - 1, &endp);
  if (endp == bp - 1)
    error ();
  bp = endp;
  next;
  return n;
}

int nl_flag = 1;
int hhmm_flag = 0;
int dhhmm_flag = 0;
int round_flag = 0;
int prec = 5;

void
output (double exp)
{
  int h, m;
  if (hhmm_flag)
    {
      m = exp * 60;
      h = m / 60;
      m -= h * 60;
      printf ("%02d:%02d", h, m);
    }
  else if (dhhmm_flag)
    {
      int d;
      m = exp * (24 * 60);
      d = m / (24 * 60);
      m -= d * (24 * 60);
      h = m / 60;
      m -= h * 60;
      printf ("%dd %02d:%02d", d, h, m);
    }
  else
    printf ("%.*g", prec, exp);
  if (nl_flag)
    puts ("");
  previous_result = exp;
  previous_result_valid_flag = 1;
}

int
main (int argc, char **argv)
{
  while (argc >= 2)
    {
      if (!strcmp (argv[1], "-n"))
	{
	  nl_flag = 0;
	  argv++;
	  argc--;
	}
      else if (!strcmp (argv[1], "-hhmm"))
	{
	  hhmm_flag = 1;
	  argv++;
	  argc--;
	}
      else if (!strcmp (argv[1], "-dhhmm"))
	{
	  dhhmm_flag = 1;
	  argv++;
	  argc--;
	}
      else if (!strcmp (argv[1], "-round"))
	{
	  round_flag = 1;
	  argv++;
	  argc--;
	}
      else if (!strcmp (argv[1], "-prec"))
	{
	  prec = atoi (argv[2]);
	  argv += 2;
	  argc -= 2;
	}
      else if (!strcmp (argv[1], "-help") || !strcmp (argv[1], "-h"))
	{
	  printf ("usage: %s [options] expr [expr ... ]\n", argv[0]);
	  printf ("   options:    -n      -- suppress newline\n");
	  printf ("               -prec n -- print n digits\n");
	  printf ("               -round  -- round to nearest integer\n");
	  printf ("               -hhmm   -- print in base 60 (time format)\n");
	  printf ("               -dhhmm  -- print in base 24,60,60 (time format)\n");
	  printf ("               -help   -- you've figured out that one\n");
	  exit (0);
	}
      else
	break;
    }

  if (argc >= 2)
    {
      int i;
      double exp;

      for (i = 1; i < argc; i++)
	{
	  expr_start_p = argv[i];
	  expr_end_p = expr_end_p + strlen (expr_start_p);
	  bp = expr_start_p;
	  next;
	  if (setjmp (top) == 0)
	    {
	      exp = expr ();
	      if (round_flag)
		exp = floor (exp + 0.5);
	      output (exp);
	    }
	}
    }
  else
    {
#define BUFSIZE 1024
      char buf[BUFSIZE];
      double exp;

      for (;;)
	{
	  fputs ("eval> ", stdout);
	  bp = fgets (buf, BUFSIZE, stdin);
	  if (bp == NULL)
	    break;
	  next;
	  if (setjmp (top) == 0)
	    {
	      exp = expr ();
	      if (round_flag)
		exp = floor (exp + 0.5);
	      output (exp);
	    }
	}
    }

  exit (0);
}
