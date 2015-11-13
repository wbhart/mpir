
Building MPIR with Microsoft Visual Studio 2015
===============================================

1. A Note On Licensing
======================

Files in this distribution that have been created for use in building
MPIR with Microsoft Visual Studio 2015 are provided under the terms 
of the LGPL v2.1+ license.

The MPIR library uses numerous files which are LGPL v3+ and so the 
overall license of the library distribution is LGPL v3+.  

2. Using the Assembler Based Build Projects
===========================================

If you wish to use the assembler files you will need VSYASM, a version
of YASM x86/x64 assembler tailored specifically for use with Microsoft
Visual Studio 2015.  You will need a recent revision of YASM from:

  http://www.tortall.net/projects/yasm/

This assembler (you need vsyasm.exe, NOT yasm.exe) should be placed
in the bin directory used by VC++, which, for Visual Stduio 2015, is
typically:

 C:\Program Files (x86)\Microsoft Visual Studio 14.0\VC\bin
 
You will need to install Python (version 3) if you wish to use the
scripts that automate the generation of MPIR build files for Visual 
Studio. Python is also needed for running the MPIR tests (although 
they can be run manually).

3. Compiling MPIR with the Visual Studio C/C++
==============================================

These VC++ build projects are primarily designed to work with 
Microsoft Visual Studio 2015 Professional. 

A number of different versions of MPIR are available. There are
generic C versions for both win32 and x64 and a number of versions
that are optimised for different processors using assembler code. 

The MPIR distribution has the following preconfigured builds for
both static and dynamic libraries:

   generic C build          (win32 and x64)
   Intel Pentium 3 (p3)             (win32)
   AMD k8                             (x64)
   Intel Core 2                       (x64)
   Intel Nehalem                      (x64)
   Intel Sandybridge                  (x64)

However, additional optimised builds are available and can be added
by running the Python script mpir_config.py (in mpir\build.vc14) and
selecting the builds required from the resulting list:

 1. gc
 2. p3                       (win32)
 3. p3_p3mmx                 (win32)
 4. p4                       (win32)
 5. p4_mmx                   (win32)
 6. p4_sse2                  (win32)
 7. p6                       (win32)
 8. p6_mmx                   (win32)
 9. p6_p3mmx                 (win32)
10. pentium4                 (win32)
11. pentium4_mmx             (win32)
12. pentium4_sse2            (win32)
13. atom                       (x64)
14. bobcat                     (x64)
15. bulldozer                  (x64)
16. bulldozer_piledriver       (x64)
17. core2                      (x64)
18. core2_penryn               (x64)
19. haswell                    (x64)
20. k8                         (x64)
21. k8_k10                     (x64)
22. k8_k10_k102                (x64)
23. nehalem                    (x64)
24. nehalem_westmere           (x64)
25. netburst                   (x64)
26. sandybridge                (x64)
27. sandybridge_ivybridge      (x64)
Space separated list of builds (1..27, 0 to exit)?

Dynamic Link Library (DLL) builds contain both the C and C++ features
of MPIR but static linrary builds contain only the C features with 
the C++ features being provided by a separate library (mpirxx.lib).

4. The Command Line Build
=========================

Once the required versions of MPIR have been added to the Visual
Studio build as descibed above, MPIR can be built by opening a DOS
command window in the directory mpir\build.vc14 and entering the
command

       msbuild processor library_type platform configuration +tests
where:
      processor      is the name of the version wanted - this is 
                     one of the 27 names listed in section 3
                     (the selected version must have been added
                     to the Visual Studio solution as described
                     in section 4)
      library_type   LIB or DLL 
      platform       win32 or x64
      configuration  Release or Debug
      +tests         if the command line ends with "+tests" (without
                     the quotes) build the tests after building MPIR

For example, in order to build static library version of MPIR for the
sandybridge_ivybridge processor (no 27 in the list above) and the tests, 
the command line is:

  > msbuild sandybridge_ivybridge lib x64 release +tests

The inputs to msbuild are not case sensitive but it is important to
use the correct platform (win32 or x64) for the chosen build.

The resulting library, the related include files and the debug symbol
files are placed in a directory that is determined by the name, library
type, platform and confguration:

  mpir\build.vc14\<lib|dll>_mpir_<processor>\<win32|x64>\<release|debug>

Here (and in what follows) the template <processor> indicates that a 
specific processor build name has to be substituted at this point, while
<a|b> indicates that either a or b  has to be chosen.  For example, the
location of the sandybridge_ivybridge release build static library is:  

  mpir\build.vc14\lib_mpir_sandybridge_ivybridge\x64\Release

In similar fashion, the C++ library output is placed in the directory:

  mpir\build.vc14\lib_mpir_cxx\<win32|x64>\<release|debug>

For convenience the last static and DLL libraries built are copied 
to the respective directories:

  mpir\lib\<win32|x64>\<release|debug>
  mpir\dll\<win32|x64>\<release|debug>

