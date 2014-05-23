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

namespace MPIR.Tests.HugeRationalTests
{
    [TestClass]
    public class Arithmetic
    {
        #region Add

        [TestMethod]
        public void RationalAddHugeRational()
        {
            using (var a = new HugeRational("222509832503450298345029835740293845720/115756986668303657898962467957"))
            using (var b = new HugeRational("222987435987982730594288574029879874539/590872612825179551336102196593"))
            using (var c = new HugeRational(a + b))
            {
                Assert.AreEqual(a.Numerator * b.Denominator + b.Numerator * a.Denominator, c.Numerator);
                Assert.AreEqual(a.Denominator * b.Denominator, c.Denominator);
            }
        }

        [TestMethod]
        public void RationalAddHugeInt()
        {
            using (var a = new HugeRational("222509832503450298345029835740293845720/115756986668303657898962467957"))
            using (var b = new HugeInt("222987435987982730594288574029879874539"))
            using (var c = new HugeRational(a + b))
            {
                Assert.AreEqual(a.Numerator + b * a.Denominator, c.Numerator);
                Assert.AreEqual(a.Denominator, c.Denominator);
                c.Value = (b + 1) + a;
                Assert.AreEqual(a.Numerator + (b + 1) * a.Denominator, c.Numerator);
                Assert.AreEqual(a.Denominator, c.Denominator);
            }
        }

        [TestMethod]
        public void RationalAddLimb()
        {
            using (var a = new HugeRational("222509832503450298345029835740293845720/115756986668303657898962467957"))
            using (var c = new HugeRational())
            {
                var b = 4288574029879874539UL;
                c.Value = a + b;
                Assert.AreEqual(a.Numerator + b * a.Denominator, c.Numerator);
                Assert.AreEqual(a.Denominator, c.Denominator);
            }
        }

        [TestMethod]
        public void RationalAddToLimb()
        {
            using (var a = new HugeRational("222509832503450298345029835740293845720/115756986668303657898962467957"))
            using (var c = new HugeRational())
            {
                var b = 4288574029879874539UL;
                c.Value = b + a;
                Assert.AreEqual(a.Numerator + b * a.Denominator, c.Numerator);
                Assert.AreEqual(a.Denominator, c.Denominator);
            }
        }

        [TestMethod]
        public void RationalAddSignedLimb()
        {
            using (var a = new HugeRational("222509832503450298345029835740293845720/115756986668303657898962467957"))
            using (var c = new HugeRational())
            {
                var b = -4288574029879874539L;
                c.Value = a + b;
                Assert.AreEqual(a.Numerator + b * a.Denominator, c.Numerator);
                Assert.AreEqual(a.Denominator, c.Denominator);
            }
        }

        [TestMethod]
        public void RationalAddToSignedLimb()
        {
            using (var a = new HugeRational("222509832503450298345029835740293845720/115756986668303657898962467957"))
            using (var c = new HugeRational())
            {
                var b = -4288574029879874539L;
                c.Value = b + a;
                Assert.AreEqual(a.Numerator + b * a.Denominator, c.Numerator);
                Assert.AreEqual(a.Denominator, c.Denominator);
            }
        }

        [TestMethod]
        public void RationalAddToMaxSignedLimb()
        {
            using (var a = new HugeRational("222509832503450298345029835740293845720/115756986668303657898962467957"))
            using (var c = new HugeRational())
            {
                var b = long.MinValue;
                c.Value = b + a;
                Assert.AreEqual(a.Numerator + b * a.Denominator, c.Numerator);
                Assert.AreEqual(a.Denominator, c.Denominator);
            }
        }

        [TestMethod]
        public void RationalAddExpressionHugeRational()
        {
            using (var a = new HugeRational("222509832503450298345029835740293845720/115756986668303657898962467957"))
            using (var b = new HugeRational("222987435987982730594288574029879874539/590872612825179551336102196593"))
            using (var c = new HugeRational())
            {
                c.Value = 1 + (a + b);
                Assert.AreEqual(a.Numerator * b.Denominator + b.Numerator * a.Denominator + a.Denominator * b.Denominator, c.Numerator);
                Assert.AreEqual(a.Denominator * b.Denominator, c.Denominator);
            }
        }

        #endregion

        #region Subtract

        [TestMethod]
        public void RationalSubtractHugeRational()
        {
            using (var a = new HugeRational("445497268491433028939318409770173720259/115756986668303657898962467957"))
            using (var b = new HugeRational("222987435987982730594288574029879874539/590872612825179551336102196593"))
            using (var c = new HugeRational(a - b))
            {
                Assert.AreEqual(a.Numerator * b.Denominator - b.Numerator * a.Denominator, c.Numerator);
                Assert.AreEqual(a.Denominator * b.Denominator, c.Denominator);
            }
        }

