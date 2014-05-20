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
            using (var a = new HugeRational("222509832503450298345029835740293845720"))
            using (var b = new HugeRational("222987435987982730594288574029879874539"))
            using (var c = new HugeRational())
            {
                c.Value = a + b;
                Assert.AreEqual("445497268491433028939318409770173720259", c.ToString());
            }
        }

        [TestMethod]
        public void RationalAddLimb()
        {
            using (var a = new HugeRational("222509832503450298345029835740293845720"))
            {
                var b = 4288574029879874539UL;
                a.Value = a + b;
                Assert.AreEqual("222509832503450298349318409770173720259", a.ToString());
            }
        }

        [TestMethod]
        public void RationalAddToLimb()
        {
            using (var a = new HugeRational("222509832503450298345029835740293845720"))
            {
                var b = 4288574029879874539UL;
                a.Value = b + a;
                Assert.AreEqual("222509832503450298349318409770173720259", a.ToString());
            }
        }

        [TestMethod]
        public void RationalAddSignedLimb()
        {
            using (var a = new HugeRational("222509832503450298345029835740293845720"))
            {
                var b = -4288574029879874539L;
                a.Value = a + b;
                Assert.AreEqual("222509832503450298340741261710413971181", a.ToString());
            }
        }

        [TestMethod]
        public void RationalAddToSignedLimb()
        {
            using (var a = new HugeRational("222509832503450298345029835740293845720"))
            {
                var b = -4288574029879874539L;
                a.Value = b + a;
                Assert.AreEqual("222509832503450298340741261710413971181", a.ToString());
            }
        }

        [TestMethod]
        public void RationalAddToMaxSignedLimb()
        {
            using (var a = new HugeRational("222509832503450298345029835740293845720"))
            {
                var b = long.MinValue;
                a.Value = b + a;
                Assert.AreEqual("222509832503450298335806463703439069912", a.ToString());
            }
        }

        [TestMethod]
        public void RationalAddExpressionHugeRational()
        {
            using (var a = new HugeRational("222509832503450298345029835740293845720"))
            using (var b = new HugeRational("222987435987982730594288574029879874539"))
            using (var c = new HugeRational())
            {
                c.Value = 1 + (a + b);
                Assert.AreEqual("445497268491433028939318409770173720260", c.ToString());
            }
        }

        #endregion

        #region Subtract

        [TestMethod]
        public void RationalSubtractHugeRational()
        {
            using (var a = new HugeRational("445497268491433028939318409770173720259"))
            using (var b = new HugeRational("222987435987982730594288574029879874539"))
            {
                a.Value = a - b;
                Assert.AreEqual("222509832503450298345029835740293845720", a.ToString());
            }
        }

        [TestMethod]
        public void RationalSubtractLimb()
        {
            using (var a = new HugeRational("222509832503450298349318409770173720259"))
            {
                var b = 4288574029879874539UL;
                a.Value = a - b;
                Assert.AreEqual("222509832503450298345029835740293845720", a.ToString());
            }
        }

        [TestMethod]
        public void RationalSubtractFromLimb()
        {
            using (var a = new HugeRational("222509832503450298349318409770173720259"))
            {
                var b = 4288574029879874539UL;
                a.Value = b - a;
                Assert.AreEqual("-222509832503450298345029835740293845720", a.ToString());
            }
        }

        [TestMethod]
        public void RationalSubtractPositiveSignedLimb()
        {
            using (var a = new HugeRational("222509832503450298349318409770173720259"))
            {
                var b = 4288574029879874539L;
                a.Value = a - b;
                Assert.AreEqual("222509832503450298345029835740293845720", a.ToString());
            }
        }

        [TestMethod]
        public void RationalSubtractFromPositiveSignedLimb()
        {
            using (var a = new HugeRational("222509832503450298349318409770173720259"))
            {
                var b = 4288574029879874539L;
                a.Value = b - a;
                Assert.AreEqual("-222509832503450298345029835740293845720", a.ToString());
            }
        }

        [TestMethod]
        public void RationalSubtractSignedLimb()
        {
            using (var a = new HugeRational("222509832503450298345029835740293845720"))
            {
                var b = -4288574029879874539L;
                a.Value = a - b;
                Assert.AreEqual("222509832503450298349318409770173720259", a.ToString());
            }
        }

        [TestMethod]
        public void RationalSubtractFromSignedLimb()
        {
            using (var a = new HugeRational("222509832503450298345029835740293845720"))
            {
                var b = -4288574029879874539L;
                a.Value = b - a;
                Assert.AreEqual("-222509832503450298349318409770173720259", a.ToString());
            }
        }

        #endregion

        #region Multiply

        [TestMethod]
        public void RationalMultiplyByHugeRational()
        {
            using (var a = new HugeRational("90234098723098475098479385345098345"))
            using (var b = new HugeRational("7859487359873459872354987610987897"))
            {
                a.Value = a * b;
                Assert.AreEqual("709193758343766370701419953880162061353595657143399816050772069730465", a.ToString());
            }
        }

        [TestMethod]
        public void RationalMultiplyByLimb()
        {
            using (var a = new HugeRational("90234098723098475098479385345098345"))
            {
                ulong b = 17390538260286101342;
                a.Value = a * b;
                Assert.AreEqual("1569219546226477273686601978789044606491747469626478990", a.ToString());
            }
        }

        [TestMethod]
        public void RationalMultiplyLimbBy()
        {
            using (var a = new HugeRational("90234098723098475098479385345098345"))
            {
                ulong b = 17390538260286101342;
                a.Value = b * a;
                Assert.AreEqual("1569219546226477273686601978789044606491747469626478990", a.ToString());
            }
        }

        [TestMethod]
        public void RationalMultiplyBySignedLimb()
        {
            using (var a = new HugeRational("90234098723098475098479385345098345"))
            {
                long b = -7390538260286101342;
                a.Value = a * b;
                Assert.AreEqual("-666878558995492522701808125338061156491747469626478990", a.ToString());
            }
        }

        [TestMethod]
        public void RationalMultiplySignedLimbBy()
        {
            using (var a = new HugeRational("90234098723098475098479385345098345"))
            {
                long b = -7390538260286101342;
                a.Value = b * a;
                Assert.AreEqual("-666878558995492522701808125338061156491747469626478990", a.ToString());
            }
        }

        #endregion

        #region Add Product

        [TestMethod]
        public void RationalAddProductHugeRational()
        {
            using (var a = new HugeRational("98750293847520938457029384572093480498357"))
            using (var c = new HugeRational("23094582093845093574093845093485039450934"))
            using (var b = new HugeRational("-394580293847502987609283945873594873409587"))
            {
                var expr = a + c*b;
                a.Value = expr;
                Assert.AreEqual("-9112666988874677841199955832262586145147830205230375090322356322089362221491205901", a.ToString());
            }
        }

        [TestMethod]
        public void RationalAddProductLimb()
        {
            using (var a = new HugeRational("98750293847520938457029384572093480498357"))
            using (var c = new HugeRational("-23094582093845093574093845093485039450934"))
            {
                ulong b = 498734523097853458;
                var expr = a + c*b;
                a.Value = expr;
                Assert.AreEqual("-11518065386718058599763388064972875060082210203928832731415", a.ToString());
            }
        }

        [TestMethod]
        public void RationalAddProductLimbTo()
        {
            using (var a = new HugeRational("98750293847520938457029384572093480498357"))
            using (var c = new HugeRational("-23094582093845093574093845093485039450934"))
            {
                ulong b = 498734523097853458;
                var expr = a + b*c;
                a.Value = expr;
                Assert.AreEqual("-11518065386718058599763388064972875060082210203928832731415", a.ToString());
            }
        }

        [TestMethod]
        public void RationalAddProductLimbTo2()
        {
            using (var a = new HugeRational("98750293847520938457029384572093480498357"))
            using (var c = new HugeRational("-23094582093845093574093845093485039450934"))
            {
                ulong b = 498734523097853458;
                var expr = b*c + a;
                a.Value = expr;
                Assert.AreEqual("-11518065386718058599763388064972875060082210203928832731415", a.ToString());
            }
        }

        [TestMethod]
        public void RationalAddProductLimbTo3()
        {
            using (var a = new HugeRational("98750293847520938457029384572093480498357"))
            using (var c = new HugeRational("-23094582093845093574093845093485039450934"))
            using (var d = new HugeRational())
            {
                ulong b = 498734523097853458;
                var expr = b*c + a;
                d.Value = expr;
                Assert.AreEqual("-11518065386718058599763388064972875060082210203928832731415", d.ToString());
            }
        }

        [TestMethod]
        public void RationalAddProductSignedLimb()
        {
            using (var a = new HugeRational("98750293847520938457029384572093480498357"))
            using (var c = new HugeRational("23094582093845093574093845093485039450934"))
            {
                long b = -498734523097853458;
                var expr = a + c*b;
                a.Value = expr;
                Assert.AreEqual("-11518065386718058599763388064972875060082210203928832731415", a.ToString());
            }
        }

        [TestMethod]
        public void RationalAddProductSignedLimbTo()
        {
            using (var a = new HugeRational("98750293847520938457029384572093480498357"))
            using (var c = new HugeRational("-23094582093845093574093845093485039450934"))
            {
                long b = 498734523097853458;
                var expr = a + b*c;
                a.Value = expr;
                Assert.AreEqual("-11518065386718058599763388064972875060082210203928832731415", a.ToString());
            }
        }

        [TestMethod]
        public void RationalAddProductSignedLimbTo2()
        {
            using (var a = new HugeRational("98750293847520938457029384572093480498357"))
            using (var c = new HugeRational("-23094582093845093574093845093485039450934"))
            {
                long b = 498734523097853458;
                var expr = b*c + a;
                a.Value = expr;
                Assert.AreEqual("-11518065386718058599763388064972875060082210203928832731415", a.ToString());
            }
        }

        [TestMethod]
        public void RationalAddProductSignedLimbTo3()
        {
            using (var a = new HugeRational("98750293847520938457029384572093480498357"))
            using (var c = new HugeRational("23094582093845093574093845093485039450934"))
            using (var d = new HugeRational())
            {
                long b = -498734523097853458;
                var expr = b*c + a;
                d.Value = expr;
                Assert.AreEqual("-11518065386718058599763388064972875060082210203928832731415", d.ToString());
            }
        }

        #endregion

        #region Subtract Product

        [TestMethod]
        public void RationalSubtractProductHugeRational()
        {
            using (var a = new HugeRational("98750293847520938457029384572093480498357"))
            using (var c = new HugeRational("23094582093845093574093845093485039450934"))
            using (var b = new HugeRational("394580293847502987609283945873594873409587"))
            {
                var expr = a - c*b;
                a.Value = expr;
                Assert.AreEqual("-9112666988874677841199955832262586145147830205230375090322356322089362221491205901", a.ToString());
            }
        }

        [TestMethod]
        public void RationalSubtractProductLimb()
        {
            using (var a = new HugeRational("98750293847520938457029384572093480498357"))
            using (var c = new HugeRational("23094582093845093574093845093485039450934"))
            {
                ulong b = 498734523097853458;
                var expr = a - c*b;
                a.Value = expr;
                Assert.AreEqual("-11518065386718058599763388064972875060082210203928832731415", a.ToString());
            }
        }

        [TestMethod]
        public void RationalSubtractProductSignedLimb()
        {
            using (var a = new HugeRational("98750293847520938457029384572093480498357"))
            using (var c = new HugeRational("-23094582093845093574093845093485039450934"))
            {
                long b = -498734523097853458;
                var expr = a - b*c;
                a.Value = expr;
                Assert.AreEqual("-11518065386718058599763388064972875060082210203928832731415", a.ToString());
            }
        }

        [TestMethod]
        public void RationalSubtractProductSignedLimb2()
        {
            using (var a = new HugeRational("98750293847520938457029384572093480498357"))
            using (var c = new HugeRational("-23094582093845093574093845093485039450934"))
            {
                long b = -498734523097853458;
                a.Value = a - c*b;
                Assert.AreEqual("-11518065386718058599763388064972875060082210203928832731415", a.ToString());
            }
        }

        [TestMethod]
        public void RationalSubtractProductSignedLimb3()
        {
            using (var a = new HugeRational("98750293847520938457029384572093480498357"))
            using (var c = new HugeRational("23094582093845093574093845093485039450934"))
            using (var d = new HugeRational())
            {
                long b = 498734523097853458;
                d.Value = a - c*b;
                Assert.AreEqual("-11518065386718058599763388064972875060082210203928832731415", d.ToString());
            }
        }

        #endregion

        #region Shift Left

        [TestMethod]
        public void RationalShiftLeft()
        {
            using (var a = new HugeRational("-12345700987ABCDEF2345CBDEFA245230948", 16))
            {
                ulong b = 40;
                a.Value = a << b;
                Assert.AreEqual("-12345700987ABCDEF2345CBDEFA2452309480000000000", a.ToString(16));
            }
        }

        #endregion

        #region Shift Right

        [TestMethod]
        public void RationalShiftRight()
        {
            using (var a = new HugeRational("ABCDEF052834524092854092874502983745029345723098457209305983434345", 16))
            {
                ulong b = 96;
                a.Value = a >> b;
                Assert.AreEqual("ABCDEF052834524092854092874502983745029345", a.ToString(16));
            }
        }

        #endregion

        #region Negate

        [TestMethod]
        public void RationalNegate()
        {
            using (var a = new HugeRational("24092854092874502983745029345723098457209"))
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
        public void RationalMakeAbsolute()
        {
            using (var a = new HugeRational("-24092854092874502983745029345723098457209"))
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
        public void RationalDivideHugeRational()
        {
            using (var a = new HugeRational("234052834524092854092874502983745029345723098457209305983434345"))
            using (var b = new HugeRational("394580293847502987609283945873594873409587"))
            {
                a.Value = a / b;
                Assert.AreEqual("593169091750307653294", a.ToString());
            }
        }

        #endregion

        #region Limb

        [TestMethod]
        public void RationalDivideLimb()
        {
            using (var a = new HugeRational("234052834524092854092874502983745029345723098457209305983434345"))
            {
                ulong b = 5931690917503076532;
                a.Value = a / b;
                Assert.AreEqual("39458029384750298767200622330399462537522498", a.ToString());
            }
        }

        #endregion

        #endregion
    }
}
