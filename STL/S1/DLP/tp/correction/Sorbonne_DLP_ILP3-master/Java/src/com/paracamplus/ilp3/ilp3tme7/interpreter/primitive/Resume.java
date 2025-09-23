package com.paracamplus.ilp3.ilp3tme7.interpreter.primitive;

import com.paracamplus.ilp1.interpreter.interfaces.EvaluationException;
import com.paracamplus.ilp1.interpreter.primitive.UnaryPrimitive;
import com.paracamplus.ilp3.ilp3tme7.interpreter.CoroutineInstance;

public class Resume extends UnaryPrimitive {

    public Resume() {
        super("resume");
    }

    @Override
    public Object apply(Object arg1) throws EvaluationException {
        if(arg1 instanceof CoroutineInstance){
            CoroutineInstance coroutineInstance = (CoroutineInstance) arg1;
            coroutineInstance.resumeCoroutine();
            return Boolean.TRUE;
        }
        return Boolean.FALSE;
    }
}
