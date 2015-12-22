
Building MPIR with Microsoft Visual Studio 2013 and Express 2013
================================================================

A Note On Licensing
===================

Files in this distribution that have been created for use in building 
MPIR with Microsoft Visual Studio 2013 are provided under the terms of
the LGPL v2.1+ license.

The MPIR library uses numerous files which are LGPL v3+ and so the 
overall license of the library distribution is LGPL v3+.  Some of 
the demos are GPL.

Using the Assembler Based Build Projects
========================================

If you wish to use the assembler files you will need VSYASM, a version 
of YASM x86/x64 assembler tailored specifically for use with Microsoft 
Visual Studio 2013.  You will need a recent revision of YASM from:

  http://www.tortall.net/projects/yasm/

This assembler (you need vsyasm.exe, NOT yasm.exe) should be placed
in the bin directory used by VC++, which, for Visual Stduio 2013, is
typically:

 C:\Program Files (x86)\Microsoft Visual Studio 12.0\VC\bin
 
You will need to install Python if you wish to use the scripts that 
automate the generation of MPIR build files for Visual Studio. Python
is also needed for running the MPIR tests (although they can be run
manually).

Compiling MPIR with the Visual Studio C/C++
===========================================

These VC++ build projects are primarily designed to work with Microsoft
Visual Studio 2013 Professional. The build projects also work with
Microsoft Visual C++ 2013 Express. 

Building MPIR
=============

1. Generic C Builds
-------------------

The basic build solution for Visual Studio contains build projects for
the generic C version of MPIR.  The MPIR build is started by opening the
Visual Studio C/C++ solution file 'mpir.sln' in the build.vc12 directory.  

It will be assumed here that the MPIR root directory is named 'mpir' so 
that the build directory is mpir\build.vc12.  The output directories
for builds are:

    mpir\lib   for static libraries
    mpir\dll   for dynamic link libraries (DLL)

MPIR is built by selecting one of the individual build projects and then
setting the library type (static or DLL), the Windows target architecture
(win32 or x64) and the build configuration (release or debug).

All projects have release and debug configurations but not all projects
support win32 and x64.

By default the Visual Studio solution provides support for these generic
C builds: 

    lib_mpir_gc     - MPIR library using generic C (win32 & x64)
    lib_mpir_cxx    - MPIR C++ library (win32 & x64)
    dll_mpir_gc     - MPIR DLL using generic C (win32 & x64)

There are two static libraries, one providing the C library functions and
the other providing the C++ functions.  The DLL library supports both the
C and the C++ functions.

2. Builds with Assembler Support
--------------------------------

By default the Visual Studio solution for MPIR provides support for
x64 builds with assembler support for AMD k8, Intel core2, nehalem
and sandybridge (x64) and pentium 3 (win32).

To build MPIR versions with assembler support for other processors,
the Python program mpir_config.py has to be run before the Visual
Studio solution is opened.  This outputs a list of the assembler
builds that are available. After a particular build is selected the
program outputs a Visual Studio project for this build and adds it
to the Visual Studio solution.  When the Visual Studio solution file
is then opened it will include this new build projects for both
static library and DLL builds with the specified assembler support.

3. The build Process
--------------------

Before any of these libraries is built the appropriate MPIR configuration
file is generated and copied into config.h.  After a static library is 
built its config.h file is copied into the output directory; the library 
and its associated files are then copied to the appropriate sub-directory
in the 'mpir\lib' sub-directory:

   mpir\lib\win32\debug
   mpir\lib\win32\release
   mpir\lib\x64\debug
   mpir\lib\x64\release

Simlarly when a DLL is built, the resulting DLL, its export libraries and
its debug symbol file are copied into the appropriate subdirectory in the
mpir\dll subdirectory:

   mpir\dll\win32\debug
   mpir\dll\win32\release
   mpir\dll\x64\debug
   mpir\dll\x64\release

This means that the 'dll' and 'lib' sub-directories respectively contain
the last MPIR DLLs and static libraries built.  These are then the 
libraries used to build software that requires MPIR or GMP.  If you use
the mpir-tests, the speed, the tune or the try programs it is important
to do so immediately after the MPIR library in question is built because
these projects link to the last library built.   

The MPIR DLL projects include the C++ files. If you want the relevant
files excluded from the DLL(s) you build, go to the 'cpp' subdirectory
of their build project in the IDE and exclude all the files in this
subdirectory from the build process.

