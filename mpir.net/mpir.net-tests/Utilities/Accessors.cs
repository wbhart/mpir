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
    internal static class Accessors<T>
    {
        private static readonly ConstructorInfo _intPtrConstructor;
        private static readonly FieldInfo _getValue;

        static Accessors()
        {
            _getValue = GetAccessor("_value");

            _intPtrConstructor = typeof(IntPtr).GetConstructor(new[] { Type.GetType("System.Void*") });
        }

        internal static FieldInfo GetAccessor(string name)
        {
            return typeof(T).GetField(name, BindingFlags.NonPublic | BindingFlags.Instance);
        }

        internal static IntPtr _value(T x)
        {
            return (IntPtr)_intPtrConstructor.Invoke(new object[] { _getValue.GetValue(x) });
        }
    }

    internal static class IntAccessors
    {
        internal static IntPtr _value(this HugeInt x)
        {
            return Accessors<HugeInt>._value(x);
        }

        internal static int NumberOfLimbsAllocated(this HugeInt x)
        {
            if (_value(x) == IntPtr.Zero)
                return 0;

            return x.AllocatedSize;
        }

        internal static int NumberOfLimbsUsed(this HugeInt x)
        {
            if (_value(x) == IntPtr.Zero)
                return 0;

            unsafe
            {
                return ((int*)_value(x).ToPointer())[1];
            }
        }

        internal static IntPtr Limbs(this HugeInt x)
        {
            if (_value(x) == IntPtr.Zero)
                return IntPtr.Zero;

            unsafe
            {
                return new IntPtr(*(void**)(((int*)_value(x).ToPointer()) + 2));
            }
        }
    }

    internal static class RationalAccessors
    {
        internal static IntPtr _value(this HugeRational x)
        {
            return Accessors<HugeRational>._value(x);
        }

        internal static int NumeratorNumberOfLimbsAllocated(this HugeRational x)
        {
            if (_value(x) == IntPtr.Zero)
                return 0;

            unsafe
            {
                return ((int*)_value(x).ToPointer())[0];
            }
        }

        internal static int NumeratorNumberOfLimbsUsed(this HugeRational x)
        {
            if (_value(x) == IntPtr.Zero)
                return 0;

            unsafe
            {
                return ((int*)_value(x).ToPointer())[1];
            }
        }

        internal static IntPtr NumeratorLimbs(this HugeRational x)
        {
            if (_value(x) == IntPtr.Zero)
                return IntPtr.Zero;

            unsafe
            {
                return new IntPtr(*(void**)(((int*) _value(x).ToPointer()) + 2));
            }
        }

        internal static int DenominatorNumberOfLimbsAllocated(this HugeRational x)
        {
            if (_value(x) == IntPtr.Zero)
                return 0;

            unsafe
            {
                return ((int*)_value(x).ToPointer())[2 + sizeof(IntPtr) / 4];
            }
        }

        internal static int DenominatorNumberOfLimbsUsed(this HugeRational x)
        {
            if (_value(x) == IntPtr.Zero)
                return 0;

            unsafe
            {
                return ((int*)_value(x).ToPointer())[3 + sizeof (IntPtr) / 4];
            }
        }

        internal static IntPtr DenominatorLimbs(this HugeRational x)
        {
            if (_value(x) == IntPtr.Zero)
                return IntPtr.Zero;

            unsafe
            {
                return new IntPtr(*(void**)(((int*)_value(x).ToPointer()) + 4 + sizeof(IntPtr) / 4));
            }
        }
    }

    internal static class FloatAccessors
    {
        internal static IntPtr _value(this HugeFloat x)
        {
            return Accessors<HugeFloat>._value(x);
        }

        internal static int NumberOfLimbsAllocated(this HugeFloat x)
        {
            if (_value(x) == IntPtr.Zero)
                return 0;

            unsafe
            {
                return ((int*)_value(x).ToPointer())[0];
            }
        }

        internal static int NumberOfLimbsUsed(this HugeFloat x)
        {
            if (_value(x) == IntPtr.Zero)
                return 0;

            unsafe
            {
                return ((int*)_value(x).ToPointer())[1];
            }
        }

        internal static int Exponent(this HugeFloat x)
        {
            if (_value(x) == IntPtr.Zero)
                return 0;

            unsafe
            {
                return ((int*)_value(x).ToPointer())[2];
            }
        }

        internal static IntPtr Limbs(this HugeFloat x)
        {
            if (_value(x) == IntPtr.Zero)
                return IntPtr.Zero;

            unsafe
            {
                return new IntPtr(*(void**)(((int*)_value(x).ToPointer()) + 2 + sizeof(IntPtr) / 4));
            }
        }
    }
}
