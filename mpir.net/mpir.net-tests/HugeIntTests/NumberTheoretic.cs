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
        public void IntIsProbablePrime()
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
        public void IntIsLikelyPrime()
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
        public void IntNextPrimeCandidate()
        {
            using (var a = new HugeInt("622288097498926496141095869268883999563096063592498055290460"))
            using (var random = MpirRandom.Default())
            {
                Assert.AreEqual(a + 1, a.NextPrimeCandidate(random));
            }
        }

        [TestMethod]
        public void IntGcd()
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
        public void IntGcdExt()
        {
            using (var a = new HugeInt(12))
            using (var b = new HugeInt(21))
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
        public void IntGcdExt1()
        {
            using (var a = new HugeInt(12))
            using (var b = new HugeInt(21))
            using (var g = new HugeInt())
            using (var s = new HugeInt())
            {
                g.Value = a.Gcd(b).SavingDiophantineMultipliersTo(s, null);
                Assert.AreEqual("3", g.ToString());
                Assert.AreEqual("2", s.ToString());
            }
        }

        [TestMethod]
        public void IntGcdExt2()
        {
            using (var a = new HugeInt(12))
            using (var b = new HugeInt(21))
            using (var g = new HugeInt())
            using (var t = new HugeInt())
            {
                g.Value = a.Gcd(b).SavingDiophantineMultipliersTo(null, t);
                Assert.AreEqual("3", g.ToString());
                Assert.AreEqual("-1", t.ToString());
            }
        }

        [TestMethod]
        public void IntGcdLimb()
        {
            using (var a = new HugeInt("29927402397991286489627837734179186385188296382227"))
            {
                a.Value = a * 127;
                Assert.AreEqual(127UL, a.Gcd(254));
            }
        }

        [TestMethod]
        public void IntLcm()
        {
            using (var a = new HugeInt("29927402397991286489627837734179186385188296382227"))
            {
                var b = a * 39;
                var c = a * 41;
                Assert.AreEqual(a * 39 * 41, b.Lcm(c));
            }
        }

        [TestMethod]
        public void IntLcmLimb()
        {
            using (var a = new HugeInt("29927402397991286489627837734179186385188296382227"))
            {
                var b = a * 41;
                Assert.AreEqual(a * 39 * 41, b.Lcm(39));
            }
        }

        [TestMethod]
        public void IntInvert()
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
        public void IntJacobi()
        {
            using (var a = new HugeInt("9288562863495827364985273645298367452"))
            using (var b = new HugeInt("876428957629387610928574612341"))
            {
                Assert.AreEqual(-1, HugeInt.Jacobi(a, b));
            }
        }

        [TestMethod]
        public void IntLegendre()
        {
            using (var a = new HugeInt("9288562863495827364985273645298367452"))
            using (var p = new HugeInt("29927402397991286489627837734179186385188296382227"))
            {
                Assert.AreEqual(-1, HugeInt.Legendre(a, p));
            }
        }

        [TestMethod]
        public void IntKronecker()
        {
            using (var a = new HugeInt("9288562863495827364985273645298367452"))
            using (var b = new HugeInt("29927402397991286489627837734179186385188296382227"))
            {
                Assert.AreEqual(-1, HugeInt.Kronecker(a, b));
                Assert.AreEqual(0, HugeInt.Kronecker(a, 2));
                Assert.AreEqual(-1, HugeInt.Kronecker(b, 2));
                Assert.AreEqual(-1, HugeInt.Kronecker(2, b));
                Assert.AreEqual(-1, HugeInt.Kronecker(b, 2U));
                Assert.AreEqual(-1, HugeInt.Kronecker(2U, b));
            }
        }

        [TestMethod]
        public void IntRemoveFactors()
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

        [TestMethod]
        public void IntFactorial()
        {
            using (var a = new HugeInt(HugeInt.Factorial(30)))
            using (var b = new HugeInt("50"))
            using (var c = new HugeInt("70"))
            using (var x = new HugeInt())
            {
                for (x.Value = b - 2; x > 0; x.Value -= 2)
                    b.Value *= x;

                for (x.Value = c - 3; x > 0; x.Value -= 3)
                    c.Value *= x;

                Assert.AreEqual("265252859812191058636308480000000", a.ToString());
                Assert.AreEqual(b, HugeInt.Factorial(50, 2));
                Assert.AreEqual(c, HugeInt.Factorial(70, 3));
            }
        }

        [TestMethod]
        public void IntPrimorial()
        {
            var N = 256U;
            var sieve = new bool[N];
            for (var x = 2; x < 16; x++)
                if (!sieve[x])
                    for (var n = x * 2; n < N; n += x)
                        sieve[n] = true;

            using (var a = new HugeInt(1))
            {
                for (var n = 2; n < N; n++)
                    if (!sieve[n])
                        a.Value *= n;

                Assert.AreEqual(a, HugeInt.Primorial(N));
            }
        }

        [TestMethod]
        public void IntBinomial()
        {
            using (var a = new HugeInt(HugeInt.Binomial(40, 15)))
            using (var b = new HugeInt("41"))
            using (var c = new HugeInt(HugeInt.Binomial(b, 15)))
            {
                Assert.AreEqual(HugeInt.Factorial(40).DivideExactly(HugeInt.Factorial(15) * HugeInt.Factorial(25)), a);
                Assert.AreEqual(a * 41 / 26, c);
            }
        }

        [TestMethod]
        public void IntFibonacci()
        {
            using (var a = new HugeInt(HugeInt.Fibonacci(300)))
            using (var b = new HugeInt())
            using (var c = new HugeInt(HugeInt.Fibonacci(300).SavingPreviousTo(b)))
            using (var even = new HugeInt("1"))
            using (var odd = new HugeInt("1"))
            {
                for(var i = 3; i <= 299; i += 2)
                {
                    odd.Value += even;
                    even.Value += odd;
                }
                Assert.AreEqual(even, a);
                Assert.AreEqual(odd, b);
                Assert.AreEqual(even, c);
            }
        }

        [TestMethod]
        public void IntLucas()
        {
            using (var f299 = new HugeInt())
            using (var f300 = new HugeInt(HugeInt.Fibonacci(300).SavingPreviousTo(f299)))
            using (var a = new HugeInt())
            {
                var f298 = f300 - f299;
                Assert.AreEqual(f299 * 2 + f300, HugeInt.Lucas(300));
                Assert.AreEqual(f299 * 2 + f300, HugeInt.Lucas(300).SavingPreviousTo(a));
                Assert.AreEqual(f298 * 2 + f299, a);
            }
        }
    }
}