All the DLLs and static libraries are multi-threaded and are linked to 
the multi-threaded Microsoft run-time libraries (DLLs are linked to DLL 
run time libraries and static libraries are linked to run time static 
libraries).

Within the 'dll' and 'lib' sub-directories used for output, the layout
is:

   DLL or LIB 
      Win32
         Release
         Debug
      x64
         Release
         Debug   

so that the appropriate library for the desired target platform can be
easily located.  The individual project sub-directories also contain the 
libraries once they have been built (as indicated earlier, the 'dll' 
and 'lib' directories are used to hold the latest built versions for 
linking the tests).

4. C++ Interface
----------------

After a MPIR library has been built, other libraries can be built.
These always use the last MPIR library (of the same type, static or 
DLL) that has been built. To build the MPIR C+ library wrapper use:

    lib_mpir_cxx  - MPIR C++ wrapper static library (win32 & x64)

The DLL projects include the C++ functions so an additional library
is not needed when they are used.

The Tests
=========

There is a separate solution for the MPIR tests: mpir-tests.sln. In
Visual Studio 2013 this is in build.vc12 folder.  

When an MPIR library is built the file 'output_params.bat' is  written to
the buid.vc12 subdirectory giving details of the build configuration. 
These details are then used to run the MPIR tests and this means that 
these tests need to be run immediately after the library to be tested 
has been built.  It is possible to test a different library by editing 
'output_params.bat' but this will only work if the files in the MPIR output
directory are correct.  In order to avoid errors, it is advisable before
testing to do a clean build of the library under test (to do a completely
clean build, the files in the build.vc12\Win32 and build.vc12\x64 
directories should be deleted.  

The version to be tested can be changed by editing the output_params.bat
file in the  mpir root directory, whose content controls the tests by 
setting the 'library type', the 'platform', and the 'configuration'. 
Its content is typically:

  (set libr=lib)  
  (set plat=x64) 
  (set conf=Release) 

If this file can be edited to test a different version of MPIR, the 
choices being lib/dll , win32/x64 and Debug/Release.  If this file
is changed, it is also necessary to copy either:

   mpir\build.vc12\mpir-tests\lib-test-config.props

or:

   mpir\build.vc12\mpir-tests\dll-test-config.props#

into:

   mpir\build.vc12\mpir-tests\test-config.props

depending on whether a static or DLL build of MPIR is to be tested.

The tests also use the C++ library functions so for testing MPIR static
libraries both the desired version of MPIR and the C++ library must be 
built before the tests are built and run.  This is not necessary for
MPIR DLLs as they contain the C++ routines.

On multi-processor systems, Visual Studio 13 will typically run several 
builds in parallel so it is advisable to build add-test-lib first before
building the tests.  

After they have been built the tests can be run using the Python script 
run-tests.py in the build.vc12\mpir-tests directory. To see the test 
output the python script should be run in a command window from within
these sub-directories:

    cmd>run-tests.py 
    
and the output can be directed to a file:

    cmd>run-tests.py >out.txt 

Speed and Tuning
================

The speed and tuning programs are built using the tune.sln solution file
and are only available on Windows x64. These applications, which are set
up to use the static library versions of MPIR, are not needed to use MPIR.

MPIR on Windows x64
===================

Although Windows x64 is a 64-bit operating system, Microsoft has decided to
make long integers 32-bits, which is inconsistent when compared with almost
all other 64-bit operating systems.   This has caused many subtle bugs when   
open source code is ported to Windows x64 because many developers reasonably
expect to find that long integers on a 64-bit operating system will be 64 
bits long.  

MPIR contains functions with suffixes of _ui and _si that are used to input
unsigned and signed integers into and convert them for use with MPIR's 
multiple precision integers (mpz types).   For example, the functions:

   void mpz_set_ui(mpz_t, unsigned long int)
   void mpz_set_si(mpz_t, signed long int)

set an mpz integer from unsigned and signed long integers respectively, and
the functions:

   unsigned long int mpz_get_ui(mpz_t)
   signed long int mpz_get_ui(mpz_t)

obtain unsigned and signed long int values from an MPIR multiple precision
integer (mpz).

To bring MPIR on Windows x64 into line with other 64-bit operating systems
two new types have been introduced throughout MPIR:

    mpir_ui    defined as unsigned long int on all but Windows x64
	           defined as unsigned long long int on Windows x64

    mpir_si    defined as signed long int on all but Windows x64
	           defined as signed long long int on Windows x64

The above prototypes in MPIR 2.6.0 are changed to:
 
   void mpz_set_ui(mpz_t, mpir_ui)
   void mpz_set_si(mpz_t, mpir_ui)

   mpir_ui mpz_get_ui(mpz_t)
   mpir_si mpz_get_si(mpz_t)

and these changes are applied to all MPIR functions with _ui and _si
suffixes.    

Using MPIR
==========

Applications that use MPIR include the mpir.h header file to provide the 
prototypes for the functions that MPIR provides. Hence when an MPIR 
distribution is being used it is important to ensure that the MPIR header 
file used matches that for the version of MPIR in use.  If MPIR is used
to build 64 bit applications, it is necessary to ensure that the compiler
define _WIN64 is set when the application is built. 

1. Using the Static Libraries
-----------------------------

To build a MPIR C or C++ based application using the the static libraries
all that needs to be done is to add the MPIR and/or the MPIR C++ static 
libraries to the application build process.  

It is, of course, important to ensure that any libraries that are used 
have been built for the target platform.

2. Using the DLL Export Libraries
---------------------------------

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
-------------------------------

If you intend to use the DLL export libraries in an application you need to:

   a. define the preprocessor symbol MSC_USE_DLL when the
      application is built so that the use of a DLL version
      of the MPIR library is recognised;
 
   b. link the application to the mpir.lib library that is in
      the same directory as the MPIR DLL library that is to be
      used (this is produced when the DLL is built);

   c. ensure that the application can locate the MPIR DLL in 
      question when it is run (for example by copying it into 
      the directory where the application exe file is located).

4. Using DLL Dynamic loading
----------------------------

The second way of linking to a DLL is to use dynamic loading.  This is more
complex and will not be discussed here. The VC++ documentation describes how 
to use DLLs in this way.

5. Using MPIR functions that use FILE's as Input or Output
----------------------------------------------------------

In Windows the different C runtime libraries each have their own stream 
input/output tables, which means that FILE* pointers cannot be passed from 
one to another. In consequence, if an application that is built with one 
library attempts to pass FILE parameters to a DLL that is built with 
another library, the FILE parameters will not be recognised and the 
program will fail.

It is hence important to build a MPIR application using the same run time 
library as that used to build any DLL that is used - in this case the 
appropriate version 12 library.

6. MPIR Applications that Require _stdcall Functions
----------------------------------------------------

Some applications, for example Visual Basic 6, require that DLL based 
functions provide a _stdcall interface, whereas the VC++ default for DLLs 
is _cdecl.

To overcome this Jim White intends to make a wrapper DLL available for MPIR 
that provides a _stdcall interface to the normal _cdecl MPIR DLLs. 

7. The MPIR Build Process in Outline
------------------------------------

It is not necessary to read this unless you want to change the build
process. 

Prebuild
--------

The first step in an MPIR build is managed by the batch file prebuilld.bat 
which has the following steps:

1. Read the configuration from the IDE input parameters which are the
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
replacing those that matter with the appropriate values for the Windows
build.

The gen_config_h batch file takes lists of symbols in the cfg.h files
within the build.vc12/cdata sub-directory and generates HAVE_NATIVE 
defines from them.   The result is then prepended onto cfg.h in the 
build.vc12 directory and the result is output as config.h into the 
mpir root directory.

The IDE build
-------------

At this point the IDE builds the MPIR library.

Postbuild
---------

After a successful MPIR build a postbuild step is managed by the batch
file postbuild.bat which has the following steps:

1. Tne $(TargetPath) parameter (%1 for the batch file) is passed to
   determine the library type (lib or dll), the platform (win32 or 
   x64), the configuration (release or debug) and the filename.

2. The final output directory is then created in the mpir root 
   directory,  mpir\lib or mpir\dll, as appropriate.
   
3. The file 'output_params.bat' is written describing the MPIR 
   configuration that has been built.  This is used to signal
   the version to be tested by the tests. In the sub-directory
   mpir-tests, the appropriate property file is copied into
   test-config.props for later use in the tests.
   
4. The header files used in the build are then copied into the 
   output directory.
   
5. The built library files (mpir.dll, mpir.exp, mpir.lib and mpir.pdb
   for a DLL, mpir.lib and mpir.pdb for a static library) are then
   copied into the output directory.

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

       Brian Gladman, April 2014
