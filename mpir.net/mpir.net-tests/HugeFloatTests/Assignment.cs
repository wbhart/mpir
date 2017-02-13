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
    public class Assignment
    {
        [TestInitialize]
        public void Setup()
        {
            HugeFloat.DefaultPrecision = 256;
        }

        [TestCleanup]
        public void Cleanup()
        {
            HugeFloat.DefaultPrecision = 64;
        }

        [TestMethod]
        public void FloatAssignCopy()
        {
            var s = "-1.22250983250345029834502983574029384572";
            using (var a = new HugeFloat(s))
            using (var b = new HugeFloat())
            {
                b.Value = a;
                FloatAssert.AreEqual(s, b);
            }
        }

        [TestMethod]
        public void FloatSwap()
        {
            using (var a = HugeFloat.Allocate(192))
            using (var b = HugeFloat.Allocate(128))
            {
                a.SetTo(123.5);
                b.SetTo(432.25);
                var aValue = a._value();
                var bValue = b._value();
                var aPrec = a._allocatedPrecision;
                var bPrec = b._allocatedPrecision;
                a.Swap(b);
                Assert.AreNotEqual(aValue, bValue);
                Assert.AreEqual(aValue, a._value());
                Assert.AreEqual(bValue, b._value());
                Assert.IsTrue(432.25 == a);
                Assert.IsTrue(123.5 == b);
                Assert.AreNotEqual(aPrec, bPrec);
                Assert.AreEqual(bPrec, a._allocatedPrecision);
                Assert.AreEqual(aPrec, b._allocatedPrecision);
            }
        }

        [TestMethod]
        public void FloatCompoundOperators()
        {
            using (var a = new HugeFloat("938475092834705928347523452345.115756986668303657898962467957"))
            {
                a.Value += 1;
                a.Value *= 10;
                a.Precision = 224;
                FloatAssert.AreEqual("9384750928347059283475234523461.15756986668303657898962467957", a);
            }
        }

        [TestMethod]
        public void FloatAssignInt()
        {
            using (var a = new HugeInt("222509832503450298345029835740293845720"))
            using (var b = new HugeFloat())
            {
                b.SetTo(a);
                FloatAssert.AreEqual("222509832503450298345029835740293845720.", b);
            }
        }

        public void FloatAssignDouble()
        {
            using(var a = new HugeFloat())
            {
                a.SetTo(22250983250345.125);
                Assert.IsTrue(a == 22250983250345.125);
            }
        }

        [TestMethod]
        public void FloatAssignRational()
        {
            using (var a = new HugeRational(1, 3))
            using (var b = new HugeFloat())
            {
                b.SetTo(a);
                FloatAssert.AreEqual(".33333333333333333333333333333333333333333333333333333333333333333333333333333333333", b);
            }
        }
        //more tests coming here
    }
}
