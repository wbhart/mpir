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

namespace MPIR.Tests.HugeFloatTests
{
    [TestClass]
    public class ConstructionAndDisposal
    {
        [TestMethod]
        public void FloatDefaultConstructor()
        {
            using (var a = new HugeFloat())
            {
                Assert.AreNotEqual(0, a.NumberOfLimbsAllocated());
                Assert.AreEqual(0, a.NumberOfLimbsUsed());
                Assert.AreEqual(0, a.Exponent());
                Assert.AreNotEqual(IntPtr.Zero, a.Limbs());
                Assert.AreEqual("0", a.ToString());
            }
        }

        [TestMethod]
        public void FloatDispose()
        {
            var a = new HugeFloat();
            a.Dispose();

            Assert.AreEqual(0, a.NumberOfLimbsAllocated());
            Assert.AreEqual(0, a.NumberOfLimbsUsed());
            Assert.AreEqual(IntPtr.Zero, a.Limbs());
        }

        [TestMethod]
        public void FloatConstructorFromLong()
        {
            var n = "123456789123456";
            using (var a = new HugeFloat(long.Parse(n)))
            {
                Assert.AreEqual(2, a.NumberOfLimbsAllocated());
                Assert.AreEqual(1, a.NumberOfLimbsUsed());
                Assert.IsTrue(a.Exponent() > 0);
                Assert.AreEqual("0." + n + "@15", a.ToString());
            }
        }

        [TestMethod]
        public void FloatConstructorFromLongNegative()
        {
            var n = "-123456789123456";
            using (var a = new HugeFloat(long.Parse(n)))
            {
                Assert.AreEqual(2, a.NumberOfLimbsAllocated());
                Assert.AreEqual(-1, a.NumberOfLimbsUsed());
                Assert.IsTrue(a.Exponent() > 0);
                Assert.AreEqual("-0." + n.Substring(1) + "@15", a.ToString());
            }
        }

        [TestMethod]
        public void FloatConstructorFromULong()
        {
            using (var a = new HugeFloat(ulong.MaxValue))
            {
                Assert.AreEqual(2, a.NumberOfLimbsAllocated());
                Assert.AreEqual(1, a.NumberOfLimbsUsed());
                Assert.IsTrue(a.Exponent() > 0);
                var n = ulong.MaxValue.ToString();
                Assert.AreEqual("0." + n + "@" + n.Length, a.ToString());
            }
        }

        [TestMethod]
        public void FloatConstructorFromDouble()
        {
            using (var a = new HugeFloat(123456789123456.75))
            {
                Assert.AreEqual("0.12345678912345675@15", a.ToString());
            }
        }

        [TestMethod]
        public void FloatConstructorFromDoubleNegative()
        {
            using (var a = new HugeFloat(-123456789123456.75))
            {
                Assert.AreEqual("-0.12345678912345675@15", a.ToString());
            }
        }

        [TestMethod]
        public void FloatAllocate()
        {
            using (var a = HugeFloat.Allocate(129))
            {
                Assert.AreEqual(4, a.NumberOfLimbsAllocated());
                Assert.AreEqual(0, a.NumberOfLimbsUsed());
                Assert.AreEqual("0", a.ToString());

                var n = "234095827340957823409582587";
                a.SetTo(n);
                Assert.AreEqual("0." + n + "@" + n.Length, a.ToString());
            }
        }

        [TestMethod]
        public void FloatDefaultConstructorHigherPrecision()
        {
            var oldPrecision = HugeFloat.DefaultPrecision;
            HugeFloat.DefaultPrecision = 256;
            using (var a = new HugeFloat())
            {
                Assert.AreEqual(5, a.NumberOfLimbsAllocated());
                Assert.AreEqual(0, a.NumberOfLimbsUsed());
                Assert.AreEqual("0", a.ToString());

                var n = "2340958273409578234534534534523425343409582587";
                a.SetTo(n);
                Assert.AreEqual("0." + n + "@" + n.Length, a.ToString());
            }
            HugeFloat.DefaultPrecision = oldPrecision;
        }

        [TestMethod]
        public void FloatStringConstructor()
        {
            var n = "0.54321098765432123457@28";
            using (var a = new HugeFloat(n))
            {
                Assert.AreEqual(2, a.NumberOfLimbsAllocated());
                Assert.AreEqual(n, a.ToString());
            }
        }

        [TestMethod]
        [ExpectedException(typeof(ArgumentException))]
        public void FloatStringConstructorInvalid()
        {
            var a = new HugeFloat("12345A");
        }

        [TestMethod]
        public void FloatStringConstructorHex()
        {
            var n = "143210ABCDEF321234";
            using (var a = new HugeFloat(n, 16))
            {
                Assert.AreEqual("0." + n + "@18", a.ToString(16));
            }
        }

        [TestMethod]
        [ExpectedException(typeof(ArgumentException))]
        public void FloatStringConstructorHexPrefix()
        {
            var n = "0x143210ABCDEF32123457ACDB324598799";
            var a = new HugeFloat(n);
        }

        [TestMethod]
        public void FloatConstructorFromExpression()
        {
            using (var a = new HugeFloat("2340958273409578234095823045.5"))
            using (var b = new HugeFloat(a + 1))
            {
                Assert.AreEqual(a + 1, b);
            }
        }

        [TestMethod]
        public void FloatConstructorFromIntExpression()
        {
            using (var a = new HugeInt("234095827340957823409582587"))
            using (var b = new HugeFloat(a + 1))
            {
                Assert.AreEqual("0.23409582734095782341@27", b.ToString());
            }
        }
    }
}
