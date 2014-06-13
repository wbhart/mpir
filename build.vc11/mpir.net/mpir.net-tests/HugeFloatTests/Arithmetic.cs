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
    public static class FloatAssert
    {
        public static void AreEqual(string expected, HugeFloat actual)
        {
            var actualStr = actual.ToString();
            if(expected[0] == '-')
            {
                Assert.AreEqual(expected[0], actualStr[0]);
                actualStr = actualStr.TrimStart('-');
                expected = expected.TrimStart('-');
            }

            var exponent = expected.IndexOf('.');
            expected = expected.Replace(".", "");

            var exponentStr = "@" + exponent;

            Assert.IsTrue(actualStr.StartsWith("0."));
            actualStr = actualStr.Substring(2);

            Assert.IsTrue(actualStr.EndsWith(exponentStr));
            actualStr = actualStr.Substring(0, actualStr.Length - exponentStr.Length);

            if (expected.Length > actualStr.Length)
            {
                var roundedUp = expected[actualStr.Length] >= '5';
                expected = expected.Substring(0, actualStr.Length);
                if(roundedUp)
                {
                    using (var a = new HugeInt(expected))
                    {
                        a.Value += 1;
                        expected = a.ToString(10);
                    }
                }
            }
            Assert.AreEqual(expected, actualStr);
        }
    }

    [TestClass]
    public class Arithmetic
    {
        #region Add

        [TestMethod]
        public void FloatAddHugeFloat()
        {
            using (var a = new HugeFloat("22250983250345029834502983.5740293845720"))
            using (var b = new HugeFloat("2229874359879827.30594288574029879874539"))
            using (var c = new HugeFloat(a + b))
            {
                FloatAssert.AreEqual("22250983252574904194382810.87997227031229879874539", c);
                c.Value = a + (b + 1);
                FloatAssert.AreEqual("22250983252574904194382811.87997227031229879874539", c);
            }
        }

        [TestMethod]
        public void FloatAddLimb()
        {
            using (var a = new HugeFloat("22250983250345029834502983.5740293845720"))
            using (var c = new HugeFloat())
            {
                var b = 4288574029879874539UL;
                c.Value = a + b;
                FloatAssert.AreEqual("22250987538919059714377522.5740293845720", c);
                c.Value = b + a;
                FloatAssert.AreEqual("22250987538919059714377522.5740293845720", c);
            }
        }

        [TestMethod]
        public void FloatAddSignedLimb()
        {
            using (var a = new HugeFloat("22250983250345029834502983.5740293845720"))
            using (var c = new HugeFloat())
            {
                var b = -4288574029879874539;
                c.Value = a + b;
                FloatAssert.AreEqual("22250978961770999954628444.5740293845720", c);
                c.Value = b + a;
                FloatAssert.AreEqual("22250978961770999954628444.5740293845720", c);
            }
        }

        #endregion

        #region Subtract

        [TestMethod]
        public void FloatSubtractHugeFloat()
        {
            using (var a = new HugeFloat("22250983250345029834502983.5740293845720"))
            using (var b = new HugeFloat("2229874359879827.30594288574029879874539"))
            using (var c = new HugeFloat(a - b))
            {
                FloatAssert.AreEqual("22250983248115155474623156.26808649883170120125461", c);
                c.Value = b - (a + 1);
                FloatAssert.AreEqual("-22250983248115155474623157.26808649883170120125461", c);
            }
        }

        [TestMethod]
        public void FloatSubtractLimb()
        {
            using(var a = new HugeFloat("22250983250345029834502983.5740293845720"))
            using(var c = new HugeFloat())
            {
                var b = 4288574029879874539UL;
                c.Value = a - b;
                FloatAssert.AreEqual("22250978961770999954628444.5740293845720", c);
                c.Value = b - a;
                FloatAssert.AreEqual("-22250978961770999954628444.5740293845720", c);
            }
        }

        [TestMethod]
        public void FloatSubtractSignedLimb()
        {
            using(var a = new HugeFloat("22250983250345029834502983.5740293845720"))
            using(var c = new HugeFloat())
            {
                var b = -4288574029879874539;
                c.Value = a - b;
                FloatAssert.AreEqual("22250987538919059714377522.5740293845720", c);
                c.Value = b - a;
                FloatAssert.AreEqual("-22250987538919059714377522.5740293845720", c);
            }
        }

        #endregion

        #region Multiply

        [TestMethod]
        public void FloatMultiplyByHugeFloat()
        {
            using (var a = new HugeFloat("90234098723098475098479385.345098345"))
            using (var b = new HugeFloat("78594873598734.59872354987610987897"))
            using (var c = new HugeFloat(a * b))
            {
                FloatAssert.AreEqual("7091937583437663707014199538801620613535.95657143399816050772069730465", c);
                c.Value = b * (a + 1);
                FloatAssert.AreEqual("7091937583437663707014199617396494212270.55529498387427038669069730465", c);
            }
        }

        [TestMethod]
        public void FloatMultiplyByLimb()
        {
            using (var a = new HugeFloat("9023409872309847509847.9385345098345"))
            using (var c = new HugeFloat())
            {
                ulong b = 17390538260286101342;
                c.Value = a * b;
                FloatAssert.AreEqual("156921954622647727368660197878904460649174.746962647899", c);
                c.Value = b * -a;
                FloatAssert.AreEqual("-156921954622647727368660197878904460649174.746962647899", c);
            }
        }

        [TestMethod]
        public void FloatMultiplyBySignedLimb()
        {
            using (var a = new HugeFloat("9023409872309847509847.9385345098345"))
            using (var c = new HugeFloat())
            {
                long b = -7390538260286101342;
                c.Value = a * b;
                FloatAssert.AreEqual("-66687855899549252270180812533806115649174.746962647899", c);
                c.Value = b * -a;
                FloatAssert.AreEqual("66687855899549252270180812533806115649174.746962647899", c);
            }
        }

        #endregion

        #region Shift Left

        [TestMethod]
        public void FloatShiftLeft()
        {
            using (var a = new HugeFloat("-12345700987ABA245230948.17607EF", 16))
            using (var e = new HugeFloat("-12345700987ABA24523094817607.EF", 16))
            {
                ulong b = 20;
                a.Value = a << b;
                Assert.AreEqual(e, a);
            }
        }

        #endregion

        #region Shift Right

        [TestMethod]
        public void FloatShiftRight()
        {
            using (var a = new HugeFloat("-12345700987ABA24523094817607.EF", 16))
            using (var e = new HugeFloat("-12345700987ABA245230948.17607EF", 16))
            {
                ulong b = 20;
                a.Value = a >> b;
                Assert.AreEqual(e, a);
            }
        }

        #endregion

        #region Negate

        [TestMethod]
        public void FloatNegate()
        {
            using(var a = new HugeFloat("9023409872309847509847.9385345098345"))
            {
                a.Value = -a;
                FloatAssert.AreEqual("-9023409872309847509847.9385345098345", a);
                a.Value = -a;
                FloatAssert.AreEqual("9023409872309847509847.9385345098345", a);
            }
        }

        #endregion

        #region Abs

        [TestMethod]
        public void FloatMakeAbsolute()
        {
            using(var a = new HugeFloat("-9023409872309847509847.9385345098345"))
            {
                a.Value = a.Abs();
                FloatAssert.AreEqual("9023409872309847509847.9385345098345", a);
                a.Value = a.Abs();
                FloatAssert.AreEqual("9023409872309847509847.9385345098345", a);
            }
        }

        #endregion

        #region Power

        [TestMethod]
        public void FloatPower()
        {
            using(var a = new HugeFloat("-902340.945098345"))
            {
                a.Value = a ^ 5;
                FloatAssert.AreEqual("-598209523815275040074985233466.4619735146023546465747916785912044", a);
            }
        }

        #endregion

        #region Divide

        #region Int

        [TestMethod]
        public void FloatDivideHugeFloat()
        {
            using (var a = new HugeFloat("1157569866683036578989624354347957.394580293847"))
            using (var b = new HugeFloat("593169091750307653294.549782395235784"))
            {
                a.Value = a / b;
                FloatAssert.AreEqual("1951500647593.2689953514865540344827449639493356367018584357", a);
            }
        }

        #endregion

        #region Limb

        [TestMethod]
        public void FloatDivideLimb()
        {
            using (var a = new HugeFloat("1157569866683036578989624354347957.394580293847"))
            {
                ulong b = 5931690917503076532;
                a.Value = a / b;
                FloatAssert.AreEqual("195150064759326.89956625512472902395197480398952074748799190", a);
            }
        }

        [TestMethod]
        public void FloatDivideSignedLimb()
        {
            using(var a = new HugeFloat("1157569866683036578989624354347957.394580293847"))
            {
                long b = -5931690917503076532;
                a.Value = a / b;
                FloatAssert.AreEqual("-195150064759326.89956625512472902395197480398952074748799190", a);
            }
        }

        [TestMethod]
        public void FloatDivideLimbBy()
        {
            using(var a = new HugeFloat("11575698666830.39458029384723405203984572"))
            {
                ulong b = 5931690917503076532;
                a.Value = b / a;
                FloatAssert.AreEqual("512426.16866833708737257760720580856722540469109813901673959", a);
            }
        }

        [TestMethod]
        public void FloatDivideSignedLimbBy()
        {
            using(var a = new HugeFloat("11575698666830.39458029384723405203984572"))
            {
                long b = -5931690917503076532;
                a.Value = b / a;
                FloatAssert.AreEqual("-512426.16866833708737257760720580856722540469109813901673959", a);
            }
        }

        #endregion

        #endregion
    }
}
