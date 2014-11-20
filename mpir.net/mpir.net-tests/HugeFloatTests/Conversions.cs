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
        [TestMethod]
        public void FloatToStringDecimal()
        {
            var n = "-0.23429384756298357462@177";
            using (var a = new HugeFloat(n))
            {
                Assert.AreEqual(n, a.ToString());
            }
        }

        [TestMethod]
        public void FloatToStringHex()
        {
            var n = "-0.23429abcdef2983574@9";
            using (var a = new HugeFloat(n, 16))
            {
                Assert.AreEqual(n, a.ToString(16, true));
                Assert.AreEqual(n.ToUpper(), a.ToString(16));
            }
        }

        [TestMethod]
        public void FloatFromUlong()
        {
            using (var a = new HugeFloat())
            {
                ulong b = 0xF84739ABCDEF4876;
                a.SetTo(b);
                Assert.AreEqual("0." + b.ToString() + "@" + b.ToString().Length, a.ToString());
            }
        }

        [TestMethod]
        public void FloatFromLong()
        {
            using (var a = new HugeFloat())
            {
                long b = -0x784739ABCDEF4876;
                a.SetTo(b);
                Assert.AreEqual("-0." + (-b).ToString() + "@" + (-b).ToString().Length, a.ToString());
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
                Assert.AreEqual("-0." + n.Substring(1) + "@" + (n.Length - 1), a.ToString(16));
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

        //private void AssertBetween(int min, int max, long actual)
        //{
        //    Assert.IsTrue(actual >= min && actual <= max, "Expected {0} to {1}, actual {2}", min, max, actual);
        //}
    }
}
