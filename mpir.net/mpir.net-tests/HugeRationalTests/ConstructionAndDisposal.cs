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
        public void RationalDefaultConstructor()
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
        public void RationalNumerator()
        {
            using (var a = new HugeRational())
            {
                a.Numerator.Dispose();
                Assert.AreNotEqual(0, a.Numerator.NumberOfLimbsAllocated());
                Assert.AreEqual("0", a.Numerator.ToString());
            }
        }

        [TestMethod]
        public void RationalDenominator()
        {
            using (var a = new HugeRational())
            {
                a.Denominator.Dispose();
                Assert.AreNotEqual(0, a.Denominator.NumberOfLimbsAllocated());
                Assert.AreEqual("1", a.Denominator.ToString());
            }
        }

        [TestMethod]
        public void RationalDispose()
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
        public void RationalConstructorFromLong()
        {
            var n = Platform.Si(123456789123456, 1234567891);
            var d = Platform.Ui(12764787846358441471U, 2860486313U);

            using (var a = new HugeRational(n, d))
            {
                Assert.AreEqual(1, a.NumeratorNumberOfLimbsAllocated());
                Assert.AreEqual(1, a.NumeratorNumberOfLimbsUsed());
                Assert.AreEqual(1, a.DenominatorNumberOfLimbsAllocated());
                Assert.AreEqual(1, a.DenominatorNumberOfLimbsUsed());
                Assert.AreEqual(n + "/" + d, a.ToString());
            }
        }

        [TestMethod]
        public void RationalConstructorFromLongNegative()
        {
            var n = Platform.Si(-123456789123456, -1234567891);
            var d = Platform.Ui(12764787846358441471U, 2860486313U);

            using (var a = new HugeRational(n, d))
            {
                Assert.AreEqual(1, a.NumeratorNumberOfLimbsAllocated());
                Assert.AreEqual(-1, a.NumeratorNumberOfLimbsUsed());
                Assert.AreEqual(1, a.DenominatorNumberOfLimbsAllocated());
                Assert.AreEqual(1, a.DenominatorNumberOfLimbsUsed());
                Assert.AreEqual(n + "/" + d, a.ToString());
            }
        }

        [TestMethod]
        public void RationalConstructorFromULong()
        {
            var n = Platform.Ui(ulong.MaxValue, uint.MaxValue);
            var d = Platform.Ui(12764787846358441471U, 2860486313U);

            using (var a = new HugeRational(n, d))
            {
                Assert.AreEqual(1, a.NumeratorNumberOfLimbsAllocated());
                Assert.AreEqual(1, a.NumeratorNumberOfLimbsUsed());
                Assert.AreEqual(1, a.DenominatorNumberOfLimbsAllocated());
                Assert.AreEqual(1, a.DenominatorNumberOfLimbsUsed());
                Assert.AreEqual(n.ToString(), a.Numerator.ToString());
                Assert.AreEqual(n.ToString() + "/" + d, a.ToString());
            }
        }

        [TestMethod]
        public void RationalConstructorFromDouble()
        {
            using (var a = new HugeRational(123456789123456.75))
            {
                Assert.AreEqual("493827156493827/4", a.ToString());
            }
        }

        [TestMethod]
        public void RationalConstructorFromDoubleNegative()
        {
            using (var a = new HugeRational(-123456789123456.75))
            {
                Assert.AreEqual("-493827156493827/4", a.ToString());
            }
        }

        [TestMethod]
        public void RationalAllocate()
        {
            using (var a = HugeRational.Allocate(129, 193))
            {
                Assert.AreEqual(1 + 128 / MpirSettings.BITS_PER_LIMB, a.NumeratorNumberOfLimbsAllocated());
                Assert.AreEqual(0, a.NumeratorNumberOfLimbsUsed());
                Assert.AreEqual(1 + 192 / MpirSettings.BITS_PER_LIMB, a.DenominatorNumberOfLimbsAllocated());
                Assert.AreEqual(1, a.DenominatorNumberOfLimbsUsed());
                Assert.AreEqual("0/1", a.ToString());
            }
        }

        [TestMethod]
        public void RationalCanonicalize()
        {
            using (var a = new HugeRational(198, 15))
            {
                a.Denominator.Value = -a.Denominator;
                Assert.AreEqual("198/-15", a.ToString());
                a.Canonicalize();
                Assert.AreEqual("-66/5", a.ToString());
            }
        }

        [TestMethod]
        public void RationalStringConstructor()
        {
            var n = "5432109876543212345789023245987/362736035870515331128527330659";
            using (var a = new HugeRational(n))
            {
                Assert.AreEqual(128 / MpirSettings.BITS_PER_LIMB, a.NumeratorNumberOfLimbsUsed());
                Assert.AreEqual(n, a.ToString());
            }
        }

        [TestMethod]
        public void RationalStringConstructorNumeratorOnly()
        {
            var n = "5432109876543212345789023245987";
            using(var a = new HugeRational(n))
            {
                Assert.AreEqual(128 / MpirSettings.BITS_PER_LIMB, a.NumeratorNumberOfLimbsUsed());
                Assert.AreEqual(n + "/1", a.ToString());
            }
        }

        [TestMethod]
        [ExpectedException(typeof(ArgumentException))]
        public void RationalStringConstructorInvalid()
        {
            var a = new HugeRational("12345A");
        }

        [TestMethod]
        [ExpectedException(typeof(ArgumentException))]
        public void RationalStringConstructorInvalid2()
        {
            var a = new HugeRational("12345/54321A");
        }

        [TestMethod]
        public void RationalStringConstructorHex()
        {
            using (var i = new HugeInt("362736035870515331128527330659"))
            {
                var d = i.ToString(16);
                var n = "143210ABCDEF32123457ACDB324598799";
                using (var a = new HugeRational(n + "/" + d, 16))
                {
                    Assert.AreEqual(n, a.Numerator.ToString(16));
                    Assert.AreEqual(1 + 128 / MpirSettings.BITS_PER_LIMB, a.NumeratorNumberOfLimbsUsed());
                    Assert.AreEqual(i, a.Denominator);
                }
            }
        }

        [TestMethod]
        public void RationalStringConstructorHexPrefix()
        {
            using (var i = new HugeInt("362736035870515331128527330659"))
            {
                var d = i.ToString(16);
                var n = "143210ABCDEF32123457ACDB324598799";
                using (var a = new HugeRational("0x" + n + "/0x" + d))
                {
                    Assert.AreEqual(n + "/" + d, a.ToString(16));
                    Assert.AreEqual(i, a.Denominator);
                }
            }
        }

        [TestMethod]
        public void RationalStringAssignmentHexPrefix()
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
        public void RationalConstructorFromExpression()
        {
            using (var a = new HugeRational("2340958273409578234095823045723490587/362736035870515331128527330659"))
            using (var b = new HugeRational(a + 1))
            {
                Assert.AreEqual(a + 1, b);
            }
        }

        [TestMethod]
        public void RationalConstructorFromFloatExpression()
        {
            using (var a = new HugeFloat("2340958273409578234.25"))
            using (var b = new HugeRational(a + 1))
            {
                Assert.AreEqual("9363833093638312941/4", b.ToString());
            }
        }

        [TestMethod]
        public void RationalConstructorFromIntExpression()
        {
            using (var a = new HugeInt("2340958273409578234095823045723490587"))
            using (var b = new HugeRational(a + 1))
            {
                Assert.AreEqual("2340958273409578234095823045723490588/1", b.ToString());
            }
        }

        [TestMethod]
        public void RationalConstructorFromIntExpression2()
        {
            using (var a = new HugeInt("2340958273409578234095823045723490587"))
            using (var d = new HugeInt("362736035870515331128527330659"))
            using (var b = new HugeRational(a + 2, d * 2))
            {
                Assert.AreEqual(a + 2, b.Numerator);
                Assert.AreEqual(d * 2, b.Denominator);
            }
        }
    }
}
