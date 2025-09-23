package com.paracamplus.ilp4.exam2020.compiler;

import java.util.Set;

import com.paracamplus.ilp1.compiler.CompilationException;
import com.paracamplus.ilp1.compiler.interfaces.IASTCglobalVariable;
import com.paracamplus.ilp4.exam2020.interfaces.IASTfilter;
import com.paracamplus.ilp4.exam2020.interfaces.IASTsum;
import com.paracamplus.ilp4.exam2020.interfaces.IASTvisitor;

public class GlobalVariableCollector
extends com.paracamplus.ilp4.compiler.GlobalVariableCollector
implements IASTvisitor<Set<IASTCglobalVariable>, Set<IASTCglobalVariable>, CompilationException> {

	@Override
	public Set<IASTCglobalVariable> visit(IASTfilter iast, Set<IASTCglobalVariable> data) throws CompilationException {
		data = iast.getSum().accept(this, data);
		data = iast.getConsequence().accept(this, data);
		data = iast.getAlternant().accept(this, data);
		return data;
	}

	@Override
	public Set<IASTCglobalVariable> visit(IASTsum iast, Set<IASTCglobalVariable> data) throws CompilationException {
		return null;
	}

}
