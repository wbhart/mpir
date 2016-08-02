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
    /// <summary>
    /// tests in this class verify that the correct precision is used when calculating floating point numbers
    /// </summary>
    [TestClass]
    public class Precision
    {
        [ClassInitialize]
        public static void Setup(TestContext context)
        {
            HugeFloat.DefaultPrecision = 128;
        }

        [ClassCleanup]
        public static void Cleanup()
        {
            HugeFloat.DefaultPrecision = 64;
        }

        #region Expression arithmetic

        [TestMethod]
        public void ExpressionsCalculatedToDestinationPrecision()
        {
            using (var a = new HugeFloat(1))
            using (var b = new HugeFloat(3))
            using (var c = new HugeFloat(5))
            using (var d = new HugeFloat(a / b + c))
            {
                Assert.AreEqual("0.5333333333333333333333333333333333333332@1", d.ToString());
                d.Reallocate(256);
                d.Value = a / b + c;
                Assert.AreEqual("0.533333333333333333333333333333333333333333333333333333333333333333333333333333@1", d.ToString());
            }
        }

        [TestMethod]
        public void ExpressionsCalculatedToSpecificPrecisionForEquals()
        {
            using (var a = new HugeFloat(1))
            using (var b = new HugeFloat(3))
            using (var c = HugeFloat.Allocate(256))
            using (var d = HugeFloat.Allocate(256))
            {
                c.SetTo("12345234589234059823475029384572323452034958723049823408955");
                Assert.IsTrue(c.Equals(c + a / b, 128));
                Assert.IsFalse(c.Equals(c + a / b, 256));

                d.SetTo("12345234589234059823475029384572323452034958723049823408955.333333333333333333333333333333333");
                Assert.IsTrue(d.Equals(c + a / b, 256));
                Assert.IsTrue(d.Equals(c + a / b, 128));
            }
        }

        [TestMethod]
        public void ExpressionHashCodeCalculatedToDefaultPrecision()
        {
            using (var a = new HugeFloat(1))
            using (var b = new HugeFloat(13))
            using (var c = new HugeFloat("12345234589234059823475029384572323"))
            {
                ShiftLeftBy62(c);
                var cHash = c.GetHashCode();
                var expr = a / b + c;
                Assert.AreEqual(cHash, expr.GetHashCode());
                HugeFloat.DefaultPrecision = 256;
                Assert.AreEqual(cHash, c.GetHashCode());
                Assert.AreNotEqual(cHash, expr.GetHashCode());
                HugeFloat.DefaultPrecision = 128;
            }
        }

        private static void ShiftLeftBy62(HugeFloat c)
        {
#if WIN64
            c.Value *= 0x4000000000000000L;
#else
            c.Value *= 0x80000000;
            c.Value *= 0x80000000;
#endif
        }

        [TestMethod]
        public void CompareToCalculatedToDefaultPrecision()
        {
            using (var a = new HugeFloat(1))
            using (var b = new HugeFloat(13))
            using (var c = new HugeFloat("12345234589234059823475029384572323"))
            using (var d = HugeFloat.Allocate(256))
            {
                ShiftLeftBy62(c);
                d.Value = c;
                var expr = a / b + c;
                Assert.AreEqual(0, c.CompareTo(expr)); //to precision of c
                Assert.AreEqual(0, expr.CompareTo(c)); //to precision of c
                Assert.IsFalse(expr > c);              //to precision of c
                Assert.IsTrue(c == expr);              //to precision of c
                Assert.AreEqual(0, (c + 0).CompareTo(expr)); //to default precision
                Assert.AreEqual(0, expr.CompareTo(c + 0));   //to default precision
                Assert.IsFalse(expr > c + 0);                //to default precision
                Assert.IsTrue(c + 0 == expr);                //to default precision
                HugeFloat.DefaultPrecision = 256;
                Assert.AreEqual(0, c.CompareTo(expr)); //to precision of c
                Assert.AreEqual(0, expr.CompareTo(c)); //to precision of c
                Assert.IsTrue(c == expr);              //to precision of c
                Assert.IsFalse(expr > c);              //to precision of c
                Assert.AreEqual(-1, d.CompareTo(expr)); //to precision of d
                Assert.AreEqual(1, expr.CompareTo(d));  //to precision of d
                Assert.IsFalse(d == expr);              //to precision of d
                Assert.IsTrue(expr > d);                //to precision of d
                Assert.AreEqual(-1, (c * 1).CompareTo(expr)); //to default precision
                Assert.AreEqual(1, expr.CompareTo(c + 0));    //to default precision
                Assert.IsFalse(c + 0 == expr);                //to default precision
                Assert.IsTrue(expr > c + 0);                  //to default precision
                HugeFloat.DefaultPrecision = 128;
            }
        }

        [TestMethod]
        public void CompareToPrimitiveCalculatedToDefaultPrecision()
        {
            using (var a = new HugeFloat(1))
            using (var b = new HugeFloat(13))
            using (var c = new HugeFloat("12345234589234059823475029384572323"))
            using (var d = HugeFloat.Allocate(256))
            {
                ShiftLeftBy62(c);
                d.Value = c;
                var expr = a / b + c - c;
                Assert.AreEqual(0, Math.Sign(expr.CompareTo(Platform.Si(0, 0))));
                Assert.AreEqual(0, Math.Sign(expr.CompareTo(Platform.Ui(0, 0))));
                Assert.AreEqual(0, Math.Sign(expr.CompareTo(0.0)));
                HugeFloat.DefaultPrecision = 256;
                Assert.AreEqual(1, Math.Sign(expr.CompareTo(Platform.Si(0, 0))));
                Assert.AreEqual(1, Math.Sign(expr.CompareTo(Platform.Ui(0, 0))));
                Assert.AreEqual(1, Math.Sign(expr.CompareTo(0.0)));
                HugeFloat.DefaultPrecision = 128;
            }
        }

        [TestMethod]
        public void EqualsToPrimitiveCalculatedToDefaultPrecision()
        {
            using (var a = new HugeFloat(1))
            using (var b = new HugeFloat(13))
            using (var c = new HugeFloat("12345234589234059823475029384572323"))
            using (var d = HugeFloat.Allocate(256))
            {
                ShiftLeftBy62(c);
                d.Value = c;
                var expr = a / b + c - c;
                Assert.IsTrue(expr.Equals(Platform.Si(0, 0)));
                Assert.IsTrue(expr.Equals(Platform.Ui(0, 0)));
                Assert.IsTrue(expr.Equals(0.0));
                HugeFloat.DefaultPrecision = 256;
                Assert.IsFalse(expr.Equals(Platform.Si(0, 0)));
                Assert.IsFalse(expr.Equals(Platform.Ui(0, 0)));
                Assert.IsFalse(expr.Equals(0.0));
                HugeFloat.DefaultPrecision = 128;
            }
        }

        [TestMethod]
        public void SignCalculatedToDefaultPrecision()
        {
            using (var a = new HugeFloat(1))
            using (var b = new HugeFloat(13))
            using (var c = new HugeFloat("12345234589234059823475029384572323"))
            using (var d = HugeFloat.Allocate(256))
            {
                ShiftLeftBy62(c);
                var expr = (a / b + c) - c;
                d.Value = expr;
                Assert.AreEqual(0, expr.Sign());
                Assert.AreEqual(1, d.Sign());
                HugeFloat.DefaultPrecision = 256;
                Assert.AreEqual(1, expr.Sign());
                Assert.AreEqual(1, d.Sign());
                d.Precision = 128;
                Assert.AreEqual(1, d.Sign());
                d.Value = expr;
                Assert.AreEqual(0, d.Sign());
                HugeFloat.DefaultPrecision = 128;
            }
        }

        [TestMethod]
        public void HugeIntSetToPerformedToDefaultPrecision()
        {
            using (var a = new HugeFloat(14))
            using (var b = new HugeFloat(13))
            using (var c = new HugeFloat("1234523458923405982347445029384572323"))
            using (var d = new HugeInt())
            {
                ShiftLeftBy62(c);
                ShiftLeftBy62(c);
                var expr = a / b + c - c;
                d.SetTo(expr);
                Assert.IsTrue(d == 0);

                HugeFloat.DefaultPrecision = 256;
                d.SetTo(expr);
                Assert.IsTrue(d == 1);
                HugeFloat.DefaultPrecision = 128;
            }
        }

        [TestMethod]
        public void HugeRationalSetToPerformedToDefaultPrecision()
        {
            using (var a = new HugeFloat(14))
            using (var b = new HugeFloat(13))
            using (var c = new HugeFloat("1234523458923405982347445029384572323"))
            using (var d = new HugeRational())
            {
                ShiftLeftBy62(c);
                ShiftLeftBy62(c);
                var expr = a / b + c - c;
                d.SetTo(expr);
                Assert.IsTrue(d == 0);

                HugeFloat.DefaultPrecision = 256;
                d.SetTo(expr);
                Assert.IsTrue(d > 1);
                Assert.IsTrue(d < 2);
                HugeFloat.DefaultPrecision = 128;
            }
        }

        #endregion
    }
}
