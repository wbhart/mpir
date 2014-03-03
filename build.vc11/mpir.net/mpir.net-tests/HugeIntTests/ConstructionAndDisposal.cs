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
    public class ConstructionAndDisposal
    {
        [TestMethod]
        public void DefaultConstructor()
        {
            var a = new HugeInt();
            Assert.AreNotEqual(0, a.NumberOfLimbsAllocated());
            Assert.AreEqual(0, a.NumberOfLimbsUsed());
            Assert.AreNotEqual(IntPtr.Zero, a.Limbs());
        }

        [TestMethod]
        public void Dispose()
        {
            var a = new HugeInt();
            a.Dispose();
            Assert.AreEqual(0, a.NumberOfLimbsAllocated());
            Assert.AreEqual(0, a.NumberOfLimbsUsed());
            Assert.AreEqual(IntPtr.Zero, a.Limbs());
        }

        [TestMethod]
        public void ConstructorFromLong()
        {
            var a = HugeInt.FromLong(123456789123456);
            Assert.AreEqual(1, a.NumberOfLimbsAllocated());
            Assert.AreEqual(1, a.NumberOfLimbsUsed());
        }

        [TestMethod]
        public void ConstructorFromLongNegative()
        {
            var a = HugeInt.FromLong(-123456789123456);
            Assert.AreEqual(1, a.NumberOfLimbsAllocated());
            Assert.AreEqual(-1, a.NumberOfLimbsUsed());
        }

        [TestMethod]
        public void ConstructorFromULong()
        {
            var a = HugeInt.FromUlong(ulong.MaxValue);
            Assert.AreEqual(1, a.NumberOfLimbsAllocated());
            Assert.AreEqual(1, a.NumberOfLimbsUsed());
        }

        [TestMethod]
        public void Allocate()
        {
            var a = new HugeInt(129);
            Assert.AreEqual(3, a.NumberOfLimbsAllocated());
            Assert.AreEqual(0, a.NumberOfLimbsUsed());
        }
    }
}
