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
        public void AddHugeInt()
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
        public void AddLimb()
        {
            using (var a = new HugeInt("222509832503450298345029835740293845720"))
            {
                var b = 4288574029879874539UL;
                a.Value = a + b;
                Assert.AreEqual("222509832503450298349318409770173720259", a.ToString());
            }
        }

        [TestMethod]
        public void AddToLimb()
        {
            using (var a = new HugeInt("222509832503450298345029835740293845720"))
            {
                var b = 4288574029879874539UL;
                a.Value = b + a;
                Assert.AreEqual("222509832503450298349318409770173720259", a.ToString());
            }
        }

        [TestMethod]
        public void AddSignedLimb()
        {
            using (var a = new HugeInt("222509832503450298345029835740293845720"))
            {
                var b = -4288574029879874539L;
                a.Value = a + b;
                Assert.AreEqual("222509832503450298340741261710413971181", a.ToString());
            }
        }

        [TestMethod]
        public void AddToSignedLimb()
        {
            using (var a = new HugeInt("222509832503450298345029835740293845720"))
            {
                var b = -4288574029879874539L;
                a.Value = b + a;
                Assert.AreEqual("222509832503450298340741261710413971181", a.ToString());
            }
        }

        [TestMethod]
        public void AddToMaxSignedLimb()
        {
            using (var a = new HugeInt("222509832503450298345029835740293845720"))
            {
                var b = long.MinValue;
                a.Value = b + a;
                Assert.AreEqual("222509832503450298335806463703439069912", a.ToString());
            }
        }

        [TestMethod]
        public void AddExpressionHugeInt()
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
        public void SubtractHugeInt()
        {
            using (var a = new HugeInt("445497268491433028939318409770173720259"))
            using (var b = new HugeInt("222987435987982730594288574029879874539"))
            {
                a.Value = a - b;
                Assert.AreEqual("222509832503450298345029835740293845720", a.ToString());
            }
        }

        [TestMethod]
        public void SubtractLimb()
        {
            using (var a = new HugeInt("222509832503450298349318409770173720259"))
            {
                var b = 4288574029879874539UL;
                a.Value = a - b;
                Assert.AreEqual("222509832503450298345029835740293845720", a.ToString());
            }
        }

        [TestMethod]
        public void SubtractFromLimb()
        {
            using (var a = new HugeInt("222509832503450298349318409770173720259"))
            {
                var b = 4288574029879874539UL;
                a.Value = b - a;
                Assert.AreEqual("-222509832503450298345029835740293845720", a.ToString());
            }
        }

        [TestMethod]
        public void SubtractPositiveSignedLimb()
        {
            using (var a = new HugeInt("222509832503450298349318409770173720259"))
            {
                var b = 4288574029879874539L;
                a.Value = a - b;
                Assert.AreEqual("222509832503450298345029835740293845720", a.ToString());
            }
        }

        [TestMethod]
        public void SubtractFromPositiveSignedLimb()
        {
            using (var a = new HugeInt("222509832503450298349318409770173720259"))
            {
                var b = 4288574029879874539L;
                a.Value = b - a;
                Assert.AreEqual("-222509832503450298345029835740293845720", a.ToString());
            }
        }

        [TestMethod]
        public void SubtractSignedLimb()
        {
            using (var a = new HugeInt("222509832503450298345029835740293845720"))
            {
                var b = -4288574029879874539L;
                a.Value = a - b;
                Assert.AreEqual("222509832503450298349318409770173720259", a.ToString());
            }
        }

        [TestMethod]
        public void SubtractFromSignedLimb()
        {
            using (var a = new HugeInt("222509832503450298345029835740293845720"))
            {
                var b = -4288574029879874539L;
                a.Value = b - a;
                Assert.AreEqual("-222509832503450298349318409770173720259", a.ToString());
            }
        }

        #endregion

        #region Multiply

        [TestMethod]
        public void MultiplyByHugeInt()
        {
            using (var a = new HugeInt("90234098723098475098479385345098345"))
            using (var b = new HugeInt("7859487359873459872354987610987897"))
            {
                a.Value = a * b;
                Assert.AreEqual("709193758343766370701419953880162061353595657143399816050772069730465", a.ToString());
            }
        }

        [TestMethod]
        public void MultiplyByLimb()
        {
            using (var a = new HugeInt("90234098723098475098479385345098345"))
            {
                ulong b = 17390538260286101342;
                a.Value = a * b;
                Assert.AreEqual("1569219546226477273686601978789044606491747469626478990", a.ToString());
            }
        }

        [TestMethod]
        public void MultiplyLimbBy()
        {
            using (var a = new HugeInt("90234098723098475098479385345098345"))
            {
                ulong b = 17390538260286101342;
                a.Value = b * a;
                Assert.AreEqual("1569219546226477273686601978789044606491747469626478990", a.ToString());
            }
        }

        [TestMethod]
        public void MultiplyBySignedLimb()
        {
            using (var a = new HugeInt("90234098723098475098479385345098345"))
            {
                long b = -7390538260286101342;
                a.Value = a * b;
                Assert.AreEqual("-666878558995492522701808125338061156491747469626478990", a.ToString());
            }
        }

        [TestMethod]
        public void MultiplySignedLimbBy()
        {
            using (var a = new HugeInt("90234098723098475098479385345098345"))
            {
                long b = -7390538260286101342;
                a.Value = b * a;
                Assert.AreEqual("-666878558995492522701808125338061156491747469626478990", a.ToString());
            }
        }

        #endregion

        #region Add Product

        [TestMethod]
        public void AddProductHugeInt()
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
        public void AddProductLimb()
        {
            using (var a = new HugeInt("98750293847520938457029384572093480498357"))
            using (var c = new HugeInt("-23094582093845093574093845093485039450934"))
            {
                ulong b = 498734523097853458;
                var expr = a + c*b;
                a.Value = expr;
                Assert.AreEqual("-11518065386718058599763388064972875060082210203928832731415", a.ToString());
            }
        }

        [TestMethod]
        public void AddProductLimbTo()
        {
            using (var a = new HugeInt("98750293847520938457029384572093480498357"))
            using (var c = new HugeInt("-23094582093845093574093845093485039450934"))
            {
                ulong b = 498734523097853458;
                var expr = a + b*c;
                a.Value = expr;
                Assert.AreEqual("-11518065386718058599763388064972875060082210203928832731415", a.ToString());
            }
        }

        [TestMethod]
        public void AddProductLimbTo2()
        {
            using (var a = new HugeInt("98750293847520938457029384572093480498357"))
            using (var c = new HugeInt("-23094582093845093574093845093485039450934"))
            {
                ulong b = 498734523097853458;
                var expr = b*c + a;
                a.Value = expr;
                //TODO how can we test a single addmul was called?
                Assert.AreEqual("-11518065386718058599763388064972875060082210203928832731415", a.ToString());
            }
        }

        [TestMethod]
        public void AddProductLimbTo3()
        {
            using (var a = new HugeInt("98750293847520938457029384572093480498357"))
            using (var c = new HugeInt("-23094582093845093574093845093485039450934"))
            using (var d = new HugeInt())
            {
                ulong b = 498734523097853458;
                var expr = b*c + a;
                d.Value = expr;
                Assert.AreEqual("-11518065386718058599763388064972875060082210203928832731415", d.ToString());
            }
        }

        [TestMethod]
        public void AddProductSignedLimb()
        {
            using (var a = new HugeInt("98750293847520938457029384572093480498357"))
            using (var c = new HugeInt("23094582093845093574093845093485039450934"))
            {
                long b = -498734523097853458;
                var expr = a + c*b;
                a.Value = expr;
                Assert.AreEqual("-11518065386718058599763388064972875060082210203928832731415", a.ToString());
            }
        }

        [TestMethod]
        public void AddProductSignedLimbTo()
        {
            using (var a = new HugeInt("98750293847520938457029384572093480498357"))
            using (var c = new HugeInt("-23094582093845093574093845093485039450934"))
            {
                long b = 498734523097853458;
                var expr = a + b*c;
                a.Value = expr;
                Assert.AreEqual("-11518065386718058599763388064972875060082210203928832731415", a.ToString());
            }
        }

        [TestMethod]
        public void AddProductSignedLimbTo2()
        {
            using (var a = new HugeInt("98750293847520938457029384572093480498357"))
            using (var c = new HugeInt("-23094582093845093574093845093485039450934"))
            {
                long b = 498734523097853458;
                var expr = b*c + a;
                a.Value = expr;
                //TODO how can we test a single addmul was called?
                Assert.AreEqual("-11518065386718058599763388064972875060082210203928832731415", a.ToString());
            }
        }

        [TestMethod]
        public void AddProductSignedLimbTo3()
        {
            using (var a = new HugeInt("98750293847520938457029384572093480498357"))
            using (var c = new HugeInt("23094582093845093574093845093485039450934"))
            using (var d = new HugeInt())
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
        public void SubtractProductHugeInt()
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
        public void SubtractProductLimb()
        {
            using (var a = new HugeInt("98750293847520938457029384572093480498357"))
            using (var c = new HugeInt("23094582093845093574093845093485039450934"))
            {
                ulong b = 498734523097853458;
                var expr = a - c*b;
                a.Value = expr;
                Assert.AreEqual("-11518065386718058599763388064972875060082210203928832731415", a.ToString());
            }
        }

        [TestMethod]
        public void SubtractProductSignedLimb()
        {
            using (var a = new HugeInt("98750293847520938457029384572093480498357"))
            using (var c = new HugeInt("-23094582093845093574093845093485039450934"))
            {
                long b = -498734523097853458;
                var expr = a - b*c;
                a.Value = expr;
                Assert.AreEqual("-11518065386718058599763388064972875060082210203928832731415", a.ToString());
            }
        }

        [TestMethod]
        public void SubtractProductSignedLimb2()
        {
            using (var a = new HugeInt("98750293847520938457029384572093480498357"))
            using (var c = new HugeInt("-23094582093845093574093845093485039450934"))
            {
                long b = -498734523097853458;
                a.Value = a - c*b;
                Assert.AreEqual("-11518065386718058599763388064972875060082210203928832731415", a.ToString());
            }
        }

        [TestMethod]
        public void SubtractProductSignedLimb3()
        {
            using (var a = new HugeInt("98750293847520938457029384572093480498357"))
            using (var c = new HugeInt("23094582093845093574093845093485039450934"))
            using (var d = new HugeInt())
            {
                long b = 498734523097853458;
                d.Value = a - c*b;
                Assert.AreEqual("-11518065386718058599763388064972875060082210203928832731415", d.ToString());
            }
        }

        #endregion

        #region Shift Left

        [TestMethod]
        public void ShiftLeft()
        {
            using (var a = new HugeInt("-12345700987ABCDEF2345CBDEFA245230948", 16))
            {
                ulong b = 40;
                a.Value = a << b;
                Assert.AreEqual("-12345700987ABCDEF2345CBDEFA2452309480000000000", a.ToString(-16));
            }
        }

        #endregion

        #region Negate

        [TestMethod]
        public void Negate()
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
        public void MakeAbsolute()
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
        public void DivideHugeInt()
        {
            using (var a = new HugeInt("234052834524092854092874502983745029345723098457209305983434345"))
            using (var b = new HugeInt("394580293847502987609283945873594873409587"))
            {
                a.Value = a / b;
                Assert.AreEqual("593169091750307653294", a.ToString());
            }
        }

        [TestMethod]
        public void DivideHugeIntCeiling()
        {
            using (var a = new HugeInt("234052834524092854092874502983745029345723098457209305983434345"))
            using (var b = new HugeInt("394580293847502987609283945873594873409587"))
            {
                a.Value = (a / b).Rounding(RoundingModes.Ceiling);
                Assert.AreEqual("593169091750307653295", a.ToString());
            }
        }

        [TestMethod]
        public void DivideHugeIntNegativeCeiling()
        {
            using (var a = new HugeInt("-234052834524092854092874502983745029345723098457209305983434345"))
            using (var b = new HugeInt("394580293847502987609283945873594873409587"))
            {
                a.Value = (a / b).Rounding(RoundingModes.Ceiling);
                Assert.AreEqual("-593169091750307653294", a.ToString());
            }
        }

        [TestMethod]
        public void DivideHugeIntFloor()
        {
            using (var a = new HugeInt("234052834524092854092874502983745029345723098457209305983434345"))
            using (var b = new HugeInt("394580293847502987609283945873594873409587"))
            {
                a.Value = (a / b).Rounding(RoundingModes.Floor);
                Assert.AreEqual("593169091750307653294", a.ToString());
            }
        }

        [TestMethod]
        public void DivideHugeIntNegativeFloor()
        {
            using (var a = new HugeInt("-234052834524092854092874502983745029345723098457209305983434345"))
            using (var b = new HugeInt("394580293847502987609283945873594873409587"))
            {
                a.Value = (a / b).Rounding(RoundingModes.Floor);
                Assert.AreEqual("-593169091750307653295", a.ToString());
            }
        }

        [TestMethod]
        public void DivideHugeIntTruncate()
        {
            using (var a = new HugeInt("234052834524092854092874502983745029345723098457209305983434345"))
            using (var b = new HugeInt("394580293847502987609283945873594873409587"))
            {
                a.Value = (a / b).Rounding(RoundingModes.Truncate);
                Assert.AreEqual("593169091750307653294", a.ToString());
            }
        }

        [TestMethod]
        public void DivideHugeIntNegativeTruncate()
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
        public void DivideHugeIntCeilingWithMod()
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
        public void DivideHugeIntNegativeCeilingWithMod()
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
        public void DivideHugeIntFloorWithMod()
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
        public void DivideHugeIntNegativeFloorWithMod()
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
        public void DivideHugeIntTruncateWithMod()
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
        public void DivideHugeIntNegativeTruncateWithMod()
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
        public void DivideLimb()
        {
            using (var a = new HugeInt("234052834524092854092874502983745029345723098457209305983434345"))
            {
                ulong b = 5931690917503076532;
                a.Value = a / b;
                Assert.AreEqual("39458029384750298767200622330399462537522498", a.ToString());
            }
        }

        [TestMethod]
        public void DivideLimbCeiling()
        {
            using (var a = new HugeInt("234052834524092854092874502983745029345723098457209305983434345"))
            {
                ulong b = 5931690917503076532;
                a.Value = (a / b).Rounding(RoundingModes.Ceiling);
                Assert.AreEqual("39458029384750298767200622330399462537522499", a.ToString());
            }
        }

        [TestMethod]
        public void DivideLimbNegativeCeiling()
        {
            using (var a = new HugeInt("-234052834524092854092874502983745029345723098457209305983434345"))
            {
                ulong b = 5931690917503076532;
                a.Value = (a / b).Rounding(RoundingModes.Ceiling);
                Assert.AreEqual("-39458029384750298767200622330399462537522498", a.ToString());
            }
        }

        [TestMethod]
        public void DivideLimbFloor()
        {
            using (var a = new HugeInt("234052834524092854092874502983745029345723098457209305983434345"))
            {
                ulong b = 5931690917503076532;
                a.Value = (a / b).Rounding(RoundingModes.Floor);
                Assert.AreEqual("39458029384750298767200622330399462537522498", a.ToString());
            }
        }

        [TestMethod]
        public void DivideLimbNegativeFloor()
        {
            using (var a = new HugeInt("-234052834524092854092874502983745029345723098457209305983434345"))
            {
                ulong b = 5931690917503076532;
                a.Value = (a / b).Rounding(RoundingModes.Floor);
                Assert.AreEqual("-39458029384750298767200622330399462537522499", a.ToString());
            }
        }

        [TestMethod]
        public void DivideLimbTruncate()
        {
            using (var a = new HugeInt("234052834524092854092874502983745029345723098457209305983434345"))
            {
                ulong b = 5931690917503076532;
                a.Value = (a / b).Rounding(RoundingModes.Truncate);
                Assert.AreEqual("39458029384750298767200622330399462537522498", a.ToString());
            }
        }

        [TestMethod]
        public void DivideLimbNegativeTruncate()
        {
            using (var a = new HugeInt("-234052834524092854092874502983745029345723098457209305983434345"))
            {
                ulong b = 5931690917503076532;
                a.Value = (a / b).Rounding(RoundingModes.Truncate);
                Assert.AreEqual("-39458029384750298767200622330399462537522498", a.ToString());
            }
        }

        #endregion

        #region Limb with Mod

        [TestMethod]
        public void DivideLimbWithMod()
        {
            using (var a = new HugeInt("234052834524092854092874502983745029345723098457209305983434345"))
            using (var c = new HugeInt())
            {
                ulong b = 5931690917503076532;
                a.Value = (a / b).SavingRemainderTo(c);
                Assert.AreEqual("39458029384750298767200622330399462537522498", a.ToString());
                Assert.AreEqual("5599417901817617409", c.ToString());
            }
        }

        [TestMethod]
        public void DivideLimbCeilingWithMod()
        {
            using (var a = new HugeInt("234052834524092854092874502983745029345723098457209305983434345"))
            using (var c = new HugeInt())
            {
                ulong b = 5931690917503076532;
                a.Value = (a / b).SavingRemainderTo(c).Rounding(RoundingModes.Ceiling);
                Assert.AreEqual("39458029384750298767200622330399462537522499", a.ToString());
                Assert.AreEqual("-332273015685459123", c.ToString());
            }
        }

        [TestMethod]
        public void DivideLimbNegativeCeilingWithMod()
        {
            using (var a = new HugeInt("-234052834524092854092874502983745029345723098457209305983434345"))
            using (var c = new HugeInt())
            {
                ulong b = 5931690917503076532;
                a.Value = (a / b).SavingRemainderTo(c).Rounding(RoundingModes.Ceiling);
                Assert.AreEqual("-39458029384750298767200622330399462537522498", a.ToString());
                Assert.AreEqual("-5599417901817617409", c.ToString());
            }
        }

        [TestMethod]
        public void DivideLimbFloorWithMod()
        {
            using (var a = new HugeInt("234052834524092854092874502983745029345723098457209305983434345"))
            using (var c = new HugeInt())
            {
                ulong b = 5931690917503076532;
                a.Value = (a / b).SavingRemainderTo(c).Rounding(RoundingModes.Floor);
                Assert.AreEqual("39458029384750298767200622330399462537522498", a.ToString());
                Assert.AreEqual("5599417901817617409", c.ToString());
            }
        }

        [TestMethod]
        public void DivideLimbNegativeFloorWithMod()
        {
            using (var a = new HugeInt("-234052834524092854092874502983745029345723098457209305983434345"))
            using (var c = new HugeInt())
            {
                ulong b = 5931690917503076532;
                a.Value = (a / b).SavingRemainderTo(c).Rounding(RoundingModes.Floor);
                Assert.AreEqual("-39458029384750298767200622330399462537522499", a.ToString());
                Assert.AreEqual("332273015685459123", c.ToString());
            }
        }

        [TestMethod]
        public void DivideLimbTruncateWithMod()
        {
            using (var a = new HugeInt("234052834524092854092874502983745029345723098457209305983434345"))
            using (var c = new HugeInt())
            {
                ulong b = 5931690917503076532;
                a.Value = (a / b).SavingRemainderTo(c).Rounding(RoundingModes.Truncate);
                Assert.AreEqual("39458029384750298767200622330399462537522498", a.ToString());
                Assert.AreEqual("5599417901817617409", c.ToString());
            }
        }

        [TestMethod]
        public void DivideLimbNegativeTruncateWithMod()
        {
            using (var a = new HugeInt("-234052834524092854092874502983745029345723098457209305983434345"))
            using (var c = new HugeInt())
            {
                ulong b = 5931690917503076532;
                a.Value = (a / b).SavingRemainderTo(c).Rounding(RoundingModes.Truncate);
                Assert.AreEqual("-39458029384750298767200622330399462537522498", a.ToString());
                Assert.AreEqual("-5599417901817617409", c.ToString());
            }
        }

        #endregion

        #region Limb with Mod x 2

        [TestMethod]
        public void DivideLimbWithMod2()
        {
            using (var a = new HugeInt("234052834524092854092874502983745029345723098457209305983434345"))
            using (var c = new HugeInt())
            {
                ulong b = 5931690917503076532;
                ulong d = 0;
                a.Value = (a / b).SettingRemainderTo(x => d = x).SavingRemainderTo(c);
                Assert.AreEqual("39458029384750298767200622330399462537522498", a.ToString());
                Assert.AreEqual("5599417901817617409", c.ToString());
                Assert.AreEqual(d.ToString(), c.ToString());
            }
        }

        [TestMethod]
        public void DivideLimbCeilingWithMod2()
        {
            using (var a = new HugeInt("234052834524092854092874502983745029345723098457209305983434345"))
            using (var c = new HugeInt())
            {
                ulong b = 5931690917503076532;
                ulong d = 0;
                a.Value = (a / b).SettingRemainderTo(x => d = x).SavingRemainderTo(c).Rounding(RoundingModes.Ceiling);
                Assert.AreEqual("39458029384750298767200622330399462537522499", a.ToString());
                Assert.AreEqual("-332273015685459123", c.ToString());
                Assert.AreEqual("-" + d.ToString(), c.ToString());
            }
        }

        [TestMethod]
        public void DivideLimbNegativeCeilingWithMod2()
        {
            using (var a = new HugeInt("-234052834524092854092874502983745029345723098457209305983434345"))
            using (var c = new HugeInt())
            {
                ulong b = 5931690917503076532;
                ulong d = 0;
                a.Value = (a / b).SettingRemainderTo(x => d = x).SavingRemainderTo(c).Rounding(RoundingModes.Ceiling);
                Assert.AreEqual("-39458029384750298767200622330399462537522498", a.ToString());
                Assert.AreEqual("-5599417901817617409", c.ToString());
                Assert.AreEqual("-" + d.ToString(), c.ToString());
            }
        }

        [TestMethod]
        public void DivideLimbFloorWithMod2()
        {
            using (var a = new HugeInt("234052834524092854092874502983745029345723098457209305983434345"))
            using (var c = new HugeInt())
            {
                ulong b = 5931690917503076532;
                ulong d = 0;
                a.Value = (a / b).SettingRemainderTo(x => d = x).SavingRemainderTo(c).Rounding(RoundingModes.Floor);
                Assert.AreEqual("39458029384750298767200622330399462537522498", a.ToString());
                Assert.AreEqual("5599417901817617409", c.ToString());
                Assert.AreEqual(d.ToString(), c.ToString());
            }
        }

        [TestMethod]
        public void DivideLimbNegativeFloorWithMod2()
        {
            using (var a = new HugeInt("-234052834524092854092874502983745029345723098457209305983434345"))
            using (var c = new HugeInt())
            {
                ulong b = 5931690917503076532;
                ulong d = 0;
                a.Value = (a / b).SettingRemainderTo(x => d = x).SavingRemainderTo(c).Rounding(RoundingModes.Floor);
                Assert.AreEqual("-39458029384750298767200622330399462537522499", a.ToString());
                Assert.AreEqual("332273015685459123", c.ToString());
                Assert.AreEqual(d.ToString(), c.ToString());
            }
        }

        [TestMethod]
        public void DivideLimbTruncateWithMod2()
        {
            using (var a = new HugeInt("234052834524092854092874502983745029345723098457209305983434345"))
            using (var c = new HugeInt())
            {
                ulong b = 5931690917503076532;
                ulong d = 0;
                a.Value = (a / b).SettingRemainderTo(x => d = x).SavingRemainderTo(c).Rounding(RoundingModes.Truncate);
                Assert.AreEqual("39458029384750298767200622330399462537522498", a.ToString());
                Assert.AreEqual("5599417901817617409", c.ToString());
                Assert.AreEqual(d.ToString(), c.ToString());
            }
        }

        [TestMethod]
        public void DivideLimbNegativeTruncateWithMod2()
        {
            using (var a = new HugeInt("-234052834524092854092874502983745029345723098457209305983434345"))
            using (var c = new HugeInt())
            {
                ulong b = 5931690917503076532;
                ulong d = 0;
                a.Value = (a / b).SettingRemainderTo(x => d = x).SavingRemainderTo(c).Rounding(RoundingModes.Truncate);
                Assert.AreEqual("-39458029384750298767200622330399462537522498", a.ToString());
                Assert.AreEqual("-5599417901817617409", c.ToString());
                Assert.AreEqual("-" + d.ToString(), c.ToString());
            }
        }

        #endregion

        #region Limb with Limb Mod

        [TestMethod]
        public void DivideLimbWithMod3()
        {
            using (var a = new HugeInt("234052834524092854092874502983745029345723098457209305983434345"))
            {
                ulong b = 5931690917503076532;
                ulong d = 0;
                a.Value = (a / b).SettingRemainderTo(x => d = x);
                Assert.AreEqual("39458029384750298767200622330399462537522498", a.ToString());
                Assert.AreEqual("5599417901817617409", d.ToString());
            }
        }

        [TestMethod]
        public void DivideLimbCeilingWithMod3()
        {
            using (var a = new HugeInt("234052834524092854092874502983745029345723098457209305983434345"))
            {
                ulong b = 5931690917503076532;
                ulong d = 0;
                a.Value = (a / b).SettingRemainderTo(x => d = x).Rounding(RoundingModes.Ceiling);
                Assert.AreEqual("39458029384750298767200622330399462537522499", a.ToString());
                Assert.AreEqual("332273015685459123", d.ToString());
            }
        }

        [TestMethod]
        public void DivideLimbNegativeCeilingWithMod3()
        {
            using (var a = new HugeInt("-234052834524092854092874502983745029345723098457209305983434345"))
            {
                ulong b = 5931690917503076532;
                ulong d = 0;
                a.Value = (a / b).SettingRemainderTo(x => d = x).Rounding(RoundingModes.Ceiling);
                Assert.AreEqual("-39458029384750298767200622330399462537522498", a.ToString());
                Assert.AreEqual("5599417901817617409", d.ToString());
            }
        }

        [TestMethod]
        public void DivideLimbFloorWithMod3()
        {
            using (var a = new HugeInt("234052834524092854092874502983745029345723098457209305983434345"))
            {
                ulong b = 5931690917503076532;
                ulong d = 0;
                a.Value = (a / b).SettingRemainderTo(x => d = x).Rounding(RoundingModes.Floor);
                Assert.AreEqual("39458029384750298767200622330399462537522498", a.ToString());
                Assert.AreEqual("5599417901817617409", d.ToString());
            }
        }

        [TestMethod]
        public void DivideLimbNegativeFloorWithMod3()
        {
            using (var a = new HugeInt("-234052834524092854092874502983745029345723098457209305983434345"))
            {
                ulong b = 5931690917503076532;
                ulong d = 0;
                a.Value = (a / b).SettingRemainderTo(x => d = x).Rounding(RoundingModes.Floor);
                Assert.AreEqual("-39458029384750298767200622330399462537522499", a.ToString());
                Assert.AreEqual("332273015685459123", d.ToString());
            }
        }

        [TestMethod]
        public void DivideLimbTruncateWithMod3()
        {
            using (var a = new HugeInt("234052834524092854092874502983745029345723098457209305983434345"))
            {
                ulong b = 5931690917503076532;
                ulong d = 0;
                a.Value = (a / b).SettingRemainderTo(x => d = x).Rounding(RoundingModes.Truncate);
                Assert.AreEqual("39458029384750298767200622330399462537522498", a.ToString());
                Assert.AreEqual("5599417901817617409", d.ToString());
            }
        }

        [TestMethod]
        public void DivideLimbNegativeTruncateWithMod3()
        {
            using (var a = new HugeInt("-234052834524092854092874502983745029345723098457209305983434345"))
            {
                ulong b = 5931690917503076532;
                ulong d = 0;
                a.Value = (a / b).SettingRemainderTo(x => d = x).Rounding(RoundingModes.Truncate);
                Assert.AreEqual("-39458029384750298767200622330399462537522498", a.ToString());
                Assert.AreEqual("5599417901817617409", d.ToString());
            }
        }

        #endregion

        #endregion

        #region Mod

        #region Int

        [TestMethod]
        public void ModHugeInt()
        {
            using (var a = new HugeInt("234052834524092854092874502983745029345723098457209305983434345"))
            using (var b = new HugeInt("394580293847502987609283945873594873409587"))
            {
                a.Value = a % b;
                Assert.AreEqual("114368714235760586972822754176083531704767", a.ToString());
            }
        }

        [TestMethod]
        public void ModHugeIntCeiling()
        {
            using (var a = new HugeInt("234052834524092854092874502983745029345723098457209305983434345"))
            using (var b = new HugeInt("394580293847502987609283945873594873409587"))
            {
                a.Value = (a % b).Rounding(RoundingModes.Ceiling);
                Assert.AreEqual("-280211579611742400636461191697511341704820", a.ToString());
            }
        }

        [TestMethod]
        public void ModHugeIntNegativeCeiling()
        {
            using (var a = new HugeInt("-234052834524092854092874502983745029345723098457209305983434345"))
            using (var b = new HugeInt("394580293847502987609283945873594873409587"))
            {
                a.Value = (a % b).Rounding(RoundingModes.Ceiling);
                Assert.AreEqual("-114368714235760586972822754176083531704767", a.ToString());
            }
        }

        [TestMethod]
        public void ModHugeIntFloor()
        {
            using (var a = new HugeInt("234052834524092854092874502983745029345723098457209305983434345"))
            using (var b = new HugeInt("394580293847502987609283945873594873409587"))
            {
                a.Value = (a % b).Rounding(RoundingModes.Floor);
                Assert.AreEqual("114368714235760586972822754176083531704767", a.ToString());
            }
        }

        [TestMethod]
        public void ModHugeIntNegativeFloor()
        {
            using (var a = new HugeInt("-234052834524092854092874502983745029345723098457209305983434345"))
            using (var b = new HugeInt("394580293847502987609283945873594873409587"))
            {
                a.Value = (a % b).Rounding(RoundingModes.Floor);
                Assert.AreEqual("280211579611742400636461191697511341704820", a.ToString());
            }
        }

        [TestMethod]
        public void ModHugeIntTruncate()
        {
            using (var a = new HugeInt("234052834524092854092874502983745029345723098457209305983434345"))
            using (var b = new HugeInt("394580293847502987609283945873594873409587"))
            {
                a.Value = (a % b).Rounding(RoundingModes.Truncate);
                Assert.AreEqual("114368714235760586972822754176083531704767", a.ToString());
            }
        }

        [TestMethod]
        public void ModHugeIntNegativeTruncate()
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
        public void ModHugeIntCeilingWithQuotient()
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
        public void ModHugeIntNegativeCeilingWithQuotient()
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
        public void ModHugeIntFloorWithQuotient()
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
        public void ModHugeIntNegativeFloorWithQuotient()
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
        public void ModHugeIntTruncateWithQuotient()
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
        public void ModHugeIntNegativeTruncateWithQuotient()
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
        public void ModLimb()
        {
            using (var a = new HugeInt("234052834524092854092874502983745029345723098457209305983434345"))
            {
                ulong b = 5931690917503076532;
                a.Value = (a % b);
                Assert.AreEqual("5599417901817617409", a.ToString());
            }
        }

        [TestMethod]
        public void ModLimbCeiling()
        {
            using (var a = new HugeInt("234052834524092854092874502983745029345723098457209305983434345"))
            {
                ulong b = 5931690917503076532;
                a.Value = (a % b).Rounding(RoundingModes.Ceiling);
                Assert.AreEqual("-332273015685459123", a.ToString());
            }
        }

        [TestMethod]
        public void ModLimbNegativeCeiling()
        {
            using (var a = new HugeInt("-234052834524092854092874502983745029345723098457209305983434345"))
            {
                ulong b = 5931690917503076532;
                a.Value = (a % b).Rounding(RoundingModes.Ceiling);
                Assert.AreEqual("-5599417901817617409", a.ToString());
            }
        }

        [TestMethod]
        public void ModLimbFloor()
        {
            using (var a = new HugeInt("234052834524092854092874502983745029345723098457209305983434345"))
            {
                ulong b = 5931690917503076532;
                a.Value = (a % b).Rounding(RoundingModes.Floor);
                Assert.AreEqual("5599417901817617409", a.ToString());
            }
        }

        [TestMethod]
        public void ModLimbNegativeFloor()
        {
            using (var a = new HugeInt("-234052834524092854092874502983745029345723098457209305983434345"))
            {
                ulong b = 5931690917503076532;
                a.Value = (a % b).Rounding(RoundingModes.Floor);
                Assert.AreEqual("332273015685459123", a.ToString());
            }
        }

        [TestMethod]
        public void ModLimbTruncate()
        {
            using (var a = new HugeInt("234052834524092854092874502983745029345723098457209305983434345"))
            {
                ulong b = 5931690917503076532;
                a.Value = (a % b).Rounding(RoundingModes.Truncate);
                Assert.AreEqual("5599417901817617409", a.ToString());
            }
        }

        [TestMethod]
        public void ModLimbNegativeTruncate()
        {
            using (var a = new HugeInt("-234052834524092854092874502983745029345723098457209305983434345"))
            {
                ulong b = 5931690917503076532;
                a.Value = (a % b).Rounding(RoundingModes.Truncate);
                Assert.AreEqual("-5599417901817617409", a.ToString());
            }
        }

        #endregion

        #region Limb With Quotient

        [TestMethod]
        public void ModLimbWithQuotient()
        {
            using (var a = new HugeInt("234052834524092854092874502983745029345723098457209305983434345"))
            using (var c = new HugeInt())
            {
                ulong b = 5931690917503076532;
                a.Value = (a % b).SavingQuotientTo(c);
                Assert.AreEqual("39458029384750298767200622330399462537522498", c.ToString());
                Assert.AreEqual("5599417901817617409", a.ToString());
            }
        }

        [TestMethod]
        public void ModLimbCeilingWithQuotient()
        {
            using (var a = new HugeInt("234052834524092854092874502983745029345723098457209305983434345"))
            using (var c = new HugeInt())
            {
                ulong b = 5931690917503076532;
                a.Value = (a % b).SavingQuotientTo(c).Rounding(RoundingModes.Ceiling);
                Assert.AreEqual("39458029384750298767200622330399462537522499", c.ToString());
                Assert.AreEqual("-332273015685459123", a.ToString());
            }
        }

        [TestMethod]
        public void ModLimbNegativeCeilingWithQuotient()
        {
            using (var a = new HugeInt("-234052834524092854092874502983745029345723098457209305983434345"))
            using (var c = new HugeInt())
            {
                ulong b = 5931690917503076532;
                a.Value = (a % b).SavingQuotientTo(c).Rounding(RoundingModes.Ceiling);
                Assert.AreEqual("-39458029384750298767200622330399462537522498", c.ToString());
                Assert.AreEqual("-5599417901817617409", a.ToString());
            }
        }

        [TestMethod]
        public void ModLimbFloorWithQuotient()
        {
            using (var a = new HugeInt("234052834524092854092874502983745029345723098457209305983434345"))
            using (var c = new HugeInt())
            {
                ulong b = 5931690917503076532;
                a.Value = (a % b).SavingQuotientTo(c).Rounding(RoundingModes.Floor);
                Assert.AreEqual("39458029384750298767200622330399462537522498", c.ToString());
                Assert.AreEqual("5599417901817617409", a.ToString());
            }
        }

        [TestMethod]
        public void ModLimbNegativeFloorWithQuotient()
        {
            using (var a = new HugeInt("-234052834524092854092874502983745029345723098457209305983434345"))
            using (var c = new HugeInt())
            {
                ulong b = 5931690917503076532;
                a.Value = (a % b).SavingQuotientTo(c).Rounding(RoundingModes.Floor);
                Assert.AreEqual("-39458029384750298767200622330399462537522499", c.ToString());
                Assert.AreEqual("332273015685459123", a.ToString());
            }
        }

        [TestMethod]
        public void ModLimbTruncateWithQuotient()
        {
            using (var a = new HugeInt("234052834524092854092874502983745029345723098457209305983434345"))
            using (var c = new HugeInt())
            {
                ulong b = 5931690917503076532;
                a.Value = (a % b).SavingQuotientTo(c).Rounding(RoundingModes.Truncate);
                Assert.AreEqual("39458029384750298767200622330399462537522498", c.ToString());
                Assert.AreEqual("5599417901817617409", a.ToString());
            }
        }

        [TestMethod]
        public void ModLimbNegativeTruncateWithQuotient()
        {
            using (var a = new HugeInt("-234052834524092854092874502983745029345723098457209305983434345"))
            using (var c = new HugeInt())
            {
                ulong b = 5931690917503076532;
                a.Value = (a % b).SavingQuotientTo(c).Rounding(RoundingModes.Truncate);
                Assert.AreEqual("-39458029384750298767200622330399462537522498", c.ToString());
                Assert.AreEqual("-5599417901817617409", a.ToString());
            }
        }

        #endregion

        #region Limb With Quotient and Limb Mod

        [TestMethod]
        public void ModLimbWithQuotient2()
        {
            using (var a = new HugeInt("234052834524092854092874502983745029345723098457209305983434345"))
            using (var c = new HugeInt())
            {
                ulong b = 5931690917503076532;
                ulong d = 0;
                a.Value = (a % b).SettingRemainderTo(x => d = x).SavingQuotientTo(c);
                Assert.AreEqual("39458029384750298767200622330399462537522498", c.ToString());
                Assert.AreEqual("5599417901817617409", a.ToString());
                Assert.AreEqual(d.ToString(), a.ToString());
            }
        }

        [TestMethod]
        public void ModLimbCeilingWithQuotient2()
        {
            using (var a = new HugeInt("234052834524092854092874502983745029345723098457209305983434345"))
            using (var c = new HugeInt())
            {
                ulong b = 5931690917503076532;
                ulong d = 0;
                a.Value = (a % b).SettingRemainderTo(x => d = x).SavingQuotientTo(c).Rounding(RoundingModes.Ceiling);
                Assert.AreEqual("39458029384750298767200622330399462537522499", c.ToString());
                Assert.AreEqual("-332273015685459123", a.ToString());
                Assert.AreEqual("-" + d.ToString(), a.ToString());
            }
        }

        [TestMethod]
        public void ModLimbNegativeCeilingWithQuotient2()
        {
            using (var a = new HugeInt("-234052834524092854092874502983745029345723098457209305983434345"))
            using (var c = new HugeInt())
            {
                ulong b = 5931690917503076532;
                ulong d = 0;
                a.Value = (a % b).SettingRemainderTo(x => d = x).SavingQuotientTo(c).Rounding(RoundingModes.Ceiling);
                Assert.AreEqual("-39458029384750298767200622330399462537522498", c.ToString());
                Assert.AreEqual("-5599417901817617409", a.ToString());
                Assert.AreEqual("-" + d.ToString(), a.ToString());
            }
        }

        [TestMethod]
        public void ModLimbFloorWithQuotient2()
        {
            using (var a = new HugeInt("234052834524092854092874502983745029345723098457209305983434345"))
            using (var c = new HugeInt())
            {
                ulong b = 5931690917503076532;
                ulong d = 0;
                a.Value = (a % b).SettingRemainderTo(x => d = x).SavingQuotientTo(c).Rounding(RoundingModes.Floor);
                Assert.AreEqual("39458029384750298767200622330399462537522498", c.ToString());
                Assert.AreEqual("5599417901817617409", a.ToString());
                Assert.AreEqual(d.ToString(), a.ToString());
            }
        }

        [TestMethod]
        public void ModLimbNegativeFloorWithQuotient2()
        {
            using (var a = new HugeInt("-234052834524092854092874502983745029345723098457209305983434345"))
            using (var c = new HugeInt())
            {
                ulong b = 5931690917503076532;
                ulong d = 0;
                a.Value = (a % b).SettingRemainderTo(x => d = x).SavingQuotientTo(c).Rounding(RoundingModes.Floor);
                Assert.AreEqual("-39458029384750298767200622330399462537522499", c.ToString());
                Assert.AreEqual("332273015685459123", a.ToString());
                Assert.AreEqual(d.ToString(), a.ToString());
            }
        }

        [TestMethod]
        public void ModLimbTruncateWithQuotient2()
        {
            using (var a = new HugeInt("234052834524092854092874502983745029345723098457209305983434345"))
            using (var c = new HugeInt())
            {
                ulong b = 5931690917503076532;
                ulong d = 0;
                a.Value = (a % b).SettingRemainderTo(x => d = x).SavingQuotientTo(c).Rounding(RoundingModes.Truncate);
                Assert.AreEqual("39458029384750298767200622330399462537522498", c.ToString());
                Assert.AreEqual("5599417901817617409", a.ToString());
                Assert.AreEqual(d.ToString(), a.ToString());
            }
        }

        [TestMethod]
        public void ModLimbNegativeTruncateWithQuotient2()
        {
            using (var a = new HugeInt("-234052834524092854092874502983745029345723098457209305983434345"))
            using (var c = new HugeInt())
            {
                ulong b = 5931690917503076532;
                ulong d = 0;
                a.Value = (a % b).SettingRemainderTo(x => d = x).SavingQuotientTo(c).Rounding(RoundingModes.Truncate);
                Assert.AreEqual("-39458029384750298767200622330399462537522498", c.ToString());
                Assert.AreEqual("-5599417901817617409", a.ToString());
                Assert.AreEqual("-" + d.ToString(), a.ToString());
            } 

        }

        #endregion

        #region Limb With Limb Mod

        [TestMethod]
        public void ModLimbWithQuotient3()
        {
            using (var a = new HugeInt("234052834524092854092874502983745029345723098457209305983434345"))
            {
                ulong b = 5931690917503076532;
                ulong d = 0;
                a.Value = (a % b).SettingRemainderTo(x => d = x);
                Assert.AreEqual("5599417901817617409", a.ToString());
                Assert.AreEqual(d.ToString(), a.ToString());
            }
        }

        [TestMethod]
        public void ModLimbCeilingWithQuotient3()
        {
            using (var a = new HugeInt("234052834524092854092874502983745029345723098457209305983434345"))
            {
                ulong b = 5931690917503076532;
                ulong d = 0;
                a.Value = (a % b).SettingRemainderTo(x => d = x).Rounding(RoundingModes.Ceiling);
                Assert.AreEqual("-332273015685459123", a.ToString());
                Assert.AreEqual("-" + d.ToString(), a.ToString());
            }
        }

        [TestMethod]
        public void ModLimbNegativeCeilingWithQuotient3()
        {
            using (var a = new HugeInt("-234052834524092854092874502983745029345723098457209305983434345"))
            {
                ulong b = 5931690917503076532;
                ulong d = 0;
                a.Value = (a % b).SettingRemainderTo(x => d = x).Rounding(RoundingModes.Ceiling);
                Assert.AreEqual("-5599417901817617409", a.ToString());
                Assert.AreEqual("-" + d.ToString(), a.ToString());
            }
        }

        [TestMethod]
        public void ModLimbFloorWithQuotient3()
        {
            using (var a = new HugeInt("234052834524092854092874502983745029345723098457209305983434345"))
            {
                ulong b = 5931690917503076532;
                ulong d = 0;
                a.Value = (a % b).SettingRemainderTo(x => d = x).Rounding(RoundingModes.Floor);
                Assert.AreEqual("5599417901817617409", a.ToString());
                Assert.AreEqual(d.ToString(), a.ToString());
            }
        }

        [TestMethod]
        public void ModLimbNegativeFloorWithQuotient3()
        {
            using (var a = new HugeInt("-234052834524092854092874502983745029345723098457209305983434345"))
            {
                ulong b = 5931690917503076532;
                ulong d = 0;
                a.Value = (a % b).SettingRemainderTo(x => d = x).Rounding(RoundingModes.Floor);
                Assert.AreEqual("332273015685459123", a.ToString());
                Assert.AreEqual(d.ToString(), a.ToString());
            }
        }

        [TestMethod]
        public void ModLimbTruncateWithQuotient3()
        {
            using (var a = new HugeInt("234052834524092854092874502983745029345723098457209305983434345"))
            {
                ulong b = 5931690917503076532;
                ulong d = 0;
                a.Value = (a % b).SettingRemainderTo(x => d = x).Rounding(RoundingModes.Truncate);
                Assert.AreEqual("5599417901817617409", a.ToString());
                Assert.AreEqual(d.ToString(), a.ToString());
            }
        }

        [TestMethod]
        public void ModLimbNegativeTruncateWithQuotient3()
        {
            using (var a = new HugeInt("-234052834524092854092874502983745029345723098457209305983434345"))
            {
                ulong b = 5931690917503076532;
                ulong d = 0;
                a.Value = (a % b).SettingRemainderTo(x => d = x).Rounding(RoundingModes.Truncate);
                Assert.AreEqual("-5599417901817617409", a.ToString());
                Assert.AreEqual("-" + d.ToString(), a.ToString());
            }

        }

        #endregion

        #region Mod Limb function

        [TestMethod]
        public void ModLimbFunction()
        {
            using (var a = new HugeInt("234052834524092854092874502983745029345723098457209305983434345"))
            {
                ulong b = 5931690917503076532;
                ulong d = a.Mod(b);
                Assert.AreEqual("5599417901817617409", d.ToString());
            }
        }

        [TestMethod]
        public void ModLimbFunctionCeiling()
        {
            using (var a = new HugeInt("234052834524092854092874502983745029345723098457209305983434345"))
            {
                ulong b = 5931690917503076532;
                ulong d = a.Mod(b, RoundingModes.Ceiling);
                Assert.AreEqual("332273015685459123", d.ToString());
            }
        }

        [TestMethod]
        public void ModLimbFunctionNegativeCeiling()
        {
            using (var a = new HugeInt("-234052834524092854092874502983745029345723098457209305983434345"))
            {
                ulong b = 5931690917503076532;
                ulong d = a.Mod(b, RoundingModes.Ceiling);
                Assert.AreEqual("5599417901817617409", d.ToString());
            }
        }

        [TestMethod]
        public void ModLimbFunctionFloor()
        {
            using (var a = new HugeInt("234052834524092854092874502983745029345723098457209305983434345"))
            {
                ulong b = 5931690917503076532;
                ulong d = a.Mod(b, RoundingModes.Floor);
                Assert.AreEqual("5599417901817617409", d.ToString());
            }
        }

        [TestMethod]
        public void ModLimbFunctionNegativeFloor()
        {
            using (var a = new HugeInt("-234052834524092854092874502983745029345723098457209305983434345"))
            {
                ulong b = 5931690917503076532;
                ulong d = a.Mod(b, RoundingModes.Floor);
                Assert.AreEqual("332273015685459123", d.ToString());
            }
        }

        [TestMethod]
        public void ModLimbFunctionTruncate()
        {
            using (var a = new HugeInt("234052834524092854092874502983745029345723098457209305983434345"))
            {
                ulong b = 5931690917503076532;
                ulong d = a.Mod(b, RoundingModes.Truncate);
                Assert.AreEqual("5599417901817617409", d.ToString());
            }
        }

        [TestMethod]
        public void ModLimbFunctionNegativeTruncate()
        {
            using (var a = new HugeInt("-234052834524092854092874502983745029345723098457209305983434345"))
            {
                ulong b = 5931690917503076532;
                ulong d = a.Mod(b, RoundingModes.Truncate);
                Assert.AreEqual("5599417901817617409", d.ToString());
            }
        }

        #endregion

        #endregion
    }
}
