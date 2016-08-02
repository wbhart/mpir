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

namespace MPIR.Tests.HugeIntTests
{
    [TestClass]
    public class Comparisons
    {
        #region CompareTo

        [TestMethod]
        public void IntCompareToHugeInt()
        {
            using (var a = new HugeInt("-222509832503450298345029835740293845721"))
            using (var b = new HugeInt("222509832503450298345029835740293845720"))
            {
                Assert.AreEqual(1, System.Math.Sign(b.CompareTo(a)));
                Assert.AreEqual(-1, System.Math.Sign(a.CompareTo(b + 1)));
                Assert.AreEqual(0, System.Math.Sign((a + 1).CompareTo(-b)));
                Assert.AreEqual(1, System.Math.Sign(a.CompareTo(null)));
            }
        }

        [TestMethod]
        public void IntCompareToObject()
        {
            using (var a = new HugeInt("-222509832503450298345029835740293845721"))
            using (var b = new HugeInt("222509832503450298345029835740293845720"))
            {
                Assert.AreEqual(1, System.Math.Sign(((IComparable)b).CompareTo((object)a)));
                Assert.AreEqual(-1, System.Math.Sign(((IComparable)a).CompareTo((object)b)));
                Assert.AreEqual(1, System.Math.Sign(((IComparable)a).CompareTo(null)));
                Assert.AreEqual(0, System.Math.Sign(((IComparable)(a + 1)).CompareTo((object)-b)));
            }
        }

        [TestMethod]
        public void IntCompareToExpression()
        {
            using (var a = new HugeInt("-222509832503450298345029835740293845721"))
            using (var b = new HugeInt("222509832503450298345029835740293845720"))
            {
                Assert.AreEqual(1, System.Math.Sign(((IComparable<IntegerExpression>)b).CompareTo(a)));
                Assert.AreEqual(-1, System.Math.Sign(((IComparable<IntegerExpression>)a).CompareTo(b)));
                Assert.AreEqual(1, System.Math.Sign(((IComparable<IntegerExpression>)a).CompareTo(null)));
                Assert.AreEqual(0, System.Math.Sign(((IComparable<IntegerExpression>)(a + 1)).CompareTo(-b)));
            }
        }

        [TestMethod]
        [ExpectedException(typeof(ArgumentException))]
        public void IntCompareToNonExpression()
        {
            using (var a = new HugeInt("-222509832503450298345029835740293845721"))
            {
                a.CompareTo("abc");
            }
        }

        [TestMethod]
        public void IntCompareToLimb()
        {
            using (var a = new HugeInt("-222509821"))
            {
                var b = Platform.Ui(222509820, 222509820);
                Assert.AreEqual(-1, System.Math.Sign(a.CompareTo(b)));
                Assert.AreEqual(-1, System.Math.Sign(a.CompareTo(b + 1)));
                Assert.AreEqual(0, System.Math.Sign((-(a + 1)).CompareTo(b)));
                Assert.AreEqual(1, System.Math.Sign((-a).CompareTo(b)));
            }
        }

        [TestMethod]
        public void IntCompareToSignedLimb()
        {
            using (var a = new HugeInt("-222509821"))
            {
                var b = Platform.Si(-222509820, -222509820);
                Assert.AreEqual(-1, System.Math.Sign(a.CompareTo(b)));
                Assert.AreEqual(-1, System.Math.Sign(a.CompareTo(b + 1)));
                Assert.AreEqual(0, System.Math.Sign((a + 1).CompareTo(b)));
                Assert.AreEqual(0, System.Math.Sign(a.CompareTo(b - 1)));
                Assert.AreEqual(1, System.Math.Sign(a.CompareTo(b - 2)));
            }
        }

