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

        [TestMethod]
        public void Gcd()
        {
            using (var a = new HugeInt("29927402397991286489627837734179186385188296382227"))
            using (var b = new HugeInt())
            using (var c = new HugeInt())
            {
                b.Value = a * 39;
                c.Value = a * 41;
                Assert.AreEqual(a, b.Gcd(c));
            }
        }

        [TestMethod]
        public void GcdExt()
        {
            using (var a = HugeInt.FromUlong(12))
            using (var b = HugeInt.FromUlong(21))
            using (var g = new HugeInt())
            using (var s = new HugeInt())
            using (var t = new HugeInt())
            {
                g.Value = a.Gcd(b).SavingDiophantineMultipliersTo(s, t);
                Assert.AreEqual("3", g.ToString());
                Assert.AreEqual("2", s.ToString());
                Assert.AreEqual("-1", t.ToString());
            }
        }

        [TestMethod]
        public void GcdExt1()
        {
            using (var a = HugeInt.FromUlong(12))
            using (var b = HugeInt.FromUlong(21))
            using (var g = new HugeInt())
            using (var s = new HugeInt())
            {
                g.Value = a.Gcd(b).SavingDiophantineMultipliersTo(s, null);
                Assert.AreEqual("3", g.ToString());
                Assert.AreEqual("2", s.ToString());
            }
        }

        [TestMethod]
        public void GcdExt2()
        {
            using (var a = HugeInt.FromUlong(12))
            using (var b = HugeInt.FromUlong(21))
            using (var g = new HugeInt())
            using (var t = new HugeInt())
            {
                g.Value = a.Gcd(b).SavingDiophantineMultipliersTo(null, t);
                Assert.AreEqual("3", g.ToString());
                Assert.AreEqual("-1", t.ToString());
            }
        }

        [TestMethod]
        public void GcdLimb()
        {
            using (var a = new HugeInt("29927402397991286489627837734179186385188296382227"))
            {
                a.Value = a * 127;
                Assert.AreEqual(127UL, a.Gcd(254));
            }
        }

        [TestMethod]
        public void Lcm()
        {
            using (var a = new HugeInt("29927402397991286489627837734179186385188296382227"))
            {
                var b = a * 39;
                var c = a * 41;
                Assert.AreEqual(a * 39 * 41, b.Lcm(c));
            }
        }

        [TestMethod]
        public void LcmLimb()
        {
            using (var a = new HugeInt("29927402397991286489627837734179186385188296382227"))
            {
                var b = a * 41;
                Assert.AreEqual(a * 39 * 41, b.Lcm(39));
            }
        }

        [TestMethod]
        public void Invert()
        {
            using (var a = new HugeInt("29927402397991286489627837734179186385188296382227"))
            using (var m = new HugeInt("622288097498926496141095869268883999563096063592498055290461"))
            using (var r = new HugeInt())
            {
                r.Value = a.Invert(m);
                Assert.IsTrue(r > 0);
                Assert.IsTrue(1 == (r * a) % m);
            }
        }

        [TestMethod]
        public void Jacobi()
        {
            using (var a = new HugeInt("9288562863495827364985273645298367452"))
            using (var b = new HugeInt("876428957629387610928574612341"))
            {
                Assert.AreEqual(-1, HugeInt.Jacobi(a, b));
            }
        }

        [TestMethod]
        public void Legendre()
        {
            using (var a = new HugeInt("9288562863495827364985273645298367452"))
            using (var p = new HugeInt("29927402397991286489627837734179186385188296382227"))
            {
                Assert.AreEqual(-1, HugeInt.Legendre(a, p));
            }
        }

        [TestMethod]
        public void Kronecker()
        {
            using (var a = new HugeInt("9288562863495827364985273645298367452"))
            using (var b = new HugeInt("29927402397991286489627837734179186385188296382227"))
            {
                Assert.AreEqual(-1, HugeInt.Kronecker(a, b));
                Assert.AreEqual(0, HugeInt.Kronecker(a, 2));
                Assert.AreEqual(-1, HugeInt.Kronecker(b, 2));
                Assert.AreEqual(-1, HugeInt.Kronecker(2, b));
                Assert.AreEqual(-1, HugeInt.Kronecker(b, 2UL));
                Assert.AreEqual(-1, HugeInt.Kronecker(2UL, b));
            }
        }

        [TestMethod]
        public void RemoveFactors()
        {
            using (var a = new HugeInt("9288562863495827364985273645298367452"))
            using (var b = new HugeInt("29927402397991286489627837734179186385188296382227"))
            {
                Assert.AreEqual(a, (a * b * b).RemoveFactors(b));
                ulong count = 0;
                Assert.AreEqual(a, (a * b * b).RemoveFactors(b).SavingCountRemovedTo(x => count = x));
                Assert.AreEqual(2UL, count);
            }
        }
    }
}
