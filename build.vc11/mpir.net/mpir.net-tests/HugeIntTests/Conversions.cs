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
    public class Conversions
    {
        [TestMethod]
        public void ToStringDecimal()
        {
            var n = "-23429384756298357462983476598345623984756";
            using (var a = new HugeInt(n))
            {
                Assert.AreEqual(n, a.ToString());
            }
        }

        [TestMethod]
        public void ToStringHex()
        {
            var n = "-23429abcdef298357462983fedcba345623984756";
            using (var a = new HugeInt(n, 16))
            {
                Assert.AreEqual(n, a.ToString(16));
                Assert.AreEqual(n.ToUpper(), a.ToString(-16));
            }
        }

        [TestMethod]
        public void ToAndFromUlong()
        {
            using (var a = new HugeInt())
            {
                ulong b = 0xF84739ABCDEF4876;
                a.AsUlong = b;
                Assert.AreEqual(b.ToString(), a.ToString());

                a.Value = -a;
                ulong c = a.AsUlong;
                Assert.AreEqual(b.ToString(), c.ToString());
            }
        }

        //todo truncated test
    }
}
