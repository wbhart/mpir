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

using Microsoft.VisualStudio.TestTools.UnitTesting;

namespace MPIR.Tests.HugeFloatTests
{
    [TestClass]
    public class MiscMath
    {
        [TestMethod]
        public void FloatFloor()
        {
            using(var a = new HugeFloat("-9023409872309847509847.9385345098345"))
            using(var b = new HugeFloat())
            {
                b.Value = a.Floor();
                FloatAssert.AreEqual("-9023409872309847509848", b);
                b.Value = (-a).Floor();
                FloatAssert.AreEqual("9023409872309847509847", b);
            }
        }

        [TestMethod]
        public void FloatCeiling()
        {
            using(var a = new HugeFloat("-9023409872309847509847.9385345098345"))
            using(var b = new HugeFloat())
            {
                b.Value = a.Ceiling();
                FloatAssert.AreEqual("-9023409872309847509847", b);
                b.Value = (-a).Ceiling();
                FloatAssert.AreEqual("9023409872309847509848", b);
            }
        }

        [TestMethod]
        public void FloatTruncate()
        {
            using(var a = new HugeFloat("-9023409872309847509847.9385345098345"))
            using(var b = new HugeFloat())
            {
                b.Value = a.Truncate();
                FloatAssert.AreEqual("-9023409872309847509847", b);
                b.Value = (-a).Truncate();
                FloatAssert.AreEqual("9023409872309847509847", b);
            }
        }
    }
}
