
A Note On Licensing
===================

The GMP files used in this distribution have been derived from 
files licensed under Gnu LGPL v2.1 license terms. The license 
under which these files are distributed here are set out in the
files in the directory <build.vc9\gnu.license>.

Other files in this distribution that have been created by me 
for use in building GMP, MPIR and/or MPFR using Microsoft Visual 
Studio 2008 are hereby placed in the public domain.

Creating the Project Directories
================================

First obtain the GMP distribution, named gmp-<ver>, where <ver> 
is the version being used, for example gmp-4.2.1.  Unzip the 
files in the ZIP archive into a  directory tree with the 
gmp-<ver> directory as its root.  

Unzip the files in this distribution so that they are merged
into the above directory tree with the directories

  build.vc9    -- build files for gmp and mpfr
  mpn/x86i     -- the YASM x86 assembler files (Pentium family)
  mpn/x86_64i  -- the YASM x64 assembler files (AMD64 and Core2)

within the gmp root directory gmp-<ver>.

Using the Assembler Based Build Projects
========================================

If you wish to use the assembler files you will need the YASM 
x86/x64 assembler (r1438 or later) for Windows which can be 
obtained from:

  http://www.tortall.net/projects/yasm/

This assembler should be placed in the bin directory used by 
VC++, which, for Visual Stduio 2008, is typically:

 C:\Program Files (x86)\Microsoft Visual Studio 9.0\VC\bin
 
You will also need to move the yasm.rules file from this 
distribution into the directory where Visual Studio 2008 
expects to find it, which is typically:

C:\Program Files (x86)\Microsoft Visual Studio 9.0
			  \VC\VCProjectDefaults
 
Alternatively you can configure the path for rules files in 
the VC++ configuration dialogue.

The NASM assembler is no longer supported as it has problems
with include file directory handling that the NASM team are
not willing to fix.

You will need to install Python if you wish to use the scripts
that automate the GMP and MPFR tests. Otherwise these have to 
be compiled and run manually.

Compiling GMP with the Visual Studio C/C++
==========================================

These VC++ build projects are based on GMP 4.2.1. They do not 
work directly with Microsoft Visual C++ 2008 Express but a 
Python program is provided to convert them into a form that can 
be used. Some files in the GMP 4.2.1 distribution need to be 
modified in order to compile with VC++. This VC++ build 
dsitribution makes these changes so you need to back up your 
GMP files if you want to retain their original form.  This 
also means that the files that change may need to be updated 
if the original GMP files from which they are derived are 
modified by the GMP team.

Building with Visual Studio 2005
================================

The Python program vc98_swap.py will convert VC9 build projects 
into those needed for Visual Studio 2005 (VC8).  It will also
convert files that have been converted in this way back 
into their original form.  It does this conversion by 
looking for *.vcproj files in the current working directory 
and its sub-directories and changing the following line in 
each of them:

    Version="9.00"

to:

    Version="8.00"

or vice versa.
 
Because it acts recursively on all sub-directories of this 
directory it is important not to run it at a directory level 
in which not all projects are to be converted.

Building with Visual Studio Express
===================================

If you wish to convert these files for use with VC++ Express, 
you will need to install Python and then run vc9_to_express.py. 
This will create a new directory 'build.vc9x' that will contain 
the project files for VC++ 2008 Express. There will be two errors
when the solution is loaded because Express cannot handle two
64-bit projects.  But these should not prevent other projects 
in the solution loading. The two unavailable 64-bit projects
should then be deleted to prevent future loading errors.

Building GMP
============

The GMP build is started by opening the Visual Studion C/C++ 
solution file 'gmp.sln'.

If you wish to convert these files for use with VC++ Express, 
you will need to install Python and then run vc9toxpr.py. This
will create a new directory 'build.vc9x' that will contain the
project files for VC++ 2008 Express.  If you are using VC++ 
Express you must build the projects manually in the following
order:

1. Build gen-gmp first.
2. the gen-* projects.
3. Then the remainder as needed. 

The 64-bit build projects won't be available with VC++ Express.

Visual Studiio 2008 can be started for building the 32 or 64 bit 
versions of GMP by clicking on the gmp.sln file in the build.vc9 
directory. If you wish to use the Intel compiler, you need to 
convert the build files by right clicking on the GMP top level 
solution and then selecting the conversion option.  Before building 
for the first time with the Intel compiler after such a conversion, 
it is advisable to clean all the build projects and to delete any 
files in the build.vc9\Win32 and build.vc9\x64 sub-directories.

Before GMP is built by using the appropriate build projects. 
Select the desired DLL or static library and then set the desired 
configuration:

    win32 or x64
    release or debug
    
