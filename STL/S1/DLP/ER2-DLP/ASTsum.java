package com.paracamplus.ilp4.exam2020.ast;

import com.paracamplus.ilp1.interfaces.IASTexpression;
import com.paracamplus.ilp1.interfaces.IASTvisitor;
import com.paracamplus.ilp4.exam2020.interfaces.IASTsum;

public class ASTsum implements IASTsum {
	
	private String discrim;
	private IASTexpression[] values;
	
	public ASTsum(String discrim, IASTexpression[] values) {
		this.discrim = discrim;
		this.values = values;
	}

	@Override
	public <Result, Data, Anomaly extends Throwable> Result accept(IASTvisitor<Result, Data, Anomaly> visitor,
			Data data) throws Anomaly {
		return ((com.paracamplus.ilp4.exam2020.interfaces.IASTvisitor<Result, Data, Anomaly>) visitor).visit(this, data);
	}

	@Override
	public String getDiscrim() {
		return this.discrim;
	}

	@Override
	public IASTexpression[] getValues() {
		return this.values;
	}

}
