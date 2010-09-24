
Building MPIR with Microsoft Visual Studio 2010
===============================================

A Note On Licensing
===================

Files in this distribution that have been created by me for use in 
building MPIR with Microsoft Visual Studio 2010 are provided under 
the LGPL v2.1+ license terms.

The MPIR library uses numerous files which are LGPL v3+ and so the 
overall license of the library distribution is LGPL v3+.  Some of 
the demos are GPL.

Using the Assembler Based Build Projects
========================================

If you wish to use the assembler files you will need VSYASM, a 
version of YASM x86/x64 assembler tailored specifically for use with
Microsoft Visual Studio 2010.  You will need a 
recent revision of YASM from:

  http://www.tortall.net/projects/yasm/

This assembler should be placed in the bin directory used by VC++, 
which, for Visual Stduio 2010, is typically:

 C:\Program Files (x86)\Microsoft Visual Studio 10.0\VC\bin
 
You will need to install Python if you wish to use the scripts that 
automate the MPIR and MPFR tests. Otherwise these have to be compiled
and run manually.

Compiling MPIR with the Visual Studio C/C++
===========================================

These VC++ build projects are primarily designed to work with Microsoft
Visual Studio 2010 Professional. The win32 build projects also work with
Microsoft Visual C++ 2010 Express. 

To build the x64 libraries with VC++ Express you will need to install 
the Windows 7.1 SDK and Python (2.6 or later). To do this, run the Python
program 'add.express.py' before starting the build process as described 
below. This converts the build projects for use with Express. If 
necessary, these changes can be removed by running the Python program 
'remove.express.py'.

Building MPIR
=============

The MPIR build is started by opening the Visual Studio C/C++ solution 
file 'mpir.sln' in the build.vc10 directory.

MPIR is built by using the appropriate build projects and, where
appropriate, setting the build configuration and platform:

    win32 or x64
    release or debug

All projects have release and debug configurations but not all projects
provide for 32 and 64 bit Windows platforms. 

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

Before any of these libraries is built the appropriate MPIR configuration
file is generated and copied into config.h.  After a static library is 
built its config.h file is copied into the output directory; the library 
and its associated files are then copied to the 'lib' sub-directory 
within the VC++ solution folder (build.vc10). Simlarly when a DLL is 
built, the resulting DLL, its export libraries and its debug symbol file
are copied to the files mpir.dll, mpir.exp, mpir.lib and mpir.pdb within 
the 'dll' sub-directory.

This means that the 'dll' and 'lib' sub-directories respectively contain
the last MPIR DLLs and static libraries built.  These are then the 
libraries used to build software that requires MPIR or GMP.  If you use
the mpir-tests, the speed, the tune or the try programs it is important
to do so immediately after the MPIR library in question is built because
these projects link to the last library built.   

The MPIR DLL projects include the C++ files. If you want the relevent
files excluded from the DLL(s) you build, go to the 'cpp' subdirectory
of their build project in the IDE and exclude all the files in this
subdirectory from the build process.

All the DLLs and static libraries are multi-threaded and are linked to 
the multi-threaded Microsoft run-time libraries (DLLs are  linked to DLL 
run time libraries and static libraries are linked to run time static 
libraries).

Within the 'dll' and 'lib' sub-directories used for output the structure 
is:

   DLL or LIB 
      Win32
         Release
         Debug
      x64
         Release
         Debug   

in order to enable the appropriate library for the desired target 
platform to be easily located.  The individual project sub-
directories also contain the libraries once they have been built (the 
'dll' and 'lib' directories are just used to hold the latest built 
versions for linking the tests that are described later). 

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

There is a separate solution for the MPIR tests: mpir-tests.sln. In
Visual Studio 2010 this is in build.vc10 folder.  

The tests are configured to always test the last version of MPIR that is 
built.  But this is controlled by the file lastbuild.txt in the mpir-tests
directory whose contents are typically:

  dll Win32 Release "..\dll\Win32\Release" 

giving the 'library type', the 'platform', the 'configuration' and the 
'MPIR binary directory' relative to the mpir-tests' directory.

This file can be edited to test a different version of MPIR but it is also
necessary to copy either:

   mpir\build.vc10\mpir-tests\lib-test-config.props

or:

   mpir\build.vc10\mpir-tests\dll-test-config.props#

into:

   mpir\build.vc10\mpir-tests\test-config.props

depending on whether a static or DLL build of MPIR is to be tested 
respectively.

The tests also use the C++ library functions so for testing MPIR static
libraries both the desired version of MPIR and the C++ library must be 
built before the tests are built and run.  This is not necessary for
MPIR DLLs as they contain the C++ routines.

Test Automation
===============

After they have been built the tests cn be run using the Python script 
run-tests.py in the build.vc10\mpir-tests directory. To see the test 
output the python script should be run in a command window from within
these sub-directories:

	cmd>run-tests.py 
	
and the output can be directed to a file:

	cmd>run-tests.py >out.txt 
	
