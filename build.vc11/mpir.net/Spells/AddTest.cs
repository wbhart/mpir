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
using System.Text;
using OmniGruntWare.Magic.Wand;

namespace Spells
{
    /// <summary>
    /// This spell generates a typical set of unit tests and adds it to the active file.
    /// </summary>
    internal class AddTest : AddTestBase, ISpell
    {
        public string Name
        {
            get { return "Add &Integer Test"; }
        }

        public void Execute(IDevelopmentEnvironment env)
        {
            var args = env.Prompt("Add Test")
                .Instructions(string.Format("Add a set of unit tests for an MPIR {1} method family to the active file ({0}):", env.ActiveFile.Name, MpirType))
                .ForString("Method").Label("&Method under test:").PreviousDefaultOr("").Required()
                .ForString("InitialValue").Label("&Initial value of the class:").PreviousDefaultOr("").Required()
                .ForBoolean("FixedArgument").Label(string.Format("All tests use a second {0} argume&nt:", MpirType)).PreviousDefaultOr(false)
                .ForString("FixedInitialValue").Label("With initial &value:").PreviousDefaultOr("").Required().EnabledBy("FixedArgument")
                .ForBoolean("FromNone").Label("Test with&out additional arguments:").PreviousDefaultOr(false)
                .ForString("NoArgumentsResultValue").Label("Res&ult Value:").PreviousDefaultOr("").Required().EnabledBy("FromNone")
                .ForBoolean("FromMpirType").Label(string.Format("Test with another &{0}:", MpirType)).PreviousDefaultOr(true)
                .ForString("MpirTypeInitialValue").Label("&With Initial Value:").PreviousDefaultOr("").Required().EnabledBy("FromMpirType")
                .ForString("MpirTypeResultValue").Label("&Result Value:").PreviousDefaultOr("").Required().EnabledBy("FromMpirType")
                .ForBoolean("FromLimb").Label("Test with a &Limb:").PreviousDefaultOr(true)
                .ForString("LimbInitialValue").Label("Wi&th Initial Value:").PreviousDefaultOr("").Required().EnabledBy("FromLimb")
                .ForString("LimbResultValue").Label("R&esult Value:").PreviousDefaultOr("").Required().EnabledBy("FromLimb")
                .ForBoolean("FromSignedLimb").Label("Test with a signed Lim&b:").PreviousDefaultOr(true)
                .ForString("SignedLimbInitialValue").Label("With Initi&al Value:").PreviousDefaultOr("").Required().EnabledBy("FromSignedLimb")
                .ForString("SignedLimbResultValue").Label("Re&sult Value:").PreviousDefaultOr("").Required().EnabledBy("FromSignedLimb")
                //  CD FG  JK    PQ      XYZ
                .Run();

            //args will be null if the user cancels the prompt
            if (args == null)
                return;

            var methods = new List<string>();
            var fixedInitialValue = (bool)args["FixedArgument"] ? args["FixedInitialValue"] : null;

            if ((bool)args["FromNone"])
            {
                methods.Add(MakeTest("", null, 
                    args["Method"],
                    args["InitialValue"],
                    fixedInitialValue,
                    null,
                    args["NoArgumentsResultValue"]));
            }

            if ((bool)args["FromMpirType"])
            {
                methods.Add(MakeTest(MpirType, MpirType,
                    args["Method"],
                    args["InitialValue"],
                    fixedInitialValue,
                    args["MpirTypeInitialValue"],
                    args["MpirTypeResultValue"]));
            }

            if ((bool)args["FromLimb"])
            {
                methods.Add(MakeTest("Limb", "ulong",
                    args["Method"],
                    args["InitialValue"],
                    fixedInitialValue,
                    args["LimbInitialValue"],
                    args["LimbResultValue"]));
            }

            if ((bool)args["FromSignedLimb"])
            {
                methods.Add(MakeTest("SignedLimb", "long",
                    args["Method"],
                    args["InitialValue"],
                    fixedInitialValue,
                    args["SignedLimbInitialValue"],
                    args["SignedLimbResultValue"]));
            }

            env.ActiveFile.Before("//more tests coming here")
                .InsertMembers(methods.ToArray());
        }

        protected override string MpirType
        {
            get { return "HugeInt"; }
        }
    }

    internal abstract class AddTestBase
    {
        protected abstract string MpirType { get; }

        protected string MakeTest(string suffix, string secondArgType, params object[] args)
        {
            var fixedArgument = (string)args[2];
            var format = new StringBuilder();

            format.AppendLine();
            format.AppendLine("[TestMethod]");
            format.AppendLine("public void {0}" + suffix + "()");
            format.AppendLine("{{");
            format.AppendLine("    using (var a = new " + MpirType + "(\"{1}\"))");

            if (fixedArgument != null)
                format.AppendLine("    using (var c = new " + MpirType + "(\"{2}\"))");

            if (secondArgType == MpirType)
                format.AppendLine("    using (var b = new " + MpirType + "(\"{3}\"))");

            format.AppendLine("    {{");

            if (secondArgType != MpirType && secondArgType != null)
                format.AppendLine("        " + secondArgType + " b = {3};");

            format.Append("        a.{0}(");
            if(fixedArgument != null)
                format.Append("c, ");
            if(secondArgType != null)
                format.Append("b");
            format.AppendLine(");");

            format.AppendLine("        Assert.AreEqual(\"{4}\", a.ToString());");
            format.AppendLine("    }}");
            format.Append    ("}}");


            return string.Format(format.ToString(), args)
                .Replace("\r", "")
                .Replace("\n", Environment.NewLine + new string(' ', 8));
        }
    }
}
