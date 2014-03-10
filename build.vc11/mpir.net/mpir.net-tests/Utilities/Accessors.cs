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
        private static readonly FieldInfo _getValue;

        static Accessors()
        {
            _getValue = GetAccessor("_value");

            _intPtrConstructor = typeof(IntPtr).GetConstructor(new[] { Type.GetType("System.Void*") });
        }

        private static FieldInfo GetAccessor(string name)
        {
            return typeof(HugeInt).GetField(name, BindingFlags.NonPublic | BindingFlags.Instance);
        }

        public static int NumberOfLimbsAllocated(this HugeInt x)
        {
            if (x._value() == IntPtr.Zero)
                return 0;

            unsafe
            {
                return ((int*)x._value().ToPointer())[0];
            }
        }

        public static int NumberOfLimbsUsed(this HugeInt x)
        {
            if (x._value() == IntPtr.Zero)
                return 0;

            unsafe
            {
                return ((int*)x._value().ToPointer())[1];
            }
        }

        public static IntPtr Limbs(this HugeInt x)
        {
            if (x._value() == IntPtr.Zero)
                return IntPtr.Zero;

            unsafe
            {
                return new IntPtr(((void**)x._value().ToPointer())[1]);
            }
        }

        public static IntPtr _value(this HugeInt x)
        {
            return (IntPtr)_intPtrConstructor.Invoke(new object[] { _getValue.GetValue(x) });
        }
    }
}
