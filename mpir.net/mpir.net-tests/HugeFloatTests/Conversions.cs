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

namespace MPIR.Tests.HugeFloatTests
{
    [TestClass]
    public class Conversions
    {
        private static HugeFloat AlmostOne;

        [ClassInitialize]
        public static void Init(TestContext context)
        {
            AlmostOne = new HugeFloat(0.99999);
            HugeFloat.DefaultPrecision = 128;
        }

        [ClassCleanup]
        public static void Cleanup()
        {
            HugeFloat.DefaultPrecision = 64;
        }

        [TestMethod]
        public void FloatToStringDecimal()
        {
            var n = "-234293847562.98357462983476598345623984756";
            using (var a = new HugeFloat(n))
            {
                FloatAssert.AreEqual(n, a);
            }
        }

        [TestMethod]
        public void FloatToStringHex()
        {
            var s = "-23429abcdef29835746298.3fedcba34562";
            using (var a = new HugeFloat(s, 16))
            {
                Assert.AreEqual("-0.23429abcdef298357462983fedcba34562@16", a.ToString(16, true));
                Assert.AreEqual("-0.23429ABCDEF298357462983FEDCBA34562@16", a.ToString(16));

                Assert.AreEqual("-0.23429abcdef298357462983fedcba34562@22", a.ToString(16, true, true));
                Assert.AreEqual("-0.23429ABCDEF298357462983FEDCBA34562@22", a.ToString(16, false, true));
            }
        }

        [TestMethod]
        public void FloatFromUlong()
        {
            using (var a = new HugeFloat())
            {
                var b = Platform.Ui(0xF84739ABCDEF4876, 0xF84739AB);
                a.SetTo(b);
                FloatAssert.AreEqual(b + ".", a);
            }
        }

        [TestMethod]
        public void FloatFromLong()
        {
            using (var a = new HugeFloat())
            {
                var b = Platform.Si(-0x784739ABCDEF4876, -0x784739AB);
                a.SetTo(b);
                FloatAssert.AreEqual(b + ".", a);
            }
        }

        [TestMethod]
        public void FloatToAndFromDouble()
        {
            using (var a = new HugeFloat())
            {
                a.SetTo(-123.25);

                double c = a.ToDouble();
                Assert.IsTrue(c.Equals(-123.25));
                Assert.IsTrue(a.Equals(-123.25));
                Assert.AreEqual("-0.12325@3", a.ToString());
            }
        }

        [TestMethod]
        public void FloatToAndFromDouble2()
        {
            using (var a = new HugeFloat())
            {
                a.SetTo(-123.25);
                var exp = 0;
                double c = a.ToDouble(out exp);
                Assert.IsTrue(c.Equals(-0.962890625));
                Assert.AreEqual(7L, exp);
                Assert.IsTrue(a.Equals(-123.25));
                Assert.AreEqual("-0.12325@3", a.ToString());
            }
        }

        [TestMethod]
        public void FloatToAndFromFloat()
        {
            using (var a = new HugeFloat())
            {
                a.SetTo(-123.125f);

                double c = a.ToDouble();
                Assert.IsTrue(-123.125 == c);
                Assert.IsTrue(-123.125 == a);
                Assert.IsTrue(-123.125f == a);
            }
        }

#if WIN64
        [TestMethod]
        public void FloatToAndFromUlong()
        {
            using (var a = new HugeFloat())
            {
                ulong b = 0xF84739ABCDEF4876;
                a.SetTo(b);
                FloatAssert.AreEqual(b + ".", a);

                a.Value = -a;
                ulong c = a.ToUlong();
                Assert.AreEqual(b, c);
            }
        }

        [TestMethod]
        public void FloatToAndFromLong()
        {
            using(var a = new HugeFloat())
            {
                long b = -0x784739ABCDEF4876;
                a.SetTo(b);
                FloatAssert.AreEqual(b + ".", a);

                long c = a.ToLong();
                Assert.AreEqual(b, c);
            }
        }
#else
        [TestMethod]
        public void FloatToAndFromUint()
        {
            using (var a = new HugeFloat())
            {
                ulong b = 0xF84739AB;
                a.SetTo(b);
                FloatAssert.AreEqual(b + ".", a);

                a.Value = -a;
                ulong c = a.ToUint();
                Assert.AreEqual(b, c);
            }
        }

