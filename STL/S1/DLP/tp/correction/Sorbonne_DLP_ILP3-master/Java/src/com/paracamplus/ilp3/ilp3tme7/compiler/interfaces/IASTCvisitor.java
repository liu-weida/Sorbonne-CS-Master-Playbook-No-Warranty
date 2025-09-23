package com.paracamplus.ilp3.ilp3tme7.compiler.interfaces;

import com.paracamplus.ilp3.ilp3tme7.interfaces.IASTvisitor;

public interface IASTCvisitor<Result, Data, Anomaly extends Throwable>
        extends com.paracamplus.ilp3.compiler.interfaces.IASTCvisitor<Result, Data, Anomaly>,
        IASTvisitor<Result, Data, Anomaly> {
    Result visit(IASTCglobalCostart iast, Data data) throws Anomaly;
}
