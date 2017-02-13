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
    public class Arithmetic
    {
        #region Add

        [TestMethod]
        public void IntAddHugeInt()
        {
            using (var a = new HugeInt("222509832503450298345029835740293845720"))
            using (var b = new HugeInt("222987435987982730594288574029879874539"))
            using (var c = new HugeInt())
            {
                c.Value = a + b;
                Assert.AreEqual("445497268491433028939318409770173720259", c.ToString());
            }
        }

        [TestMethod]
        public void IntAddLimb()
        {
            using (var a = new HugeInt("222509832503450298345029835740293845720"))
            {
                var b = Platform.Ui(4288574029879874539UL, 4288574029U);
                a.Value = a + b;
                Assert.AreEqual(Platform.Select("222509832503450298349318409770173720259", "222509832503450298345029835744582419749"), a.ToString());
            }
        }

        [TestMethod]
        public void IntAddToLimb()
        {
            using (var a = new HugeInt("222509832503450298345029835740293845720"))
            {
                var b = Platform.Ui(4288574029879874539UL, 4288574029U);
                a.Value = b + a;
                Assert.AreEqual(Platform.Select("222509832503450298349318409770173720259", "222509832503450298345029835744582419749"), a.ToString());
            }
        }

        [TestMethod]
        public void IntAddSignedLimb()
        {
            using (var a = new HugeInt("222509832503450298345029835740293845720"))
            {
                var b = Platform.Si(-4288574029879874539L, -1288574029);
                a.Value = a + b;
                Assert.AreEqual(Platform.Select("222509832503450298340741261710413971181", "222509832503450298345029835739005271691"), a.ToString());
            }
        }

        [TestMethod]
        public void IntAddToSignedLimb()
        {
            using (var a = new HugeInt("222509832503450298345029835740293845720"))
            {
                var b = Platform.Si(-4288574029879874539L, -1288574029);
                a.Value = b + a;
                Assert.AreEqual(Platform.Select("222509832503450298340741261710413971181", "222509832503450298345029835739005271691"), a.ToString());
            }
        }

        [TestMethod]
        public void IntAddToMaxSignedLimb()
        {
            using (var a = new HugeInt("222509832503450298345029835740293845720"))
            {
                var b = Platform.Si(long.MinValue, int.MinValue);
                a.Value = b + a;
                Assert.AreEqual(Platform.Select("222509832503450298335806463703439069912", "222509832503450298345029835738146362072"), a.ToString());
            }
        }

        [TestMethod]
        public void IntAddExpressionHugeInt()
        {
            using (var a = new HugeInt("222509832503450298345029835740293845720"))
            using (var b = new HugeInt("222987435987982730594288574029879874539"))
            using (var c = new HugeInt())
            {
                c.Value = 1 + (a + b);
                Assert.AreEqual("445497268491433028939318409770173720260", c.ToString());
            }
        }

        #endregion

        #region Subtract

        [TestMethod]
        public void IntSubtractHugeInt()
        {
            using (var a = new HugeInt("445497268491433028939318409770173720259"))
            using (var b = new HugeInt("222987435987982730594288574029879874539"))
            {
                a.Value = a - b;
                Assert.AreEqual("222509832503450298345029835740293845720", a.ToString());
            }
        }

        [TestMethod]
        public void IntSubtractLimb()
        {
            using (var a = new HugeInt("222509832503450298349318409770173720259"))
            {
                var b = Platform.Ui(4288574029879874539UL, 2885740298U);
                a.Value = a - b;
                Assert.AreEqual(Platform.Select("222509832503450298345029835740293845720", "222509832503450298349318409767287979961"), a.ToString());
            }
        }

        [TestMethod]
        public void IntSubtractFromLimb()
        {
            using (var a = new HugeInt("222509832503450298349318409770173720259"))
            {
                var b = Platform.Ui(4288574029879874539UL, 2885740298U);
                a.Value = b - a;
                Assert.AreEqual(Platform.Select("-222509832503450298345029835740293845720", "-222509832503450298349318409767287979961"), a.ToString());
            }
        }

        [TestMethod]
        public void IntSubtractPositiveSignedLimb()
        {
            using (var a = new HugeInt("222509832503450298349318409770173720259"))
            {
                var b = Platform.Si(4288574029879874539L, 1885740298);
                a.Value = a - b;
                Assert.AreEqual(Platform.Select("222509832503450298345029835740293845720", "222509832503450298349318409768287979961"), a.ToString());
            }
        }

        [TestMethod]
        public void IntSubtractFromPositiveSignedLimb()
        {
            using (var a = new HugeInt("222509832503450298349318409770173720259"))
            {
                var b = Platform.Si(4288574029879874539L, 1885740298);
                a.Value = b - a;
                Assert.AreEqual(Platform.Select("-222509832503450298345029835740293845720", "-222509832503450298349318409768287979961"), a.ToString());
            }
        }

        [TestMethod]
        public void IntSubtractSignedLimb()
        {
            using (var a = new HugeInt("222509832503450298345029835740293845720"))
            {
                var b = Platform.Si(-4288574029879874539L, -1885740298);
                a.Value = a - b;
                Assert.AreEqual(Platform.Select("222509832503450298349318409770173720259", "222509832503450298345029835742179586018"), a.ToString());
            }
        }

        [TestMethod]
        public void IntSubtractFromSignedLimb()
        {
            using (var a = new HugeInt("222509832503450298345029835740293845720"))
            {
                var b = Platform.Si(-4288574029879874539L, -1885740298);
                a.Value = b - a;
                Assert.AreEqual(Platform.Select("-222509832503450298349318409770173720259", "-222509832503450298345029835742179586018"), a.ToString());
            }
        }

        #endregion

        #region Multiply

        [TestMethod]
        public void IntMultiplyByHugeInt()
        {
            using (var a = new HugeInt("90234098723098475098479385345098345"))
            using (var b = new HugeInt("7859487359873459872354987610987897"))
            {
                a.Value = a * b;
                Assert.AreEqual("709193758343766370701419953880162061353595657143399816050772069730465", a.ToString());
            }
        }

        [TestMethod]
        public void IntMultiplyByLimb()
        {
            using (var a = new HugeInt("90234098723098475098479385345098345"))
            {
                var b = Platform.Ui(17390538260286101342, 1500450271);
                a.Value = a * b;
                Assert.AreEqual(Platform.Select("1569219546226477273686601978789044606491747469626478990", "135391777882513860921200145428966240276901495"), a.ToString());
            }
        }

        [TestMethod]
        public void IntMultiplyLimbBy()
        {
            using (var a = new HugeInt("90234098723098475098479385345098345"))
            {
                var b = Platform.Ui(17390538260286101342, 1500450271);
                a.Value = b * a;
                Assert.AreEqual(Platform.Select("1569219546226477273686601978789044606491747469626478990", "135391777882513860921200145428966240276901495"), a.ToString());
            }
        }

        [TestMethod]
        public void IntMultiplyBySignedLimb()
        {
            using (var a = new HugeInt("90234098723098475098479385345098345"))
            {
                var b = Platform.Si(-7390538260286101342, -1500450271);
                a.Value = a * b;
                Assert.AreEqual(Platform.Select("-666878558995492522701808125338061156491747469626478990", "-135391777882513860921200145428966240276901495"), a.ToString());
            }
        }

        [TestMethod]
        public void IntMultiplySignedLimbBy()
        {
            using (var a = new HugeInt("90234098723098475098479385345098345"))
            {
                var b = Platform.Si(-7390538260286101342, -1500450271);
                a.Value = b * a;
                Assert.AreEqual(Platform.Select("-666878558995492522701808125338061156491747469626478990", "-135391777882513860921200145428966240276901495"), a.ToString());
            }
        }

        #endregion

        #region Add Product

        [TestMethod]
        public void IntAddProductHugeInt()
        {
            using (var a = new HugeInt("98750293847520938457029384572093480498357"))
            using (var c = new HugeInt("23094582093845093574093845093485039450934"))
            using (var b = new HugeInt("-394580293847502987609283945873594873409587"))
            {
                var expr = a + c*b;
                a.Value = expr;
                Assert.AreEqual("-9112666988874677841199955832262586145147830205230375090322356322089362221491205901", a.ToString());
            }
        }

        [TestMethod]
        public void IntAddProductHugeInt2()
        {
            using (var a = new HugeInt("98750293847520938457029384572093480498357"))
            using (var c = new HugeInt("23094582093845093574093845093485039450934"))
            using (var b = new HugeInt("-394580293847502987609283945873594873409587"))
            {
                var expr = c * b + a;
                a.Value = expr;
                Assert.AreEqual("-9112666988874677841199955832262586145147830205230375090322356322089362221491205901", a.ToString());
            }
        }

        [TestMethod]
        public void IntAddProductLimb()
        {
            using (var a = new HugeInt("98750293847520938457029384572093480498357"))
            using (var c = new HugeInt("-23094582093845093574093845093485039450934"))
            {
                var b = Platform.Ui(498734523097853458, 3997853458);
                var expr = a + c*b;
                a.Value = expr;
                Assert.AreEqual(Platform.Select("-11518065386718058599763388064972875060082210203928832731415", "-92328754786193194014003719366476113668089432731415"), a.ToString());
            }
        }

        [TestMethod]
        public void IntAddProductLimbTo()
        {
            using (var a = new HugeInt("98750293847520938457029384572093480498357"))
            using (var c = new HugeInt("-23094582093845093574093845093485039450934"))
            {
                var b = Platform.Ui(498734523097853458, 3997853458);
                var expr = a + b*c;
                a.Value = expr;
                Assert.AreEqual(Platform.Select("-11518065386718058599763388064972875060082210203928832731415", "-92328754786193194014003719366476113668089432731415"), a.ToString());
            }
        }

        [TestMethod]
        public void IntAddProductLimbTo2()
        {
            using (var a = new HugeInt("98750293847520938457029384572093480498357"))
            using (var c = new HugeInt("-23094582093845093574093845093485039450934"))
            {
                var b = Platform.Ui(498734523097853458, 3997853458);
                var expr = b * c + a;
                a.Value = expr;
                Assert.AreEqual(Platform.Select("-11518065386718058599763388064972875060082210203928832731415", "-92328754786193194014003719366476113668089432731415"), a.ToString());
            }
        }

        [TestMethod]
        public void IntAddProductLimbTo3()
        {
            using (var a = new HugeInt("98750293847520938457029384572093480498357"))
            using (var c = new HugeInt("-23094582093845093574093845093485039450934"))
            using (var d = new HugeInt())
            {
                var b = Platform.Ui(498734523097853458, 3997853458);
                var expr = b * c + a;
                d.Value = expr;
                Assert.AreEqual(Platform.Select("-11518065386718058599763388064972875060082210203928832731415", "-92328754786193194014003719366476113668089432731415"), d.ToString());
            }
        }

        [TestMethod]
        public void IntAddProductSignedLimb()
        {
            using (var a = new HugeInt("98750293847520938457029384572093480498357"))
            using (var c = new HugeInt("23094582093845093574093845093485039450934"))
            {
                var b = Platform.Si(-498734523097853458, -2017853458);
                var expr = a + c*b;
                a.Value = expr;
                Assert.AreEqual(Platform.Select("-11518065386718058599763388064972875060082210203928832731415", "-46601482240379908737297906081375735555240112731415"), a.ToString());
            }
        }

        [TestMethod]
        public void IntAddProductSignedLimbTo()
        {
            using (var a = new HugeInt("98750293847520938457029384572093480498357"))
            using (var c = new HugeInt("-23094582093845093574093845093485039450934"))
            {
                var b = Platform.Si(498734523097853458, 2017853458);
                var expr = a + b*c;
                a.Value = expr;
                Assert.AreEqual(Platform.Select("-11518065386718058599763388064972875060082210203928832731415", "-46601482240379908737297906081375735555240112731415"), a.ToString());
            }
        }

        [TestMethod]
        public void IntAddProductSignedLimbTo2()
        {
            using (var a = new HugeInt("98750293847520938457029384572093480498357"))
            using (var c = new HugeInt("-23094582093845093574093845093485039450934"))
            {
                var b = Platform.Si(498734523097853458, 2017853458);
                var expr = b * c + a;
                a.Value = expr;
                Assert.AreEqual(Platform.Select("-11518065386718058599763388064972875060082210203928832731415", "-46601482240379908737297906081375735555240112731415"), a.ToString());
            }
        }

        [TestMethod]
        public void IntAddProductSignedLimbTo3()
        {
            using (var a = new HugeInt("98750293847520938457029384572093480498357"))
            using (var c = new HugeInt("23094582093845093574093845093485039450934"))
            using (var d = new HugeInt())
            {
                var b = Platform.Si(-498734523097853458, -2017853458);
                var expr = b * c + a;
                d.Value = expr;
                Assert.AreEqual(Platform.Select("-11518065386718058599763388064972875060082210203928832731415", "-46601482240379908737297906081375735555240112731415"), d.ToString());
            }
        }

        [TestMethod]
        public void IntAddProductSignedLimbTo4()
        {
            using (var a = new HugeInt("98750293847520938457029384572093480498357"))
            using (var c = new HugeInt("23094582093845093574093845093485039450934"))
            {
                var b = Platform.Si(-498734523097853458, -2017853458);
                var expr = b * c + a;
                a.Value = expr;
                Assert.AreEqual(Platform.Select("-11518065386718058599763388064972875060082210203928832731415", "-46601482240379908737297906081375735555240112731415"), a.ToString());
            }
        }

        #endregion

        #region Subtract Product

        [TestMethod]
        public void IntSubtractProductHugeInt()
        {
            using (var a = new HugeInt("98750293847520938457029384572093480498357"))
            using (var c = new HugeInt("23094582093845093574093845093485039450934"))
            using (var b = new HugeInt("394580293847502987609283945873594873409587"))
            {
                var expr = a - c*b;
                a.Value = expr;
                Assert.AreEqual("-9112666988874677841199955832262586145147830205230375090322356322089362221491205901", a.ToString());
            }
        }

        [TestMethod]
        public void IntSubtractProductHugeIntFrom()
        {
            using (var a = new HugeInt("98750293847520938457029384572093480498359"))
            using (var c = new HugeInt("23094582093845093574093845093485039450934"))
            using (var b = new HugeInt("394580293847502987609283945873594873409587"))
            {
                var expr = c * b - a;
                a.Value = expr;
                Assert.AreEqual("9112666988874677841199955832262586145147830205230375090322356322089362221491205899", a.ToString());
            }
        }

        [TestMethod]
        public void IntSubtractProductLimb()
        {
            using (var a = new HugeInt("98750293847520938457029384572093480498357"))
            using (var c = new HugeInt("23094582093845093574093845093485039450934"))
            {
                var b = Platform.Ui(498734523097853458, 3997853458);
                var expr = a - c*b;
                a.Value = expr;
                Assert.AreEqual(Platform.Select("-11518065386718058599763388064972875060082210203928832731415", "-92328754786193194014003719366476113668089432731415"), a.ToString());
            }
        }

        [TestMethod]
        public void IntSubtractProductLimbFrom()
        {
            using (var a = new HugeInt("98750293847520938457029384572093480498359"))
            using (var c = new HugeInt("23094582093845093574093845093485039450934"))
            {
                var b = Platform.Ui(498734523097853458, 3997853458);
                var expr = c * b - a;
                a.Value = expr;
                Assert.AreEqual(Platform.Select("11518065386718058599763388064972875060082210203928832731413", "92328754786193194014003719366476113668089432731413"), a.ToString());
            }
        }

        [TestMethod]
        public void IntSubtractProductSignedLimb()
        {
            using (var a = new HugeInt("98750293847520938457029384572093480498357"))
            using (var c = new HugeInt("-23094582093845093574093845093485039450934"))
            {
                var b = Platform.Si(-498734523097853458, -2017853458);
                var expr = a - b*c;
                a.Value = expr;
                Assert.AreEqual(Platform.Select("-11518065386718058599763388064972875060082210203928832731415", "-46601482240379908737297906081375735555240112731415"), a.ToString());
            }
        }

        [TestMethod]
        public void IntSubtractProductSignedLimbFrom()
        {
            using (var a = new HugeInt("98750293847520938457029384572093480498359"))
            using (var c = new HugeInt("-23094582093845093574093845093485039450934"))
            {
                var b = Platform.Si(-498734523097853458, -2017853458);
                var expr = b * c - a;
                a.Value = expr;
                Assert.AreEqual(Platform.Select("11518065386718058599763388064972875060082210203928832731413", "46601482240379908737297906081375735555240112731413"), a.ToString());
            }
        }

        [TestMethod]
        public void IntSubtractProductSignedLimb2()
        {
            using (var a = new HugeInt("98750293847520938457029384572093480498357"))
            using (var c = new HugeInt("-23094582093845093574093845093485039450934"))
            {
                var b = Platform.Si(-498734523097853458, -2017853458);
                a.Value = a - c*b;
                Assert.AreEqual(Platform.Select("-11518065386718058599763388064972875060082210203928832731415", "-46601482240379908737297906081375735555240112731415"), a.ToString());
            }
        }

        [TestMethod]
        public void IntSubtractProductSignedLimb3()
        {
            using (var a = new HugeInt("98750293847520938457029384572093480498357"))
            using (var c = new HugeInt("23094582093845093574093845093485039450934"))
            using (var d = new HugeInt())
            {
                var b = Platform.Si(498734523097853458, 2017853458);
                d.Value = a - c*b;
                Assert.AreEqual(Platform.Select("-11518065386718058599763388064972875060082210203928832731415", "-46601482240379908737297906081375735555240112731415"), d.ToString());
            }
        }

        [TestMethod]
        public void IntSubtractProductSignedLimbFrom3()
        {
            using (var a = new HugeInt("98750293847520938457029384572093480498359"))
            using (var c = new HugeInt("-23094582093845093574093845093485039450934"))
            {
                var b = Platform.Si(498734523097853458, 2017853458);
                a.Value = c * b - a;
                Assert.AreEqual(Platform.Select("-11518065386718058797263975760014751974140979348115793728131", "-46601482437880496432339782995434504699427073728131"), a.ToString());
            }
        }

        [TestMethod]
        public void IntSubtractProductSignedLimb4()
        {
            using (var a = new HugeInt("98750293847520938457029384572093480498357"))
            using (var c = new HugeInt("23094582093845093574093845093485039450934"))
            {
                var b = Platform.Si(498734523097853458, 2017853458);
                a.Value = a - c * b;
                Assert.AreEqual(Platform.Select("-11518065386718058599763388064972875060082210203928832731415", "-46601482240379908737297906081375735555240112731415"), a.ToString());
            }
        }

        #endregion

        #region Shift Left

        [TestMethod]
        public void IntShiftLeft()
        {
            using (var a = new HugeInt("-12345700987ABCDEF2345CBDEFA245230948", 16))
            {
                uint b = 40;
                a.Value = a << b;
                Assert.AreEqual("-12345700987ABCDEF2345CBDEFA2452309480000000000", a.ToString(16));
            }
        }

        #endregion

        #region Shift Right

        [TestMethod]
        public void IntShiftRight()
        {
            using (var a = new HugeInt("ABCDEF052834524092854092874502983745029345723098457209305983434345", 16))
            {
                uint b = 96;
                a.Value = a >> b;
                Assert.AreEqual("ABCDEF052834524092854092874502983745029345", a.ToString(16));
            }
        }

        [TestMethod]
        public void IntShiftRightCeiling()
        {
            using (var a = new HugeInt("ABCDEF052834524092854092874502983745029345723098457209305983434345", 16))
            {
                uint b = 96;
                a.Value = (a >> b).Rounding(RoundingModes.Ceiling);
                Assert.AreEqual("ABCDEF052834524092854092874502983745029346", a.ToString(16));
            }
        }

        [TestMethod]
        public void IntShiftRightNegativeCeiling()
        {
            using (var a = new HugeInt("-ABCDEF052834524092854092874502983745029345723098457209305983434345", 16))
            {
                uint b = 96;
                a.Value = (a >> b).Rounding(RoundingModes.Ceiling);
                Assert.AreEqual("-ABCDEF052834524092854092874502983745029345", a.ToString(16));
            }
        }

        [TestMethod]
        public void IntShiftRightFloor()
        {
            using (var a = new HugeInt("ABCDEF052834524092854092874502983745029345723098457209305983434345", 16))
            {
                uint b = 96;
                a.Value = (a >> b).Rounding(RoundingModes.Floor);
                Assert.AreEqual("ABCDEF052834524092854092874502983745029345", a.ToString(16));
            }
        }

        [TestMethod]
        public void IntShiftRightNegativeFloor()
        {
            using (var a = new HugeInt("-ABCDEF052834524092854092874502983745029345723098457209305983434345", 16))
            {
                uint b = 96;
                a.Value = (a >> b).Rounding(RoundingModes.Floor);
                Assert.AreEqual("-ABCDEF052834524092854092874502983745029346", a.ToString(16));
            }
        }

        [TestMethod]
        public void IntShiftRightTruncate()
        {
            using (var a = new HugeInt("ABCDEF052834524092854092874502983745029345723098457209305983434345", 16))
            {
                uint b = 96;
                a.Value = (a >> b).Rounding(RoundingModes.Truncate);
                Assert.AreEqual("ABCDEF052834524092854092874502983745029345", a.ToString(16));
            }
        }

        [TestMethod]
        public void IntShiftRightNegativeTruncate()
        {
            using (var a = new HugeInt("-ABCDEF052834524092854092874502983745029345723098457209305983434345", 16))
            {
                uint b = 96;
                a.Value = (a >> b).Rounding(RoundingModes.Truncate);
                Assert.AreEqual("-ABCDEF052834524092854092874502983745029345", a.ToString(16));
            }
        }

        #endregion

        #region Negate

        [TestMethod]
        public void IntNegate()
        {
            using (var a = new HugeInt("24092854092874502983745029345723098457209"))
            {
                a.Value = -a;
                Assert.AreEqual("-24092854092874502983745029345723098457209", a.ToString());
                a.Value = -a;
                Assert.AreEqual("24092854092874502983745029345723098457209", a.ToString());
            }
        }

        #endregion

        #region Abs

        [TestMethod]
        public void IntMakeAbsolute()
        {
            using (var a = new HugeInt("-24092854092874502983745029345723098457209"))
            {
                a.Value = a.Abs();
                Assert.AreEqual("24092854092874502983745029345723098457209", a.ToString());
                a.Value = a.Abs();
                Assert.AreEqual("24092854092874502983745029345723098457209", a.ToString());
            }
        }

        #endregion

        #region Divide

        #region Int

        [TestMethod]
        public void IntDivideHugeInt()
        {
            using (var a = new HugeInt("234052834524092854092874502983745029345723098457209305983434345"))
            using (var b = new HugeInt("394580293847502987609283945873594873409587"))
            {
                a.Value = a / b;
                Assert.AreEqual("593169091750307653294", a.ToString());
            }
        }

        [TestMethod]
        public void IntDivideHugeIntCeiling()
        {
            using (var a = new HugeInt("234052834524092854092874502983745029345723098457209305983434345"))
            using (var b = new HugeInt("394580293847502987609283945873594873409587"))
            {
                a.Value = (a / b).Rounding(RoundingModes.Ceiling);
                Assert.AreEqual("593169091750307653295", a.ToString());
            }
        }

        [TestMethod]
        public void IntDivideHugeIntNegativeCeiling()
        {
            using (var a = new HugeInt("-234052834524092854092874502983745029345723098457209305983434345"))
            using (var b = new HugeInt("394580293847502987609283945873594873409587"))
            {
                a.Value = (a / b).Rounding(RoundingModes.Ceiling);
                Assert.AreEqual("-593169091750307653294", a.ToString());
            }
        }

        [TestMethod]
        public void IntDivideHugeIntFloor()
        {
            using (var a = new HugeInt("234052834524092854092874502983745029345723098457209305983434345"))
            using (var b = new HugeInt("394580293847502987609283945873594873409587"))
            {
                a.Value = (a / b).Rounding(RoundingModes.Floor);
                Assert.AreEqual("593169091750307653294", a.ToString());
            }
        }

        [TestMethod]
        public void IntDivideHugeIntNegativeFloor()
        {
            using (var a = new HugeInt("-234052834524092854092874502983745029345723098457209305983434345"))
            using (var b = new HugeInt("394580293847502987609283945873594873409587"))
            {
                a.Value = (a / b).Rounding(RoundingModes.Floor);
                Assert.AreEqual("-593169091750307653295", a.ToString());
            }
        }

        [TestMethod]
        public void IntDivideHugeIntTruncate()
        {
            using (var a = new HugeInt("234052834524092854092874502983745029345723098457209305983434345"))
            using (var b = new HugeInt("394580293847502987609283945873594873409587"))
            {
                a.Value = (a / b).Rounding(RoundingModes.Truncate);
                Assert.AreEqual("593169091750307653294", a.ToString());
            }
        }

        [TestMethod]
        public void IntDivideHugeIntNegativeTruncate()
        {
            using (var a = new HugeInt("-234052834524092854092874502983745029345723098457209305983434345"))
            using (var b = new HugeInt("394580293847502987609283945873594873409587"))
            {
                a.Value = (a / b).Rounding(RoundingModes.Truncate);
                Assert.AreEqual("-593169091750307653294", a.ToString());
            }
        }

        #endregion

        #region Int with mod

        [TestMethod]
        public void IntDivideHugeIntCeilingWithMod()
        {
            using (var a = new HugeInt("234052834524092854092874502983745029345723098457209305983434345"))
            using (var b = new HugeInt("394580293847502987609283945873594873409587"))
            using (var c = new HugeInt())
            {
                a.Value = (a / b).SavingRemainderTo(c).Rounding(RoundingModes.Ceiling);
                Assert.AreEqual("593169091750307653295", a.ToString());
                Assert.AreEqual("-280211579611742400636461191697511341704820", c.ToString());
            }
        }

        [TestMethod]
        public void IntDivideHugeIntNegativeCeilingWithMod()
        {
            using (var a = new HugeInt("-234052834524092854092874502983745029345723098457209305983434345"))
            using (var b = new HugeInt("394580293847502987609283945873594873409587"))
            using (var c = new HugeInt())
            {
                a.Value = (a / b).SavingRemainderTo(c).Rounding(RoundingModes.Ceiling);
                Assert.AreEqual("-593169091750307653294", a.ToString());
                Assert.AreEqual("-114368714235760586972822754176083531704767", c.ToString());
            }
        }

        [TestMethod]
        public void IntDivideHugeIntFloorWithMod()
        {
            using (var a = new HugeInt("234052834524092854092874502983745029345723098457209305983434345"))
            using (var b = new HugeInt("394580293847502987609283945873594873409587"))
            using (var c = new HugeInt())
            {
                a.Value = (a / b).SavingRemainderTo(c).Rounding(RoundingModes.Floor);
                Assert.AreEqual("593169091750307653294", a.ToString());
                Assert.AreEqual("114368714235760586972822754176083531704767", c.ToString());
            }
        }

        [TestMethod]
        public void IntDivideHugeIntNegativeFloorWithMod()
        {
            using (var a = new HugeInt("-234052834524092854092874502983745029345723098457209305983434345"))
            using (var b = new HugeInt("394580293847502987609283945873594873409587"))
            using (var c = new HugeInt())
            {
                a.Value = (a / b).SavingRemainderTo(c).Rounding(RoundingModes.Floor);
                Assert.AreEqual("-593169091750307653295", a.ToString());
                Assert.AreEqual("280211579611742400636461191697511341704820", c.ToString());
            }
        }

        [TestMethod]
        public void IntDivideHugeIntTruncateWithMod()
        {
            using (var a = new HugeInt("234052834524092854092874502983745029345723098457209305983434345"))
            using (var b = new HugeInt("394580293847502987609283945873594873409587"))
            using (var c = new HugeInt())
            {
                a.Value = (a / b).SavingRemainderTo(c).Rounding(RoundingModes.Truncate);
                Assert.AreEqual("593169091750307653294", a.ToString());
                Assert.AreEqual("114368714235760586972822754176083531704767", c.ToString());
            }
        }

        [TestMethod]
        public void IntDivideHugeIntNegativeTruncateWithMod()
        {
            using (var a = new HugeInt("-234052834524092854092874502983745029345723098457209305983434345"))
            using (var b = new HugeInt("394580293847502987609283945873594873409587"))
            using (var c = new HugeInt())
            {
                a.Value = (a / b).SavingRemainderTo(c).Rounding(RoundingModes.Truncate);
                Assert.AreEqual("-593169091750307653294", a.ToString());
                Assert.AreEqual("-114368714235760586972822754176083531704767", c.ToString());
            }
        }
            
        #endregion

        #region Limb

        [TestMethod]
        public void IntDivideLimb()
        {
            using (var a = new HugeInt("234052834524092854092874502983745029345723098457209305983434345"))
            {
                var b = Platform.Ui(5931690917503076532, 3593190912);
                a.Value = a / b;
                Assert.AreEqual(Platform.Select("39458029384750298767200622330399462537522498", "65137878909366687748339296418588133550785040629984011"), a.ToString());
            }
        }

        [TestMethod]
        public void IntDivideLimbCeiling()
        {
            using (var a = new HugeInt("234052834524092854092874502983745029345723098457209305983434345"))
            {
                var b = Platform.Ui(5931690917503076532, 3593190912);
                a.Value = (a / b).Rounding(RoundingModes.Ceiling);
                Assert.AreEqual(Platform.Select("39458029384750298767200622330399462537522499", "65137878909366687748339296418588133550785040629984012"), a.ToString());
            }
        }

        [TestMethod]
        public void IntDivideLimbNegativeCeiling()
        {
            using (var a = new HugeInt("-234052834524092854092874502983745029345723098457209305983434345"))
            {
                var b = Platform.Ui(5931690917503076532, 3593190912);
                a.Value = (a / b).Rounding(RoundingModes.Ceiling);
                Assert.AreEqual(Platform.Select("-39458029384750298767200622330399462537522498", "-65137878909366687748339296418588133550785040629984011"), a.ToString());
            }
        }

        [TestMethod]
        public void IntDivideLimbFloor()
        {
            using (var a = new HugeInt("234052834524092854092874502983745029345723098457209305983434345"))
            {
                var b = Platform.Ui(5931690917503076532, 3593190912);
                a.Value = (a / b).Rounding(RoundingModes.Floor);
                Assert.AreEqual(Platform.Select("39458029384750298767200622330399462537522498", "65137878909366687748339296418588133550785040629984011"), a.ToString());
            }
        }

        [TestMethod]
        public void IntDivideLimbNegativeFloor()
        {
            using (var a = new HugeInt("-234052834524092854092874502983745029345723098457209305983434345"))
            {
                var b = Platform.Ui(5931690917503076532, 3593190912);
                a.Value = (a / b).Rounding(RoundingModes.Floor);
                Assert.AreEqual(Platform.Select("-39458029384750298767200622330399462537522499", "-65137878909366687748339296418588133550785040629984012"), a.ToString());
            }
        }

        [TestMethod]
        public void IntDivideLimbTruncate()
        {
            using (var a = new HugeInt("234052834524092854092874502983745029345723098457209305983434345"))
            {
                var b = Platform.Ui(5931690917503076532, 3593190912);
                a.Value = (a / b).Rounding(RoundingModes.Truncate);
                Assert.AreEqual(Platform.Select("39458029384750298767200622330399462537522498", "65137878909366687748339296418588133550785040629984011"), a.ToString());
            }
        }

        [TestMethod]
        public void IntDivideLimbNegativeTruncate()
        {
            using (var a = new HugeInt("-234052834524092854092874502983745029345723098457209305983434345"))
            {
                var b = Platform.Ui(5931690917503076532, 3593190912);
                a.Value = (a / b).Rounding(RoundingModes.Truncate);
                Assert.AreEqual(Platform.Select("-39458029384750298767200622330399462537522498", "-65137878909366687748339296418588133550785040629984011"), a.ToString());
            }
        }

        #endregion

        #region Limb with Mod

        [TestMethod]
        public void IntDivideLimbWithMod()
        {
            using (var a = new HugeInt("234052834524092854092874502983745029345723098457209305983434345"))
            using (var c = new HugeInt())
            {
                var b = Platform.Ui(5931690917503076532, 3593190912);
                a.Value = (a / b).SavingRemainderTo(c);
                Assert.AreEqual(Platform.Select("39458029384750298767200622330399462537522498", "65137878909366687748339296418588133550785040629984011"), a.ToString());
                Assert.AreEqual(Platform.Select("5599417901817617409", "2952926313"), c.ToString());
            }
        }

        [TestMethod]
        public void IntDivideLimbCeilingWithMod()
        {
            using (var a = new HugeInt("234052834524092854092874502983745029345723098457209305983434345"))
            using (var c = new HugeInt())
            {
                var b = Platform.Ui(5931690917503076532, 3593190912);
                a.Value = (a / b).SavingRemainderTo(c).Rounding(RoundingModes.Ceiling);
                Assert.AreEqual(Platform.Select("39458029384750298767200622330399462537522499", "65137878909366687748339296418588133550785040629984012"), a.ToString());
                Assert.AreEqual(Platform.Select("-332273015685459123", "-640264599"), c.ToString());
            }
        }

        [TestMethod]
        public void IntDivideLimbNegativeCeilingWithMod()
        {
            using (var a = new HugeInt("-234052834524092854092874502983745029345723098457209305983434345"))
            using (var c = new HugeInt())
            {
                var b = Platform.Ui(5931690917503076532, 3593190912);
                a.Value = (a / b).SavingRemainderTo(c).Rounding(RoundingModes.Ceiling);
                Assert.AreEqual(Platform.Select("-39458029384750298767200622330399462537522498", "-65137878909366687748339296418588133550785040629984011"), a.ToString());
                Assert.AreEqual(Platform.Select("-5599417901817617409", "-2952926313"), c.ToString());
            }
        }

        [TestMethod]
        public void IntDivideLimbFloorWithMod()
        {
            using (var a = new HugeInt("234052834524092854092874502983745029345723098457209305983434345"))
            using (var c = new HugeInt())
            {
                var b = Platform.Ui(5931690917503076532, 3593190912);
                a.Value = (a / b).SavingRemainderTo(c).Rounding(RoundingModes.Floor);
                Assert.AreEqual(Platform.Select("39458029384750298767200622330399462537522498", "65137878909366687748339296418588133550785040629984011"), a.ToString());
                Assert.AreEqual(Platform.Select("5599417901817617409", "2952926313"), c.ToString());
            }
        }

        [TestMethod]
        public void IntDivideLimbNegativeFloorWithMod()
        {
            using (var a = new HugeInt("-234052834524092854092874502983745029345723098457209305983434345"))
            using (var c = new HugeInt())
            {
                var b = Platform.Ui(5931690917503076532, 3593190912);
                a.Value = (a / b).SavingRemainderTo(c).Rounding(RoundingModes.Floor);
                Assert.AreEqual(Platform.Select("-39458029384750298767200622330399462537522499", "-65137878909366687748339296418588133550785040629984012"), a.ToString());
                Assert.AreEqual(Platform.Select("332273015685459123", "640264599"), c.ToString());
            }
        }

        [TestMethod]
        public void IntDivideLimbTruncateWithMod()
        {
            using (var a = new HugeInt("234052834524092854092874502983745029345723098457209305983434345"))
            using (var c = new HugeInt())
            {
                var b = Platform.Ui(5931690917503076532, 3593190912);
                a.Value = (a / b).SavingRemainderTo(c).Rounding(RoundingModes.Truncate);
                Assert.AreEqual(Platform.Select("39458029384750298767200622330399462537522498", "65137878909366687748339296418588133550785040629984011"), a.ToString());
                Assert.AreEqual(Platform.Select("5599417901817617409", "2952926313"), c.ToString());
            }
        }

        [TestMethod]
        public void IntDivideLimbNegativeTruncateWithMod()
        {
            using (var a = new HugeInt("-234052834524092854092874502983745029345723098457209305983434345"))
            using (var c = new HugeInt())
            {
                var b = Platform.Ui(5931690917503076532, 3593190912);
                a.Value = (a / b).SavingRemainderTo(c).Rounding(RoundingModes.Truncate);
                Assert.AreEqual(Platform.Select("-39458029384750298767200622330399462537522498", "-65137878909366687748339296418588133550785040629984011"), a.ToString());
                Assert.AreEqual(Platform.Select("-5599417901817617409", "-2952926313"), c.ToString());
            }
        }

        #endregion

        #region Limb with Mod x 2

        [TestMethod]
        public void IntDivideLimbWithMod2()
        {
            using (var a = new HugeInt("234052834524092854092874502983745029345723098457209305983434345"))
            using (var c = new HugeInt())
            {
                var b = Platform.Ui(5931690917503076532, 3593190912);
                var d = Platform.Ui(0, 0);
                a.Value = (a / b).SettingRemainderTo(x => d = x).SavingRemainderTo(c);
                Assert.AreEqual(Platform.Select("39458029384750298767200622330399462537522498", "65137878909366687748339296418588133550785040629984011"), a.ToString());
                Assert.AreEqual(Platform.Select("5599417901817617409", "2952926313"), c.ToString());
                Assert.AreEqual(d.ToString(), c.ToString());
            }
        }

        [TestMethod]
        public void IntDivideLimbCeilingWithMod2()
        {
            using (var a = new HugeInt("234052834524092854092874502983745029345723098457209305983434345"))
            using (var c = new HugeInt())
            {
                var b = Platform.Ui(5931690917503076532, 3593190912);
                var d = Platform.Ui(0, 0);
                a.Value = (a / b).SettingRemainderTo(x => d = x).SavingRemainderTo(c).Rounding(RoundingModes.Ceiling);
                Assert.AreEqual(Platform.Select("39458029384750298767200622330399462537522499", "65137878909366687748339296418588133550785040629984012"), a.ToString());
                Assert.AreEqual(Platform.Select("-332273015685459123", "-640264599"), c.ToString());
                Assert.AreEqual("-" + d.ToString(), c.ToString());
            }
        }

        [TestMethod]
        public void IntDivideLimbNegativeCeilingWithMod2()
        {
            using (var a = new HugeInt("-234052834524092854092874502983745029345723098457209305983434345"))
            using (var c = new HugeInt())
            {
                var b = Platform.Ui(5931690917503076532, 3593190912);
                var d = Platform.Ui(0, 0);
                a.Value = (a / b).SettingRemainderTo(x => d = x).SavingRemainderTo(c).Rounding(RoundingModes.Ceiling);
                Assert.AreEqual(Platform.Select("-39458029384750298767200622330399462537522498", "-65137878909366687748339296418588133550785040629984011"), a.ToString());
                Assert.AreEqual(Platform.Select("-5599417901817617409", "-2952926313"), c.ToString());
                Assert.AreEqual("-" + d.ToString(), c.ToString());
            }
        }

        [TestMethod]
        public void IntDivideLimbFloorWithMod2()
        {
            using (var a = new HugeInt("234052834524092854092874502983745029345723098457209305983434345"))
            using (var c = new HugeInt())
            {
                var b = Platform.Ui(5931690917503076532, 3593190912);
                var d = Platform.Ui(0, 0);
                a.Value = (a / b).SettingRemainderTo(x => d = x).SavingRemainderTo(c).Rounding(RoundingModes.Floor);
                Assert.AreEqual(Platform.Select("39458029384750298767200622330399462537522498", "65137878909366687748339296418588133550785040629984011"), a.ToString());
                Assert.AreEqual(Platform.Select("5599417901817617409", "2952926313"), c.ToString());
                Assert.AreEqual(d.ToString(), c.ToString());
            }
        }

        [TestMethod]
        public void IntDivideLimbNegativeFloorWithMod2()
        {
            using (var a = new HugeInt("-234052834524092854092874502983745029345723098457209305983434345"))
            using (var c = new HugeInt())
            {
                var b = Platform.Ui(5931690917503076532, 3593190912);
                var d = Platform.Ui(0, 0);
                a.Value = (a / b).SettingRemainderTo(x => d = x).SavingRemainderTo(c).Rounding(RoundingModes.Floor);
                Assert.AreEqual(Platform.Select("-39458029384750298767200622330399462537522499", "-65137878909366687748339296418588133550785040629984012"), a.ToString());
                Assert.AreEqual(Platform.Select("332273015685459123", "640264599"), c.ToString());
                Assert.AreEqual(d.ToString(), c.ToString());
            }
        }

        [TestMethod]
        public void IntDivideLimbTruncateWithMod2()
        {
            using (var a = new HugeInt("234052834524092854092874502983745029345723098457209305983434345"))
            using (var c = new HugeInt())
            {
                var b = Platform.Ui(5931690917503076532, 3593190912);
                var d = Platform.Ui(0, 0);
                a.Value = (a / b).SettingRemainderTo(x => d = x).SavingRemainderTo(c).Rounding(RoundingModes.Truncate);
                Assert.AreEqual(Platform.Select("39458029384750298767200622330399462537522498", "65137878909366687748339296418588133550785040629984011"), a.ToString());
                Assert.AreEqual(Platform.Select("5599417901817617409", "2952926313"), c.ToString());
                Assert.AreEqual(d.ToString(), c.ToString());
            }
        }

        [TestMethod]
        public void IntDivideLimbNegativeTruncateWithMod2()
        {
            using (var a = new HugeInt("-234052834524092854092874502983745029345723098457209305983434345"))
            using (var c = new HugeInt())
            {
                var b = Platform.Ui(5931690917503076532, 3593190912);
                var d = Platform.Ui(0, 0);
                a.Value = (a / b).SettingRemainderTo(x => d = x).SavingRemainderTo(c).Rounding(RoundingModes.Truncate);
                Assert.AreEqual(Platform.Select("-39458029384750298767200622330399462537522498", "-65137878909366687748339296418588133550785040629984011"), a.ToString());
                Assert.AreEqual(Platform.Select("-5599417901817617409", "-2952926313"), c.ToString());
                Assert.AreEqual("-" + d.ToString(), c.ToString());
            }
        }

        #endregion

        #region Limb with Limb Mod

        [TestMethod]
        public void IntDivideLimbWithMod3()
        {
            using (var a = new HugeInt("234052834524092854092874502983745029345723098457209305983434345"))
            {
                var b = Platform.Ui(5931690917503076532, 3593190912);
                var d = Platform.Ui(0, 0);
                a.Value = (a / b).SettingRemainderTo(x => d = x);
                Assert.AreEqual(Platform.Select("39458029384750298767200622330399462537522498", "65137878909366687748339296418588133550785040629984011"), a.ToString());
                Assert.AreEqual(Platform.Select("5599417901817617409", "2952926313"), d.ToString());
            }
        }

        [TestMethod]
        public void IntDivideLimbCeilingWithMod3()
        {
            using (var a = new HugeInt("234052834524092854092874502983745029345723098457209305983434345"))
            {
                var b = Platform.Ui(5931690917503076532, 3593190912);
                var d = Platform.Ui(0, 0);
                a.Value = (a / b).SettingRemainderTo(x => d = x).Rounding(RoundingModes.Ceiling);
                Assert.AreEqual(Platform.Select("39458029384750298767200622330399462537522499", "65137878909366687748339296418588133550785040629984012"), a.ToString());
                Assert.AreEqual(Platform.Select("332273015685459123", "640264599"), d.ToString());
            }
        }

        [TestMethod]
        public void IntDivideLimbNegativeCeilingWithMod3()
        {
            using (var a = new HugeInt("-234052834524092854092874502983745029345723098457209305983434345"))
            {
                var b = Platform.Ui(5931690917503076532, 3593190912);
                var d = Platform.Ui(0, 0);
                a.Value = (a / b).SettingRemainderTo(x => d = x).Rounding(RoundingModes.Ceiling);
                Assert.AreEqual(Platform.Select("-39458029384750298767200622330399462537522498", "-65137878909366687748339296418588133550785040629984011"), a.ToString());
                Assert.AreEqual(Platform.Select("5599417901817617409", "2952926313"), d.ToString());
            }
        }

        [TestMethod]
        public void IntDivideLimbFloorWithMod3()
        {
            using (var a = new HugeInt("234052834524092854092874502983745029345723098457209305983434345"))
            {
                var b = Platform.Ui(5931690917503076532, 3593190912);
                var d = Platform.Ui(0, 0);
                a.Value = (a / b).SettingRemainderTo(x => d = x).Rounding(RoundingModes.Floor);
                Assert.AreEqual(Platform.Select("39458029384750298767200622330399462537522498", "65137878909366687748339296418588133550785040629984011"), a.ToString());
                Assert.AreEqual(Platform.Select("5599417901817617409", "2952926313"), d.ToString());
            }
        }

        [TestMethod]
        public void IntDivideLimbNegativeFloorWithMod3()
        {
            using (var a = new HugeInt("-234052834524092854092874502983745029345723098457209305983434345"))
            {
                var b = Platform.Ui(5931690917503076532, 3593190912);
                var d = Platform.Ui(0, 0);
                a.Value = (a / b).SettingRemainderTo(x => d = x).Rounding(RoundingModes.Floor);
                Assert.AreEqual(Platform.Select("-39458029384750298767200622330399462537522499", "-65137878909366687748339296418588133550785040629984012"), a.ToString());
                Assert.AreEqual(Platform.Select("332273015685459123", "640264599"), d.ToString());
            }
        }

        [TestMethod]
        public void IntDivideLimbTruncateWithMod3()
        {
            using (var a = new HugeInt("234052834524092854092874502983745029345723098457209305983434345"))
            {
                var b = Platform.Ui(5931690917503076532, 3593190912);
                var d = Platform.Ui(0, 0);
                a.Value = (a / b).SettingRemainderTo(x => d = x).Rounding(RoundingModes.Truncate);
                Assert.AreEqual(Platform.Select("39458029384750298767200622330399462537522498", "65137878909366687748339296418588133550785040629984011"), a.ToString());
                Assert.AreEqual(Platform.Select("5599417901817617409", "2952926313"), d.ToString());
            }
        }

        [TestMethod]
        public void IntDivideLimbNegativeTruncateWithMod3()
        {
            using (var a = new HugeInt("-234052834524092854092874502983745029345723098457209305983434345"))
            {
                var b = Platform.Ui(5931690917503076532, 3593190912);
                var d = Platform.Ui(0, 0);
                a.Value = (a / b).SettingRemainderTo(x => d = x).Rounding(RoundingModes.Truncate);
                Assert.AreEqual(Platform.Select("-39458029384750298767200622330399462537522498", "-65137878909366687748339296418588133550785040629984011"), a.ToString());
                Assert.AreEqual(Platform.Select("5599417901817617409", "2952926313"), d.ToString());
            }
        }

        #endregion
 
        #region Exact

        [TestMethod]
        public void IntDivideExactlyHugeInt()
        {
            using (var a = new HugeInt("234052834524092854092874502983745029345723098457209305983434345"))
            using (var b = new HugeInt("394580293847502987609283945873594873409587"))
            using (var c = new HugeInt())
            {
                c.Value = (a * b).DivideExactly(b);
                Assert.AreEqual(a.ToString(), c.ToString());
            }
        }

        [TestMethod]
        public void IntDivideExactlyLimb()
        {
            using (var a = new HugeInt("234052834524092854092874502983745029345723098457209305983434345"))
            using (var c = new HugeInt())
            {
                var b = Platform.Ui(5931690917503076532, 3593190912);
                c.Value = (a * b).DivideExactly(b);
                Assert.AreEqual(a.ToString(), c.ToString());
            }
        }

        #endregion

        #region IsDivisible

        [TestMethod]
        public void IntIsDivisibleByHugeInt()
        {
            using (var a = new HugeInt("234052834524092854092874502983745029345723098457209305983434345"))
            using (var b = new HugeInt("394580293847502987609283945873594873409587"))
            {
                a.Value = (a * b);
                Assert.IsTrue(a.IsDivisibleBy(b));
                a.Value = a + 1;
                Assert.IsFalse(a.IsDivisibleBy(b));
            }
        }

        [TestMethod]
        public void IntIsDivisibleByLimb()
        {
            using (var a = new HugeInt("234052834524092854092874502983745029345723098457209305983434345"))
            {
                var b = Platform.Ui(5931690917503076532, 3593169091);
                a.Value = (a * b);
                Assert.IsTrue(a.IsDivisibleBy(b));
                a.Value = a + 1;
                Assert.IsFalse(a.IsDivisibleBy(b));
            }
        }

        [TestMethod]
        public void IntIsDivisibleByPowerOf2()
        {
            using (var a = new HugeInt("234052834524092854092874502983745029345723098457209305983434345"))
            {
                uint b = 40;
                a.Value = (a << b);
                Assert.IsTrue(a.IsDivisibleByPowerOf2(b));
                Assert.IsFalse(a.IsDivisibleByPowerOf2(b + 1));
            }
        }

        #endregion
        
        #region IsCongruent

        [TestMethod]
        public void IntIsCongruentToHugeInt()
        {
            using (var a = new HugeInt("234052834524092854092874502983745029345723098457209305983434345"))
            using (var b = new HugeInt("394580293847502987609283945873594873409587"))
            using (var c = new HugeInt("293847502987609283945873594873409587"))
            {
                a.Value = (a * b) + c;
                Assert.IsTrue(a.IsCongruentTo(c, b));
                a.Value = a + 1;
                Assert.IsFalse(a.IsCongruentTo(c, b));
            }
        }

        [TestMethod]
        public void IntIsCongruentToLimb()
        {
            using (var a = new HugeInt("234052834524092854092874502983745029345723098457209305983434345"))
            {
                var b = Platform.Ui(5931690917503076532, 533076532);
                var c = Platform.Ui(98764938475983745, 97983745);
                a.Value = (a * b) + c;
                Assert.IsTrue(a.IsCongruentTo(c, b));
                a.Value = a + 1;
                Assert.IsFalse(a.IsCongruentTo(c, b));
            }
        }

        [TestMethod]
        public void IntIsCongruentToPowerOf2()
        {
            using (var a = new HugeInt("234052834524092854092874502983745029345723098457209305983434345"))
            using (var c = new HugeInt("293847502987609283945873594873409587"))
            {
                uint b = 120;
                a.Value = (a << b) + c;
                Assert.IsTrue(a.IsCongruentToModPowerOf2(c, b));
                Assert.IsFalse(a.IsCongruentToModPowerOf2(c, b + 3));
                c.Value = c + 1;
                Assert.IsFalse(a.IsCongruentToModPowerOf2(c, b));
            }
        }

        #endregion

        #endregion

        #region Mod

        #region Int

        [TestMethod]
        public void IntModHugeInt()
        {
            using (var a = new HugeInt("234052834524092854092874502983745029345723098457209305983434345"))
            using (var b = new HugeInt("394580293847502987609283945873594873409587"))
            {
                a.Value = a % b;
                Assert.AreEqual("114368714235760586972822754176083531704767", a.ToString());
            }
        }

        [TestMethod]
        public void IntModHugeIntCeiling()
        {
            using (var a = new HugeInt("234052834524092854092874502983745029345723098457209305983434345"))
            using (var b = new HugeInt("394580293847502987609283945873594873409587"))
            {
                a.Value = (a % b).Rounding(RoundingModes.Ceiling);
                Assert.AreEqual("-280211579611742400636461191697511341704820", a.ToString());
            }
        }

        [TestMethod]
        public void IntModHugeIntNegativeCeiling()
        {
            using (var a = new HugeInt("-234052834524092854092874502983745029345723098457209305983434345"))
            using (var b = new HugeInt("394580293847502987609283945873594873409587"))
            {
                a.Value = (a % b).Rounding(RoundingModes.Ceiling);
                Assert.AreEqual("-114368714235760586972822754176083531704767", a.ToString());
            }
        }

        [TestMethod]
        public void IntModHugeIntFloor()
        {
            using (var a = new HugeInt("234052834524092854092874502983745029345723098457209305983434345"))
            using (var b = new HugeInt("394580293847502987609283945873594873409587"))
            {
                a.Value = (a % b).Rounding(RoundingModes.Floor);
                Assert.AreEqual("114368714235760586972822754176083531704767", a.ToString());
            }
        }

        [TestMethod]
        public void IntModHugeIntNegativeFloor()
        {
            using (var a = new HugeInt("-234052834524092854092874502983745029345723098457209305983434345"))
            using (var b = new HugeInt("394580293847502987609283945873594873409587"))
            {
                a.Value = (a % b).Rounding(RoundingModes.Floor);
                Assert.AreEqual("280211579611742400636461191697511341704820", a.ToString());
            }
        }

        [TestMethod]
        public void IntModHugeIntTruncate()
        {
            using (var a = new HugeInt("234052834524092854092874502983745029345723098457209305983434345"))
            using (var b = new HugeInt("394580293847502987609283945873594873409587"))
            {
                a.Value = (a % b).Rounding(RoundingModes.Truncate);
                Assert.AreEqual("114368714235760586972822754176083531704767", a.ToString());
            }
        }

        [TestMethod]
        public void IntModHugeIntNegativeTruncate()
        {
            using (var a = new HugeInt("-234052834524092854092874502983745029345723098457209305983434345"))
            using (var b = new HugeInt("394580293847502987609283945873594873409587"))
            {
                a.Value = (a % b).Rounding(RoundingModes.Truncate);
                Assert.AreEqual("-114368714235760586972822754176083531704767", a.ToString());
            }
        }

        #endregion

        #region Int with quotient

        [TestMethod]
        public void IntModHugeIntCeilingWithQuotient()
        {
            using (var a = new HugeInt("234052834524092854092874502983745029345723098457209305983434345"))
            using (var b = new HugeInt("394580293847502987609283945873594873409587"))
            using (var c = new HugeInt())
            {
                c.Value = (a % b).SavingQuotientTo(a).Rounding(RoundingModes.Ceiling);
                Assert.AreEqual("593169091750307653295", a.ToString());
                Assert.AreEqual("-280211579611742400636461191697511341704820", c.ToString());
            }
        }

        [TestMethod]
        public void IntModHugeIntNegativeCeilingWithQuotient()
        {
            using (var a = new HugeInt("-234052834524092854092874502983745029345723098457209305983434345"))
            using (var b = new HugeInt("394580293847502987609283945873594873409587"))
            using (var c = new HugeInt())
            {
                c.Value = (a % b).SavingQuotientTo(a).Rounding(RoundingModes.Ceiling);
                Assert.AreEqual("-593169091750307653294", a.ToString());
                Assert.AreEqual("-114368714235760586972822754176083531704767", c.ToString());
            }
        }

        [TestMethod]
        public void IntModHugeIntFloorWithQuotient()
        {
            using (var a = new HugeInt("234052834524092854092874502983745029345723098457209305983434345"))
            using (var b = new HugeInt("394580293847502987609283945873594873409587"))
            using (var c = new HugeInt())
            {
                c.Value = (a % b).SavingQuotientTo(a).Rounding(RoundingModes.Floor);
                Assert.AreEqual("593169091750307653294", a.ToString());
                Assert.AreEqual("114368714235760586972822754176083531704767", c.ToString());
            }
        }

        [TestMethod]
        public void IntModHugeIntNegativeFloorWithQuotient()
        {
            using (var a = new HugeInt("-234052834524092854092874502983745029345723098457209305983434345"))
            using (var b = new HugeInt("394580293847502987609283945873594873409587"))
            using (var c = new HugeInt())
            {
                c.Value = (a % b).SavingQuotientTo(a).Rounding(RoundingModes.Floor);
                Assert.AreEqual("-593169091750307653295", a.ToString());
                Assert.AreEqual("280211579611742400636461191697511341704820", c.ToString());
            }
        }

        [TestMethod]
        public void IntModHugeIntTruncateWithQuotient()
        {
            using (var a = new HugeInt("234052834524092854092874502983745029345723098457209305983434345"))
            using (var b = new HugeInt("394580293847502987609283945873594873409587"))
            using (var c = new HugeInt())
            {
                c.Value = (a % b).SavingQuotientTo(a).Rounding(RoundingModes.Truncate);
                Assert.AreEqual("593169091750307653294", a.ToString());
                Assert.AreEqual("114368714235760586972822754176083531704767", c.ToString());
            }
        }

        [TestMethod]
        public void IntModHugeIntNegativeTruncateWithQuotient()
        {
            using (var a = new HugeInt("-234052834524092854092874502983745029345723098457209305983434345"))
            using (var b = new HugeInt("394580293847502987609283945873594873409587"))
            using (var c = new HugeInt())
            {
                c.Value = (a % b).SavingQuotientTo(a).Rounding(RoundingModes.Truncate);
                Assert.AreEqual("-593169091750307653294", a.ToString());
                Assert.AreEqual("-114368714235760586972822754176083531704767", c.ToString());
            }
        }

        #endregion

        #region Limb

        [TestMethod]
        public void IntModLimb()
        {
            using (var a = new HugeInt("234052834524092854092874502983745029345723098457209305983434345"))
            {
                var b = Platform.Ui(5931690917503076532, 3593190912);
                a.Value = (a % b);
                Assert.AreEqual(Platform.Select("5599417901817617409", "2952926313"), a.ToString());
            }
        }

        [TestMethod]
        public void IntModLimbCeiling()
        {
            using (var a = new HugeInt("234052834524092854092874502983745029345723098457209305983434345"))
            {
                var b = Platform.Ui(5931690917503076532, 3593190912);
                a.Value = (a % b).Rounding(RoundingModes.Ceiling);
                Assert.AreEqual(Platform.Select("-332273015685459123", "-640264599"), a.ToString());
            }
        }

        [TestMethod]
        public void IntModLimbNegativeCeiling()
        {
            using (var a = new HugeInt("-234052834524092854092874502983745029345723098457209305983434345"))
            {
                var b = Platform.Ui(5931690917503076532, 3593190912);
                a.Value = (a % b).Rounding(RoundingModes.Ceiling);
                Assert.AreEqual(Platform.Select("-5599417901817617409", "-2952926313"), a.ToString());
            }
        }

        [TestMethod]
        public void IntModLimbFloor()
        {
            using (var a = new HugeInt("234052834524092854092874502983745029345723098457209305983434345"))
            {
                var b = Platform.Ui(5931690917503076532, 3593190912);
                a.Value = (a % b).Rounding(RoundingModes.Floor);
                Assert.AreEqual(Platform.Select("5599417901817617409", "2952926313"), a.ToString());
            }
        }

        [TestMethod]
        public void IntModLimbNegativeFloor()
        {
            using (var a = new HugeInt("-234052834524092854092874502983745029345723098457209305983434345"))
            {
                var b = Platform.Ui(5931690917503076532, 3593190912);
                a.Value = (a % b).Rounding(RoundingModes.Floor);
                Assert.AreEqual(Platform.Select("332273015685459123", "640264599"), a.ToString());
            }
        }

        [TestMethod]
        public void IntModLimbTruncate()
        {
            using (var a = new HugeInt("234052834524092854092874502983745029345723098457209305983434345"))
            {
                var b = Platform.Ui(5931690917503076532, 3593190912);
                a.Value = (a % b).Rounding(RoundingModes.Truncate);
                Assert.AreEqual(Platform.Select("5599417901817617409", "2952926313"), a.ToString());
            }
        }

        [TestMethod]
        public void IntModLimbNegativeTruncate()
        {
            using (var a = new HugeInt("-234052834524092854092874502983745029345723098457209305983434345"))
            {
                var b = Platform.Ui(5931690917503076532, 3593190912);
                a.Value = (a % b).Rounding(RoundingModes.Truncate);
                Assert.AreEqual(Platform.Select("-5599417901817617409", "-2952926313"), a.ToString());
            }
        }

        #endregion

        #region Limb With Quotient

        [TestMethod]
        public void IntModLimbWithQuotient()
        {
            using (var a = new HugeInt("234052834524092854092874502983745029345723098457209305983434345"))
            using (var c = new HugeInt())
            {
                var b = Platform.Ui(5931690917503076532, 3593190912);
                a.Value = (a % b).SavingQuotientTo(c);
                Assert.AreEqual(Platform.Select("39458029384750298767200622330399462537522498", "65137878909366687748339296418588133550785040629984011"), c.ToString());
                Assert.AreEqual(Platform.Select("5599417901817617409", "2952926313"), a.ToString());
            }
        }

        [TestMethod]
        public void IntModLimbCeilingWithQuotient()
        {
            using (var a = new HugeInt("234052834524092854092874502983745029345723098457209305983434345"))
            using (var c = new HugeInt())
            {
                var b = Platform.Ui(5931690917503076532, 3593190912);
                a.Value = (a % b).SavingQuotientTo(c).Rounding(RoundingModes.Ceiling);
                Assert.AreEqual(Platform.Select("39458029384750298767200622330399462537522499", "65137878909366687748339296418588133550785040629984012"), c.ToString());
                Assert.AreEqual(Platform.Select("-332273015685459123", "-640264599"), a.ToString());
            }
        }

        [TestMethod]
        public void IntModLimbNegativeCeilingWithQuotient()
        {
            using (var a = new HugeInt("-234052834524092854092874502983745029345723098457209305983434345"))
            using (var c = new HugeInt())
            {
                var b = Platform.Ui(5931690917503076532, 3593190912);
                a.Value = (a % b).SavingQuotientTo(c).Rounding(RoundingModes.Ceiling);
                Assert.AreEqual(Platform.Select("-39458029384750298767200622330399462537522498", "-65137878909366687748339296418588133550785040629984011"), c.ToString());
                Assert.AreEqual(Platform.Select("-5599417901817617409", "-2952926313"), a.ToString());
            }
        }

        [TestMethod]
        public void IntModLimbFloorWithQuotient()
        {
            using (var a = new HugeInt("234052834524092854092874502983745029345723098457209305983434345"))
            using (var c = new HugeInt())
            {
                var b = Platform.Ui(5931690917503076532, 3593190912);
                a.Value = (a % b).SavingQuotientTo(c).Rounding(RoundingModes.Floor);
                Assert.AreEqual(Platform.Select("39458029384750298767200622330399462537522498", "65137878909366687748339296418588133550785040629984011"), c.ToString());
                Assert.AreEqual(Platform.Select("5599417901817617409", "2952926313"), a.ToString());
            }
        }

        [TestMethod]
        public void IntModLimbNegativeFloorWithQuotient()
        {
            using (var a = new HugeInt("-234052834524092854092874502983745029345723098457209305983434345"))
            using (var c = new HugeInt())
            {
                var b = Platform.Ui(5931690917503076532, 3593190912);
                a.Value = (a % b).SavingQuotientTo(c).Rounding(RoundingModes.Floor);
                Assert.AreEqual(Platform.Select("-39458029384750298767200622330399462537522499", "-65137878909366687748339296418588133550785040629984012"), c.ToString());
                Assert.AreEqual(Platform.Select("332273015685459123", "640264599"), a.ToString());
            }
        }

        [TestMethod]
        public void IntModLimbTruncateWithQuotient()
        {
            using (var a = new HugeInt("234052834524092854092874502983745029345723098457209305983434345"))
            using (var c = new HugeInt())
            {
                var b = Platform.Ui(5931690917503076532, 3593190912);
                a.Value = (a % b).SavingQuotientTo(c).Rounding(RoundingModes.Truncate);
                Assert.AreEqual(Platform.Select("39458029384750298767200622330399462537522498", "65137878909366687748339296418588133550785040629984011"), c.ToString());
                Assert.AreEqual(Platform.Select("5599417901817617409", "2952926313"), a.ToString());
            }
        }

        [TestMethod]
        public void IntModLimbNegativeTruncateWithQuotient()
        {
            using (var a = new HugeInt("-234052834524092854092874502983745029345723098457209305983434345"))
            using (var c = new HugeInt())
            {
                var b = Platform.Ui(5931690917503076532, 3593190912);
                a.Value = (a % b).SavingQuotientTo(c).Rounding(RoundingModes.Truncate);
                Assert.AreEqual(Platform.Select("-39458029384750298767200622330399462537522498", "-65137878909366687748339296418588133550785040629984011"), c.ToString());
                Assert.AreEqual(Platform.Select("-5599417901817617409", "-2952926313"), a.ToString());
            }
        }

        #endregion

        #region Limb With Quotient and Limb Mod

        [TestMethod]
        public void IntModLimbWithQuotient2()
        {
            using (var a = new HugeInt("234052834524092854092874502983745029345723098457209305983434345"))
            using (var c = new HugeInt())
            {
                var b = Platform.Ui(5931690917503076532, 3593190912);
                var d = Platform.Ui(0, 0);
                a.Value = (a % b).SettingRemainderTo(x => d = x).SavingQuotientTo(c);
                Assert.AreEqual(Platform.Select("39458029384750298767200622330399462537522498", "65137878909366687748339296418588133550785040629984011"), c.ToString());
                Assert.AreEqual(Platform.Select("5599417901817617409", "2952926313"), a.ToString());
                Assert.AreEqual(d.ToString(), a.ToString());
            }
        }

        [TestMethod]
        public void IntModLimbCeilingWithQuotient2()
        {
            using (var a = new HugeInt("234052834524092854092874502983745029345723098457209305983434345"))
            using (var c = new HugeInt())
            {
                var b = Platform.Ui(5931690917503076532, 3593190912);
                var d = Platform.Ui(0, 0);
                a.Value = (a % b).SettingRemainderTo(x => d = x).SavingQuotientTo(c).Rounding(RoundingModes.Ceiling);
                Assert.AreEqual(Platform.Select("39458029384750298767200622330399462537522499", "65137878909366687748339296418588133550785040629984012"), c.ToString());
                Assert.AreEqual(Platform.Select("-332273015685459123", "-640264599"), a.ToString());
                Assert.AreEqual("-" + d.ToString(), a.ToString());
            }
        }

        [TestMethod]
        public void IntModLimbNegativeCeilingWithQuotient2()
        {
            using (var a = new HugeInt("-234052834524092854092874502983745029345723098457209305983434345"))
            using (var c = new HugeInt())
            {
                var b = Platform.Ui(5931690917503076532, 3593190912);
                var d = Platform.Ui(0, 0);
                a.Value = (a % b).SettingRemainderTo(x => d = x).SavingQuotientTo(c).Rounding(RoundingModes.Ceiling);
                Assert.AreEqual(Platform.Select("-39458029384750298767200622330399462537522498", "-65137878909366687748339296418588133550785040629984011"), c.ToString());
                Assert.AreEqual(Platform.Select("-5599417901817617409", "-2952926313"), a.ToString());
                Assert.AreEqual("-" + d.ToString(), a.ToString());
            }
        }

        [TestMethod]
        public void IntModLimbFloorWithQuotient2()
        {
            using (var a = new HugeInt("234052834524092854092874502983745029345723098457209305983434345"))
            using (var c = new HugeInt())
            {
                var b = Platform.Ui(5931690917503076532, 3593190912);
                var d = Platform.Ui(0, 0);
                a.Value = (a % b).SettingRemainderTo(x => d = x).SavingQuotientTo(c).Rounding(RoundingModes.Floor);
                Assert.AreEqual(Platform.Select("39458029384750298767200622330399462537522498", "65137878909366687748339296418588133550785040629984011"), c.ToString());
                Assert.AreEqual(Platform.Select("5599417901817617409", "2952926313"), a.ToString());
                Assert.AreEqual(d.ToString(), a.ToString());
            }
        }

        [TestMethod]
        public void IntModLimbNegativeFloorWithQuotient2()
        {
            using (var a = new HugeInt("-234052834524092854092874502983745029345723098457209305983434345"))
            using (var c = new HugeInt())
            {
                var b = Platform.Ui(5931690917503076532, 3593190912);
                var d = Platform.Ui(0, 0);
                a.Value = (a % b).SettingRemainderTo(x => d = x).SavingQuotientTo(c).Rounding(RoundingModes.Floor);
                Assert.AreEqual(Platform.Select("-39458029384750298767200622330399462537522499", "-65137878909366687748339296418588133550785040629984012"), c.ToString());
                Assert.AreEqual(Platform.Select("332273015685459123", "640264599"), a.ToString());
                Assert.AreEqual(d.ToString(), a.ToString());
            }
        }

        [TestMethod]
        public void IntModLimbTruncateWithQuotient2()
        {
            using (var a = new HugeInt("234052834524092854092874502983745029345723098457209305983434345"))
            using (var c = new HugeInt())
            {
                var b = Platform.Ui(5931690917503076532, 3593190912);
                var d = Platform.Ui(0, 0);
                a.Value = (a % b).SettingRemainderTo(x => d = x).SavingQuotientTo(c).Rounding(RoundingModes.Truncate);
                Assert.AreEqual(Platform.Select("39458029384750298767200622330399462537522498", "65137878909366687748339296418588133550785040629984011"), c.ToString());
                Assert.AreEqual(Platform.Select("5599417901817617409", "2952926313"), a.ToString());
                Assert.AreEqual(d.ToString(), a.ToString());
            }
        }

        [TestMethod]
        public void IntModLimbNegativeTruncateWithQuotient2()
        {
            using (var a = new HugeInt("-234052834524092854092874502983745029345723098457209305983434345"))
            using (var c = new HugeInt())
            {
                var b = Platform.Ui(5931690917503076532, 3593190912);
                var d = Platform.Ui(0, 0);
                a.Value = (a % b).SettingRemainderTo(x => d = x).SavingQuotientTo(c).Rounding(RoundingModes.Truncate);
                Assert.AreEqual(Platform.Select("-39458029384750298767200622330399462537522498", "-65137878909366687748339296418588133550785040629984011"), c.ToString());
                Assert.AreEqual(Platform.Select("-5599417901817617409", "-2952926313"), a.ToString());
                Assert.AreEqual("-" + d.ToString(), a.ToString());
            } 

        }

        #endregion

        #region Limb With Limb Mod

        [TestMethod]
        public void IntModLimbWithQuotient3()
        {
            using (var a = new HugeInt("234052834524092854092874502983745029345723098457209305983434345"))
            {
                var b = Platform.Ui(5931690917503076532, 3593190912);
                var d = Platform.Ui(0, 0);
                a.Value = (a % b).SettingRemainderTo(x => d = x);
                Assert.AreEqual(Platform.Select("5599417901817617409", "2952926313"), a.ToString());
                Assert.AreEqual(d.ToString(), a.ToString());
            }
        }

        [TestMethod]
        public void IntModLimbCeilingWithQuotient3()
        {
            using (var a = new HugeInt("234052834524092854092874502983745029345723098457209305983434345"))
            {
                var b = Platform.Ui(5931690917503076532, 3593190912);
                var d = Platform.Ui(0, 0);
                a.Value = (a % b).SettingRemainderTo(x => d = x).Rounding(RoundingModes.Ceiling);
                Assert.AreEqual(Platform.Select("-332273015685459123", "-640264599"), a.ToString());
                Assert.AreEqual("-" + d.ToString(), a.ToString());
            }
        }

        [TestMethod]
        public void IntModLimbNegativeCeilingWithQuotient3()
        {
            using (var a = new HugeInt("-234052834524092854092874502983745029345723098457209305983434345"))
            {
                var b = Platform.Ui(5931690917503076532, 3593190912);
                var d = Platform.Ui(0, 0);
                a.Value = (a % b).SettingRemainderTo(x => d = x).Rounding(RoundingModes.Ceiling);
                Assert.AreEqual(Platform.Select("-5599417901817617409", "-2952926313"), a.ToString());
                Assert.AreEqual("-" + d.ToString(), a.ToString());
            }
        }

        [TestMethod]
        public void IntModLimbFloorWithQuotient3()
        {
            using (var a = new HugeInt("234052834524092854092874502983745029345723098457209305983434345"))
            {
                var b = Platform.Ui(5931690917503076532, 3593190912);
                var d = Platform.Ui(0, 0);
                a.Value = (a % b).SettingRemainderTo(x => d = x).Rounding(RoundingModes.Floor);
                Assert.AreEqual(Platform.Select("5599417901817617409", "2952926313"), a.ToString());
                Assert.AreEqual(d.ToString(), a.ToString());
            }
        }

        [TestMethod]
        public void IntModLimbNegativeFloorWithQuotient3()
        {
            using (var a = new HugeInt("-234052834524092854092874502983745029345723098457209305983434345"))
            {
                var b = Platform.Ui(5931690917503076532, 3593190912);
                var d = Platform.Ui(0, 0);
                a.Value = (a % b).SettingRemainderTo(x => d = x).Rounding(RoundingModes.Floor);
                Assert.AreEqual(Platform.Select("332273015685459123", "640264599"), a.ToString());
                Assert.AreEqual(d.ToString(), a.ToString());
            }
        }

        [TestMethod]
        public void IntModLimbTruncateWithQuotient3()
        {
            using (var a = new HugeInt("234052834524092854092874502983745029345723098457209305983434345"))
            {
                var b = Platform.Ui(5931690917503076532, 3593190912);
                var d = Platform.Ui(0, 0);
                a.Value = (a % b).SettingRemainderTo(x => d = x).Rounding(RoundingModes.Truncate);
                Assert.AreEqual(Platform.Select("5599417901817617409", "2952926313"), a.ToString());
                Assert.AreEqual(d.ToString(), a.ToString());
            }
        }

        [TestMethod]
        public void IntModLimbNegativeTruncateWithQuotient3()
        {
            using (var a = new HugeInt("-234052834524092854092874502983745029345723098457209305983434345"))
            {
                var b = Platform.Ui(5931690917503076532, 3593190912);
                var d = Platform.Ui(0, 0);
                a.Value = (a % b).SettingRemainderTo(x => d = x).Rounding(RoundingModes.Truncate);
                Assert.AreEqual(Platform.Select("-5599417901817617409", "-2952926313"), a.ToString());
                Assert.AreEqual("-" + d.ToString(), a.ToString());
            }

        }

        #endregion

        #region Mod Limb function

        [TestMethod]
        public void IntModLimbFunction()
        {
            using (var a = new HugeInt("234052834524092854092874502983745029345723098457209305983434345"))
            {
                var b = Platform.Ui(5931690917503076532U, 593169092U);
                var expected = Platform.Select("5599417901817617409", "119445933");
                var d = a.Mod(b);
                Assert.AreEqual(expected, d.ToString());
            }
        }

        [TestMethod]
        public void IntModLimbFunctionCeiling()
        {
            using (var a = new HugeInt("234052834524092854092874502983745029345723098457209305983434345"))
            {
                var b = Platform.Ui(5931690917503076532U, 593169092U);
                var expected = Platform.Select("332273015685459123", "473723159");
                var d = a.Mod(b, RoundingModes.Ceiling);
                Assert.AreEqual(expected, d.ToString());
            }
        }

        [TestMethod]
        public void IntModLimbFunctionNegativeCeiling()
        {
            using (var a = new HugeInt("-234052834524092854092874502983745029345723098457209305983434345"))
            {
                var b = Platform.Ui(5931690917503076532U, 593169092U);
                var expected = Platform.Select("5599417901817617409", "119445933");
                ulong d = a.Mod(b, RoundingModes.Ceiling);
                Assert.AreEqual(expected, d.ToString());
            }
        }

        [TestMethod]
        public void IntModLimbFunctionFloor()
        {
            using (var a = new HugeInt("234052834524092854092874502983745029345723098457209305983434345"))
            {
                var b = Platform.Ui(5931690917503076532U, 593169092U);
                var expected = Platform.Select("5599417901817617409", "119445933");
                var d = a.Mod(b, RoundingModes.Floor);
                Assert.AreEqual(expected, d.ToString());
            }
        }

        [TestMethod]
        public void IntModLimbFunctionNegativeFloor()
        {
            using (var a = new HugeInt("-234052834524092854092874502983745029345723098457209305983434345"))
            {
                var b = Platform.Ui(5931690917503076532U, 593169092U);
                var expected = Platform.Select("332273015685459123", "473723159");
                var d = a.Mod(b, RoundingModes.Floor);
                Assert.AreEqual(expected, d.ToString());
            }
        }

        [TestMethod]
        public void IntModLimbFunctionTruncate()
        {
            using (var a = new HugeInt("234052834524092854092874502983745029345723098457209305983434345"))
            {
                var b = Platform.Ui(5931690917503076532U, 593169092U);
                var expected = Platform.Select("5599417901817617409", "119445933");
                var d = a.Mod(b, RoundingModes.Truncate);
                Assert.AreEqual(expected, d.ToString());
            }
        }

        [TestMethod]
        public void IntModLimbFunctionNegativeTruncate()
        {
            using (var a = new HugeInt("-234052834524092854092874502983745029345723098457209305983434345"))
            {
                var b = Platform.Ui(5931690917503076532U, 593169092U);
                var expected = Platform.Select("5599417901817617409", "119445933");
                var d = a.Mod(b, RoundingModes.Truncate);
                Assert.AreEqual(expected, d.ToString());
            }
        }

        #endregion

        #region Shift Right Remainder

        [TestMethod]
        public void IntShiftRightRemainder()
        {
            using (var a = new HugeInt("ABCDEF052834524092854092874502983745029345723098457209305983434345", 16))
            {
                uint b = 96;
                a.Value = (a >> b).Remainder();
                Assert.AreEqual("723098457209305983434345", a.ToString(16));
            }
        }

        [TestMethod]
        public void IntShiftRightRemainderCeiling()
        {
            using (var a = new HugeInt("ABCDEF052834524092854092874502983745029345723098457209305983434345", 16))
            {
                uint b = 96;
                a.Value = (a >> b).Remainder().Rounding(RoundingModes.Ceiling);
                Assert.AreEqual("-8DCF67BA8DF6CFA67CBCBCBB", a.ToString(16));
            }
        }

        [TestMethod]
        public void IntShiftRightRemainderNegativeCeiling()
        {
            using (var a = new HugeInt("-ABCDEF052834524092854092874502983745029345723098457209305983434345", 16))
            {
                uint b = 96;
                a.Value = (a >> b).Remainder().Rounding(RoundingModes.Ceiling);
                Assert.AreEqual("-723098457209305983434345", a.ToString(16));
            }
        }

        [TestMethod]
        public void IntShiftRightRemainderFloor()
        {
            using (var a = new HugeInt("ABCDEF052834524092854092874502983745029345723098457209305983434345", 16))
            {
                uint b = 96;
                a.Value = (a >> b).Remainder().Rounding(RoundingModes.Floor);
                Assert.AreEqual("723098457209305983434345", a.ToString(16));
            }
        }

        [TestMethod]
        public void IntShiftRightRemainderNegativeFloor()
        {
            using (var a = new HugeInt("-ABCDEF052834524092854092874502983745029345723098457209305983434345", 16))
            {
                uint b = 96;
                a.Value = (a >> b).Remainder().Rounding(RoundingModes.Floor);
                Assert.AreEqual("8DCF67BA8DF6CFA67CBCBCBB", a.ToString(16));
            }
        }

        [TestMethod]
        public void IntShiftRightRemainderTruncate()
        {
            using (var a = new HugeInt("ABCDEF052834524092854092874502983745029345723098457209305983434345", 16))
            {
                uint b = 96;
                a.Value = (a >> b).Remainder().Rounding(RoundingModes.Truncate);
                Assert.AreEqual("723098457209305983434345", a.ToString(16));
            }
        }

        [TestMethod]
        public void IntShiftRightRemainderNegativeTruncate()
        {
            using (var a = new HugeInt("-ABCDEF052834524092854092874502983745029345723098457209305983434345", 16))
            {
                uint b = 96;
                a.Value = (a >> b).Remainder().Rounding(RoundingModes.Truncate);
                Assert.AreEqual("-723098457209305983434345", a.ToString(16));
            }
        }

        #endregion

        #endregion
    }
}
