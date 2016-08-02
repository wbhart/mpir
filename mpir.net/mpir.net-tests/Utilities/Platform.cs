/*
Copyright 2014 Alex Dyachenko

This file is part of the MPIR Library.

The MPIR Library is free software; you can redistribute it and/or modify
it under the terms of the GNU Lesser General Public License as published
by the Free Software Foundation; either version 3 of the License, or (at
your option) any later version.

The MPIR Library is distributed in the hope that it will be useful, but
WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public
License for more details.

You should have received a copy of the GNU Lesser General Public License
along with the MPIR Library.  If not, see http://www.gnu.org/licenses/.  
*/

using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Reflection;

namespace MPIR.Tests
{
    /// <summary>
    /// This class provides a syntactic shortcut for selecting one of two (or several) values based on the selected solution platform (CPU architecture)
    /// </summary>
    internal static class Platform
    {
#if WIN64
        public static ulong Ui(ulong win64, uint win32) { return win64; }
        public static long Si(long win64, int win32) { return win64; }
        public static string Select(string win64, string win32) { return win64; }
        public static double Select(double win64, double win32) { return win64; }
#else
        public static uint Ui(ulong win64, uint win32) { return win32; }
        public static int Si(long win64, int win32) { return win32; }
        public static string Select(string win64, string win32) { return win32; }
        public static double Select(double win64, double win32) { return win32; }
#endif
    }
}
