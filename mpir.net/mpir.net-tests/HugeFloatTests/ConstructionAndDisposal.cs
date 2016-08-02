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
        [TestInitialize]
        public void Setup()
        {
            HugeFloat.DefaultPrecision = 128;
        }

        [TestCleanup]
        public void Cleanup()
        {
            HugeFloat.DefaultPrecision = 64;
        }

        [TestMethod]
        public void FloatDefaultConstructor()
        {
            using (var a = new HugeFloat())
            {
                Assert.AreEqual(0, a.NumberOfLimbsUsed());
                Assert.AreEqual(128UL, a.Precision);
                Assert.AreEqual(128UL, a._allocatedPrecision);
                Assert.AreNotEqual(IntPtr.Zero, a.Limbs());
                Assert.AreEqual(0, a.Exponent());
                Assert.AreEqual("0", a.ToString());
            }
        }

        [TestMethod]
        public void FloatDispose()
        {
            var a = new HugeFloat();
            a.Dispose();

            Assert.AreEqual(0, a.NumberOfLimbsUsed());
            Assert.AreEqual(IntPtr.Zero, a.Limbs());
        }

        [TestMethod]
        public void FloatConstructorFromLong()
        {
            var n = Platform.Si(0x3123456789123456L, 0x49384756);
            using (var a = new HugeFloat(n))
            {
                Assert.AreEqual(128UL, a._allocatedPrecision);
                Assert.AreEqual(128UL, a.Precision);
                Assert.AreEqual(1, a.Exponent());
                Assert.AreEqual("0." + n.ToString("X") + "@" + MpirSettings.BITS_PER_LIMB / 4, a.ToString(16, false, true));
            }
        }

        [TestMethod]
        public void FloatConstructorFromLongNegative()
        {
            var n = Platform.Si(0x3123456789123456L, 0x49384756);
            using(var a = new HugeFloat(-n))
            {
                Assert.AreEqual(128UL, a._allocatedPrecision);
                Assert.AreEqual(128UL, a.Precision);
                Assert.AreEqual(1, a.Exponent());
                Assert.AreEqual("-0." + n.ToString("X") + "@" + MpirSettings.BITS_PER_LIMB / 4, a.ToString(16, false, true));
            }
        }

        [TestMethod]
        public void FloatConstructorFromULong()
        {
            var n = Platform.Ui(ulong.MaxValue, uint.MaxValue);
            using(var a = new HugeFloat(n))
            {
                Assert.AreEqual(128UL, a._allocatedPrecision);
                Assert.AreEqual(128UL, a.Precision);
                Assert.AreEqual(1, a.Exponent());
                Assert.AreEqual("0." + n.ToString("X") + "@" + MpirSettings.BITS_PER_LIMB / 4, a.ToString(16, false, true));
            }
        }

        [TestMethod]
        public void FloatConstructorFromDouble()
        {
            using (var a = new HugeFloat(123456789123456.75))
            {
                FloatAssert.AreEqual("123456789123456.75", a);
            }
        }

        [TestMethod]
        public void FloatConstructorFromDoubleNegative()
        {
            using (var a = new HugeFloat(-123456789123456.75))
            {
                FloatAssert.AreEqual("-123456789123456.75", a);
            }
        }

        [TestMethod]
        public void FloatAllocate()
        {
            using (var a = HugeFloat.Allocate(193))
            {
                Assert.AreEqual(192U + MpirSettings.BITS_PER_LIMB, a.Precision);
                Assert.AreEqual(192U + MpirSettings.BITS_PER_LIMB, a._allocatedPrecision);
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
                Assert.AreEqual(256 / MpirSettings.BITS_PER_LIMB + 1, a.NumberOfLimbsAllocated());
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
            var n = "5432109876543212345789.70515331128527330659";
            using (var a = new HugeFloat(n))
            {
                FloatAssert.AreEqual(n, a);
            }
        }

        [TestMethod]
        public void FloatStringConstructorWithAlternativeExponentMarker()
        {
            var n = "5432109876543212345789.70515331128527330659e3";
            using(var a = new HugeFloat(n))
            {
                FloatAssert.AreEqual("5432109876543212345789705.15331128527330659", a);
            }
        }

        [TestMethod]
        public void FloatStringConstructorWithAlternativeExponentMarker2()
        {
            var n = "5432109876543212345789.70515331128527330659E3";
            using(var a = new HugeFloat(n))
            {
                FloatAssert.AreEqual("5432109876543212345789705.15331128527330659", a);
            }
        }

        [TestMethod]
        [ExpectedException(typeof(ArgumentException))]
        public void FloatStringConstructorWithAlternativeExponentMarkerInvalid()
        {
            var n = "5432109876543212345789.70515331128527330659e3";
            var a = new HugeFloat(n, 11);
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
            var n = "143210ABCDEF32123457ACDB324.59879";
            using (var a = new HugeFloat(n, 16))
            {
                Assert.AreEqual("0.143210ABCDEF32123457ACDB32459879@27", a.ToString(16, false, true));
            }
        }

        [TestMethod]
        public void FloatStringConstructorHexExponentDecimal()
        {
            var n = "143210ABCDEF32123457ACDB324.59879@10";
            using(var a = new HugeFloat(n, 16))
            {
                Assert.AreEqual("0.143210ABCDEF32123457ACDB32459879@37", a.ToString(16, false, true));
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
        public void FloatStringConstructorHexExponentInBase()
        {
            var n = "143210ABCDEF32123457ACDB324.59879@10";
            using(var a = new HugeFloat(n, 16, false))
            {
                Assert.AreEqual("0.143210ABCDEF32123457ACDB32459879@2B", a.ToString(16, false, false));
                Assert.AreEqual("0.143210ABCDEF32123457ACDB32459879@43", a.ToString(16, false, true));
            }
        }

        [TestMethod]
        public void FloatConstructorFromExpression()
        {
            using (var a = new HugeFloat("29340273582305894258424059.2345293574"))
            using (var b = new HugeFloat(a + 1))
            {
                Assert.AreEqual(a + 1, b);
            }
        }

        [TestMethod]
        public void FloatConstructorFromIntExpression()
        {
            using (var a = new HugeInt("2340958273409578234095823045723490587"))
            using (var b = new HugeFloat(a + 1))
            {
                FloatAssert.AreEqual("2340958273409578234095823045723490588.", b);
            }
        }

        [TestMethod]
        public void FloatSetPrecision()
        {
            using (var a = new HugeFloat(1))
            using (var b = new HugeFloat())
            {
                a.Value = a/3;
                b.Value = a;
                Assert.AreEqual(128UL, a._allocatedPrecision);
                Assert.AreEqual(128UL, a.Precision);
                Assert.AreEqual("0.3333333333333333333333333333333333333333@0", a.ToString());

                a.Precision = 64;
                Assert.AreEqual(128UL, a._allocatedPrecision);
                Assert.AreEqual(64UL, a.Precision);
                Assert.AreEqual("0.333333333333333333333@0", a.ToString());
                Assert.AreEqual(a, b);

                a.Precision = 128;
                Assert.AreEqual("0.3333333333333333333333333333333333333333@0", a.ToString());

                a.Precision = 64;
                a.SetTo(1);
                a.Value = a / 3;
                Assert.AreNotEqual(a, b);
                Assert.AreEqual("0.333333333333333333333@0", a.ToString());

                a.Precision = 128;
                Assert.AreNotEqual(a, b);
            }
        }

        [TestMethod]
        public void FloatAllocatedPrecision()
        {
            using (var a = new HugeFloat(1))
            using (var b = HugeFloat.Allocate(256))
            {
                var bAllocated = b.AllocatedPrecision;
                var aAllocated = a.AllocatedPrecision;
                Assert.IsTrue(bAllocated > aAllocated);

                a.Precision = 64;
                b.Precision = 64;
                Assert.AreEqual(bAllocated, b.AllocatedPrecision);
                Assert.AreEqual(aAllocated, a.AllocatedPrecision);
            }
        }

        [TestMethod]
        [ExpectedException(typeof(ArgumentException))]
        public void FloatSettingPrecisionOverAllocated()
        {
            using (var a = new HugeFloat())
            {
                a.Precision++;
            }
        }

        [TestMethod]
        public void FloatReallocate()
        {
            using (var a = HugeFloat.Allocate(128))
            {
                Assert.AreEqual(128UL, a._allocatedPrecision);
                Assert.AreEqual(128UL, a.Precision);

                a.Reallocate(256);
                Assert.AreEqual(256UL, a._allocatedPrecision);
                Assert.AreEqual(256UL, a.Precision);
            }
        }
    }
}
