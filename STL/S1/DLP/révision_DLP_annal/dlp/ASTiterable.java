package com.paracamplus.ilp4.exam1920.ast;

import com.paracamplus.ilp1.ast.ASTexpression;
import com.paracamplus.ilp1.interfaces.IASTexpression;
import com.paracamplus.ilp1.interfaces.IASTvariable;
import com.paracamplus.ilp4.exam1920.interfaces.IASTvisitor;
import com.paracamplus.ilp4.exam1920.interfaces.IASTiterable;

public class ASTiterable extends ASTexpression implements IASTiterable{

	IASTexpression value;
	IASTvariable variable; 
	IASTexpression body;
	public ASTiterable(IASTexpression value , IASTvariable variable, IASTexpression body ) {
		this.value = value;
		this.variable = variable;
		this.body = body;
	}
	 @Override
	    public <Result, Data, Anomaly extends Throwable> Result accept(
				com.paracamplus.ilp1.interfaces.IASTvisitor<Result, Data, Anomaly> visitor,
				Data data) throws Anomaly {
	        return ((IASTvisitor<Result, Data, Anomaly>) visitor).visit(this, data);
	    }

	@Override
	public IASTexpression getValue() {
		// TODO Auto-generated method stub
		return value;
	}

	@Override
	public IASTvariable getVariable() {
		// TODO Auto-generated method stub
		return variable;
	}

	@Override
	public IASTexpression getBody() {
		// TODO Auto-generated method stub
		return body;
	}

}
