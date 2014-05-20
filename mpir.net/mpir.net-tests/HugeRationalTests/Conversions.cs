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
            using (var a = new HugeRational(n))
            {
                Assert.AreEqual(n, a.ToString());
            }
        }

        [TestMethod]
        public void RationalToStringHex()
        {
            var n = "-23429abcdef298357462983fedcba345623984756";
            using (var a = new HugeRational(n, 16))
            {
                Assert.AreEqual(n, a.ToString(16, true));
                Assert.AreEqual(n.ToUpper(), a.ToString(16));
            }
        }

        [TestMethod]
        public void RationalFromUlong()
        {
            using (var a = new HugeRational())
            {
                ulong b = 0xF84739ABCDEF4876;
                ulong d = 12764787846358441471;
                a.SetTo(b, d);
                Assert.AreEqual(b.ToString() + "/" + d.ToString(), a.ToString());
            }
        }

        [TestMethod]
        public void RationalFromLong()
        {
            using (var a = new HugeRational())
            {
                long b = -0x784739ABCDEF4876;
                ulong d = 12764787846358441471;
                a.SetTo(b, d);
                Assert.AreEqual(b.ToString() + "/" + d.ToString(), a.ToString());
            }
        }

        [TestMethod]
        public void RationalToAndFromDouble()
        {
            using (var a = new HugeRational())
            using (var lo = new HugeRational())
            using (var hi = new HugeRational())
            {
                a.SetTo(-123.45e20);

                double c = a.ToDouble();
                Assert.AreEqual(-123.45e20, c);
                Assert.AreEqual(-123.45e20, a);
            }
        }

        [TestMethod]
        public void RationalToAndFromFloat()
        {
            using (var a = new HugeRational())
            using (var lo = new HugeRational())
            using (var hi = new HugeRational())
            {
                a.SetTo(-123.45e20f);

                double c = a.ToDouble();
                Assert.AreEqual(-123.45e20, c);
                Assert.AreEqual(-123.45e20, a);
                Assert.AreEqual(-123.45e20f, a);
            }
        }

        [TestMethod]
        public void RationalFromString()
        {
            using (var a = new HugeRational())
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
        public void RationalFromInvalidString()
        {
            using (var a = new HugeRational())
            {
                a.SetTo("12345A");
            }
        }

        [TestMethod]
        public void RationalToStringTruncated()
        {
            var n = string.Concat("123456789".Select(c => new string(c, 30)));
            using (var a = new HugeRational(n))
            {
                Assert.AreEqual(n, a.ToString(10));
                Assert.AreEqual("..." + n.Substring(n.Length - 256), a.ToString());
                a.Value = -a;
                Assert.AreEqual("-..." + n.Substring(n.Length - 256), a.ToString());
            }
        }

        [TestMethod]
        public void RationalApproximateSizeInBase()
        {
            using (var a = new HugeRational("2983475029834750293429834750298347502934298347502983475029342983475029834750293429834750298347502934"))
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