When an MPIR library is built the file 'last_build.txt' is  written to
the buid.vc10 subdirectory giving details of the build configuration. 
These details are then used to run the MPIR tests and this means that 
these tests need to be run immediately after the library to be tested 
has been built.  It is possible to test a different library by editing 
'lastbuild.txt' but this will only work if the files in the MPIR output
directory are correct.  In order to avoid errors, it is advisable before
testing to do a clean build of the library under test (to do a completely
clean build, the files in the build.vc10\Win32 and build.vc10\x64 
directories should be deleted.  

Two Tests Fail
==============

The tests for cxx/locale and misc/locale fail to link because the test 
defines a symbol - localeconv - that is in the Microsoft runtime libraries.  
This is not significant for MPIR numeric operations.  Some tests are skipped
for the DLL verssion as they are not relevant in this case.

Using MPIR
==========

Applications that use MPIR include the mpir.h header file to provide the 
prototypes for the functions that MPIR provides. Hence when avMPIR 
distribution is being used it is important to ensure that anyvMPIR header 
file used matches that for the version of MPIR in use.

1. Using the Static Libraries
=============================

To build a MPIR C or C++ based application using the the static libraries
all that needs to be done is to add the MPIR and/or the MPIR C++ static 
libraries to the application build process.  

It is, of course, important to ensure that any libraries that are used 
have been built for the target platform.

2. Using the DLL Export Libraries
=================================

The DLLs built by VC++ use the _cdecl calling convention in which exported
symbols have their C names prefixed with an extra '_' character.  Some 
applications expect the _stdcall convention to be used in which there is
an underscore prefix and a suffix of '@n' where n is the number of bytes 
used for the function arguments on the stack.  Such applications will need
to be modified to work with the MPIR DLLs provided here. The alternative 
of attempting to build MPIR using the _stdcall convention is not 
recommended (and won't work with the assembler based builds anyway). This
is further complicated if the builds for x64 are used since the conventions
here are different again.

There are two ways of linking to a DLL. The first way is to use one or more 
of the DLL export libraries built as described earlier (note that these are
not the same as static libraries although they are used in a similar way when
an application is built).

3. Using the DLL Export Library
===============================

If you intend to use the DLL export libraries in an application you need to:

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

The second way of linking to a DLL is to use dynamic loading.  This is more
complex and will not be discussed here. The VC++ documentation describes how 
to use DLLs in this way.

5. Using MPIR functions that use FILE's as Input or Output
==========================================================

In Windows the different C runtime libraries each have their own stream 
input/output tables, which means that FILE* pointers cannot be passed from 
one to another. In consequence, if an application that is built with one 
library attempts to pass FILE parameters to a DLL that is built with 
another, the FILE parameters will not be recognised and the program will 
fail.

It is hence important to build a MPIR application using the same run time 
library as that used to build any DLL that is used - in this case the 
appropriate version 10 library.

6. MPIR Applications that Require _stdcall Functions
====================================================

Some applications, for example Visual Basic 6, require that DLL based 
functions provide a _stdcall interface, whereas the VC++ default for DLLs 
is _cdecl.

To overcome this Jim White intends to make a wrapper DLL available for MPIR 
that provides a _stdcall interface to the normal _cdecl MPIR DLLs. 

7. The MPIR Build Process in Outline
====================================

Prebuild
--------

It is not necessary to read this unless you wnat to change the buiild
process.   The first step in an MPIR build is managed by the batch file
prebuilld.bat which has the following steps:

1. Read the connfiguation from the IDE input parameters which are the
   version (generic, core2, k8, k10, nehalem, p0, p3 or p4). For the 
   generic version there is a second parameter for a win32 build.

2. Set the source directory for the mpn source code and the platform
   (win32 or x64).

3. Call the batch file gen_mpir_h.bat (described later) to generate 
   mpir.h in the mpir root directory. 

4. Call the batch file gen_config_h.bat (described later) to generate 
   config.h in the mpir root directory.

5. use the batch file out_copy_rename to copy the appropriate version
   of gmp-mparam.h into the mpir root directory.

The gen_mpir_h batch file inputs gmp_h.in and searches for @symbol@,
replacing those that matter with the appropiate values for the Windows
build.

The gen_config_h batch file takes lists of symbols in the cfg.h files
in the mpn sub-directories and generates HAVE_NATIVE defines from them. 
The result is then prepended onto cfg.h in the build.vc10 directory and
the result is output as config.h into the mpir root directory.

The IDE build
-------------

At this point the IDE builds the MPIR library.

Postbuild
---------

After a successful MPIR build a postbuild step is managed by the batch
file postbuild.bat which has the following steps:

1. Tne $(TargetPath) parameter (%1 for the batch file) is parsed to
   determine the library type (lib or dll), the platform (win32 or 
   x64), the configuration (release or debug) and the filename.

2. The final output directory is then creaated (mpir\build.vc10\lib
   or mpir\build.vc10\dll) relative to the Visual Stduio solution
   directory (build,vc10).

3. A text file 'lastbuild.txt' is then output in the mpir-tests sub-
   directory describing the build configuration.

4. The header files used in the build are then copied into the output
   directory.
   
5. The built library files (mpir.dll, mpir.exp, mpir.lib and mpir.pdb
   for a DLL, mpir.lib and mpir.pdb for a static library) are then
   copied into the output directory.

6.  In the sub-directory mpir-tests, either dll-test-config.props (for 
    a DLL) or lib-test-config.props (for a static library) is copied
	into test-config.props to set up the tests for the version of MPIR
	being built.

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

       Brian Gladman, August 2010
