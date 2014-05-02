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
using System.Linq;
using Microsoft.VisualStudio.TestTools.UnitTesting;

namespace MPIR.Tests.HugeIntTests
{
    [TestClass]
    public class Conversions
    {
        [TestMethod]
        public void ToStringDecimal()
        {
            var n = "-23429384756298357462983476598345623984756";
            using (var a = new HugeInt(n))
            {
                Assert.AreEqual(n, a.ToString());
            }
        }

        [TestMethod]
        public void ToStringHex()
        {
            var n = "-23429abcdef298357462983fedcba345623984756";
            using (var a = new HugeInt(n, 16))
            {
                Assert.AreEqual(n, a.ToString(16, true));
                Assert.AreEqual(n.ToUpper(), a.ToString(16));
            }
        }

        [TestMethod]
        public void ToAndFromUlong()
        {
            using (var a = new HugeInt())
            {
                ulong b = 0xF84739ABCDEF4876;
                a.SetTo(b);
                Assert.AreEqual(b.ToString(), a.ToString());

                a.Value = -a;
                ulong c = a.ToUlong();
                Assert.AreEqual(b.ToString(), c.ToString());
            }
        }

        [TestMethod]
        public void ToAndFromLong()
        {
            using (var a = new HugeInt())
            {
                long b = -0x784739ABCDEF4876;
                a.SetTo(b);
                Assert.AreEqual(b.ToString(), a.ToString());

                long c = a.ToLong();
                Assert.AreEqual(b.ToString(), c.ToString());
            }
        }

        [TestMethod]
        public void ToAndFromDouble()
        {
            using (var a = new HugeInt())
            using (var lo = new HugeInt())
            using (var hi = new HugeInt())
            {
                a.SetTo(-123.45e20);
                lo.Value = (a/10000000000).Rounding(RoundingModes.Floor);
                hi.Value = (a/10000000000).Rounding(RoundingModes.Ceiling);

                Assert.IsTrue(lo.ToString() == "-1234500000000" || hi.ToString() == "-1234500000000");

                double c = a.ToDouble();
                Assert.AreEqual(-123.45e20, c);
                
                long exp;
                a.Value = a + a;
                c = a.ToDouble(out exp);

                Assert.AreEqual(75, exp);
                c *= System.Math.Pow(2, exp);

                Assert.IsTrue(a + 10000000000 >= c);
                Assert.IsTrue(a - 10000000000 <= c);
            }
        }

        [TestMethod]
        public void FromString()
        {
            using (var a = new HugeInt())
            {
                var n = "98762934876529834765234123984761";
                a.SetTo(n);
                Assert.AreEqual(n, a.ToString());

                n = "-98ABCDEF876529834765234123984761";
                a.SetTo(n, 16);
                Assert.AreEqual(n, a.ToString(16));
            }
        }

        [TestMethod]
        [ExpectedException(typeof(ArgumentException))]
        public void FromInvalidString()
        {
            using (var a = new HugeInt())
            {
                a.SetTo("12345A");
            }
        }

        [TestMethod]
        public void ToStringTruncated()
        {
            var n = string.Concat("123456789".Select(c => new string(c, 30)));
            using (var a = new HugeInt(n))
            {
                Assert.AreEqual(n, a.ToString(10));
                Assert.AreEqual("..." + n.Substring(n.Length - 256), a.ToString());
                a.Value = -a;
                Assert.AreEqual("-..." + n.Substring(n.Length - 256), a.ToString());
            }
        }

        [TestMethod]
        public void FitsUlong()
        {
            using (var a = HugeInt.FromUlong(ulong.MaxValue))
            {
                Assert.IsTrue(a.FitsUlong());
                a.Value = a + 1;
                Assert.IsFalse(a.FitsUlong());
                a.SetTo(0);
                Assert.IsTrue(a.FitsUlong());
                a.Value = a - 1;
                Assert.IsFalse(a.FitsUlong());
            }
        }

        [TestMethod]
        public void FitsLong()
        {
            using (var a = HugeInt.FromLong(long.MaxValue))
            {
                Assert.IsTrue(a.FitsLong());
                a.Value = a + 1;
                Assert.IsFalse(a.FitsLong());
                a.SetTo(long.MinValue);
                Assert.IsTrue(a.FitsLong());
                a.Value = a - 1;
                Assert.IsFalse(a.FitsLong());
            }
        }

        [TestMethod]
        public void FitsUint()
        {
            using (var a = HugeInt.FromUlong(uint.MaxValue))
            {
                Assert.IsTrue(a.FitsUint());
                a.Value = a + 1;
                Assert.IsFalse(a.FitsUint());
                a.SetTo(0);
                Assert.IsTrue(a.FitsUint());
                a.Value = a - 1;
                Assert.IsFalse(a.FitsUint());
            }
        }

        [TestMethod]
        public void FitsInt()
        {
            using (var a = HugeInt.FromLong(int.MaxValue))
            {
                Assert.IsTrue(a.FitsInt());
                a.Value = a + 1;
                Assert.IsFalse(a.FitsInt());
                a.SetTo(int.MinValue);
                Assert.IsTrue(a.FitsInt());
                a.Value = a - 1;
                Assert.IsFalse(a.FitsInt());
            }
        }

        [TestMethod]
        public void FitsUshort()
        {
            using (var a = HugeInt.FromUlong(ushort.MaxValue))
            {
                Assert.IsTrue(a.FitsUshort());
                a.Value = a + 1;
                Assert.IsFalse(a.FitsUshort());
                a.SetTo(0);
                Assert.IsTrue(a.FitsUshort());
                a.Value = a - 1;
                Assert.IsFalse(a.FitsUshort());
            }
        }

        [TestMethod]
        public void FitsShort()
        {
            using (var a = HugeInt.FromLong(short.MaxValue))
            {
                Assert.IsTrue(a.FitsShort());
                a.Value = a + 1;
                Assert.IsFalse(a.FitsShort());
                a.SetTo(short.MinValue);
                Assert.IsTrue(a.FitsShort());
                a.Value = a - 1;
                Assert.IsFalse(a.FitsShort());
            }
        }

        [TestMethod]
        public void ApproximateSizeInBase()
        {
            using (var a = new HugeInt("2983475029834750293429834750298347502934298347502983475029342983475029834750293429834750298347502934"))
            {
                AssertEither(100, 101, a.ApproximateSizeInBase(10));
                AssertEither(331, 332, a.ApproximateSizeInBase(2));
                AssertEither(83, 84, a.ApproximateSizeInBase(16));
                AssertEither(64, 65, a.ApproximateSizeInBase(36));
                AssertEither(56, 57, a.ApproximateSizeInBase(62));
            }
        }

        private void AssertEither(int expected1, int expected2, long actual)
        {
            Assert.IsTrue(actual == expected1 || actual == expected2, "Expected {0} or {1}, actual {2}", expected1, expected2, actual);
        }
    }
}