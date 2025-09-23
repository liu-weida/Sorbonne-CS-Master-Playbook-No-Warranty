package com.paracamplus.ilp4.exam2020.compiler;

import java.util.Arrays;
import java.util.HashSet;
import java.util.Set;

import com.paracamplus.ilp1.compiler.CompilationException;
import com.paracamplus.ilp1.compiler.interfaces.IASTClocalVariable;
import com.paracamplus.ilp1.interfaces.IASTvariable;
import com.paracamplus.ilp2.compiler.interfaces.IASTCfunctionDefinition;
import com.paracamplus.ilp4.compiler.interfaces.IASTCprogram;
import com.paracamplus.ilp4.exam2020.interfaces.IASTfilter;
import com.paracamplus.ilp4.exam2020.interfaces.IASTsum;
import com.paracamplus.ilp4.exam2020.interfaces.IASTvisitor;

public class FreeVariableCollector 
extends com.paracamplus.ilp4.compiler.FreeVariableCollector
implements IASTvisitor<Void, Set<IASTClocalVariable>, CompilationException>{

	public FreeVariableCollector(IASTCprogram program) {
		super(program);
	}

	@Override
	public Void visit(IASTfilter iast, Set<IASTClocalVariable> data) throws CompilationException {
		Set<IASTClocalVariable> newvars = new HashSet<>();
        iast.getConsequence().accept(this, newvars);
        
        IASTvariable[] vars = iast.getVars();
        newvars.removeAll(Arrays.asList(vars));
        
        try {
            IASTCfilter filter = (IASTCfilter) iast;
            filter.setConsequenceVars(newvars);
            
            for (IASTvariable v : newvars) {
                ((IASTClocalVariable) v).setClosed();
            }
        } catch (ClassCastException exc) {
            throw new RuntimeException("should not occur");
        }
        data.addAll(newvars);
        return null;
	}

	@Override
	public Void visit(IASTsum iast, Set<IASTClocalVariable> data) throws CompilationException {
		return null;
	}

}
