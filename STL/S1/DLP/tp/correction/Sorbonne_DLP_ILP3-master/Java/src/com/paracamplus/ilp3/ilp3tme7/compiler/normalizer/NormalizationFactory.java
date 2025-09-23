package com.paracamplus.ilp3.ilp3tme7.compiler.normalizer;

import com.paracamplus.ilp1.compiler.interfaces.IASTCglobalVariable;
import com.paracamplus.ilp1.interfaces.IASTexpression;
import com.paracamplus.ilp3.ilp3tme7.ast.ASTcostart;
import com.paracamplus.ilp3.ilp3tme7.compiler.ast.ASTCglobalCostart;

public class NormalizationFactory extends com.paracamplus.ilp3.compiler.normalizer.NormalizationFactory implements INormalizationFactory {
    @Override
    public IASTexpression newCostart(IASTCglobalVariable coroutine, IASTexpression[] arguments) {
        return new ASTCglobalCostart(coroutine, arguments);
    }
}
