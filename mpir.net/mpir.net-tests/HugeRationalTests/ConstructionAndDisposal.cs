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

namespace MPIR.Tests.HugeRationalTests
{
    [TestClass]
    public class ConstructionAndDisposal
    {
        [TestMethod]
        public void DefaultConstructor()
        {
            using (var a = new HugeRational())
            {
                Assert.AreNotEqual(0, a.NumeratorNumberOfLimbsAllocated());
                Assert.AreEqual(0, a.NumeratorNumberOfLimbsUsed());
                Assert.AreNotEqual(IntPtr.Zero, a.NumeratorLimbs());
                Assert.AreEqual("0", a.Numerator.ToString());

                Assert.AreNotEqual(0, a.DenominatorNumberOfLimbsAllocated());
                Assert.AreEqual(1, a.DenominatorNumberOfLimbsUsed());
                Assert.AreNotEqual(IntPtr.Zero, a.DenominatorLimbs());
                Assert.AreEqual("1", a.Denominator.ToString());
            }
        }

        [TestMethod]
        public void Numerator()
        {
            using (var a = new HugeRational())
            {
                a.Numerator.Dispose();
                Assert.AreNotEqual(0, a.Numerator.NumberOfLimbsAllocated());
                Assert.AreEqual("0", a.Numerator.ToString());
            }
        }

        [TestMethod]
        public void Denominator()
        {
            using (var a = new HugeRational())
            {
                a.Denominator.Dispose();
                Assert.AreNotEqual(0, a.Denominator.NumberOfLimbsAllocated());
                Assert.AreEqual("1", a.Denominator.ToString());
            }
        }

        [TestMethod]
        public void Dispose()
        {
            var a = new HugeRational();
            a.Dispose();
            
            Assert.AreEqual(0, a.NumeratorNumberOfLimbsAllocated());
            Assert.AreEqual(0, a.NumeratorNumberOfLimbsUsed());
            Assert.AreEqual(IntPtr.Zero, a.NumeratorLimbs());

            Assert.AreEqual(0, a.DenominatorNumberOfLimbsAllocated());
            Assert.AreEqual(0, a.DenominatorNumberOfLimbsUsed());
            Assert.AreEqual(IntPtr.Zero, a.DenominatorLimbs());
        }

        [TestMethod]
        public void ConstructorFromLong()
        {
            var n = "123456789123456";
            var d = "12764787846358441471";
            using (var a = new HugeRational(long.Parse(n), ulong.Parse(d)))
            {
                Assert.AreEqual(1, a.NumeratorNumberOfLimbsAllocated());
                Assert.AreEqual(1, a.NumeratorNumberOfLimbsUsed());
                Assert.AreEqual(1, a.DenominatorNumberOfLimbsAllocated());
                Assert.AreEqual(1, a.DenominatorNumberOfLimbsUsed());
                Assert.AreEqual(n + "/" + d, a.ToString());
            }
        }

        [TestMethod]
        public void ConstructorFromLongNegative()
        {
            var n = "-123456789123456";
            var d = "12764787846358441471";
            using (var a = new HugeRational(long.Parse(n), ulong.Parse(d)))
            {
                Assert.AreEqual(1, a.NumeratorNumberOfLimbsAllocated());
                Assert.AreEqual(-1, a.NumeratorNumberOfLimbsUsed());
                Assert.AreEqual(1, a.DenominatorNumberOfLimbsAllocated());
                Assert.AreEqual(1, a.DenominatorNumberOfLimbsUsed());
                Assert.AreEqual(n + "/" + d, a.ToString());
            }
        }

