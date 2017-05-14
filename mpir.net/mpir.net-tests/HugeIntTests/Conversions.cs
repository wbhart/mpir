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
        public void IntToStringDecimal()
        {
            var n = "-23429384756298357462983476598345623984756";
            using (var a = new HugeInt(n))
            {
                Assert.AreEqual(n, a.ToString());
            }
        }

        [TestMethod]
        public void IntToStringHex()
        {
            var n = "-23429abcdef298357462983fedcba345623984756";
            using (var a = new HugeInt(n, 16))
            {
                Assert.AreEqual(n, a.ToString(16, true));
                Assert.AreEqual(n.ToUpper(), a.ToString(16));
            }
        }

#if WIN64
        [TestMethod]
        public void IntToAndFromUlong()
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
        public void IntToAndFromLong()
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
#else
        [TestMethod]
        public void IntToAndFromUint()
        {
            using (var a = new HugeInt())
            {
                uint b = 0xF84739AB;
                a.SetTo(b);
                Assert.AreEqual(b.ToString(), a.ToString());

                a.Value = -a;
                uint c = a.ToUint();
                Assert.AreEqual(b.ToString(), c.ToString());
            }
        }

        [TestMethod]
        public void IntToAndFromInt()
        {
            using (var a = new HugeInt())
            {
                int b = -0x784739AB;
                a.SetTo(b);
                Assert.AreEqual(b.ToString(), a.ToString());

                long c = a.ToInt();
                Assert.AreEqual(b.ToString(), c.ToString());
            }
        }
#endif

        [TestMethod]
        public void IntToAndFromDouble()
        {
            using (var a = new HugeInt())
            using (var lo = new HugeInt())
            using (var hi = new HugeInt())
            {
                var source = Platform.Select(-123.45e20, -123.45e19);
                var zillion = Platform.Ui(10000000000U, 1000000000U);
                var factor = Platform.Ui(1, 10);
                var exp = 0;

                a.SetTo(source);
                lo.Value = (a/zillion).Rounding(RoundingModes.Floor);
                hi.Value = (a/zillion).Rounding(RoundingModes.Ceiling);

                Assert.IsTrue(lo.ToString() == "-1234500000000" || hi.ToString() == "-1234500000000");

                double c = a.ToDouble();
                Assert.AreEqual(source, c);

                a.Value = (a + a) * factor;
                c = a.ToDouble(out exp);

                Assert.AreEqual(75, exp);
                c *= System.Math.Pow(2, exp);

                Assert.IsTrue(a + zillion >= c);
                Assert.IsTrue(a - zillion <= c);
            }
        }

        [TestMethod]
        public void IntFromString()
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
        public void IntFromInvalidString()
        {
            using (var a = new HugeInt())
            {
                a.SetTo("12345A");
            }
        }

        [TestMethod]
        public void IntToStringTruncated()
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

#if WIN64
        [TestMethod]
        public void IntFitsUlong()
        {
            using (var a = new HugeInt(ulong.MaxValue))
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
        public void IntFitsLong()
        {
            using (var a = new HugeInt(long.MaxValue))
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
#endif

        [TestMethod]
        public void IntFitsUint()
        {
            using (var a = new HugeInt(uint.MaxValue))
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
        public void IntFitsInt()
        {
            using (var a = new HugeInt(int.MaxValue))
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
        public void IntFitsUshort()
        {
            using (var a = new HugeInt(ushort.MaxValue))
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
        public void IntFitsShort()
        {
            using (var a = new HugeInt(short.MaxValue))
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
        public void IntApproximateSizeInBase()
        {
            using (var a = new HugeInt("2983475029834750293429834750298347502934298347502983475029342983475029834750293429834750298347502934"))
            {
                AssertEither(100, 101, a.ApproximateSizeInBase(10));
                AssertEither(331, 331, a.ApproximateSizeInBase(2));
                AssertEither(83, 83, a.ApproximateSizeInBase(16));
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