        [TestMethod]
        public void RationalSubtractHugeInt()
        {
            using (var a = new HugeRational("222509832503450298345029835740293845720/115756986668303657898962467957"))
            using (var b = new HugeInt("222987435987982730594288574029879874539"))
            using (var c = new HugeRational(a - b))
            {
                Assert.AreEqual(a.Numerator - b * a.Denominator, c.Numerator);
                Assert.AreEqual(a.Denominator, c.Denominator);
                c.Value = (b + 1) - a;
                Assert.AreEqual((b + 1) * a.Denominator - a.Numerator, c.Numerator);
                Assert.AreEqual(a.Denominator, c.Denominator);
            }
        }

        [TestMethod]
        public void RationalSubtractLimb()
        {
            var b = 4288574029879874539UL;
            using (var a = new HugeRational("222509832503450298349318409770173720259/115756986668303657898962467957"))
            using (var c = new HugeRational(a - b))
            {
                Assert.AreEqual(a.Numerator - b * a.Denominator, c.Numerator);
                Assert.AreEqual(a.Denominator, c.Denominator);
            }
        }

        [TestMethod]
        public void RationalSubtractFromLimb()
        {
            var b = 4288574029879874539UL;
            using (var a = new HugeRational("222509832503450298349318409770173720259/115756986668303657898962467957"))
            using (var c = new HugeRational(b - a))
            {
                Assert.AreEqual(b * a.Denominator - a.Numerator, c.Numerator);
                Assert.AreEqual(a.Denominator, c.Denominator);
            }
        }

        [TestMethod]
        public void RationalSubtractPositiveSignedLimb()
        {
            var b = 4288574029879874539L;
            using (var a = new HugeRational("222509832503450298349318409770173720259/115756986668303657898962467957"))
            using (var c = new HugeRational(a - b))
            {
                Assert.AreEqual(a.Numerator - b * a.Denominator, c.Numerator);
                Assert.AreEqual(a.Denominator, c.Denominator);
            }
        }

        [TestMethod]
        public void RationalSubtractFromPositiveSignedLimb()
        {
            var b = 4288574029879874539L;
            using (var a = new HugeRational("222509832503450298349318409770173720259/115756986668303657898962467957"))
            using (var c = new HugeRational(a - b))
            {
                Assert.AreEqual(a.Numerator - b * a.Denominator, c.Numerator);
                Assert.AreEqual(a.Denominator, c.Denominator);
            }
        }

        [TestMethod]
        public void RationalSubtractSignedLimb()
        {
            var b = -4288574029879874539L;
            using (var a = new HugeRational("222509832503450298345029835740293845720/115756986668303657898962467957"))
            using (var c = new HugeRational(a - b))
            {
                Assert.AreEqual(a.Numerator - b * a.Denominator, c.Numerator);
                Assert.AreEqual(a.Denominator, c.Denominator);
            }
        }

        [TestMethod]
        public void RationalSubtractFromSignedLimb()
        {
            var b = -4288574029879874539L;
            using (var a = new HugeRational("222509832503450298345029835740293845720/115756986668303657898962467957"))
            using (var c = new HugeRational(b - a))
            {
                Assert.AreEqual(b * a.Denominator - a.Numerator, c.Numerator);
                Assert.AreEqual(a.Denominator, c.Denominator);
            }
        }

        #endregion

        #region Multiply

        [TestMethod]
        public void RationalMultiplyByHugeRational()
        {
            using (var a = new HugeRational("90234098723098475098479385345098345/115756986668303657898962467957"))
            using (var b = new HugeRational("7859487359873459872354987610987897/590872612825179551336102196593"))
            using (var c = new HugeRational(a * b))
            {
                Assert.AreEqual(a.Numerator * b.Numerator, c.Numerator);
                Assert.AreEqual(a.Denominator * b.Denominator, c.Denominator);
            }
        }

        [TestMethod]
        public void RationalMultiplytHugeInt()
        {
            using (var a = new HugeRational("222509832503450298345029835740293845720/115756986668303657898962467957"))
            using (var b = new HugeInt("222987435987982730594288574029879874539"))
            using (var c = new HugeRational(a * b))
            {
                Assert.AreEqual(a.Numerator * b, c.Numerator);
                Assert.AreEqual(a.Denominator, c.Denominator);
                c.Value = (b + 1) * a;
                Assert.AreEqual((b + 1) * a.Numerator, c.Numerator);
                Assert.AreEqual(a.Denominator, c.Denominator);
            }
        }