        [TestMethod]
        public void IntCompareToSignedDouble()
        {
            using (var a = new HugeInt("-222509821"))
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
        public void IntOperatorLessThan()
        {
            using (var a = new HugeInt("-222509832503450298345029835740293845721"))
            using (var b = new HugeInt("222509832503450298345029835740293845720"))
            using (var c = new HugeInt())
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
        public void IntOperatorLessThanOrEqual()
        {
            using (var a = new HugeInt("-222509832503450298345029835740293845721"))
            using (var b = new HugeInt("222509832503450298345029835740293845720"))
            using (var c = new HugeInt())
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
        public void IntOperatorGreaterThan()
        {
            using (var a = new HugeInt("-222509832503450298345029835740293845721"))
            using (var b = new HugeInt("222509832503450298345029835740293845720"))
            using (var c = new HugeInt())
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
        public void IntOperatorGreaterThanOrEqual()
        {
            using (var a = new HugeInt("-222509832503450298345029835740293845721"))
            using (var b = new HugeInt("222509832503450298345029835740293845720"))
            using (var c = new HugeInt())
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
        public void IntOperatorLessThanLimb()
        {
            using (var a = new HugeInt("3845721"))
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
        public void IntOperatorLessThanOrEqualLimb()
        {
            using (var a = new HugeInt("3845721"))
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
        public void IntOperatorGreaterThanLimb()
        {
            using (var a = new HugeInt("3845721"))
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
        public void IntOperatorGreaterThanOrEqualLimb()
        {
            using (var a = new HugeInt("3845721"))
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
        public void IntOperatorLessThanSignedLimb()
        {
            using (var a = new HugeInt("-3845721"))
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
        public void IntOperatorLessThanOrEqualSignedLimb()
        {
            using (var a = new HugeInt("-3845721"))
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
        public void IntOperatorGreaterThanSignedLimb()
        {
            using (var a = new HugeInt("-3845721"))
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
        public void IntOperatorGreaterThanOrEqualSignedLimb()
        {
            using (var a = new HugeInt("-3845721"))
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
        public void IntOperatorLessThanDouble()
        {
            using (var a = new HugeInt("-3845721"))
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
        public void IntOperatorLessThanOrEqualDouble()
        {
            using (var a = new HugeInt("-3845721"))
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
        public void IntOperatorGreaterThanDouble()
        {
            using (var a = new HugeInt("-3845721"))
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
        public void IntOperatorGreaterThanOrEqualDouble()
        {
            using (var a = new HugeInt("-3845721"))
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
        public void IntEqualsHugeInt()
        {
            using (var a = new HugeInt("-222509832503450298345029835740293845721"))
            using (var b = new HugeInt("222509832503450298345029835740293845720"))
            {
                Assert.IsFalse(b.Equals(a));
                Assert.IsFalse(a.Equals(b + 1));
                Assert.IsTrue((a + 1).Equals(-b));
                Assert.IsFalse(a.Equals(null));
                Assert.IsTrue(Equals(a + 1, -b));
            }
        }

        [TestMethod]
        public void IntEqualsExpression()
        {
            using (var a = new HugeInt("-222509832503450298345029835740293845721"))
            using (var b = new HugeInt("222509832503450298345029835740293845720"))
            {
                Assert.IsFalse(((IEquatable<IntegerExpression>)b).Equals(a));
                Assert.IsFalse(((IEquatable<IntegerExpression>)a).Equals(b));
                Assert.IsFalse(((IEquatable<IntegerExpression>)a).Equals(null));
                Assert.IsTrue(((IEquatable<IntegerExpression>)(a + 1)).Equals(-b));
            }
        }

        [TestMethod]
        public void IntEqualsNonExpression()
        {
            using (var a = new HugeInt("-222509832503450298345029835740293845721"))
            {
                Assert.IsFalse(a.Equals("abc"));
            }
        }

        [TestMethod]
        public void IntEqualsLimb()
        {
            var source = Platform.Select("222509832503", "2225032503");
            var b = Platform.Ui(222509832504, 2225032504);

            using (var a = new HugeInt(source))
            {
                Assert.IsFalse(a.Equals(b + 1));
                Assert.IsTrue(a.Equals(b - 1));
                Assert.IsTrue((a + 1).Equals(b));
            }
        }

        [TestMethod]
        public void IntEqualsSignedLimb()
        {
            var source = Platform.Select("-222509832505", "-225032505");
            var b = Platform.Si(-222509832504, -225032504);

            using (var a = new HugeInt(source))
            {
                Assert.IsFalse(a.Equals(b + 1));
                Assert.IsTrue(a.Equals(b - 1));
                Assert.IsTrue((a + 1).Equals(b));
            }
        }

        [TestMethod]
        public void IntEqualsDouble()
        {
            using (var a = new HugeInt("-222509832505"))
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
        public void IntEqualsOperatorHugeInt()
        {
            using (var a = new HugeInt("-222509832503450298345029835740293845721"))
            using (var b = new HugeInt("222509832503450298345029835740293845720"))
            {
                Assert.IsFalse(b == a);
                Assert.IsFalse(a == b + 1);
                Assert.IsTrue(a + 1 == -b);
                Assert.IsFalse(a == null);
            }
        }

        [TestMethod]
        public void IntNotEqualOperatorHugeInt()
        {
            using (var a = new HugeInt("-222509832503450298345029835740293845721"))
            using (var b = new HugeInt("222509832503450298345029835740293845720"))
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
        public void IntEqualsOperatorLimb()
        {
            using (var a = new HugeInt("-835740293845721"))
            {
                ulong b = 835740293845720;
                Assert.IsFalse(b == a);
                Assert.IsFalse(a == b + 1);
                Assert.IsTrue(-(a + 1) == b);
            }
        }

        [TestMethod]
        public void IntNotEqualOperatorLimb()
        {
            using (var a = new HugeInt("-835740293845721"))
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
        public void IntEqualsOperatorSignedLimb()
        {
            using (var a = new HugeInt("-835740293845721"))
            {
                long b = -835740293845720;
                Assert.IsFalse(b == a);
                Assert.IsFalse(a == b + 1);
                Assert.IsTrue(a + 1 == b);
            }
        }

        [TestMethod]
        public void IntNotEqualOperatorSignedLimb()
        {
            using (var a = new HugeInt("-835740293845721"))
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
        public void IntEqualsOperatorDouble()
        {
            using (var a = new HugeInt("-835740293845721"))
            {
                double b = -835740293845720;
                Assert.IsFalse(b == a);
                Assert.IsFalse(a == b + 1);
                Assert.IsTrue(a + 1 == b);
                Assert.IsFalse(a + 1 == b + 0.1);
            }
        }

        [TestMethod]
        public void IntNotEqualOperatorDouble()
        {
            using (var a = new HugeInt("-835740293845721"))
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
        public void IntGetHashCodeTest()
        {
            using (var a = new HugeInt("-222509832503450298345029835740293845721"))
            {
                Assert.AreNotEqual(0, a.GetHashCode());
                Assert.AreEqual(a.GetHashCode(), (a + 0).GetHashCode());
                Assert.AreNotEqual(a.GetHashCode(), (-a).GetHashCode());
            }
        }

        #endregion

        #region CompareAbsTo

        [TestMethod]
        public void IntCompareAbsToHugeInt()
        {
            using (var a = new HugeInt("-222509832503450298345029835740293845721"))
            using (var b = new HugeInt("222509832503450298345029835740293845720"))
            {
                Assert.AreEqual(1, System.Math.Sign(a.CompareAbsTo(b)));
                Assert.AreEqual(-1, System.Math.Sign(b.CompareAbsTo(a)));
                Assert.AreEqual(0, System.Math.Sign(a.CompareAbsTo(b + 1)));
                Assert.AreEqual(0, System.Math.Sign((a + 1).CompareAbsTo(b)));
                Assert.AreEqual(0, System.Math.Sign((a + 1).CompareAbsTo(-b)));
            }
        }

        [TestMethod]
        public void IntCompareAbsToLimb()
        {
            using (var a = new HugeInt("-222509821"))
            {
                ulong b = 222509820;
                Assert.AreEqual(1, System.Math.Sign(a.CompareAbsTo(b)));
                Assert.AreEqual(0, System.Math.Sign(a.CompareAbsTo(b + 1)));
                Assert.AreEqual(0, System.Math.Sign((-a).CompareAbsTo(b + 1)));
                Assert.AreEqual(0, System.Math.Sign((a + 1).CompareAbsTo(b)));
                Assert.AreEqual(1, System.Math.Sign(a.CompareAbsTo(b)));
                Assert.AreEqual(1, System.Math.Sign((-a).CompareAbsTo(b)));
            }
        }

        [TestMethod]
        public void IntCompareAbsToDouble()
        {
            using (var a = new HugeInt("-222509821"))
            {
                double b = 222509820;
                Assert.AreEqual(1, System.Math.Sign(a.CompareAbsTo(b)));
                Assert.AreEqual(0, System.Math.Sign(a.CompareAbsTo(b + 1)));
                Assert.AreEqual(0, System.Math.Sign((-a).CompareAbsTo(b + 1)));
                Assert.AreEqual(0, System.Math.Sign((a + 1).CompareAbsTo(b)));
                Assert.AreEqual(0, System.Math.Sign((a + 1).CompareAbsTo(-b)));
                Assert.AreEqual(1, System.Math.Sign((a + 1).CompareAbsTo(b - 0.1)));
                Assert.AreEqual(1, System.Math.Sign(a.CompareAbsTo(b)));
                Assert.AreEqual(1, System.Math.Sign((-a).CompareAbsTo(b)));
            }
        }

        #endregion

        #region Sign

        [TestMethod]
        public void IntSign()
        {
            using (var a = new HugeInt("-222509832503450298345029835740293845721"))
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