        [TestMethod]
        public void FloatToAndFromInt()
        {
            using (var a = new HugeFloat())
            {
                long b = -0x784739AB;
                a.SetTo(b);
                FloatAssert.AreEqual(b + ".", a);

                long c = a.ToInt();
                Assert.AreEqual(b, c);
            }
        }
#endif

        [TestMethod]
        public void FloatToAndFromDoubleOutExp()
        {
            using(var a = new HugeFloat())
            {
                a.SetTo(-123.45e20);
                var exp = 0;
                var zillion = Platform.Si(10000000000, 1000000000);
                a.Value = a + a;
                double c = a.ToDouble(out exp);

                Assert.AreEqual(75, exp);
                c *= Math.Pow(2, exp);

                Assert.IsTrue(a + zillion >= c);
                Assert.IsTrue(a - zillion <= c);
            }
        }

#if WIN64
        [TestMethod]
        public void FloatToLong2()
        {
            using (var a = new HugeFloat())
            using (var small = new HugeFloat(0.0001))
            {
                long b = long.MaxValue;
                a.SetTo(b);
                a.Value = a + 1 - small;
                var c = a.ToLong();

                Assert.AreEqual(b, c);

                b = long.MinValue;
                a.SetTo(b);
                a.Value -= 1 - small;
                c = a.ToLong();
                Assert.AreEqual(b, c);
            }
        }
#else
        [TestMethod]
        public void FloatToInt2()
        {
            using (var a = new HugeFloat())
            using (var small = new HugeFloat(0.0001))
            {
                int b = int.MaxValue;
                a.SetTo(b);
                a.Value = a + 1 - small;
                var c = a.ToInt();

                Assert.AreEqual(b, c);

                b = int.MinValue;
                a.SetTo(b);
                a.Value -= 1 - small;
                c = a.ToInt();
                Assert.AreEqual(b, c);
            }
        }
#endif

        [TestMethod]
        public void FloatFromString()
        {
            using (var a = HugeFloat.Allocate(256))
            {
                var n = "98762934876529834765234123984761";
                a.SetTo(n);
                Assert.AreEqual("0." + n + "@" + n.Length, a.ToString());

                n = "-98ABCDEF876529834765234123984761";
                a.SetTo(n, 16);
                Assert.AreEqual("-0." + n.Substring(1) + "@" + (n.Length - 1).ToString("x"), a.ToString(16));
            }
        }

        [TestMethod]
        public void FloatFromStringExpDecimal()
        {
            using(var a = new HugeFloat())
            {
                var n = "0.12354523094527035843ABCDEF54@10";
                
                a.SetTo(n, 16);
                Assert.AreEqual("0.12354523094527035843ABCDEF54@10", a.ToString(16, false, true));
                Assert.AreEqual("0.12354523094527035843ABCDEF54@A", a.ToString(16, false, false));

                a.SetTo(n, 16, false);
                Assert.AreEqual("0.12354523094527035843ABCDEF54@16", a.ToString(16, false, true));
                Assert.AreEqual("0.12354523094527035843ABCDEF54@10", a.ToString(16, false, false));
            }
        }

        [TestMethod]
        [ExpectedException(typeof(ArgumentException))]
        public void FloatFromInvalidString()
        {
            using (var a = new HugeFloat())
            {
                a.SetTo("12345A");
            }
        }

        [TestMethod]
        public void FloatToStringTruncated()
        {
            var n = string.Concat("123456783".Select(c => new string(c, 30)));
            using (var a = HugeFloat.Allocate(2000))
            {
                a.SetTo(n);
                Assert.AreEqual("0." + n + "@" + n.Length, a.ToString(10));
                Assert.AreEqual("0." + n.Substring(0, 256) + "@" + n.Length, a.ToString());
                a.Value = -a;
                Assert.AreEqual("-0." + n.Substring(0, 256) + "@" + n.Length, a.ToString());
            }
        }

#if WIN64
        [TestMethod]
        public void FloatFitsUlong()
        {
            using (var a = new HugeFloat(ulong.MaxValue))
            using (var small = new HugeFloat(0.0001))
            {
                Assert.IsTrue(a.FitsUlong());
                a.Value = a + 1;
                Assert.IsFalse(a.FitsUlong());
                a.Value = a - small;
                Assert.IsTrue(a.FitsUlong());
                a.SetTo(0);
                Assert.IsTrue(a.FitsUlong());
                a.Value = a - 1;
                Assert.IsFalse(a.FitsUlong());
                a.Value = a + small;
                Assert.IsTrue(a.FitsUlong());
            }
        }

