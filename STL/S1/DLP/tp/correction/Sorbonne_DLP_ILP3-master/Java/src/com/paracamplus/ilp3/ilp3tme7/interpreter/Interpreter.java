package com.paracamplus.ilp3.ilp3tme7.interpreter;

import com.paracamplus.ilp1.interfaces.IASTexpression;
import com.paracamplus.ilp1.interpreter.interfaces.*;
import com.paracamplus.ilp3.ilp3tme7.interfaces.IASTcostart;
import com.paracamplus.ilp3.ilp3tme7.interfaces.IASTvisitor;

import java.util.ArrayList;
import java.util.List;

public class Interpreter extends com.paracamplus.ilp3.interpreter.Interpreter
        implements IASTvisitor<Object, ILexicalEnvironment, EvaluationException> {
    public Interpreter(IGlobalVariableEnvironment globalVariableEnvironment, IOperatorEnvironment operatorEnvironment) {
        super(globalVariableEnvironment, operatorEnvironment);
    }

    @Override
    public Object visit(IASTcostart iast, ILexicalEnvironment iLexicalEnvironment) throws EvaluationException {
        Object function = iast.getCoroutine().accept(this, iLexicalEnvironment);
        if (function instanceof Invocable) {
            Invocable func = (Invocable) function;
            List<Object> args = new ArrayList<>();
            for (IASTexpression arg : iast.getArguments()) {
                Object a = arg.accept(this, iLexicalEnvironment);
                args.add(a);
            }
            CoroutineInstance coroutineInstance = new CoroutineInstance(func, args, this);
            coroutineInstance.start();
            return coroutineInstance;
        }else{
            throw new EvaluationException("costart cannot apply " + function.toString());
        }
    }
}
