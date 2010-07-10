
Building MPIR with Microsoft Visual Studio 2010
===============================================

A Note On Licensing
===================

Files in this distribution that have been created for use in 
building MPIR with Microsoft Visual Studio 2010 are provided
under the LGPL v2.1+ license terms.

The MPIR library uses numerous files which are LGPL v3+ and
so the overall license of the library distribution is LGPL 
v3+.  Some of the demos are GPL.

Using the Assembler Based Build Projects
========================================

If you wish to use the assembler files you will need VSYASM, 
a version of YASM x86/x64 assembler tailored specifically for
use with Microsoft Visual Studio 2010.  You will need a 
recent revision of YASM from:

  http://www.tortall.net/projects/yasm/

This assembler should be placed in the bin directory used by 
VC++, which, for Visual Stduio 2010, is typically:

 C:\Program Files (x86)\Microsoft Visual Studio 10.0\VC\bin
 
You will need to install Python if you wish to use the scripts
that automate the MPIR and MPFR tests. Otherwise these have to 
be compiled and run manually.

Compiling MPIR with the Visual Studio C/C++
===========================================

These VC++ build projects are primarily designed to work with 
Microsoft Visual Studio 2010 Professional. The win32 build
projects also work with Microsoft Visual C++ 2010 Express. 

To build the x64 libraries with VC++ Express you will need to
install the Windows 7.1 SDK and Python (2.6 or later). To do
this, run the Python program 'add.express.py' before starting
the build process as described below. This converts the build
projects for use with Express. If necessary, these changes can
be removed by running the Python program 'remove.express.py'.

Building MPIR
=============

The MPIR build is started by opening the Visual Studio C/C++ 
solution file 'mpir.sln' in the build.vc10 directory.

MPIR is built by using the appropriate build projects and, where
appropriate, setting the build configuration and platform:

    win32 or x64
    release or debug

All projects have release and debug configurations but not all 
projects provide for 32 and 64 bit Windows platforms. 

The supported platforms and library formats are as follows:   

1. Generic Build Projects (both 32 and 64 bit)

    lib_mpir_gc     - MPIR library using generic C (win32 & x64)
    lib_mpir_cxx    - MPIR C++ library (win32 & x64)
    dll_mpir_gc     - MPIR DLL using generic C (win32 & x64)

