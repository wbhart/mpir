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
    internal static class Accessors
    {
        private static readonly ConstructorInfo _intPtrConstructor;
        private static readonly FieldInfo _getNumberOfLimbsAllocated;
        private static readonly FieldInfo _getNumberOfLimbsUsed;
        private static readonly FieldInfo _getLimbs;

        static Accessors()
        {
            _getNumberOfLimbsAllocated = GetAccessor("_numberOfLimbsAllocated");
            _getNumberOfLimbsUsed = GetAccessor("_numberOfLimbsUsed");
            _getLimbs = GetAccessor("_limbs");

            _intPtrConstructor = typeof(IntPtr).GetConstructor(new[] { Type.GetType("System.Void*") });
        }

        private static FieldInfo GetAccessor(string name)
        {
            return typeof(HugeInt).GetField(name, BindingFlags.NonPublic | BindingFlags.Instance);
        }

        public static int NumberOfLimbsAllocated(this HugeInt x)
        {
            return (int)_getNumberOfLimbsAllocated.GetValue(x);
        }

        public static int NumberOfLimbsUsed(this HugeInt x)
        {
            return (int)_getNumberOfLimbsUsed.GetValue(x);
        }

        public static IntPtr Limbs(this HugeInt x)
        {
            return (IntPtr)_intPtrConstructor.Invoke(new object[] { _getLimbs.GetValue(x) });
        }
    }
}
