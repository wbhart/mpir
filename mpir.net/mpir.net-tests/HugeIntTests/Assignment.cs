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
    public class Assignment
    {
        [TestMethod]
        public void IntAssignCopy()
        {
            using (var a = new HugeInt("-222509832503450298345029835740293845720"))
            using (var b = new HugeInt())
            {
                b.Value = a;
                Assert.AreEqual("-222509832503450298345029835740293845720", b.ToString());
            }
        }

        [TestMethod]
        public void IntAssignRational()
        {
            using (var a = new HugeRational("222509832503450298345029835740293845720/7"))
            using (var b = new HugeInt())
            {
                b.SetTo(a);
                Assert.AreEqual("31787118929064328335004262248613406531", b.ToString());
            }
        }

        [TestMethod]
        public void IntSwap()
        {
            var strA = "-222509832503450298345029835740293845720";
            var strB = "2039847290878794872059384789347534534";
            using (var a = new HugeInt(strA))
            using (var b = new HugeInt(strB))
            {
                var aValue = a._value();
                var bValue = b._value();
                a.Swap(b);
                Assert.AreEqual(aValue, a._value());
                Assert.AreEqual(bValue, b._value());
                Assert.AreEqual(strA, b.ToString());
                Assert.AreEqual(strB, a.ToString());
            }
        }

        [TestMethod]
        public void IntCompoundOperators()
        {
            using (var a = new HugeInt("938475092834705928347523452345"))
            {
                a.Value += 1;
                a.Value *= 10;
                Assert.AreEqual("9384750928347059283475234523460", a.ToString());
            }
        }
        //more tests coming here
    }
}
