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
    public class Comparisons
    {
        #region CompareTo

        [TestMethod]
        public void FloatCompareToHugeFloat()
        {
            using (var a = new HugeFloat("-222509832503450298345029835740293845721/115756986668303657898962467957"))
            using (var b = new HugeFloat("222509832503450298345029835740293845720/115756986668303657898962467957"))
            using (var c = new HugeFloat("1/115756986668303657898962467957"))
            {
                Assert.AreEqual(1, System.Math.Sign(b.CompareTo(a)));
                Assert.AreEqual(-1, System.Math.Sign(a.CompareTo(b + 1)));
                Assert.AreEqual(0, System.Math.Sign((a + c).CompareTo(-b)));
                Assert.AreEqual(1, System.Math.Sign(a.CompareTo(null)));
            }
        }

        [TestMethod]
        public void FloatCompareToObject()
        {
            using (var a = new HugeFloat("-222509832503450298345029835740293845721/115756986668303657898962467957"))
            using (var b = new HugeFloat("222509832503450298345029835740293845720/115756986668303657898962467957"))
            using (var c = new HugeFloat("1/115756986668303657898962467957"))
            {
                Assert.AreEqual(1, System.Math.Sign(((IComparable)b).CompareTo((object)a)));
                Assert.AreEqual(-1, System.Math.Sign(((IComparable)a).CompareTo((object)b)));
                Assert.AreEqual(1, System.Math.Sign(((IComparable)a).CompareTo(null)));
                Assert.AreEqual(0, System.Math.Sign(((IComparable)(a + c)).CompareTo((object)-b)));
            }
        }

        [TestMethod]
        public void FloatCompareToExpression()
        {
            using (var a = new HugeFloat("-222509832503450298345029835740293845721/115756986668303657898962467957"))
            using (var b = new HugeFloat("222509832503450298345029835740293845720/115756986668303657898962467957"))
            using (var c = new HugeFloat("1/115756986668303657898962467957"))
            {
                Assert.AreEqual(1, System.Math.Sign(((IComparable<FloatExpression>)b).CompareTo(a)));
                Assert.AreEqual(-1, System.Math.Sign(((IComparable<FloatExpression>)a).CompareTo(b)));
                Assert.AreEqual(1, System.Math.Sign(((IComparable<FloatExpression>)a).CompareTo(null)));
                Assert.AreEqual(0, System.Math.Sign(((IComparable<FloatExpression>)(a + c)).CompareTo(-b)));
            }
        }

        [TestMethod]
        [ExpectedException(typeof(ArgumentException))]
        public void FloatCompareToNonExpression()
        {
            using (var a = new HugeFloat("-222509832503450298345029835740293845721/115756986668303657898962467957"))
            {
                a.CompareTo("abc");
            }
        }

        [TestMethod]
        public void FloatCompareToLimb()
        {
            using (var a = new HugeFloat("-222509821"))
            {
                ulong b = 222509820;
                Assert.AreEqual(-1, System.Math.Sign(a.CompareTo(b)));
                Assert.AreEqual(-1, System.Math.Sign(a.CompareTo(b + 1)));
                Assert.AreEqual(0, System.Math.Sign((-(a + 1)).CompareTo(b)));
                Assert.AreEqual(1, System.Math.Sign((-a).CompareTo(b)));
            }
        }

        [TestMethod]
        public void FloatCompareToLimb2()
        {
            using(var a = new HugeFloat("-222509947/127"))
            {
                ulong b = 222509820;
                ulong d = 127;
                Assert.AreEqual(-1, System.Math.Sign(a.CompareTo(b, d)));
                Assert.AreEqual(-1, System.Math.Sign(a.CompareTo(b + 1, d)));
                Assert.AreEqual(0, System.Math.Sign((-(a + 1)).CompareTo(b, d)));
                Assert.AreEqual(0, System.Math.Sign((-a).CompareTo(b + d, d)));
                Assert.AreEqual(1, System.Math.Sign((-a).CompareTo(b, d)));
            }
        }

        [TestMethod]
        public void FloatCompareToSignedLimb()
        {
            using (var a = new HugeFloat("-222509821"))
            {
                long b = -222509820;
                Assert.AreEqual(-1, System.Math.Sign(a.CompareTo(b)));
                Assert.AreEqual(-1, System.Math.Sign(a.CompareTo(b + 1)));
                Assert.AreEqual(0, System.Math.Sign((a + 1).CompareTo(b)));
                Assert.AreEqual(0, System.Math.Sign(a.CompareTo(b - 1)));
                Assert.AreEqual(1, System.Math.Sign(a.CompareTo(b - 2)));
            }
        }

        [TestMethod]
        public void FloatCompareToSignedLimb2()
        {
            using(var a = new HugeFloat("-222509947/127"))
            {
                long b = -222509820;
                long d = 127;
                Assert.AreEqual(-1, System.Math.Sign(a.CompareTo(b, (ulong)d)));
                Assert.AreEqual(-1, System.Math.Sign(a.CompareTo(b + 1, (ulong)d)));
                Assert.AreEqual(0, System.Math.Sign((a + 1).CompareTo(b, (ulong)d)));
                Assert.AreEqual(-1, System.Math.Sign(a.CompareTo(b - d + 1, (ulong)d)));
                Assert.AreEqual(0, System.Math.Sign(a.CompareTo(b - d, (ulong)d)));
                Assert.AreEqual(1, System.Math.Sign(a.CompareTo(b - d - 1, (ulong)d)));
            }
        }

        [TestMethod]
        public void FloatCompareToSignedDouble()
        {
            using (var a = new HugeFloat("-222509821"))
            {
                double b = -222509820;
                Assert.AreEqual(-1, System.Math.Sign(a.CompareTo(b)));
                Assert.AreEqual(-1, System.Math.Sign(a.CompareTo(b + 1)));
                Assert.AreEqual(0, System.Math.Sign((a + 1).CompareTo(b)));
                Assert.AreEqual(0, System.Math.Sign(a.CompareTo(b - 1)));
                Assert.AreEqual(1, System.Math.Sign(a.CompareTo(b - 1.1)));
            }
        }

        #endregion

        #region comparison operators with expr

        [TestMethod]
        public void FloatOperatorLessThan()
        {
            using (var a = new HugeFloat("-222509832503450298345029835740293845721/115756986668303657898962467957"))
            using (var b = new HugeFloat("222509832503450298345029835740293845720/115756986668303657898962467957"))
            using (var c = new HugeFloat())
            {
                c.Value = a;
                Assert.IsTrue(a < b);
                Assert.IsFalse(b < a);
                Assert.IsFalse(a < c);
                Assert.IsFalse(a < null);
                Assert.IsTrue(null < a);
            }
        }

        [TestMethod]
        public void FloatOperatorLessThanOrEqual()
        {
            using (var a = new HugeFloat("-222509832503450298345029835740293845721/115756986668303657898962467957"))
            using (var b = new HugeFloat("222509832503450298345029835740293845720/115756986668303657898962467957"))
            using (var c = new HugeFloat())
            {
                c.Value = a;
                Assert.IsTrue(a <= b);
                Assert.IsFalse(b <= a);
                Assert.IsTrue(a <= c);
                Assert.IsFalse(a <= null);
                Assert.IsTrue(null <= a);
            }
        }

        [TestMethod]
        public void FloatOperatorGreaterThan()
        {
            using (var a = new HugeFloat("-222509832503450298345029835740293845721/115756986668303657898962467957"))
            using (var b = new HugeFloat("222509832503450298345029835740293845720/115756986668303657898962467957"))
            using (var c = new HugeFloat())
            {
                c.Value = a;
                Assert.IsFalse(a > b);
                Assert.IsTrue(b > a);
                Assert.IsFalse(a > c);
                Assert.IsTrue(a > null);
                Assert.IsFalse(null > a);
            }
        }

        [TestMethod]
        public void FloatOperatorGreaterThanOrEqual()
        {
            using (var a = new HugeFloat("-222509832503450298345029835740293845721/115756986668303657898962467957"))
            using (var b = new HugeFloat("222509832503450298345029835740293845720/115756986668303657898962467957"))
            using (var c = new HugeFloat())
            {
                c.Value = a;
                Assert.IsFalse(a >= b);
                Assert.IsTrue(b >= a);
                Assert.IsTrue(a >= c);
                Assert.IsTrue(a >= null);
                Assert.IsFalse(null >= a);
            }
        }

        #endregion

        #region comparison operators with limb

        [TestMethod]
        public void FloatOperatorLessThanLimb()
        {
            using (var a = new HugeFloat("3845721"))
            {
                ulong c = 5432;
                ulong b = 5432349587;
                Assert.IsTrue(a < b);
                Assert.IsFalse(b < a);
                Assert.IsFalse(a < c);
                Assert.IsTrue(c < a);
            }
        }

        [TestMethod]
        public void FloatOperatorLessThanOrEqualLimb()
        {
            using (var a = new HugeFloat("3845721"))
            {
                ulong c = 5432;
                ulong b = 5432349587;
                ulong d = 3845721;
                Assert.IsTrue(a <= b);
                Assert.IsFalse(b <= a);
                Assert.IsFalse(a <= c);
                Assert.IsTrue(c <= a);
                Assert.IsTrue(a <= d);
                Assert.IsTrue(d <= a);
            }
        }

        [TestMethod]
        public void FloatOperatorGreaterThanLimb()
        {
            using (var a = new HugeFloat("3845721"))
            {
                ulong c = 5432;
                ulong b = 5432349587;
                Assert.IsFalse(a > b);
                Assert.IsTrue(b > a);
                Assert.IsTrue(a > c);
                Assert.IsFalse(c > a);
            }
        }

        [TestMethod]
        public void FloatOperatorGreaterThanOrEqualLimb()
        {
            using (var a = new HugeFloat("3845721"))
            {
                ulong c = 5432;
                ulong b = 5432349587;
                ulong d = 3845721;
                Assert.IsFalse(a >= b);
                Assert.IsTrue(b >= a);
                Assert.IsTrue(a >= c);
                Assert.IsFalse(c >= a);
                Assert.IsTrue(a >= d);
                Assert.IsTrue(d >= a);
            }
        }

        #endregion

        #region comparison operators with signed limb

        [TestMethod]
        public void FloatOperatorLessThanSignedLimb()
        {
            using (var a = new HugeFloat("-3845721"))
            {
                long c = -543254325432;
                long b = -9587;
                Assert.IsTrue(a < b);
                Assert.IsFalse(b < a);
                Assert.IsFalse(a < c);
                Assert.IsTrue(c < a);
            }
        }

        [TestMethod]
        public void FloatOperatorLessThanOrEqualSignedLimb()
        {
            using (var a = new HugeFloat("-3845721"))
            {
                long c = -543254325432;
                long b = -9587;
                long d = -3845721;
                Assert.IsTrue(a <= b);
                Assert.IsFalse(b <= a);
                Assert.IsFalse(a <= c);
                Assert.IsTrue(c <= a);
                Assert.IsTrue(a <= d);
                Assert.IsTrue(d <= a);
            }
        }

        [TestMethod]
        public void FloatOperatorGreaterThanSignedLimb()
        {
            using (var a = new HugeFloat("-3845721"))
            {
                long c = -543254325432;
                long b = -9587;
                Assert.IsFalse(a > b);
                Assert.IsTrue(b > a);
                Assert.IsTrue(a > c);
                Assert.IsFalse(c > a);
            }
        }

        [TestMethod]
        public void FloatOperatorGreaterThanOrEqualSignedLimb()
        {
            using (var a = new HugeFloat("-3845721"))
            {
                long c = -543254325432;
                long b = -9587;
                long d = -3845721;
                Assert.IsFalse(a >= b);
                Assert.IsTrue(b >= a);
                Assert.IsTrue(a >= c);
                Assert.IsFalse(c >= a);
                Assert.IsTrue(a >= d);
                Assert.IsTrue(d >= a);
            }
        }

        #endregion

        #region comparison operators with double

        [TestMethod]
        public void FloatOperatorLessThanDouble()
        {
            using (var a = new HugeFloat("-3845721"))
            {
                double c = -543254325432;
                double b = -9587;
                Assert.IsTrue(a < b);
                Assert.IsFalse(b < a);
                Assert.IsFalse(a < c);
                Assert.IsTrue(c < a);
            }
        }

        [TestMethod]
        public void FloatOperatorLessThanOrEqualDouble()
        {
            using (var a = new HugeFloat("-3845721"))
            {
                double c = -543254325432;
                double b = -9587;
                double d = -3845721;
                Assert.IsTrue(a <= b);
                Assert.IsFalse(b <= a);
                Assert.IsFalse(a <= c);
                Assert.IsTrue(c <= a);
                Assert.IsTrue(a <= d);
                Assert.IsTrue(d <= a);
            }
        }

        [TestMethod]
        public void FloatOperatorGreaterThanDouble()
        {
            using (var a = new HugeFloat("-3845721"))
            {
                double c = -543254325432;
                double b = -9587;
                Assert.IsFalse(a > b);
                Assert.IsTrue(b > a);
                Assert.IsTrue(a > c);
                Assert.IsFalse(c > a);
            }
        }

        [TestMethod]
        public void FloatOperatorGreaterThanOrEqualDouble()
        {
            using (var a = new HugeFloat("-3845721"))
            {
                double c = -543254325432;
                double b = -9587;
                double d = -3845721;
                Assert.IsFalse(a >= b);
                Assert.IsTrue(b >= a);
                Assert.IsTrue(a >= c);
                Assert.IsFalse(c >= a);
                Assert.IsTrue(a >= d);
                Assert.IsTrue(d >= a);
                Assert.IsFalse(d - 0.1 >= a);
            }
        }

        #endregion

        #region Equals

        [TestMethod]
        public void FloatEqualsHugeFloat()
        {
            using (var a = new HugeFloat("-222509832503450298345029835740293845721/115756986668303657898962467957"))
            using (var b = new HugeFloat("222509832503450298345029835740293845720/115756986668303657898962467957"))
            using (var c = new HugeFloat("1/115756986668303657898962467957"))
            {
                Assert.IsFalse(b.Equals(a));
                Assert.IsFalse(a.Equals(b + c));
                Assert.IsTrue((a + c).Equals(-b));
                Assert.IsFalse(a.Equals(null));
                Assert.IsTrue(Equals(a + c, -b));
            }
        }

        [TestMethod]
        public void FloatEqualsExpression()
        {
            using (var a = new HugeFloat("-222509832503450298345029835740293845721/115756986668303657898962467957"))
            using (var b = new HugeFloat("222509832503450298345029835740293845720/115756986668303657898962467957"))
            using (var c = new HugeFloat("1/115756986668303657898962467957"))
            {
                Assert.IsFalse(((IEquatable<FloatExpression>)b).Equals(a));
                Assert.IsFalse(((IEquatable<FloatExpression>)a).Equals(b));
                Assert.IsFalse(((IEquatable<FloatExpression>)a).Equals(null));
                Assert.IsTrue(((IEquatable<FloatExpression>)(a + c)).Equals(-b));
            }
        }

        [TestMethod]
        public void FloatEqualsNonExpression()
        {
            using (var a = new HugeFloat("-222509832503450298345029835740293845721/115756986668303657898962467957"))
            {
                Assert.IsFalse(a.Equals("abc"));
            }
        }

        [TestMethod]
        public void FloatEqualsLimb()
        {
            using (var a = new HugeFloat("222509832503"))
            {
                ulong b = 222509832504;
                Assert.IsFalse(a.Equals(b + 1));
                Assert.IsTrue(a.Equals(b - 1));
                Assert.IsTrue((a + 1).Equals(b));
            }
        }

        [TestMethod]
        public void FloatEqualsSignedLimb()
        {
            using (var a = new HugeFloat("-222509832505"))
            {
                long b = -222509832504;
                Assert.IsFalse(a.Equals(b + 1));
                Assert.IsTrue(a.Equals(b - 1));
                Assert.IsTrue((a + 1).Equals(b));
            }
        }

        [TestMethod]
        public void FloatEqualsLimb2()
        {
            using(var a = new HugeFloat("222509832377/127"))
            {
                ulong b = 222509832504;
                ulong d = 127;
                Assert.IsFalse(a.Equals(b + 1, d));
                Assert.IsTrue(a.Equals(b - d, d));
                Assert.IsTrue((a + 1).Equals(b, d));
            }
        }

        [TestMethod]
        public void FloatEqualsSignedLimb2()
        {
            using(var a = new HugeFloat("-222509832377/127"))
            {
                long b = -222509832504;
                ulong d = 127;
                Assert.IsFalse(a.Equals(b + 1, d));
                Assert.IsTrue(a.Equals(b + (long)d, d));
                Assert.IsTrue((a - 1).Equals(b, d));
            }
        }

        [TestMethod]
        public void FloatEqualsDouble()
        {
            using (var a = new HugeFloat("-222509832505"))
            {
                double b = -222509832504;
                Assert.IsFalse(a.Equals(b + 1));
                Assert.IsTrue(a.Equals(b - 1));
                Assert.IsTrue((a + 1).Equals(b));
                Assert.IsFalse((a + 1).Equals(b + 0.1));
            }
        }

        #endregion

        #region Equality operators with expr

        [TestMethod]
        public void FloatEqualsOperatorHugeFloat()
        {
            using (var a = new HugeFloat("-222509832503450298345029835740293845721/115756986668303657898962467957"))
            using (var b = new HugeFloat("222509832503450298345029835740293845720/115756986668303657898962467957"))
            using (var c = new HugeFloat("1/115756986668303657898962467957"))
            {
                Assert.IsFalse(b == a);
                Assert.IsFalse(a == b + c);
                Assert.IsTrue(a + c == -b);
                Assert.IsFalse(a == null);
            }
        }

        [TestMethod]
        public void FloatNotEqualOperatorHugeFloat()
        {
            using (var a = new HugeFloat("-222509832503450298345029835740293845721/115756986668303657898962467957"))
            using (var b = new HugeFloat("222509832503450298345029835740293845720/115756986668303657898962467957"))
            using (var c = new HugeFloat("1/115756986668303657898962467957"))
            {
                Assert.IsTrue(b != a);
                Assert.IsTrue(a != b + c);
                Assert.IsFalse(a + c != -b);
                Assert.IsTrue(a != null);
            }
        }

        #endregion

        #region Equality operators with Limb

        [TestMethod]
        public void FloatEqualsOperatorLimb()
        {
            using (var a = new HugeFloat("-835740293845721"))
            {
                ulong b = 835740293845720;
                Assert.IsFalse(b == a);
                Assert.IsFalse(a == b + 1);
                Assert.IsTrue(-(a + 1) == b);
            }
        }

        [TestMethod]
        public void FloatNotEqualOperatorLimb()
        {
            using (var a = new HugeFloat("-835740293845721"))
            {
                ulong b = 835740293845720;
                Assert.IsTrue(b != a);
                Assert.IsTrue(a != b + 1);
                Assert.IsFalse(-(a + 1) != b);
            }
        }

        #endregion
        
        #region Equality operators with Signed Limb

        [TestMethod]
        public void FloatEqualsOperatorSignedLimb()
        {
            using (var a = new HugeFloat("-835740293845721"))
            {
                long b = -835740293845720;
                Assert.IsFalse(b == a);
                Assert.IsFalse(a == b + 1);
                Assert.IsTrue(a + 1 == b);
            }
        }

        [TestMethod]
        public void FloatNotEqualOperatorSignedLimb()
        {
            using (var a = new HugeFloat("-835740293845721"))
            {
                long b = -835740293845720;
                Assert.IsTrue(b != a);
                Assert.IsTrue(a != b + 1);
                Assert.IsFalse(a + 1 != b);
            }
        }

        #endregion

        #region Equality operators with Double

        [TestMethod]
        public void FloatEqualsOperatorDouble()
        {
            using (var a = new HugeFloat("-835740293845721"))
            {
                double b = -835740293845720;
                Assert.IsFalse(b == a);
                Assert.IsFalse(a == b + 1);
                Assert.IsTrue(a + 1 == b);
                Assert.IsFalse(a + 1 == b + 0.1);
            }
        }

        [TestMethod]
        public void FloatNotEqualOperatorDouble()
        {
            using (var a = new HugeFloat("-835740293845721"))
            {
                double b = -835740293845720;
                Assert.IsTrue(b != a);
                Assert.IsTrue(a != b + 1);
                Assert.IsFalse(a + 1 != b);
                Assert.IsTrue(a + 1 != b + 0.1);
            }
        }

        #endregion

        #region GetHashCode

        [TestMethod]
        public void FloatGetHashCodeTest()
        {
            using (var a = new HugeFloat("-222509832503450298345029835740293845721/115756986668303657898962467957"))
            {
                Assert.AreNotEqual(0, a.GetHashCode());
                Assert.AreEqual(a.GetHashCode(), (a + 0).GetHashCode());
                Assert.AreNotEqual(a.GetHashCode(), (-a).GetHashCode());
            }
        }

        #endregion

        #region Sign

        [TestMethod]
        public void FloatSign()
        {
            using (var a = new HugeFloat("-222509832503450298345029835740293845721/115756986668303657898962467957"))
            {
                Assert.AreEqual(-1, a.Sign());
                Assert.AreEqual(1, (-a).Sign());
                Assert.AreEqual(0, (a-a).Sign());
            }
        }

        #endregion

        //more tests coming here
    }
}