All the DLLs and static libraries are multi-threaded and are linked to the 
multi-threaded Microsoft run-time libraries (the MPIR libraries are linked 
to Microsoft libraries of the same type).

If built, the tests are placed in one of the directories:

    mpir\build.vc14\<win32|x64>\<release|debug>

They can be run by executing the Python program run-tests.py in the appropriate
Visual Studio build sub-directory:

    mpir/build.vc14/mpir-tests/run-tests.py

5. The Build Using Visual Studio
================================
 
The MPIR build in Visual Studio is started by opening the Visual
Studio C/C++ solution file 'mpir.sln' in the build.vc14 directory.  

MPIR is built by selecting one of the build projects provided (which
determines the library type - static or dynamic - and the processor
architecture) and then setting the platform (win32 or x64) and the
configuration (release or debug). Once selected the Viusal Studio
build command can be issued to complete the build.

The MPIR static library builds only contain the MPIR functions of the
C API so the lib_mpir_cxx project has to be built to provide the 
additional functions for the C++ API (DLL builds contain both the C
and C++ functions).

The locations of the resulting library outputs are the same as those
described above for the commandline build.

The default Visual Studio build does not contain all the possible
optimised builds.  Optimised builds that are not available can be
added using the mpir_config.py Python script as described earlier
in section 3.

6. The MPIR Libraries
=====================

The output locations of the MPIR libraries are described in section 4.
As described the static libraries and related files are also copied
to the locations:

  mpir\lib\<win32|x64>\<release|debug>

Simlarly when a DLL is built, the resulting DLL, its export libraries
and its debug symbol file are copied to:

  mpir\dll\<win32|x64>\<release|debug>

This means that the 'dll' and 'lib' sub-directories respectively contain
the last MPIR DLLs and static libraries built.  These are convenient
library locations that can be used to build MPIR applications.  But it
is important to remember that these locations always contain the last
MPIR (static or dynamic) libraaries built.

The MPIR DLL projects include the C++ files. If you want the relevant
files excluded from the DLL(s) you build, go to the 'cpp' subdirectory
of their build project in the IDE and exclude all the files in this
subdirectory from the build process.

All the DLLs and static libraries are multi-threaded and are linked to 
the multi-threaded Microsoft run-time libraries (DLLs are linked to DLL 
run time libraries and static libraries are linked to run time static 
libraries).

7. The Tests
============

There is a separate solution for the MPIR tests: mpir-tests.sln. In
Visual Studio 2015 this is in build.vc14 folder.  To run the tests
it is important that both mpir.lib (the C library) and mpirxx.lib
(the C++ library) are built prior to building the tests themselves.

When an MPIR library is built, the files 'output_params.bat' (in 
build.vc14) and test-config.props (in buid.vc14\mpir_tests) are created
and contain details that are used to set up the tests for the library
that has just been built.  These details are then used to run the MPIR
tests and this means that these tests need to be run immediately after
the library to be tested has been built.  

A different library can be tested by editing 'output_params.bat' and
test-config.props but this requires considerable knowledge of the 
internal mechanisms involved in the build process and is not hence
recomended.

The tests also use the C++ library functions so for testing MPIR static
libraries both the desired version of MPIR and the C++ library must be 
built before the tests are built and run.  This is not necessary for
MPIR DLLs as they contain the C++ routines.

On multi-processor systems, Visual Studio 13 will typically run several 
builds in parallel so it is advisable to build add-test-lib first before
building the tests.  

After they have been built the tests can be run using the Python script 
run-tests.py in the build.vc14\mpir-tests directory. To see the test 
output the python script should be run in a command window from within
these sub-directories:

    cmd>run-tests.py 
    
and the output can be directed to a file:

    cmd>run-tests.py >out.txt 

8. Speed and Tuning
===================

The speed and tuning programs are built using the tune.sln solution
file and are only available on Windows x64. These applications, which
are set up to use the static library versions of MPIR, are not needed
to use MPIR.

9. MPIR on Windows x64
======================

Although Windows x64 is a 64-bit operating system, Microsoft has decided
to make long integers 32-bits, which is inconsistent when compared with
almost all other 64-bit operating systems.   This has caused many subtle
bugs when open source code is ported to Windows x64 because many
developers reasonably expect to find that long integers on a 64-bit
operating system will be 64 bits long.  

MPIR contains functions with suffixes of _ui and _si that are used to
input unsigned and signed integers and convert them for use with MPIR's
multiple precision integers (mpz types).   For example, the functions:

   void mpz_set_ui(mpz_t, unsigned long int)
   void mpz_set_si(mpz_t, signed long int)

set an mpz integer from unsigned and signed long integers respectively,
and the functions:

   unsigned long int mpz_get_ui(mpz_t)
   signed long int mpz_get_ui(mpz_t)

obtain unsigned and signed long int values from an MPIR multiple
precision integer (mpz).

