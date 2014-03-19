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
using System.Collections.Generic;
using System.Linq;
using System.Reflection;
using System.Text;
using System.Threading.Tasks;
using Microsoft.VisualStudio.TestTools.UnitTesting;

namespace MPIR.Tests.HugeIntTests
{
    [TestClass]
    public class ExpressionTests
    {
        [TestMethod]
        public void TestAllExpressions()
        {
            var baseExpr = typeof(MpirExpression);
            var allExpressions =
                baseExpr.Assembly.GetTypes()
                .Where(x => baseExpr.IsAssignableFrom(x) && !x.IsAbstract)
                .ToList();

            using (var a = new HugeInt("-9853204702983745098347985373498752039470593874"))
            using (var b = new HugeInt("43827349058270349857203948752093847502934875203"))
            {
                var expr = a + (-a * 34905823049587UL) * -34598203452879L * (a.Abs() * -345 + 9870987987788UL - a * a) + 4 + a;
                VerifyPartialResult(expr, "1155271201321676280644265576487030177551546144606868786264755274144013089760350323811075841993564191741000111852166670926558456544453200851203842962962472025062534692");
                expr = expr + a * 25 + (a+b) * (b + 998989870978UL) * (b + -234409) * b - (b * -a) - (b * 55UL) - a + (b << 234) - ((a*b) << 15);
                VerifyPartialResult(expr, "2860119216199941536354515972366384286047558544144468598293127129993055149639508359635093477375507907998472593637122254485772011662777815308345009141366885709424712843645103515111206450495");
                expr = expr - 3453 - 92938457U + (b - (a << 4)) + (b * b - 15UL) * (b - a) * (a - 5432) * (b - 9874UL) - (448739847 - a) + (52034987UL - b);
                VerifyPartialResult(expr, "-44527683379886113418724460764726984856717156388775636972448492072920883239528380583662970502609288272739831325295051352018030110273519077513070609558584766157041440002331614418072245492559788050566162568775201655691530703581293856377");
                expr = expr + (-4253049870987L - 2 * a) + (9872340944UL - 4 * b) + -(a + b*2) + (3 * a).Abs();
                VerifyPartialResult(expr, "-44527683379886113418724460764726984856717156388775636972448492072920883239528380583662970502609288272739831325295051352018030110273519077513070609558584766157041440002331614418072245492763632916697882197328337435963101280605257074394");
                MarkExpressionsUsed(allExpressions, expr);
            }

            Assert.AreEqual(0, allExpressions.Count, "Expression types not exercised: " + string.Join("",
                allExpressions.Select(x => Environment.NewLine + x.Name).OrderBy(x => x)));
        }

        private void VerifyPartialResult(MpirExpression expr, string expected)
        {
            using (var r = new HugeInt())
            {
                r.Value = expr;
                Assert.AreEqual(expected, r.ToString());
            }
        }

        private void MarkExpressionsUsed(List<Type> allExpressions, MpirExpression expr)
        {
            var type = expr.GetType();
            allExpressions.Remove(type);
            
            var children = type.GetFields(BindingFlags.NonPublic | BindingFlags.Instance)
                .Where(x => typeof(MpirExpression).IsAssignableFrom(x.FieldType))
                .Select(x => (MpirExpression)x.GetValue(expr))
                .ToList();

            foreach (var childExpr in children)
                MarkExpressionsUsed(allExpressions, childExpr);
        }
    }
}
