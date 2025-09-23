package com.paracamplus.ilp3.ilp3tme7.compiler;

import com.paracamplus.ilp1.compiler.CompilationException;
import com.paracamplus.ilp1.compiler.interfaces.IASTClocalVariable;
import com.paracamplus.ilp1.interfaces.IASTexpression;
import com.paracamplus.ilp3.compiler.interfaces.IASTCprogram;
import com.paracamplus.ilp3.ilp3tme7.compiler.interfaces.IASTCglobalCostart;
import com.paracamplus.ilp3.ilp3tme7.compiler.interfaces.IASTCvisitor;
import com.paracamplus.ilp3.ilp3tme7.interfaces.IASTcostart;

import java.util.Set;

public class FreeVariableCollector extends com.paracamplus.ilp3.compiler.FreeVariableCollector
implements IASTCvisitor<Void, Set<IASTClocalVariable>, CompilationException> {
    public FreeVariableCollector(IASTCprogram program) {
        super(program);
    }

    @Override
    public Void visit(IASTCglobalCostart iast, Set<IASTClocalVariable> variables) throws CompilationException {
        iast.getCoroutine().accept(this, variables);
        for ( IASTexpression arg : iast.getArguments() ) {
            arg.accept(this, variables);
        }
        return null;
    }

    @Override
    public Void visit(IASTcostart iast, Set<IASTClocalVariable> variables) throws CompilationException {
        return visit((IASTCglobalCostart) iast, variables);
    }
}