        [TestMethod]
        public void FloatFitsLong()
        {
            using (var a = new HugeFloat(long.MaxValue))
            using (var small = new HugeFloat(0.0001))
            {
                Assert.IsTrue(a.FitsLong());
                a.Value = a + 1;
                Assert.IsFalse(a.FitsLong());
                a.Value = a - small;
                Assert.IsTrue(a.FitsLong());
                a.SetTo(long.MinValue);
                Assert.IsTrue(a.FitsLong());
                a.Value = a - 1;
                Assert.IsFalse(a.FitsLong());
                a.Value = a + small;
                Assert.IsTrue(a.FitsLong());
            }
        }
#endif

        [TestMethod]
        public void FloatFitsUint()
        {
            using (var a = new HugeFloat(uint.MaxValue))
            using (var small = new HugeFloat(0.0001))
            {
                Assert.IsTrue(a.FitsUint());
                a.Value = a + 1;
                Assert.IsFalse(a.FitsUint());
                a.Value = a - small;
                Assert.IsTrue(a.FitsUint());
                a.SetTo(0);
                Assert.IsTrue(a.FitsUint());
                a.Value = a - 1;
                Assert.IsFalse(a.FitsUint());
                a.Value = a + small;
                Assert.IsTrue(a.FitsUint());
            }
        }

        [TestMethod]
        public void FloatFitsInt()
        {
            using (var a = new HugeFloat(int.MaxValue))
            using (var small = new HugeFloat(0.0001))
            {
                Assert.IsTrue(a.FitsInt());
                a.Value = a + 1;
                Assert.IsFalse(a.FitsInt());
                a.Value = a - small;
                Assert.IsTrue(a.FitsInt());
                a.SetTo(int.MinValue);
                Assert.IsTrue(a.FitsInt());
                a.Value = a - 1;
                Assert.IsFalse(a.FitsInt());
                a.Value = a + small;
                Assert.IsTrue(a.FitsInt());
            }
        }

        [TestMethod]
        public void FloatFitsUshort()
        {
            using (var a = new HugeFloat(ushort.MaxValue))
            using (var small = new HugeFloat(0.0001))
            {
                Assert.IsTrue(a.FitsUshort());
                a.Value = a + 1;
                Assert.IsFalse(a.FitsUshort());
                a.Value = a - small;
                Assert.IsTrue(a.FitsUshort());
                a.SetTo(0);
                Assert.IsTrue(a.FitsUshort());
                a.Value = a - 1;
                Assert.IsFalse(a.FitsUshort());
                a.Value = a + small;
                Assert.IsTrue(a.FitsUshort());
            }
        }

        [TestMethod]
        public void FloatFitsShort()
        {
            using (var a = new HugeFloat(short.MaxValue))
            using (var small = new HugeFloat(0.0001))
            {
                Assert.IsTrue(a.FitsShort());
                a.Value = a + 1;
                Assert.IsFalse(a.FitsShort());
                a.Value = a - small;
                Assert.IsTrue(a.FitsShort());
                a.SetTo(short.MinValue);
                Assert.IsTrue(a.FitsShort());
                a.Value = a - 1;
                Assert.IsFalse(a.FitsShort());
                a.Value = a + small;
                Assert.IsTrue(a.FitsShort());
            }
        }

        [TestMethod]
        public void FloatIsInteger()
        {
            using (var a = new HugeFloat("-233454059287409285742345.125"))
            {
                Assert.IsFalse(a.IsInteger());
                a.Value = a * 8;
                Assert.IsTrue(a.IsInteger());
            }
        }

        //private void AssertBetween(int min, int max, long actual)
        //{
        //    Assert.IsTrue(actual >= min && actual <= max, "Expected {0} to {1}, actual {2}", min, max, actual);
        //}
    }
}
