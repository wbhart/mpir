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
    public class Bitwise
    {
        #region And

        [TestMethod]
        public void AndHugeInt()
        {
            using (var a = new HugeInt("0x10123456789ABCDEF0123456789ABCDEF0123456789ABCDEF"))
            using (var b = new HugeInt("0x100000000000000000123456789ABCDEFFFFFFFFFFFFFFFFF"))
            using (var c = new HugeInt())
            {
                c.Value = a & b;
                Assert.AreEqual("100000000000000000123456789ABCDEF0123456789ABCDEF", c.ToString(16));
            }
        }

        [TestMethod]
        public void OrHugeInt()
        {
            using (var a = new HugeInt("0x10123456789ABCDEF0123456789ABCDEF0123456789ABCDEF"))
            using (var b = new HugeInt("0x100000000000000000123456789ABCDEFFFFFFFFFFFFFFFFF"))
            using (var c = new HugeInt())
            {
                c.Value = a | b;
                Assert.AreEqual("10123456789ABCDEF0123456789ABCDEFFFFFFFFFFFFFFFFF", c.ToString(16));
            }
        }

        [TestMethod]
        public void XorHugeInt()
        {
            using (var a = new HugeInt("0x10123456789ABCDEF0123456789ABCDEF0123456789ABCDEF"))
            using (var b = new HugeInt("0x000000000000000000123456789ABCDEFFFFFFFFFFFFFFFFF"))
            using (var c = new HugeInt())
            {
                c.Value = a ^ b;
                Assert.AreEqual("10123456789ABCDEF0000000000000000FEDCBA9876543210", c.ToString(16));
            }
        }

        [TestMethod]
        public void Not()
        {
            using (var a = new HugeInt("0x10123456789ABCDEF0123456789ABCDEF0123456789ABCDEF"))
            using (var c = new HugeInt())
            {
                c.Value = ~a;
                Assert.AreEqual(-a - 1, c);
            }
        }

        #endregion
    }
}
