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

namespace MPIR.Tests.HugeRationalTests
{
    [TestClass]
    public class Conversions
    {
        [TestMethod]
        public void RationalToStringDecimal()
        {
            var n = "-23429384756298357462983476598345623984756";
            var d = "115756986668303657898962467957";
            var s = n + "/" + d;
            using (var a = new HugeRational(s))
            {
                Assert.AreEqual(s, a.ToString());
            }
        }

        [TestMethod]
        public void RationalToStringHex()
        {
            var n = "-23429abcdef298357462983fedcba345623984756";
            var d = "17607ef654eb9a13ffa163c75";
            var s = n + "/" + d;
            using (var a = new HugeRational(s, 16))
            {
                Assert.AreEqual(s, a.ToString(16, true));
                Assert.AreEqual(s.ToUpper(), a.ToString(16));
            }
        }

        [TestMethod]
        public void RationalFromUlong()
        {
            using (var a = new HugeRational())
            {
                var b = Platform.Ui(0xF84739ABCDEF4876, 0xF84776);
                var d = Platform.Ui(12764787846358441471, 3628273133);
                a.SetTo(b, d);
                Assert.AreEqual(b.ToString() + "/" + d.ToString(), a.ToString());

                a.SetTo(b);
                Assert.AreEqual(b.ToString() + "/1", a.ToString());
            }
        }

        [TestMethod]
        public void RationalFromLong()
        {
            using (var a = new HugeRational())
            {
                var b = Platform.Si(-0x784739ABCDEF4876, -0xF84776);
                var d = Platform.Ui(12764787846358441471, 3628273133);
                a.SetTo(b, d);
                Assert.AreEqual(b.ToString() + "/" + d.ToString(), a.ToString());

                a.SetTo(b);
                Assert.AreEqual(b.ToString() + "/1", a.ToString());
            }
        }

        [TestMethod]
        public void RationalToAndFromDouble()
        {
            using (var a = new HugeRational())
            {
                a.SetTo(-123.25);

                double c = a.ToDouble();
                Assert.IsTrue(c.Equals(-123.25));
                Assert.IsTrue(a.Equals(-123.25));
            }
        }

        [TestMethod]
        public void RationalToAndFromFloat()
        {
            using (var a = new HugeRational())
            {
                a.SetTo(-123.125f);

                double c = a.ToDouble();
                Assert.IsTrue(-123.125 == c);
                Assert.IsTrue(-123.125 == a);
                Assert.IsTrue(-123.125f == a);
            }
        }

        [TestMethod]
        public void RationalFromString()
        {
            using (var a = new HugeRational())
            {
                var n = "98762934876529834765234123984761/115756986668303657898962467957";
                a.SetTo(n);
                Assert.AreEqual(n, a.ToString());

                n = "-98ABCDEF876529834765234123984761/17607EF654EB9A13FFA163C75";
                a.SetTo(n, 16);
                Assert.AreEqual(n, a.ToString(16));

                n = "-98ABCDEF876529834765234123984761";
                a.SetTo(n, 16);
                Assert.AreEqual(n + "/1", a.ToString(16));
            }
        }

        [TestMethod]
        [ExpectedException(typeof(ArgumentException))]
        public void RationalFromInvalidString()
        {
            using (var a = new HugeRational())
            {
                a.SetTo("12345A");
            }
        }

        [TestMethod]
        [ExpectedException(typeof(ArgumentException))]
        public void RationalFromInvalidString2()
        {
            using (var a = new HugeRational())
            {
                a.SetTo("12345/13345A");
            }
        }

        [TestMethod]
        public void RationalToStringTruncated()
        {
            using (var d = new HugeInt("115756986668303657898962467957"))
            using (var e = new HugeInt(d ^ 10))
            {
                var s = e.ToString(10);
                var n = string.Concat("123456789".Select(c => new string(c, 30)));
                using (var a = new HugeRational(n + "/" + s))
                {
                    Assert.AreEqual(n + "/" + s, a.ToString(10));
                    Assert.AreEqual("..." + n.Substring(n.Length - 256) + "/..." + s.Substring(s.Length - 256), a.ToString());
                    a.Value = -a;
                    Assert.AreEqual("-..." + n.Substring(n.Length - 256) + "/..." + s.Substring(s.Length - 256), a.ToString());
                }
            }
        }

        [TestMethod]
        public void RationalApproximateSizeInBase()
        {
            using (var a = new HugeRational("2983475029834750293429834750298347502934298347502983475029342983475029834750293429834750298347502934/115756986668303657898962467957"))
            {
                AssertBetween(131, 133, a.ApproximateSizeInBase(10));
                AssertBetween(429, 429, a.ApproximateSizeInBase(2));
                AssertBetween(109, 109, a.ApproximateSizeInBase(16));
                AssertBetween(84, 86, a.ApproximateSizeInBase(36));
                AssertBetween(74, 76, a.ApproximateSizeInBase(62));
            }
        }

        private void AssertBetween(int min, int max, long actual)
        {
            Assert.IsTrue(actual >= min && actual <= max, "Expected {0} to {1}, actual {2}", min, max, actual);
        }
    }
}
