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
    public class Comparisons
    {
        #region CompareTo

        [TestMethod]
        public void RationalCompareToHugeRational()
        {
            using (var a = new HugeRational("-222509832503450298345029835740293845721"))
            using (var b = new HugeRational("222509832503450298345029835740293845720"))
            {
                Assert.AreEqual(1, System.Math.Sign(b.CompareTo(a)));
                Assert.AreEqual(-1, System.Math.Sign(a.CompareTo(b + 1)));
                Assert.AreEqual(0, System.Math.Sign((a + 1).CompareTo(-b)));
                Assert.AreEqual(1, System.Math.Sign(a.CompareTo(null)));
            }
        }

        [TestMethod]
        public void RationalCompareToObject()
        {
            using (var a = new HugeRational("-222509832503450298345029835740293845721"))
            using (var b = new HugeRational("222509832503450298345029835740293845720"))
            {
                Assert.AreEqual(1, System.Math.Sign(((IComparable)b).CompareTo((object)a)));
                Assert.AreEqual(-1, System.Math.Sign(((IComparable)a).CompareTo((object)b)));
                Assert.AreEqual(1, System.Math.Sign(((IComparable)a).CompareTo(null)));
                Assert.AreEqual(0, System.Math.Sign(((IComparable)(a + 1)).CompareTo((object)-b)));
            }
        }

        [TestMethod]
        public void RationalCompareToExpression()
        {
            using (var a = new HugeRational("-222509832503450298345029835740293845721"))
            using (var b = new HugeRational("222509832503450298345029835740293845720"))
            {
                Assert.AreEqual(1, System.Math.Sign(((IComparable<RationalExpression>)b).CompareTo(a)));
                Assert.AreEqual(-1, System.Math.Sign(((IComparable<RationalExpression>)a).CompareTo(b)));
                Assert.AreEqual(1, System.Math.Sign(((IComparable<RationalExpression>)a).CompareTo(null)));
                Assert.AreEqual(0, System.Math.Sign(((IComparable<RationalExpression>)(a + 1)).CompareTo(-b)));
            }
        }

        [TestMethod]
        [ExpectedException(typeof(ArgumentException))]
        public void RationalCompareToNonExpression()
        {
            using (var a = new HugeRational("-222509832503450298345029835740293845721"))
            {
                a.CompareTo("abc");
            }
        }

        [TestMethod]
        public void RationalCompareToLimb()
        {
            using (var a = new HugeRational("-222509821"))
            {
                ulong b = 222509820;
                Assert.AreEqual(-1, System.Math.Sign(a.CompareTo(b)));
                Assert.AreEqual(-1, System.Math.Sign(a.CompareTo(b + 1)));
                Assert.AreEqual(0, System.Math.Sign((-(a + 1)).CompareTo(b)));
                Assert.AreEqual(1, System.Math.Sign((-a).CompareTo(b)));
            }
        }

        [TestMethod]
        public void RationalCompareToSignedLimb()
        {
            using (var a = new HugeRational("-222509821"))
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
        public void RationalCompareToSignedDouble()
        {
            using (var a = new HugeRational("-222509821"))
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
        public void RationalOperatorLessThan()
        {
            using (var a = new HugeRational("-222509832503450298345029835740293845721"))
            using (var b = new HugeRational("222509832503450298345029835740293845720"))
            using (var c = new HugeRational())
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
        public void RationalOperatorLessThanOrEqual()
        {
            using (var a = new HugeRational("-222509832503450298345029835740293845721"))
            using (var b = new HugeRational("222509832503450298345029835740293845720"))
            using (var c = new HugeRational())
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
        public void RationalOperatorGreaterThan()
        {
            using (var a = new HugeRational("-222509832503450298345029835740293845721"))
            using (var b = new HugeRational("222509832503450298345029835740293845720"))
            using (var c = new HugeRational())
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
        public void RationalOperatorGreaterThanOrEqual()
        {
            using (var a = new HugeRational("-222509832503450298345029835740293845721"))
            using (var b = new HugeRational("222509832503450298345029835740293845720"))
            using (var c = new HugeRational())
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
        public void RationalOperatorLessThanLimb()
        {
            using (var a = new HugeRational("3845721"))
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
        public void RationalOperatorLessThanOrEqualLimb()
        {
            using (var a = new HugeRational("3845721"))
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
        public void RationalOperatorGreaterThanLimb()
        {
            using (var a = new HugeRational("3845721"))
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
        public void RationalOperatorGreaterThanOrEqualLimb()
        {
            using (var a = new HugeRational("3845721"))
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
        public void RationalOperatorLessThanSignedLimb()
        {
            using (var a = new HugeRational("-3845721"))
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
        public void RationalOperatorLessThanOrEqualSignedLimb()
        {
            using (var a = new HugeRational("-3845721"))
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
        public void RationalOperatorGreaterThanSignedLimb()
        {
            using (var a = new HugeRational("-3845721"))
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
        public void RationalOperatorGreaterThanOrEqualSignedLimb()
        {
            using (var a = new HugeRational("-3845721"))
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
        public void RationalOperatorLessThanDouble()
        {
            using (var a = new HugeRational("-3845721"))
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
        public void RationalOperatorLessThanOrEqualDouble()
        {
            using (var a = new HugeRational("-3845721"))
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
        public void RationalOperatorGreaterThanDouble()
        {
            using (var a = new HugeRational("-3845721"))
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
        public void RationalOperatorGreaterThanOrEqualDouble()
        {
            using (var a = new HugeRational("-3845721"))
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
        public void RationalEqualsHugeRational()
        {
            using (var a = new HugeRational("-222509832503450298345029835740293845721"))
            using (var b = new HugeRational("222509832503450298345029835740293845720"))
            {
                Assert.IsFalse(b.Equals(a));
                Assert.IsFalse(a.Equals(b + 1));
                Assert.IsTrue((a + 1).Equals(-b));
                Assert.IsFalse(a.Equals(null));
                Assert.IsTrue(Equals(a + 1, -b));
            }
        }

        [TestMethod]
        public void RationalEqualsExpression()
        {
            using (var a = new HugeRational("-222509832503450298345029835740293845721"))
            using (var b = new HugeRational("222509832503450298345029835740293845720"))
            {
                Assert.IsFalse(((IEquatable<RationalExpression>)b).Equals(a));
                Assert.IsFalse(((IEquatable<RationalExpression>)a).Equals(b));
                Assert.IsFalse(((IEquatable<RationalExpression>)a).Equals(null));
                Assert.IsTrue(((IEquatable<RationalExpression>)(a + 1)).Equals(-b));
            }
        }

        [TestMethod]
        public void RationalEqualsNonExpression()
        {
            using (var a = new HugeRational("-222509832503450298345029835740293845721"))
            {
                Assert.IsFalse(a.Equals("abc"));
            }
        }

        [TestMethod]
        public void RationalEqualsLimb()
        {
            using (var a = new HugeRational("222509832503"))
            {
                ulong b = 222509832504;
                Assert.IsFalse(a.Equals(b + 1));
                Assert.IsTrue(a.Equals(b - 1));
                Assert.IsTrue((a + 1).Equals(b));
            }
        }

        [TestMethod]
        public void RationalEqualsSignedLimb()
        {
            using (var a = new HugeRational("-222509832505"))
            {
                long b = -222509832504;
                Assert.IsFalse(a.Equals(b + 1));
                Assert.IsTrue(a.Equals(b - 1));
                Assert.IsTrue((a + 1).Equals(b));
            }
        }

        [TestMethod]
        public void RationalEqualsDouble()
        {
            using (var a = new HugeRational("-222509832505"))
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
        public void RationalEqualsOperatorHugeRational()
        {
            using (var a = new HugeRational("-222509832503450298345029835740293845721"))
            using (var b = new HugeRational("222509832503450298345029835740293845720"))
            {
                Assert.IsFalse(b == a);
                Assert.IsFalse(a == b + 1);
                Assert.IsTrue(a + 1 == -b);
                Assert.IsFalse(a == null);
            }
        }

        [TestMethod]
        public void RationalNotEqualOperatorHugeRational()
        {
            using (var a = new HugeRational("-222509832503450298345029835740293845721"))
            using (var b = new HugeRational("222509832503450298345029835740293845720"))
            {
                Assert.IsTrue(b != a);
                Assert.IsTrue(a != b + 1);
                Assert.IsFalse(a + 1 != -b);
                Assert.IsTrue(a != null);
            }
        }

        #endregion

        #region Equality operators with Limb

        [TestMethod]
        public void RationalEqualsOperatorLimb()
        {
            using (var a = new HugeRational("-835740293845721"))
            {
                ulong b = 835740293845720;
                Assert.IsFalse(b == a);
                Assert.IsFalse(a == b + 1);
                Assert.IsTrue(-(a + 1) == b);
            }
        }

        [TestMethod]
        public void RationalNotEqualOperatorLimb()
        {
            using (var a = new HugeRational("-835740293845721"))
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
        public void RationalEqualsOperatorSignedLimb()
        {
            using (var a = new HugeRational("-835740293845721"))
            {
                long b = -835740293845720;
                Assert.IsFalse(b == a);
                Assert.IsFalse(a == b + 1);
                Assert.IsTrue(a + 1 == b);
            }
        }

        [TestMethod]
        public void RationalNotEqualOperatorSignedLimb()
        {
            using (var a = new HugeRational("-835740293845721"))
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
        public void RationalEqualsOperatorDouble()
        {
            using (var a = new HugeRational("-835740293845721"))
            {
                double b = -835740293845720;
                Assert.IsFalse(b == a);
                Assert.IsFalse(a == b + 1);
                Assert.IsTrue(a + 1 == b);
                Assert.IsFalse(a + 1 == b + 0.1);
            }
        }

        [TestMethod]
        public void RationalNotEqualOperatorDouble()
        {
            using (var a = new HugeRational("-835740293845721"))
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
        public void RationalGetHashCodeTest()
        {
            using (var a = new HugeRational("-222509832503450298345029835740293845721"))
            {
                Assert.AreNotEqual(0, a.GetHashCode());
                Assert.AreEqual(a.GetHashCode(), (a + 0).GetHashCode());
                Assert.AreNotEqual(a.GetHashCode(), (-a).GetHashCode());
            }
        }

        #endregion

        #region Sign

        [TestMethod]
        public void RationalSign()
        {
            using (var a = new HugeRational("-222509832503450298345029835740293845721"))
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