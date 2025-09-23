package com.paracamplus.ilp4.exam2020.ast;

import com.paracamplus.ilp1.interfaces.IASTexpression;
import com.paracamplus.ilp1.interfaces.IASTvariable;
import com.paracamplus.ilp4.exam2020.interfaces.IASTfactory;
import com.paracamplus.ilp4.exam2020.interfaces.IASTfilter;
import com.paracamplus.ilp4.exam2020.interfaces.IASTsum;

public class ASTfactory extends com.paracamplus.ilp4.ast.ASTfactory implements IASTfactory {

	@Override
	public IASTsum newSum(String discrim, IASTexpression[] values) {
		return new ASTsum(discrim, values);
	}

	@Override
	public IASTfilter newFilter(IASTexpression sum, String tag, IASTvariable[] vars, IASTexpression consequence,
			IASTexpression alternant) {
		return new ASTfilter(sum, tag, vars, consequence, alternant);
	}

}