To bring MPIR on Windows x64 into line with other 64-bit operating
systems two new types have been introduced throughout MPIR:

    mpir_ui    defined as unsigned long int on all but Windows x64
	           defined as unsigned long long int on Windows x64

    mpir_si    defined as signed long int on all but Windows x64
	           defined as signed long long int on Windows x64

The above prototypes in MPIR 2.6.0+ are changed to:
 
   void mpz_set_ui(mpz_t, mpir_ui)
   void mpz_set_si(mpz_t, mpir_ui)

   mpir_ui mpz_get_ui(mpz_t)
   mpir_si mpz_get_si(mpz_t)

and these changes are applied to all MPIR functions with _ui and _si
suffixes.    

10. Using MPIR
==============

Applications that use MPIR include the mpir.h header file to provide
the prototypes for the functions that MPIR provides. Hence when an MPIR 
distribution is being used it is important to ensure that the MPIR
header file used matches that for the version of MPIR in use.  If MPIR
is used to build 64 bit applications, it is necessary to ensure that the
define _WIN64 is set when the application is built. 

10.1. Using the Static Libraries
--------------------------------

To build a MPIR C or C++ based application using the the static 
libraries all that needs to be done is to add the MPIR and/or the MPIR
C++ static libraries to the application build process.  

It is, of course, important to ensure that any libraries that are used 
have been built for the target platform.

10.2. Using the DLL Export Libraries
------------------------------------

The DLLs built by VC++ use the _cdecl calling convention in which
exported symbols have their C names prefixed with an extra '_' 
character.  Some applications expect the _stdcall convention to be
used in which there is an underscore prefix and a suffix of '@n' where
n is the number of bytes used for the function arguments on the stack.  
Such applications will need to be modified to work with the MPIR DLLs
provided here. The alternative of attempting to build MPIR using the
_stdcall convention is not recommended (and won't work with the 
assembler based builds anyway). This is further complicated if the 
builds for x64 are used since the conventions here are different again.

There are two ways of linking to a DLL. The first way is to use one or
more of the DLL export libraries built as described earlier (note that
these are not the same as static libraries although they are used in a
similar way when an application is built).

10.3. Using the DLL Export Library
----------------------------------

If you intend to use the DLL export libraries in an application you
need to:

   a. define the preprocessor symbol MSC_USE_DLL when the
      application is built so that the use of a DLL version
      of the MPIR library is recognised;
 
   b. link the application to the mpir.lib library that is in
      the same directory as the MPIR DLL library that is to be
      used (this is produced when the DLL is built);

   c. ensure that the application can locate the MPIR DLL in 
      question when it is run (for example by copying it into 
      the directory where the application exe file is located).

10.4. Using DLL Dynamic loading
-------------------------------

The second way of linking to a DLL is to use dynamic loading.  This is
more complex and will not be discussed here. The VC++ documentation
describes how  to use DLLs in this way.

10.5. Using MPIR functions that use FILE's as Input or Output
-------------------------------------------------------------

In Windows the different C runtime libraries each have their own
stream input/output tables, which means that FILE* pointers cannot 
be passed from one to another. In consequence, if an application that
is built with one library attempts to pass FILE parameters to a DLL 
that is built with another library, the FILE parameters will not be
recognised and the program will fail.

It is hence important to build a MPIR application using the same run
time library as that used to build any DLL that is used - in this case
the appropriate version 14 library.

10.6. MPIR Applications that Require _stdcall Functions
-------------------------------------------------------

Some applications, for example Visual Basic 6, require that DLL based 
functions provide a _stdcall interface, whereas the VC++ default for
DLLs is _cdecl.

To overcome this Jim White intends to make a wrapper DLL available for
MPIR that provides a _stdcall interface to the normal _cdecl MPIR DLLs. 

10.7. The MPIR Build Process in Outline
---------------------------------------

It is not necessary to read this unless you want to change the build
process. 

Prebuild
--------

The first step in an MPIR build is managed by the batch file
prebuilld.bat which has the following steps:

1. Read the configuration from the IDE input parameters which are the
   version (e.g. generic, core2, k8, k10, nehalem, p0, p3 or p4). For
   the generic version there is a second parameter for a win32 build.

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
within the build.vc14/cdata sub-directory and generates HAVE_NATIVE 
defines from them.   The result is then prepended onto cfg.h in the 
build.vc14 directory and the result is output as config.h into the 
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

11. ACKNOWLEDGEMENTS
====================

My thanks to:

1. The GMP team for their work on GMP and the MPFR team 
   for their work on MPFR
2. The MPIR team
3. Patrick Pelissier, Vincent Lefèvre and Paul Zimmermann
   for helping to resolve VC++ issues in MPFR.
4. Jeff Gilchrist for his help in testing, debugging and 
   improving the readme giving the VC++ build instructions

       Brian Gladman, 15 June 2015.
