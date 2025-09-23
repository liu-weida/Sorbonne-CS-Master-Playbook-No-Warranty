package com.paracamplus.ilp4.exam2020.interfaces;

import com.paracamplus.ilp1.interfaces.IASTexpression;
import com.paracamplus.ilp1.interfaces.IASTvariable;

public interface IASTfactory extends com.paracamplus.ilp4.interfaces.IASTfactory {
	
	IASTsum newSum(String discrim, IASTexpression[] values);
	IASTfilter newFilter(IASTexpression sum, String tag, IASTvariable[] vars, IASTexpression consequence, IASTexpression alternant);

}
