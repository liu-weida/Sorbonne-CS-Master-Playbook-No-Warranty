package com.paracamplus.ilp4.exam2020.ast;

import com.paracamplus.ilp1.interfaces.IASTexpression;
import com.paracamplus.ilp1.interfaces.IASTvariable;
import com.paracamplus.ilp1.interfaces.IASTvisitor;
import com.paracamplus.ilp4.exam2020.interfaces.IASTfilter;

public class ASTfilter implements IASTfilter {
	
	private IASTexpression sum;
	private String tag;
	private IASTvariable[] vars;
	private IASTexpression consequence;
	private IASTexpression alternant;
	
	public ASTfilter(IASTexpression sum, String tag, IASTvariable[] vars, IASTexpression consequence, IASTexpression alternant) {
		this.sum = sum;
		this.tag = tag;
		this.vars = vars;
		this.consequence = consequence;
		this.alternant = alternant;
	}

	@Override
	public <Result, Data, Anomaly extends Throwable> Result accept(IASTvisitor<Result, Data, Anomaly> visitor,
			Data data) throws Anomaly {
		return ((com.paracamplus.ilp4.exam2020.interfaces.IASTvisitor<Result, Data, Anomaly>) visitor).visit(this, data);
	}

	@Override
	public IASTexpression getSum() {
		return this.sum;
	}

	@Override
	public String getTag() {
		return this.tag;
	}

	@Override
	public IASTvariable[] getVars() {
		return this.vars;
	}

	@Override
	public IASTexpression getConsequence() {
		return this.consequence;
	}

	@Override
	public IASTexpression getAlternant() {
		return this.alternant;
	}

}
