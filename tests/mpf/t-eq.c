/* Test mpf_eq

Copyright 2009 Jason Moxham

This file is part of the MPIR Library.

The MPIR Library is free software; you can redistribute it and/or modify
it under the terms of the GNU Lesser General Public License as published
by the Free Software Foundation; either version 2.1 of the License, or (at
your option) any later version.

The MPIR Library is distributed in the hope that it will be useful, but
WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public
License for more details.

You should have received a copy of the GNU Lesser General Public License
along with the MPIR Library; see the file COPYING.LIB.  If not, write
to the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
Boston, MA 02110-1301, USA.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "mpir.h"
#include "gmp-impl.h"
#include "tests.h"

int
main (int argc, char *argv[])
{mpf_t x,y;
 int i;
 
  tests_start ();
mpf_init(x);mpf_init(y);

mpf_set_ui(x,88);mpf_set_ui(y,89);
for(i=0;i<=6;i++)if(!mpf_eq(x,y,i)){printf("mpf error\n");abort();}
for(i=7;i<123;i++)if(mpf_eq(x,y,i)){printf("mpf error\n");abort();}

mpf_set_ui(x,256);mpf_set_ui(y,257);
for(i=0;i<=8;i++)if(!mpf_eq(x,y,i)){printf("mpf error\n");abort();}
for(i=9;i<123;i++)if(mpf_eq(x,y,i)){printf("mpf error\n");abort();}



mpf_clear(x);
mpf_clear(y);
  tests_end ();
  exit (0);
}
