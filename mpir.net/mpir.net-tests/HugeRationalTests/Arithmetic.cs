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
                var b = Platform.Ui(4288574029879874539UL, 4288574029U);
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
                var b = Platform.Ui(4288574029879874539UL, 4279874539U);
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
                var b = Platform.Si(-4288574029879874539L, -1279874539);
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
                var b = Platform.Si(-4288574029879874539L, -1279874539);
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
                var b = Platform.Si(long.MinValue, int.MinValue);
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
                var b = Platform.Ui(4288574029879874539UL, 2885740298U);
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
                var b = Platform.Ui(4288574029879874539UL, 2885740298U);
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
                var b = Platform.Si(4288574029879874539L, 1885740298);
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
                var b = Platform.Si(4288574029879874539L, 1885740298);
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
                var b = Platform.Si(-4288574029879874539L, -1885740298);
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
                var b = Platform.Si(-4288574029879874539L, -1885740298);
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
                var b = Platform.Ui(17390538260286101342, 1500450271);
                a.Value = a * b;
                Assert.AreEqual(Platform.Select(
                    "1569219546226477273686601978789044606491747469626478990/115756986668303657898962467957",
                    "135391777882513860921200145428966240276901495/115756986668303657898962467957"),
                    a.ToString());
            }
        }

        [TestMethod]
        public void RationalMultiplyLimbBy()
        {
            using (var a = new HugeRational("90234098723098475098479385345098345/115756986668303657898962467957"))
            {
                var b = Platform.Ui(17390538260286101342, 1500450271);
                a.Value = b * a;
                Assert.AreEqual(Platform.Select(
                    "1569219546226477273686601978789044606491747469626478990/115756986668303657898962467957",
                    "135391777882513860921200145428966240276901495/115756986668303657898962467957"),
                    a.ToString());
            }
        }

        [TestMethod]
        public void RationalMultiplyBySignedLimb()
        {
            using (var a = new HugeRational("90234098723098475098479385345098345/115756986668303657898962467957"))
            {
                var b = Platform.Si(-7390538260286101342, -1987450271);
                a.Value = a * b;
                Assert.AreEqual(Platform.Select(
                    "-666878558995492522701808125338061156491747469626478990/115756986668303657898962467957",
                    "-179335783960662818294159606092029134291901495/115756986668303657898962467957"),
                    a.ToString());
            }
        }

        [TestMethod]
        public void RationalMultiplySignedLimbBy()
        {
            using (var a = new HugeRational("90234098723098475098479385345098345/115756986668303657898962467957"))
            {
                var b = Platform.Si(-7390538260286101342, -1987450271);
                a.Value = b * a;
                Assert.AreEqual(Platform.Select(
                    "-666878558995492522701808125338061156491747469626478990/115756986668303657898962467957",
                    "-179335783960662818294159606092029134291901495/115756986668303657898962467957"),
                    a.ToString());
            }
        }

        #endregion

        #region Shift Left

        [TestMethod]
        public void RationalShiftLeft()
        {
            using (var a = new HugeRational("-12345700987ABCDEF2345CBDEFA245230948/17607EF654EB9A13FFA163C75", 16))
            {
                uint b = 40;
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
                uint b = 96;
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

        #region Invert

        [TestMethod]
        public void RationalInvert()
        {
            using(var a = new HugeRational("-24092854092874502983745029345723098457209/115756986668303657898962467957"))
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
        public void RationalPower()
        {
            using(var n = new HugeInt("-24092854092874502983745029345723098457209"))
            using(var d = new HugeInt("115756986668303657898962467957"))
            using(var a = new HugeRational(n, d))
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

        [TestMethod]
        public void RationalDivideHugeInt()
        {
            using (var a = new HugeRational("222509832503450298345029835740293845720/115756986668303657898962467957"))
            using (var b = new HugeInt("222987435987982730594288574029879874539"))
            using (var c = new HugeRational(a / b))
            using (var d = new HugeRational())
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
        public void RationalDivideLimb()
        {
            using (var a = new HugeRational("115756986668303657898962467957/39458029384750298767200622330399462537522498"))
            {
                var b = Platform.Ui(5931690917503076532, 3367900313);
                a.Value = a / b;
                Assert.AreEqual(Platform.Select(
                    "115756986668303657898962467957/234052834524092854092874502983745029345723092857791404165816936",
                    "115756986668303657898962467957/132890709515263728644898490080347139295153795258741874"),
                    a.ToString());
            }
        }

        [TestMethod]
        public void RationalDivideSignedLimb()
        {
            using(var a = new HugeRational("115756986668303657898962467957/39458029384750298767200622330399462537522498"))
            {
                var b = Platform.Si(-5931690917503076532, -1500450271);
                a.Value = a / b;
                Assert.AreEqual(Platform.Select(
                    "-115756986668303657898962467957/234052834524092854092874502983745029345723092857791404165816936",
                    "-115756986668303657898962467957/59204810883474549052577139687016525102679979792696958"),
                    a.ToString());
            }
        }

        [TestMethod]
        public void RationalDivideLimbBy()
        {
            using(var a = new HugeRational("115756986668303657898962467957/39458029384750298767200622330399462537522498"))
            {
                var b = Platform.Ui(5931690917503076532, 3367900313);
                a.Value = b / a;
                Assert.AreEqual(Platform.Select(
                    "234052834524092854092874502983745029345723092857791404165816936/115756986668303657898962467957",
                    "132890709515263728644898490080347139295153795258741874/115756986668303657898962467957"),
                    a.ToString());
            }
        }

        [TestMethod]
        public void RationalDivideSignedLimbBy()
        {
            using(var a = new HugeRational("115756986668303657898962467957/39458029384750298767200622330399462537522498"))
            {
                var b = Platform.Si(-5931690917503076532, -1500450271);
                a.Value = b / a;
                Assert.AreEqual(Platform.Select(
                    "-234052834524092854092874502983745029345723092857791404165816936/115756986668303657898962467957",
                    "-59204810883474549052577139687016525102679979792696958/115756986668303657898962467957"),
                    a.ToString());
            }
        }

        #endregion

        #endregion
    }
}
