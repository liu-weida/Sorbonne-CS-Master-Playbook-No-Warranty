package com.paracamplus.ilp3.ilp3tme7.compiler.normalizer;

import com.paracamplus.ilp1.compiler.CompilationException;
import com.paracamplus.ilp1.compiler.interfaces.IASTCglobalVariable;
import com.paracamplus.ilp1.compiler.normalizer.INormalizationEnvironment;
import com.paracamplus.ilp1.interfaces.IASTexpression;

import com.paracamplus.ilp3.ilp3tme7.interfaces.IASTcostart;
import com.paracamplus.ilp3.ilp3tme7.interfaces.IASTvisitor;

public class Normalizer extends com.paracamplus.ilp3.compiler.normalizer.Normalizer
        implements IASTvisitor<IASTexpression, INormalizationEnvironment, CompilationException> {

    private final INormalizationFactory factory;
    public Normalizer(INormalizationFactory factory) {
        super(factory);
        this.factory = factory;
    }

    @Override
    public IASTexpression visit(IASTcostart iast, INormalizationEnvironment iNormalizationEnvironment) throws CompilationException {
        IASTexpression coroutine = iast.getCoroutine().accept(this, iNormalizationEnvironment);

        if(! (coroutine instanceof IASTCglobalVariable))
            throw new CompilationException("Function of coroutine is not global");
        IASTCglobalVariable function = (IASTCglobalVariable)coroutine;

        int length = iast.getArguments().length;
        IASTexpression[] arguments = iast.getArguments();
        IASTexpression[] args = new IASTexpression[length];
        for (int i = 0; i < length; i++){
            args[i] = arguments[i].accept(this, iNormalizationEnvironment);
        }
        return factory.newCostart(function, args);
    }
}