To build the GMP dynamic link libraries (DLLs) choose one (or more)
of:

    dll_gmp_amd64  - GMP DLL using AMD64 assembler (x64)
    dll_gmp_gc     - GMP DLL using generic C (win32 & x64)
    dll_gmp_p0     - GMP DLL using Pentium assembler (win32)
    dll_gmp_p3     - GMP DLL using Pentium III assembler (win32)
    dll_gmp_p4     - GMP DLL using Pentium IV assembler (win32)

To build GMP static libraries choose one (or more) of:

    lib_gmp_amd64  - GMP library using AMD64 assembler (x64)
    lib_gmp_gc     - GMP library using generic C (win32 & x64)
    lib_gmp_p0     - GMP library using Pentium assembler (win32)
    lib_gmp_p3     - GMP library using Pentium III assembler (win32
    lib_gmp_p4     - GMP library using Pentium IV assembler (win32)
    
The two 64-bit libraries that use assembler default to use code that 
is optimised for AMD processors but there are alternative routines 
optimised for Intel Core 2 processors.  If you wish to use the latter 
you need to open the dll_gmp_amd64 and the lib_gmp_amd64 projects in 
the IDE and exclude these four files from the project:

    aors_mul_1.asm
    mul_1.asm
    mul_basecase.asm
    sqr.basecase.asm
 
You then need to include the core 2 versions:

    aors_mul_1.core2.asm
    mul_1.core2.asm
    mul_basecase.core2.asm
    sqr.basecase.core2.asm
    
and copy gmp-mparam.core2.h into gmp-mparam.h (which is by default
a copy of gmp-mparam.amd64.h).
 
Before any of these libraries is built the appropriate GMP 
configuration file is automatically copied into config.h.  After a 
static library is built its config.h file is copied into the output
directory; the library and its assocaited files are then copied to 
the 'lib' sub-directory within the VC++ solution folder (build.vc9).
Simlarly when a DLL is built, the resulting DLL, its export libraries
and its debug symbol file are copied to the files gmp.dll, gmp.exp, 
gmp.lib and gmp.pdb within the 'dll' sub-directory.
 
This means that the 'dll' and 'lib' sub-directories respectively 
contain the last GMP DLLs and static libraries built.  These are
then the libraries used in build any other libraries that are built
later.

The GMP DLL projects include the C++ files. If you do not want 
these the relevent files needed to be excluded from the DLL(s) you 
build. Go to the 'cpp' subdirectory of their build project in the 
IDE and exclude all the files in this subdirectory from the build 
process.

All the DLLs and static libraries are multi-threaded and are 
linked to the multi-threaded Microsoft run-time libraries (DLLs are 
linked to DLL run time libraries and static libraries are linked to 
run time static libraries).

Within the 'dll' and 'lib' sub-directories used for output the 
structure is:

   DLL or LIB 
      Win32
         Release
         Debug
      x64
         Release
         Debug   

in order to enable the appropriate library for the desired target 
platform to be easily located.  The individual project sub-
directories also contain the libraries once they have been built 
(the 'dll' and 'lib' directories are just used to hold the latest 
built versions for linking the tests that are described later). 

C++ Interface
=============

After a GMP library has been built, other libraries can be built.
These always use the last GMP library (of the same type, static or DLL) 
that has been built.  To build the GMP C+ library wrapper use:

    lib_gmpxx  - GMP C++ wrapper static library (win32 & x64)

The Tests
=========

There is a separate solution for the GMP tests: gmp-tests.sln. In 
Visual Studio 2008 these are in the gmp-tests project folder.  These
tests should be run immediately after the DLL or static library 
under test has been built.  Before running the tests it is necessary
to build the add-test-lib project in the 'gmp tests' solution folder 
(note that the Win32/x64 and Debug/Release configuration built must 
match the intended test configuration). 

The GMP tests are all configured using the property file:

	test-config.vsprops

located in the gmp-tests sub-directory. These cover the C and the 
C++ tests for win32 and 64 builds in both release and debug 
configurations.  All these property files use an IDE macro named 
$(BinDir) that determines whether the tests are applied to the the 
static LIB or the DLL versions versions of the libraries. The 
default is:

	$(BinDir) = $(SolutionDir)lib

for linking the tests to the static libraries but this can be 
changed to 

	$(BinDir) = $(SolutionDir)dll
	
to link the test to the DLL libraries.  A second macro $(LIBS)
is also needed to set the libaries to be used:

	$(BinDir)$(PlatformName)\$(ConfigurationName)\gmp.lib 

for testing the DLL and 

	$(BinDir)$(PlatformName)\$(ConfigurationName)\gmp.lib 	
	$(BinDir)$(PlatformName)\$(ConfigurationName)\gmpxx.lib

for testing the static libraries (enter these with a ' ' between 
them when setting up the macro).

Note, however, tha the DLL tests are not useful at the moment 
because they use internal features of GMP that are not exported
by the DLLs. Hence they fail to link in almost all cases.

There is also another macro, $(TestDir), that specifies where 
the executable test files are placed but changing this will 
prevent the test scripts (see later) from being used.

Test Automation
===============

After they have been built the tests cn be run using the 
Python script run-tests.py in the build.vc9\gmp-tests
directory (build.vc9x\gmp-tests for VC++ express). To 
see the test output the python script should be run in
a command window from within these sub-directories:

	cmd>gmp-tests.py 
	
and the output can be directed to a file:

	cmd>gmp-tests.py >out.txt 
	
When a GMP library is built the file 'last_build.txt' is  
written to the buid.vc9 subdirectory giving details of the 
build configuration. These details are then used to run the 
GMP tests and this means that these tests need to be run 
immediately after the library to be tested has been built.  
It is possible to test a different library by editing 
'last_build.txt' but this will only work if the files in the 
$(BinDir) are correct.  In order to avoid errors, it is 
advisable before testing to do a clean build of the library 
under test (to do a completely clean build, the files in 
the build.vc9\Win32 and build.vc9\x64 directories should be 
deleted.  

Two Tests Fail
==============

The tests for cxx/locale and misc/locale fail to link 
because the test defines a symbol - localeconv - that is 
in the Microsoft runtime libraries.  This is not significant 
for GMP numeric operations. 

Using GMP
=========

Applications that use GMP include the gmp.h header file to provide 
the prototypes for the functions that GMP provides. Hence when a
GMP distribution is being used it is important to ensure that any
GMP header file used matches that for the version of GMP in use.

1. Using the Static Libraries
=============================

To build a GMP C or C++ based application using the the static
libraries all that needs to be done is to add the GMP or GMPXX
static libraries to the application build process.  

It is, of course, important to ensure that any libraries that are
used have been built for the target platform.

2. Using the DLL Export Libraries
=================================

The DLLs built by VC++ use the _cdecl calling convention in 
which exported symbols have their C names prefixed with an 
extra '_' character.  Some applications expect the _stdcall 
convention to be used in which there is an underscore prefix 
and a suffix of '@n' where n is the number of bytes used for 
the function arguments on the stack.  Such applications will 
need to be modified to work with the GMP DLLs provided here. 
The alternative of attempting to build GMP using the _stdcall 
convention is not recommended (and won't work with the 
assembler based builds anyway). This is further complicated 
if the builds for x64 are used since the conventions here are 
different again.

There are two ways of linking to a DLL. The first way is to
use one or more of the DLL export libraries built as described
earlier (note that these are not the same as static libraries
although they are used in a similar way when an application 
is built).

3. Using the DLL Export Library
===============================

If you intend to use the DLL export libraries in an application
you need to:

   a. ensure that the application can locate the GMP DLLs in 
      question when it is run.  This involves putting the 
      DLL(s) on a recognised directory path.

   b. define __GMP_LIBGMP_DLL when the application is built
      in order to ensure that GMP's DLL export symbols are 
      properly recognised as such so that they can be 
      accessed via the GMP import library
   
   c. link the application to the gmp.lib library that is
      provided with the DLL you intend to use (this is
      produced when the DLL is built)

4. Using DLL Dynamic loading
============================

The second way of linking to a DLL is to use dynamic 
loading.  This is more complex and will not be discussed 
here. The VC++ documentation describes how to use DLLs in 
this way.

5. Using GMP functions that use FILE's as Input or Output
=========================================================

In Windows the different C runtime libraries each have 
their own stream input/output tables, which means that 
FILE* pointers cannot be passed from one to another. In 
consequence, if an application that is built with one 
library attempts to pass FILE parameters to a DLL that 
is built with another, the FILE parameters will not be 
recognised and the program will fail.

It is hence important to build a GMP application using 
the same run time library as that used to build any 
DLL that is used - in this case the appropriate version 
9 library.

If this is not possible, Jim White has made a DLL 
available that will map all stream Input/Output 
functions in a way that ensures that they use the 
correct runtime library.

6. GMP Applications that Require _stdcall Functions
===================================================

Some applications, for example Visual Basic 6, require 
that DLL based functions provide a _stdcall interface, 
whereas the VC++ default for DLLs is _cdecl.

To overcome this Jim White intends to make a wrapper 
DLL available for GMP that provides a _stdcall interface 
to the normal _cdecl GMP DLLs. 

KNOWN BUILD ISSUES FOR VC++ v9
==============================

1. A few test files require minor modifications to compile in this 
   build as follows;
   
File: tests\misc.c
24  24  #include <ctype.h>
25  25  #include <signal.h>
26  26  #include <stdio.h>
27  27  #include <stdlib.h>     /* for getenv */
28  28  #include <string.h>
29  29
------------------------------------------------------------------------
30      #if HAVE_FLOAT_H
------------------------------------------------------------------------
    30  #if HAVE_FLOAT_H || defined( _MSC_VER ) /* BRG */
------------------------------------------------------------------------
31  31  #include <float.h>      /* for DBL_MANT_DIG */
32  32  #endif
33  33
34  34  #if TIME_WITH_SYS_TIME
35  35  # include <sys/time.h>  /* for struct timeval */
36  36  # include <time.h>
------------------------------------------------------------------------
------------------------------------------------------------------------
474 474   case 1: rc = 3; break;  /* tozero  */
475 475   case 2: rc = 2; break;  /* up      */
476 476   case 3: rc = 1; break;  /* down    */
477 477   default:
478 478     return 0;
479 479   }
------------------------------------------------------------------------
    480 #if defined( _MSC_VER )
    481   {     unsigned int cw;
    482         _controlfp_s(&cw, 0, 0);
    483         _controlfp_s(&cw, (cw & ~0xC00) | (rc << 10), _MCW_RC);
    484   }
    485 #else
------------------------------------------------------------------------
480 486   x86_fldcw ((x86_fstcw () & ~0xC00) | (rc << 10));
------------------------------------------------------------------------
    487 #endif
------------------------------------------------------------------------
481 488   return 1;
482 489 #endif
483 490
484 491   return 0;
485 492 }
486 493
487 494 /* Return the hardware floating point rounding mode, or -1 if unknown. */
488 495 int
489 496 tests_hardware_getround (void)
490 497 {
491 498 #if HAVE_HOST_CPU_FAMILY_x86
------------------------------------------------------------------------
492       switch ((x86_fstcw () & ~0xC00) >> 10) {
------------------------------------------------------------------------
    499   unsigned int cw;
    500 #if defined( _MSC_VER )
    501   _controlfp_s(&cw, 0, 0);
    502 #else
    503   cw = x86_fstcw();
    504 #endif
    505
    506   switch ((cw & ~0xC00) >> 10) {
------------------------------------------------------------------------
493 507   case 0: return 0; break;  /* nearest */
494 508   case 1: return 3; break;  /* down    */
495 509   case 2: return 2; break;  /* up      */
496 510   case 3: return 1; break;  /* tozero  */
497 511   }
498 512 #endif
------------------------------------------------------------------------

File: tests\mpz\t-perfsqr.c
23 23 #include <stdlib.h>
24 24
25 25 #include "gmp.h"
26 26 #include "gmp-impl.h"
27 27 #include "tests.h"
28 28
------------------------------------------------------------------------
   29 #ifdef _MSC_VER     /* BRG */
   30 #include "perfsqr.h"
   31 #else
------------------------------------------------------------------------
29 32 #include "mpn/perfsqr.h"
------------------------------------------------------------------------
30
------------------------------------------------------------------------
   33 #endif
------------------------------------------------------------------------
31 34
32 35 /* check_modulo() exercises mpz_perfect_square_p on squares which cover each
33 36    possible quadratic residue to each divisor used within
34 37    mpn_perfect_square_p, ensuring those residues aren't incorrectly claimed
35 38    to be non-residues.
36 39
------------------------------------------------------------------------

File: tests\mpn\t-perfsqr.c
23 23 #include <stdlib.h>
24 24
25 25 #include "gmp.h"
26 26 #include "gmp-impl.h"
27 27 #include "tests.h"
28 28
------------------------------------------------------------------------
   29 #ifdef _MSC_VER     /* BRG */
   30 #include "perfsqr.h"
   31 #else
------------------------------------------------------------------------
29 32 #include "mpn/perfsqr.h"
------------------------------------------------------------------------
30
------------------------------------------------------------------------
   33 #endif
------------------------------------------------------------------------
31 34
32 35 #define PERFSQR_MOD_MASK   ((CNST_LIMB(1) << PERFSQR_MOD_BITS) - 1)
33 36
34 37 void
35 38 check_mod_2 (mp_limb_t d, mp_limb_t inv, mp_limb_t got_hi, mp_limb_t got_lo)
36 39 {
------------------------------------------------------------------------

ACKNOWLEDGEMENTS
================

My thanks to:

1. The GMP team for their work on GMP and the MPFR team for their work on MPFR
2. Sam Krasnik and Mike Loehr for suggestions on how to improve and correct errors in earlier releases.
3. Patrick Pelissier, Vincent Lefèvre and Paul Zimmermann for helping to resolve VC++ issues in MPFR.
4. Michael Abshoff and Jim White for agreeing to host the pre-built binaries for GMP on Windows.

       Brian Gladman, 2nd May 2008
