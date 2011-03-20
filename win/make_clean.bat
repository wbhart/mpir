@echo off
::  Copyright 2011 Jason Moxham
::
::  This file is part of the MPIR Library.
::
::  The MPIR Library is free software; you can redistribute it and/or modify
::  it under the terms of the GNU Lesser General Public License as published
::  by the Free Software Foundation; either version 2.1 of the License, or (at
::  your option) any later version.
::
::  The MPIR Library is distributed in the hope that it will be useful, but
::  WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
::  or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public
::  License for more details.
::
::  You should have received a copy of the GNU Lesser General Public License
::  along with the MPIR Library; see the file COPYING.LIB.  If not, write
::  to the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
::  Boston, MA 02110-1301, USA.
::

del *.obj ..\mpir.h ..\config.h mpir.lib *.idb *.pdb > nul 2>&1
del /S/Q mpn mpz mpq mpf scanf printf > nul 2>&1
rmdir mpn mpz mpq mpf scanf printf > nul 2>&1

