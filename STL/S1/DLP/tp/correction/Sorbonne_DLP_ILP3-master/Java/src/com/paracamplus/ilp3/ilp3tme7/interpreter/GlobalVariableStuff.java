package com.paracamplus.ilp3.ilp3tme7.interpreter;

import com.paracamplus.ilp1.interpreter.interfaces.IGlobalVariableEnvironment;
import com.paracamplus.ilp1.interpreter.primitive.Newline;
import com.paracamplus.ilp1.interpreter.primitive.Print;
import com.paracamplus.ilp3.ilp3tme7.interpreter.primitive.Resume;
import com.paracamplus.ilp3.ilp3tme7.interpreter.primitive.Yield;
import com.paracamplus.ilp3.interpreter.primitive.Throw;

import java.io.Writer;
import java.math.BigDecimal;

public class GlobalVariableStuff {
    public static void fillGlobalVariables (
            IGlobalVariableEnvironment env,
            Writer out ) {
        env.addGlobalVariableValue("pi", new BigDecimal("3.1415926535"));
        env.addGlobalVariableValue(new Print(out));
        env.addGlobalVariableValue(new Newline(out));
        env.addGlobalVariableValue(new Throw());
        env.addGlobalVariableValue(new Resume());
        env.addGlobalVariableValue(new Yield());
    }
}
