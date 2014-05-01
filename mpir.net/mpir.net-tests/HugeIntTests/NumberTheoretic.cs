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
using System.IO;
using System.Text;
using System.Linq;
using Microsoft.VisualStudio.TestTools.UnitTesting;

namespace MPIR.Tests.HugeIntTests
{
    [TestClass]
    public class NumberTheoretic
    {
        [TestMethod]
        public void IsProbablePrime()
        {
            using (var a = new HugeInt("622288097498926496141095869268883999563096063592498055290461"))
            using (var random = MpirRandom.Default())
            {
                Assert.IsTrue(a.IsProbablePrime(random, 10, 0));
                a.Value = a * 2;
                Assert.IsFalse(a.IsProbablePrime(random, 10, 0));
            }
        }

        [TestMethod]
        public void IsLikelyPrime()
        {
            using (var a = new HugeInt("622288097498926496141095869268883999563096063592498055290461"))
            using (var random = MpirRandom.Default())
            {
                Assert.IsTrue(a.IsLikelyPrime(random, 0));
                a.Value = a * 2;
                Assert.IsFalse(a.IsLikelyPrime(random, 0));
            }
        }

        [TestMethod]
        public void NextPrimeCandidate()
        {
            using (var a = new HugeInt("622288097498926496141095869268883999563096063592498055290460"))
            using (var random = MpirRandom.Default())
            {
                Assert.AreEqual(a + 1, a.NextPrimeCandidate(random));
            }
        }
    }
}
