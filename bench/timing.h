/* timing.h

Copyright 2003 Free Software Foundation, Inc.

This file is part of MPIRbench.

This program is free software; you can redistribute it and/or modify it under
the terms of the GNU General Public License as published by the Free Software
Foundation; either version 2 of the License, or (at your option) any later
version.

This program is distributed in the hope that it will be useful, but WITHOUT ANY
WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A
PARTICULAR PURPOSE.  See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with
this program; if not, write to the Free Software Foundation, Inc., 59 Temple
Place - Suite 330, Boston, MA 02111-1307, USA.  */

/* Return user CPU time measured in milliseconds.  */

#ifndef _TIMING_H
#define _TIMING_H

#if !defined (__sun) && (defined (USG) \
    || defined (__SVR4) || defined (_UNICOS) || defined (__hpux))

#include <time.h>

inline int
cputime()
{
  return (int) ((double) clock () * 1000 / CLOCKS_PER_SEC);
}

#else

#ifdef _MSC_VER

#include "getrusage.h"

#define inline __inline

#else

#include <sys/types.h>
#include <sys/time.h>
#include <sys/resource.h>

#endif

inline int
cputime ()
{
  struct rusage rus;

  getrusage (0, &rus);
  return rus.ru_utime.tv_sec * 1000 + rus.ru_utime.tv_usec / 1000;
}

#endif

#define TIME(t,func)						\
  do { long int __t0, __times, __t, __tmp;	\
    __times = 1;						    \
    {func;}							        \
    do						    			\
      {							    		\
	__times <<= 1;							\
	__t0 = cputime ();						\
	for (__t = 0; __t < __times; __t++)		\
	  {func;}						    	\
	__tmp = cputime () - __t0;				\
      }									    \
    while (__tmp < 250);					\
    (t) = (double) __tmp / __times;			\
  } while (0)

#endif /* _TIMING_H */
