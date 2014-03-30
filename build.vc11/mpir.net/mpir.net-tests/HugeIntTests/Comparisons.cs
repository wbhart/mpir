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

        //more tests coming here
    }
}
