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
            if(exponent < 0) exponent = expected.Length;
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
            using (var a = new HugeFloat("222509832503450298345029835740293845720/115756986668303657898962467957"))
            using (var b = new HugeFloat("222987435987982730594288574029879874539/590872612825179551336102196593"))
            using (var c = new HugeFloat(a + b))
            {
                Assert.AreEqual(a.Numerator * b.Denominator + b.Numerator * a.Denominator, c.Numerator);
                Assert.AreEqual(a.Denominator * b.Denominator, c.Denominator);
            }
        }

        [TestMethod]
        public void FloatAddHugeInt()
        {
            using (var a = new HugeFloat("222509832503450298345029835740293845720/115756986668303657898962467957"))
            using (var b = new HugeInt("222987435987982730594288574029879874539"))
            using (var c = new HugeFloat(a + b))
            {
                Assert.AreEqual(a.Numerator + b * a.Denominator, c.Numerator);
                Assert.AreEqual(a.Denominator, c.Denominator);
                c.Value = (b + 1) + a;
                Assert.AreEqual(a.Numerator + (b + 1) * a.Denominator, c.Numerator);
                Assert.AreEqual(a.Denominator, c.Denominator);
            }
        }

        [TestMethod]
        public void FloatAddLimb()
        {
            using (var a = new HugeFloat("222509832503450298345029835740293845720/115756986668303657898962467957"))
            using (var c = new HugeFloat())
            {
                var b = 4288574029879874539UL;
                c.Value = a + b;
                Assert.AreEqual(a.Numerator + b * a.Denominator, c.Numerator);
                Assert.AreEqual(a.Denominator, c.Denominator);
            }
        }

        [TestMethod]
        public void FloatAddToLimb()
        {
            using (var a = new HugeFloat("222509832503450298345029835740293845720/115756986668303657898962467957"))
            using (var c = new HugeFloat())
            {
                var b = 4288574029879874539UL;
                c.Value = b + a;
                Assert.AreEqual(a.Numerator + b * a.Denominator, c.Numerator);
                Assert.AreEqual(a.Denominator, c.Denominator);
            }
        }

        [TestMethod]
        public void FloatAddSignedLimb()
        {
            using (var a = new HugeFloat("222509832503450298345029835740293845720/115756986668303657898962467957"))
            using (var c = new HugeFloat())
            {
                var b = -4288574029879874539L;
                c.Value = a + b;
                Assert.AreEqual(a.Numerator + b * a.Denominator, c.Numerator);
                Assert.AreEqual(a.Denominator, c.Denominator);
            }
        }

        [TestMethod]
        public void FloatAddToSignedLimb()
        {
            using (var a = new HugeFloat("222509832503450298345029835740293845720/115756986668303657898962467957"))
            using (var c = new HugeFloat())
            {
                var b = -4288574029879874539L;
                c.Value = b + a;
                Assert.AreEqual(a.Numerator + b * a.Denominator, c.Numerator);
                Assert.AreEqual(a.Denominator, c.Denominator);
            }
        }

        [TestMethod]
        public void FloatAddToMaxSignedLimb()
        {
            using (var a = new HugeFloat("222509832503450298345029835740293845720/115756986668303657898962467957"))
            using (var c = new HugeFloat())
            {
                var b = long.MinValue;
                c.Value = b + a;
                Assert.AreEqual(a.Numerator + b * a.Denominator, c.Numerator);
                Assert.AreEqual(a.Denominator, c.Denominator);
            }
        }

        [TestMethod]
        public void FloatAddExpressionHugeFloat()
        {
            using (var a = new HugeFloat("222509832503450298345029835740293845720/115756986668303657898962467957"))
            using (var b = new HugeFloat("222987435987982730594288574029879874539/590872612825179551336102196593"))
            using (var c = new HugeFloat())
            {
                c.Value = 1 + (a + b);
                Assert.AreEqual(a.Numerator * b.Denominator + b.Numerator * a.Denominator + a.Denominator * b.Denominator, c.Numerator);
                Assert.AreEqual(a.Denominator * b.Denominator, c.Denominator);
            }
        }

        #endregion

        #region Subtract

        [TestMethod]
        public void FloatSubtractHugeFloat()
        {
            using (var a = new HugeFloat("445497268491433028939318409770173720259/115756986668303657898962467957"))
            using (var b = new HugeFloat("222987435987982730594288574029879874539/590872612825179551336102196593"))
            using (var c = new HugeFloat(a - b))
            {
                Assert.AreEqual(a.Numerator * b.Denominator - b.Numerator * a.Denominator, c.Numerator);
                Assert.AreEqual(a.Denominator * b.Denominator, c.Denominator);
            }
        }

        [TestMethod]
        public void FloatSubtractHugeInt()
        {
            using (var a = new HugeFloat("222509832503450298345029835740293845720/115756986668303657898962467957"))
            using (var b = new HugeInt("222987435987982730594288574029879874539"))
            using (var c = new HugeFloat(a - b))
            {
                Assert.AreEqual(a.Numerator - b * a.Denominator, c.Numerator);
                Assert.AreEqual(a.Denominator, c.Denominator);
                c.Value = (b + 1) - a;
                Assert.AreEqual((b + 1) * a.Denominator - a.Numerator, c.Numerator);
                Assert.AreEqual(a.Denominator, c.Denominator);
            }
        }

        [TestMethod]
        public void FloatSubtractLimb()
        {
            var b = 4288574029879874539UL;
            using (var a = new HugeFloat("222509832503450298349318409770173720259/115756986668303657898962467957"))
            using (var c = new HugeFloat(a - b))
            {
                Assert.AreEqual(a.Numerator - b * a.Denominator, c.Numerator);
                Assert.AreEqual(a.Denominator, c.Denominator);
            }
        }

        [TestMethod]
        public void FloatSubtractFromLimb()
        {
            var b = 4288574029879874539UL;
            using (var a = new HugeFloat("222509832503450298349318409770173720259/115756986668303657898962467957"))
            using (var c = new HugeFloat(b - a))
            {
                Assert.AreEqual(b * a.Denominator - a.Numerator, c.Numerator);
                Assert.AreEqual(a.Denominator, c.Denominator);
            }
        }

        [TestMethod]
        public void FloatSubtractPositiveSignedLimb()
        {
            var b = 4288574029879874539L;
            using (var a = new HugeFloat("222509832503450298349318409770173720259/115756986668303657898962467957"))
            using (var c = new HugeFloat(a - b))
            {
                Assert.AreEqual(a.Numerator - b * a.Denominator, c.Numerator);
                Assert.AreEqual(a.Denominator, c.Denominator);
            }
        }

        [TestMethod]
        public void FloatSubtractFromPositiveSignedLimb()
        {
            var b = 4288574029879874539L;
            using (var a = new HugeFloat("222509832503450298349318409770173720259/115756986668303657898962467957"))
            using (var c = new HugeFloat(a - b))
            {
                Assert.AreEqual(a.Numerator - b * a.Denominator, c.Numerator);
                Assert.AreEqual(a.Denominator, c.Denominator);
            }
        }

        [TestMethod]
        public void FloatSubtractSignedLimb()
        {
            var b = -4288574029879874539L;
            using (var a = new HugeFloat("222509832503450298345029835740293845720/115756986668303657898962467957"))
            using (var c = new HugeFloat(a - b))
            {
                Assert.AreEqual(a.Numerator - b * a.Denominator, c.Numerator);
                Assert.AreEqual(a.Denominator, c.Denominator);
            }
        }

        [TestMethod]
        public void FloatSubtractFromSignedLimb()
        {
            var b = -4288574029879874539L;
            using (var a = new HugeFloat("222509832503450298345029835740293845720/115756986668303657898962467957"))
            using (var c = new HugeFloat(b - a))
            {
                Assert.AreEqual(b * a.Denominator - a.Numerator, c.Numerator);
                Assert.AreEqual(a.Denominator, c.Denominator);
            }
        }

        #endregion

        #region Multiply

        [TestMethod]
        public void FloatMultiplyByHugeFloat()
        {
            using (var a = new HugeFloat("90234098723098475098479385345098345/115756986668303657898962467957"))
            using (var b = new HugeFloat("7859487359873459872354987610987897/590872612825179551336102196593"))
            using (var c = new HugeFloat(a * b))
            {
                Assert.AreEqual(a.Numerator * b.Numerator, c.Numerator);
                Assert.AreEqual(a.Denominator * b.Denominator, c.Denominator);
            }
        }

        [TestMethod]
        public void FloatMultiplytHugeInt()
        {
            using (var a = new HugeFloat("222509832503450298345029835740293845720/115756986668303657898962467957"))
            using (var b = new HugeInt("222987435987982730594288574029879874539"))
            using (var c = new HugeFloat(a * b))
            {
                Assert.AreEqual(a.Numerator * b, c.Numerator);
                Assert.AreEqual(a.Denominator, c.Denominator);
                c.Value = (b + 1) * a;
                Assert.AreEqual((b + 1) * a.Numerator, c.Numerator);
                Assert.AreEqual(a.Denominator, c.Denominator);
            }
        }

        [TestMethod]
        public void FloatMultiplyByLimb()
        {
            using (var a = new HugeFloat("90234098723098475098479385345098345/115756986668303657898962467957"))
            {
                ulong b = 17390538260286101342;
                a.Value = a * b;
                Assert.AreEqual("1569219546226477273686601978789044606491747469626478990/115756986668303657898962467957", a.ToString());
            }
        }

        [TestMethod]
        public void FloatMultiplyLimbBy()
        {
            using (var a = new HugeFloat("90234098723098475098479385345098345/115756986668303657898962467957"))
            {
                ulong b = 17390538260286101342;
                a.Value = b * a;
                Assert.AreEqual("1569219546226477273686601978789044606491747469626478990/115756986668303657898962467957", a.ToString());
            }
        }

        [TestMethod]
        public void FloatMultiplyBySignedLimb()
        {
            using (var a = new HugeFloat("90234098723098475098479385345098345/115756986668303657898962467957"))
            {
                long b = -7390538260286101342;
                a.Value = a * b;
                Assert.AreEqual("-666878558995492522701808125338061156491747469626478990/115756986668303657898962467957", a.ToString());
            }
        }

        [TestMethod]
        public void FloatMultiplySignedLimbBy()
        {
            using (var a = new HugeFloat("90234098723098475098479385345098345/115756986668303657898962467957"))
            {
                long b = -7390538260286101342;
                a.Value = b * a;
                Assert.AreEqual("-666878558995492522701808125338061156491747469626478990/115756986668303657898962467957", a.ToString());
            }
        }

        #endregion

        #region Shift Left

        [TestMethod]
        public void FloatShiftLeft()
        {
            using (var a = new HugeFloat("-12345700987ABCDEF2345CBDEFA245230948/17607EF654EB9A13FFA163C75", 16))
            {
                ulong b = 40;
                a.Value = a << b;
                Assert.AreEqual("-12345700987ABCDEF2345CBDEFA2452309480000000000/17607EF654EB9A13FFA163C75", a.ToString(16));
            }
        }

        #endregion

        #region Shift Right

        [TestMethod]
        public void FloatShiftRight()
        {
            using (var a = new HugeFloat("ABCDEF052834524092854092874502983745029345723098457209305983434345/17607EF654EB9A13FFA163C75", 16))
            {
                ulong b = 96;
                a.Value = a >> b;
                Assert.AreEqual("ABCDEF052834524092854092874502983745029345723098457209305983434345/17607EF654EB9A13FFA163C75000000000000000000000000", a.ToString(16));
            }
        }

        #endregion

        #region Negate

        [TestMethod]
        public void FloatNegate()
        {
            using (var a = new HugeFloat("24092854092874502983745029345723098457209/115756986668303657898962467957"))
            {
                a.Value = -a;
                Assert.AreEqual("-24092854092874502983745029345723098457209/115756986668303657898962467957", a.ToString());
                a.Value = -a;
                Assert.AreEqual("24092854092874502983745029345723098457209/115756986668303657898962467957", a.ToString());
            }
        }

        #endregion

        #region Abs

        [TestMethod]
        public void FloatMakeAbsolute()
        {
            using (var a = new HugeFloat("-24092854092874502983745029345723098457209/115756986668303657898962467957"))
            {
                a.Value = a.Abs();
                Assert.AreEqual("24092854092874502983745029345723098457209/115756986668303657898962467957", a.ToString());
                a.Value = a.Abs();
                Assert.AreEqual("24092854092874502983745029345723098457209/115756986668303657898962467957", a.ToString());
            }
        }

        #endregion

        #region Invert

        [TestMethod]
        public void FloatInvert()
        {
            using(var a = new HugeFloat("-24092854092874502983745029345723098457209/115756986668303657898962467957"))
            {
                a.Value = a.Invert();
                Assert.AreEqual("-115756986668303657898962467957/24092854092874502983745029345723098457209", a.ToString());
                a.Value = a.Invert();
                Assert.AreEqual("-24092854092874502983745029345723098457209/115756986668303657898962467957", a.ToString());
            }
        }

        #endregion

        #region Power

        [TestMethod]
        public void FloatPower()
        {
            using(var n = new HugeInt("-24092854092874502983745029345723098457209"))
            using(var d = new HugeInt("115756986668303657898962467957"))
            using(var a = new HugeFloat(n, d))
            {
                a.Value = a ^ 5;
                Assert.AreEqual(n ^ 5, a.Numerator);
                Assert.AreEqual(d ^ 5, a.Denominator);
            }
        }

        #endregion

        #region Divide

        #region Int

        [TestMethod]
        public void FloatDivideHugeFloat()
        {
            using (var a = new HugeFloat("115756986668303657898962467957/394580293847502987609283945873594873409587"))
            using (var b = new HugeFloat("593169091750307653294"))
            {
                a.Value = a / b;
                Assert.AreEqual("115756986668303657898962467957/234052834524092854092760134269509268758750275703033222451729578", a.ToString());
            }
        }

        [TestMethod]
        public void FloatDivideHugeFloat2()
        {
            using (var a = new HugeFloat("90234098723098475098479385345098345/115756986668303657898962467957"))
            using (var b = new HugeFloat("6847944682037444681162770672798288913849/590872612825179551336102196593"))
            using (var c = new HugeFloat(a / b))
            {
                Assert.AreEqual(a.Numerator * b.Denominator, c.Numerator);
                Assert.AreEqual(a.Denominator * b.Numerator, c.Denominator);
            }
        }

        [TestMethod]
        public void FloatDivideHugeInt()
        {
            using (var a = new HugeFloat("222509832503450298345029835740293845720/115756986668303657898962467957"))
            using (var b = new HugeInt("222987435987982730594288574029879874539"))
            using (var c = new HugeFloat(a / b))
            using (var d = new HugeFloat())
            {
                Assert.AreEqual(a.Numerator, c.Numerator);
                Assert.AreEqual(a.Denominator * b, c.Denominator);
                c.Value = (b + 1) / a;
                d.Numerator.Value = (b + 1) * a.Denominator;
                d.Denominator.Value = a.Numerator;
                d.Canonicalize();
                Assert.AreEqual(d, c);
            }
        }

        #endregion

        #region Limb

        [TestMethod]
        public void FloatDivideLimb()
        {
            using (var a = new HugeFloat("115756986668303657898962467957/39458029384750298767200622330399462537522498"))
            {
                ulong b = 5931690917503076532;
                a.Value = a / b;
                Assert.AreEqual("115756986668303657898962467957/234052834524092854092874502983745029345723092857791404165816936", a.ToString());
            }
        }

        [TestMethod]
        public void FloatDivideSignedLimb()
        {
            using(var a = new HugeFloat("115756986668303657898962467957/39458029384750298767200622330399462537522498"))
            {
                long b = -5931690917503076532;
                a.Value = a / b;
                Assert.AreEqual("-115756986668303657898962467957/234052834524092854092874502983745029345723092857791404165816936", a.ToString());
            }
        }

        [TestMethod]
        public void FloatDivideLimbBy()
        {
            using(var a = new HugeFloat("115756986668303657898962467957/39458029384750298767200622330399462537522498"))
            {
                ulong b = 5931690917503076532;
                a.Value = b / a;
                Assert.AreEqual("234052834524092854092874502983745029345723092857791404165816936/115756986668303657898962467957", a.ToString());
            }
        }

        [TestMethod]
        public void FloatDivideSignedLimbBy()
        {
            using(var a = new HugeFloat("115756986668303657898962467957/39458029384750298767200622330399462537522498"))
            {
                long b = -5931690917503076532;
                a.Value = b / a;
                Assert.AreEqual("-234052834524092854092874502983745029345723092857791404165816936/115756986668303657898962467957", a.ToString());
            }
        }

        #endregion

        #endregion
    }
}
