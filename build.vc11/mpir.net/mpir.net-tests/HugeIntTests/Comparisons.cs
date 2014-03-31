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
        [TestMethod]
        public void CompareToHugeInt()
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
        public void CompareToObject()
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
        public void CompareToExpression()
        {
            using (var a = new HugeInt("-222509832503450298345029835740293845721"))
            using (var b = new HugeInt("222509832503450298345029835740293845720"))
            {
                Assert.AreEqual(1, System.Math.Sign(((IComparable<MpirExpression>)b).CompareTo(a)));
                Assert.AreEqual(-1, System.Math.Sign(((IComparable<MpirExpression>)a).CompareTo(b)));
                Assert.AreEqual(1, System.Math.Sign(((IComparable<MpirExpression>)a).CompareTo(null)));
                Assert.AreEqual(0, System.Math.Sign(((IComparable<MpirExpression>)(a + 1)).CompareTo(-b)));
            }
        }

        [TestMethod]
        [ExpectedException(typeof(ArgumentException))]
        public void CompareToNonExpression()
        {
            using (var a = new HugeInt("-222509832503450298345029835740293845721"))
            {
                a.CompareTo("abc");
            }
        }

        [TestMethod]
        public void OperatorLessThan()
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
        public void OperatorLessThanOrEqual()
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
        public void OperatorGreaterThan()
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
        public void OperatorGreaterThanOrEqual()
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

        [TestMethod]
        public void OperatorLessThanLimb()
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
        public void OperatorLessThanOrEqualLimb()
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
        public void OperatorGreaterThanLimb()
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
        public void OperatorGreaterThanOrEqualLimb()
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

        [TestMethod]
        public void OperatorLessThanSignedLimb()
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
        public void OperatorLessThanOrEqualSignedLimb()
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
        public void OperatorGreaterThanSignedLimb()
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
        public void OperatorGreaterThanOrEqualSignedLimb()
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
        //more tests coming here
    }
}