        [TestMethod]
        public void RationalMultiplyByLimb()
        {
            using (var a = new HugeRational("90234098723098475098479385345098345/115756986668303657898962467957"))
            {
                ulong b = 17390538260286101342;
                a.Value = a * b;
                Assert.AreEqual("1569219546226477273686601978789044606491747469626478990/115756986668303657898962467957", a.ToString());
            }
        }

        [TestMethod]
        public void RationalMultiplyLimbBy()
        {
            using (var a = new HugeRational("90234098723098475098479385345098345/115756986668303657898962467957"))
            {
                ulong b = 17390538260286101342;
                a.Value = b * a;
                Assert.AreEqual("1569219546226477273686601978789044606491747469626478990/115756986668303657898962467957", a.ToString());
            }
        }

        [TestMethod]
        public void RationalMultiplyBySignedLimb()
        {
            using (var a = new HugeRational("90234098723098475098479385345098345/115756986668303657898962467957"))
            {
                long b = -7390538260286101342;
                a.Value = a * b;
                Assert.AreEqual("-666878558995492522701808125338061156491747469626478990/115756986668303657898962467957", a.ToString());
            }
        }

        [TestMethod]
        public void RationalMultiplySignedLimbBy()
        {
            using (var a = new HugeRational("90234098723098475098479385345098345/115756986668303657898962467957"))
            {
                long b = -7390538260286101342;
                a.Value = b * a;
                Assert.AreEqual("-666878558995492522701808125338061156491747469626478990/115756986668303657898962467957", a.ToString());
            }
        }

        #endregion

        #region Shift Left

        [TestMethod]
        public void RationalShiftLeft()
        {
            using (var a = new HugeRational("-12345700987ABCDEF2345CBDEFA245230948/17607EF654EB9A13FFA163C75", 16))
            {
                ulong b = 40;
                a.Value = a << b;
                Assert.AreEqual("-12345700987ABCDEF2345CBDEFA2452309480000000000/17607EF654EB9A13FFA163C75", a.ToString(16));
            }
        }

        #endregion

        #region Shift Right

        [TestMethod]
        public void RationalShiftRight()
        {
            using (var a = new HugeRational("ABCDEF052834524092854092874502983745029345723098457209305983434345/17607EF654EB9A13FFA163C75", 16))
            {
                ulong b = 96;
                a.Value = a >> b;
                Assert.AreEqual("ABCDEF052834524092854092874502983745029345723098457209305983434345/17607EF654EB9A13FFA163C75000000000000000000000000", a.ToString(16));
            }
        }

        #endregion

        #region Negate

        [TestMethod]
        public void RationalNegate()
        {
            using (var a = new HugeRational("24092854092874502983745029345723098457209/115756986668303657898962467957"))
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
        public void RationalMakeAbsolute()
        {
            using (var a = new HugeRational("-24092854092874502983745029345723098457209/115756986668303657898962467957"))
            {
                a.Value = a.Abs();
                Assert.AreEqual("24092854092874502983745029345723098457209/115756986668303657898962467957", a.ToString());
                a.Value = a.Abs();
                Assert.AreEqual("24092854092874502983745029345723098457209/115756986668303657898962467957", a.ToString());
            }
        }

        #endregion

        #region Divide

        #region Int

        [TestMethod]
        public void RationalDivideHugeRational()
        {
            using (var a = new HugeRational("115756986668303657898962467957/394580293847502987609283945873594873409587"))
            using (var b = new HugeRational("593169091750307653294"))
            {
                a.Value = a / b;
                Assert.AreEqual("115756986668303657898962467957/234052834524092854092760134269509268758750275703033222451729578", a.ToString());
            }
        }

        [TestMethod]
        public void RationalDivideHugeRational2()
        {
            using (var a = new HugeRational("90234098723098475098479385345098345/115756986668303657898962467957"))
            using (var b = new HugeRational("6847944682037444681162770672798288913849/590872612825179551336102196593"))
            using (var c = new HugeRational(a / b))
            {
                Assert.AreEqual(a.Numerator * b.Denominator, c.Numerator);
                Assert.AreEqual(a.Denominator * b.Numerator, c.Denominator);
            }
        }

        #endregion

        #region Limb

        [TestMethod]
        public void RationalDivideLimb()
        {
            using (var a = new HugeRational("115756986668303657898962467957/39458029384750298767200622330399462537522498"))
            {
                ulong b = 5931690917503076532;
                a.Value = a / b;
                Assert.AreEqual("115756986668303657898962467957/234052834524092854092874502983745029345723092857791404165816936", a.ToString());
            }
        }

        #endregion

        #endregion
    }
}
