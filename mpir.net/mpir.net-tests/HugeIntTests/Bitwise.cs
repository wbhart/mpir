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
        [TestMethod]
        public void IntAndHugeInt()
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
        public void IntOrHugeInt()
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
        public void IntXorHugeInt()
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
        public void IntNot()
        {
            using (var a = new HugeInt("0x10123456789ABCDEF0123456789ABCDEF0123456789ABCDEF"))
            using (var c = new HugeInt())
            {
                c.Value = ~a;
                Assert.AreEqual(-a - 1, c);
            }
        }

        [TestMethod]
        public void IntPopCount()
        {
            using (var a = new HugeInt("0x1ABCDEF8984948281360922385394772450147012613851354303"))
            {
                var max = Platform.Ui(ulong.MaxValue, uint.MaxValue);
                Assert.AreEqual(83UL, a.PopCount());
                Assert.AreEqual(max, (-a).PopCount());
            }
        }

        [TestMethod]
        public void IntHammingDistance()
        {
            using (var a = new HugeInt("0x1ABCDE08984948281360922385394772450147012613851354F03"))
            using (var b = new HugeInt("0x1ABCDEF8984948281360922345394772450147012613851354303"))
            {
                var max = Platform.Ui(ulong.MaxValue, uint.MaxValue);
                Assert.AreEqual(8U, a.HammingDistance(b));
                Assert.AreEqual(8U, (-b).HammingDistance(-a));
                Assert.AreEqual(max, (-a).HammingDistance(b));
                Assert.AreEqual(max, b.HammingDistance(-a));
            }
        }

        [TestMethod]
        public void IntFindBit()
        {
            using (var a = new HugeInt("0xA0000000000000000000800000000001"))
            {
                var max = Platform.Ui(ulong.MaxValue, uint.MaxValue);
                Assert.AreEqual(0UL, a.FindBit(true, 0));
                Assert.AreEqual(47UL, a.FindBit(true, 1));
                Assert.AreEqual(47UL, a.FindBit(true, 47));
                Assert.AreEqual(125UL, a.FindBit(true, 48));
                Assert.AreEqual(127UL, a.FindBit(true, 126));
                Assert.AreEqual(max, a.FindBit(true, 128));

                Assert.AreEqual(1UL, a.FindBit(false, 0));
                Assert.AreEqual(1UL, a.FindBit(false, 1));
                Assert.AreEqual(9UL, a.FindBit(false, 9));
                Assert.AreEqual(128UL, a.FindBit(false, 127));
                Assert.AreEqual(227UL, a.FindBit(false, 227));

                a.Value = ~a;

                Assert.AreEqual(0UL, a.FindBit(false, 0));
                Assert.AreEqual(47UL, a.FindBit(false, 1));
                Assert.AreEqual(47UL, a.FindBit(false, 47));
                Assert.AreEqual(125UL, a.FindBit(false, 48));
                Assert.AreEqual(127UL, a.FindBit(false, 126));
                Assert.AreEqual(max, a.FindBit(false, 128));

                Assert.AreEqual(1UL, a.FindBit(true, 0));
                Assert.AreEqual(1UL, a.FindBit(true, 1));
                Assert.AreEqual(9UL, a.FindBit(true, 9));
                Assert.AreEqual(128UL, a.FindBit(true, 127));
                Assert.AreEqual(227UL, a.FindBit(true, 227));
            }
        }

        [TestMethod]
        public void IntSetBit()
        {
            using (var a = new HugeInt("0xA0000000000000000000200000000001"))
            {
                a.SetBit(47, true);
                Assert.AreEqual("A0000000000000000000A00000000001", a.ToString(16));
                a.SetBit(47, true);
                Assert.AreEqual("A0000000000000000000A00000000001", a.ToString(16));
                a.SetBit(45, false);
                Assert.AreEqual("A0000000000000000000800000000001", a.ToString(16));
                a.SetBit(45, false);
                Assert.AreEqual("A0000000000000000000800000000001", a.ToString(16));
                a.SetBit(131, false);
                Assert.AreEqual("A0000000000000000000800000000001", a.ToString(16));
                a.SetBit(131, true);
                Assert.AreEqual("8A0000000000000000000800000000001", a.ToString(16));
            }
        }

        [TestMethod]
        public void IntGetBit()
        {
            using (var a = new HugeInt("0xA000000000000000000200000000001"))
            {
                var limbs = 128 / MpirSettings.BITS_PER_LIMB;

                Assert.IsTrue(a.GetBit(45));
                Assert.IsFalse(a.GetBit(46));
                Assert.AreEqual(limbs, a.NumberOfLimbsUsed());
                Assert.AreEqual(limbs + 1, a.NumberOfLimbsAllocated());
                Assert.IsFalse(a.GetBit(246));
                Assert.AreEqual(limbs, a.NumberOfLimbsUsed());
                Assert.AreEqual(limbs + 1, a.NumberOfLimbsAllocated());
                a.Value = ~a;
                Assert.AreEqual(-limbs, a.NumberOfLimbsUsed());
                Assert.AreEqual(limbs + 1, a.NumberOfLimbsAllocated());
                Assert.IsTrue(a.GetBit(246));
                Assert.AreEqual(-limbs, a.NumberOfLimbsUsed());
                Assert.AreEqual(limbs + 1, a.NumberOfLimbsAllocated());
            }
        }

        [TestMethod]
        public void IntComplementBit()
        {
            using (var a = new HugeInt("0xA0000000000000000000800000000001"))
            {
                a.ComplementBit(46);
                Assert.AreEqual("A0000000000000000000C00000000001", a.ToString(16));
                a.ComplementBit(47);
                Assert.AreEqual("A0000000000000000000400000000001", a.ToString(16));
                a.ComplementBit(131);
                Assert.AreEqual("8A0000000000000000000400000000001", a.ToString(16));
            }
        }
        //more tests coming here
    }
}