2. 32-bit Build Projects

    lib_mpir_p0     - MPIR library using Pentium assembler (win32)
    lib_mpir_p3     - MPIR library using Pentium III assembler (win32
    lib_mpir_p4     - MPIR library using Pentium IV assembler (win32)
    dll_mpir_p0     - MPIR DLL using Pentium assembler (win32)
    dll_mpir_p3     - MPIR DLL using Pentium III assembler (win32)
    dll_mpir_p4     - MPIR DLL using Pentium IV assembler (win32)

3. 64-bit Build Projects
    
    lib_mpir_k8      - MPIR library using AMD k8 assembler (x64)
    lib_mpir_k10     - MPIR library using AMD k10 assembler (x64)
    lib_mpir_core2   - MPIR library Intel Core2 assembler (x64)
    lib_mpir_nehalem - MPIR library Intel Core2 assembler (x64)
    dll_mpir_k8      - MPIR DLL using AMD k8 assembler (x64)
    dll_mpir_k8      - MPIR DLL using AMD k10 assembler (x64)
    dll_mpir_core2   - MPIR DLL using Intel Core2 assembler (x64)
    dll_mpir_nehalem - MPIR DLL using Intel Core2 assembler (x64)

Staatic library outputs are put in one of the four directories

    mpir\lib\win32\release\
	mpir\lib\win32\debug\
    mpir\lib\x64\release\
	mpir\lib\x64\debug\

as determined by the configuration that is built.  Similarly the
DLL library outputs are put in one of:

    mpir\dll\win32\release\
	mpir\dll\win32\debug\
    mpir\dll\x64\release\
	mpir\dll\x64\debug\

The latest build overwrites any previous builds. It a build fails
it is adbisable to delete all the files in these output directories 
before a build is started.

If you use the mpir-tests, the speed, the tune or the try programs,
it is very important to do so immediately after the MPIR library
in question is built because these projects link to the last
library built.  

The MPIR DLL projects include the C++ files. If you want the relevent
files excluded from the DLL(s) you build, go to the 'cpp' subdirectory
of their build project in the IDE and exclude all the files in this
subdirectory from the build process.

All the DLLs and static libraries are multi-threaded and are 
linked to the multi-threaded Microsoft run-time libraries (DLLs are 
linked to DLL run time libraries and static libraries are linked to 
run time static libraries).

C++ Interface
=============

After a MPIR library has been built, other libraries can be built.
These always use the last MPIR library (of the same type, static or 
DLL) that has been built. To build the MPIR C+ library wrapper use:

    lib_mpir_cxx  - MPIR C++ wrapper static library (win32 & x64)

The DLL projects include the C++ functions so an additional library
is not needed when they are used.

The Tests
=========

The tests are configured by default to test the static library versions
of MPIR but the DLL versions can be tested by changing the default
property files before the tests are buillt. These property files are
in the directory 

  mpir\build.vc10\mpir-tests

and the two files:

  tests.release.props
  tests.debug.props

set the configuration for the tests of the static libraries in release
and debug configuration respectively.  These are the same as the files:

  lib_tests.release.props
  lib_tests.debug.props

The two files

  dll_tests.release.props
  dll_tests.debug.props

set the configuration for the tests of the DLL libraries in release and
debug configuration respectively. By copying these files to:

  tests.release.props
  tests.debug.props

the tests will be run for the DLL libraries.

For the static library tests both the C and C++ libraries must be built
but the MPIR DLL contains both the C and C++ functions.

There is a separate solution for the MPIR tests: mpir-tests.sln. In 
Visual Studio 2010 these are in build.vc10 folder.  These tests must
be run immediately after the DLL or the static C and C++ libraries
have been built because they test the most recently built versions.
Before running the tests it is necessary to build the add-test-lib 
project.  Note also that the Win32/x64 and Debug/Release choices
for the tests must match that of the libraries under test.

Test Automation
===============

After they have been built the tests cn be run using the Python script
run_lib_tests.py for the static libraries or run_dll_tests.py for the
DLL libraries.  To see the test output the python script should be run
in a command window from within these sub-directories:

	cmd>run_lib_tests.py 
	
and the output can be directed to a file:

	cmd>run_lib_tests.py >out.txt 
	
The tests need to be run immediately after the library to be tested 
has been built.  

Test Failures
=============

The tests for cxx/locale and misc/locale fail to link because
the test defines a symbol - localeconv - that is in the Microsoft
runtime libraries.  This is not significant for MPIR numeric 
operations. Some tests are also skipped in the DLL tests as they
are not relevant if a DLL is being used.

Other Programs
==============

The programs speed, tune and try work with the static library builds
only

Using MPIR
==========

Applications that use MPIR include the mpir.h header file to provide 
the prototypes for the functions that MPIR provides. Hence when a
MPIR distribution is being used it is important to ensure that any
MPIR header file used matches that for the version of MPIR in use.

1. Using the Static Libraries
=============================

To build a MPIR C or C++ based application using the the static
libraries all that needs to be done is to add the MPIR and/or the 
MPIR C++ static libraries to the application build process.  

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
need to be modified to work with the MPIR DLLs provided here. 
The alternative of attempting to build MPIR using the _stdcall 
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

   a. ensure that the application can locate the MPIR DLLs in 
      question when it is run.  This involves putting the 
      DLL(s) on a recognised directory path.

   b. define __GMP_LIBGMP_DLL when the application is built
      in order to ensure that MPIR's DLL export symbols are 
      properly recognised as such so that they can be 
      accessed via the MPIR import library
   
   c. link the application to the gmp.lib library that is
      provided with the DLL you intend to use (this is
      produced when the DLL is built)

4. Using DLL Dynamic loading
============================

The second way of linking to a DLL is to use dynamic 
loading.  This is more complex and will not be discussed 
here. The VC++ documentation describes how to use DLLs in 
this way.

5. Using MPIR functions that use FILE's as Input or Output
==========================================================

In Windows the different C runtime libraries each have 
their own stream input/output tables, which means that 
FILE* pointers cannot be passed from one to another. In 
consequence, if an application that is built with one 
library attempts to pass FILE parameters to a DLL that 
is built with another, the FILE parameters will not be 
recognised and the program will fail.

It is hence important to build a MPIR application using 
the same run time library as that used to build any 
DLL that is used - in this case the appropriate version 
9 library.

6. MPIR Applications that Require _stdcall Functions
====================================================

Some applications, for example Visual Basic 6, require 
that DLL based functions provide a _stdcall interface, 
whereas the VC++ default for DLLs is _cdecl.

ACKNOWLEDGEMENTS
================

My thanks to:

1. The GMP team for their work on GMP and the MPFR team 
   for their work on MPFR
2. The MPIR team
3. Patrick Pelissier, Vincent Lefèvre and Paul Zimmermann
   for helping to resolve VC++ issues in MPFR.
4. Jeff Gilchrist for his help in testing, debugging and 
   improving the readme giving the VC++ build instructions

       Brian Gladman, July 2010