        [TestMethod]
        public void ConstructorFromULong()
        {
            var d = "12764787846358441471";
            using (var a = new HugeRational(ulong.MaxValue, ulong.Parse(d)))
            {
                Assert.AreEqual(1, a.NumeratorNumberOfLimbsAllocated());
                Assert.AreEqual(1, a.NumeratorNumberOfLimbsUsed());
                Assert.AreEqual(1, a.DenominatorNumberOfLimbsAllocated());
                Assert.AreEqual(1, a.DenominatorNumberOfLimbsUsed());
                Assert.AreEqual(ulong.MaxValue, a.Numerator);
                Assert.AreEqual(ulong.MaxValue.ToString() + "/" + d, a.ToString());
            }
        }

        [TestMethod]
        public void ConstructorFromDouble()
        {
            using (var a = new HugeRational(123456789123456.9))
            {
                Assert.AreEqual("1234567891234569/10", a.ToString());
            }
        }

        [TestMethod]
        public void ConstructorFromDoubleNegative()
        {
            using (var a = new HugeRational(-123456789123456.9))
            {
                Assert.AreEqual("-1234567891234569/10", a.ToString());
            }
        }

        [TestMethod]
        public void Allocate()
        {
            using (var a = new HugeRational(129, 193))
            {
                Assert.AreEqual(3, a.NumeratorNumberOfLimbsAllocated());
                Assert.AreEqual(0, a.NumeratorNumberOfLimbsUsed());
                Assert.AreEqual(4, a.DenominatorNumberOfLimbsAllocated());
                Assert.AreEqual(1, a.DenominatorNumberOfLimbsUsed());
                Assert.AreEqual("0/1", a.ToString());
            }
        }

        [TestMethod]
        public void StringConstructor()
        {
            var n = "5432109876543212345789023245987/362736035870515331128527330659";
            using (var a = new HugeRational(n))
            {
                Assert.AreEqual(2, a.NumeratorNumberOfLimbsUsed());
                Assert.AreEqual(n, a.ToString());
            }
        }

        [TestMethod]
        [ExpectedException(typeof(ArgumentException))]
        public void StringConstructorInvalid()
        {
            var a = new HugeRational("12345A");
        }

        [TestMethod]
        [ExpectedException(typeof(ArgumentException))]
        public void StringConstructorInvalid2()
        {
            var a = new HugeRational("12345/54321A");
        }

        [TestMethod]
        public void StringConstructorHex()
        {
            using (var i = new HugeInt("362736035870515331128527330659"))
            {
                var d = i.ToString(16);
                using (var a = new HugeRational("143210ABCDEF32123457ACDB324598799/" + d, 16))
                {
                    Assert.AreEqual(3, a.NumeratorNumberOfLimbsUsed());
                    Assert.AreEqual(i, a.Denominator);
                }
            }
        }

        [TestMethod]
        public void StringConstructorHexPrefix()
        {
            using (var i = new HugeInt("362736035870515331128527330659"))
            {
                var d = i.ToString(16);
                var n = "143210ABCDEF32123457ACDB324598799";
                using (var a = new HugeRational("0x" + n + "/0x" + d))
                {
                    Assert.AreEqual(n, a.ToString(16));
                    Assert.AreEqual(i, a.Denominator);
                }
            }
        }

        [TestMethod]
        public void StringAssignmentHexPrefix()
        {
            using (var i = new HugeInt("362736035870515331128527330659"))
            {
                var d = i.ToString(16);
                var n = "143210ABCDEF32123457ACDB324598799";
                using (var a = new HugeRational("0x" + n + "/0x" + d))
                {
                    Assert.AreEqual(n + "/" + d, a.ToString(16));
                    Assert.AreEqual(n + "/" + d, a.ToString(16, false));
                    Assert.AreEqual((n + "/" + d).ToLower(), a.ToString(16, true));
                    a.SetTo("-0x" + n + "/0x17");
                    Assert.AreEqual("-" + n + "/17", a.ToString(16));
                }
            }
        }

        [TestMethod]
        public void ConstructorFromExpression()
        {
            using (var a = new HugeRational("2340958273409578234095823045723490587/362736035870515331128527330659"))
            using (var b = new HugeRational(a + 1))
            {
                Assert.AreEqual(a + 1, b);
            }
        }
    }
}
