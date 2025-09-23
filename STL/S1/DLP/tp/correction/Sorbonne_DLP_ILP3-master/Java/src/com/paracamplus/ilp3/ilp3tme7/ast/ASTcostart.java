package com.paracamplus.ilp3.ilp3tme7.ast;

import com.paracamplus.ilp1.ast.ASTexpression;
import com.paracamplus.ilp1.interfaces.IASTexpression;
import com.paracamplus.ilp3.ilp3tme7.interfaces.IASTvisitor;
import com.paracamplus.ilp3.ilp3tme7.interfaces.IASTcostart;

public class ASTcostart extends ASTexpression implements IASTcostart {

    public ASTcostart (IASTexpression coroutine, IASTexpression[] arguments) {
        this.coroutine = coroutine;
        this.arguments = arguments;
    }
    private final IASTexpression coroutine;
    private final IASTexpression[] arguments;

    @Override
    public IASTexpression getCoroutine () {
        return coroutine;
    }
    @Override
    public IASTexpression[] getArguments () {
        return arguments;
    }


    @Override
    public <Result, Data, Anomaly extends Throwable> Result accept(
            com.paracamplus.ilp1.interfaces.IASTvisitor<Result, Data, Anomaly> visitor, Data data)
            throws Anomaly {
        return ((IASTvisitor<Result, Data, Anomaly>)visitor).visit(this, data);
    }
}
