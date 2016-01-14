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
using Microsoft.VisualStudio.TestTools.UnitTesting;

namespace MPIR.Tests.HugeIntTests
{
    [TestClass]
    public class ConstructionAndDisposal
    {
        [TestMethod]
        public void IntDefaultConstructor()
        {
            using (var a = new HugeInt())
            {
                Assert.AreNotEqual(0, a.NumberOfLimbsAllocated());
                Assert.AreEqual(0, a.NumberOfLimbsUsed());
                Assert.AreNotEqual(IntPtr.Zero, a.Limbs());
                Assert.AreEqual("0", a.ToString());
            }
        }

        [TestMethod]
        public void IntDispose()
        {
            var a = new HugeInt();
            a.Dispose();
            Assert.AreEqual(0, a.NumberOfLimbsAllocated());
            Assert.AreEqual(0, a.NumberOfLimbsUsed());
            Assert.AreEqual(IntPtr.Zero, a.Limbs());
        }

        [TestMethod]
        public void IntConstructorFromLong()
        {
            var n = "123456789123456";
            using (var a = new HugeInt(long.Parse(n)))
            {
                Assert.AreEqual(64 / MpirSettings.BITS_PER_LIMB, a.NumberOfLimbsAllocated());
                Assert.AreEqual(64 / MpirSettings.BITS_PER_LIMB, a.NumberOfLimbsUsed());
                Assert.AreEqual(n, a.ToString());
            }
        }

        [TestMethod]
        public void IntConstructorFromLongNegative()
        {
            var n = "-123456789123456";
            using (var a = new HugeInt(long.Parse(n)))
            {
                Assert.AreEqual(64 / MpirSettings.BITS_PER_LIMB, a.NumberOfLimbsAllocated());
                Assert.AreEqual(-64 / MpirSettings.BITS_PER_LIMB, a.NumberOfLimbsUsed());
                Assert.AreEqual(n, a.ToString());
            }
        }

        [TestMethod]
        public void IntConstructorFromULong()
        {
            var max = Platform.Ui(ulong.MaxValue, uint.MaxValue);
            using (var a = new HugeInt(max))
            {
                Assert.AreEqual(1, a.NumberOfLimbsAllocated());
                Assert.AreEqual(1, a.NumberOfLimbsUsed());
                Assert.AreEqual(max.ToString(), a.ToString());
            }
        }

        [TestMethod]
        public void IntConstructorFromDouble()
        {
            using (var a = new HugeInt(123456789123456.9))
            {
                Assert.AreEqual("123456789123456", a.ToString());
            }
        }

        [TestMethod]
        public void IntConstructorFromPower()
        {
            using (var a = new HugeInt(HugeInt.Power(10, 41)))
            {
                Assert.AreEqual("100000000000000000000000000000000000000000", a.ToString());
            }
        }

        [TestMethod]
        public void IntConstructorFromDoubleNegative()
        {
            using (var a = new HugeInt(-123456789123456.9))
            {
                Assert.AreEqual("-123456789123456", a.ToString());
            }
        }

        [TestMethod]
        public void IntAllocate()
        {
            using (var a = HugeInt.Allocate(129))
            {
                Assert.AreEqual(1 + 128 / MpirSettings.BITS_PER_LIMB, a.NumberOfLimbsAllocated());
                Assert.AreEqual(0, a.NumberOfLimbsUsed());
                Assert.AreEqual("0", a.ToString());
            }
        }

        [TestMethod]
        public void IntReallocate()
        {
            using (var a = new HugeInt("543209879487374938579837"))
            {
                Assert.AreEqual(2 + 64 / MpirSettings.BITS_PER_LIMB, a.NumberOfLimbsAllocated());
                Assert.AreEqual("543209879487374938579837", a.ToString());

                a.Reallocate(257);
                Assert.AreEqual(1 + 256 / MpirSettings.BITS_PER_LIMB, a.NumberOfLimbsAllocated());
                Assert.AreEqual("543209879487374938579837", a.ToString());

                a.Reallocate(129);
                Assert.AreEqual(1 + 128 / MpirSettings.BITS_PER_LIMB, a.NumberOfLimbsAllocated());
                Assert.AreEqual("543209879487374938579837", a.ToString());

                a.Reallocate(64);
                Assert.AreEqual(64 / MpirSettings.BITS_PER_LIMB, a.NumberOfLimbsAllocated());
                Assert.AreEqual("0", a.ToString());
            }
        }

        [TestMethod]
        public void IntStringConstructor()
        {
            var n = "5432109876543212345789023245987";
            using (var a = new HugeInt(n))
            {
                Assert.AreEqual(128 / MpirSettings.BITS_PER_LIMB, a.NumberOfLimbsUsed());
                Assert.AreEqual(n, a.ToString());
            }
        }

        [TestMethod]
        [ExpectedException(typeof(ArgumentException))]
        public void IntStringConstructorInvalid()
        {
            var a = new HugeInt("12345A");
        }

        [TestMethod]
        public void IntStringConstructorHex()
        {
            using (var a = new HugeInt("143210ABCDEF32123457ACDB324598799", 16))
            {
                Assert.AreEqual(1 + 128 / MpirSettings.BITS_PER_LIMB, a.NumberOfLimbsUsed());
            }
        }

        [TestMethod]
        public void IntStringConstructorHexPrefix()
        {
            var n = "143210ABCDEF32123457ACDB324598799";
            using (var a = new HugeInt("0x" + n))
            {
                Assert.AreEqual(n, a.ToString(16));
            }
        }

        [TestMethod]
        public void IntStringAssignmentHexPrefix()
        {
            var n = "143210ABCDEF32123457ACDB324598799";
            using (var a = new HugeInt("0x" + n))
            {
                Assert.AreEqual(n, a.ToString(16));
                Assert.AreEqual(n, a.ToString(16, false));
                Assert.AreEqual(n.ToLower(), a.ToString(16, true));
                a.SetTo("-0x" + n);
                Assert.AreEqual("-" + n, a.ToString(16));
            }
        }

        [TestMethod]
        public void IntConstructorFromExpression()
        {
            using (var a = new HugeInt("2340958273409578234095823045723490587"))
            using (var b = new HugeInt(a + 1))
            {
                Assert.AreEqual(a + 1, b);
            }
        }
    }
}
