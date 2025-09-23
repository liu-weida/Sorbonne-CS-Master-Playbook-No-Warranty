package com.paracamplus.ilp4.exam2020.interfaces;

import com.paracamplus.ilp1.interfaces.IASTexpression;
import com.paracamplus.ilp1.interfaces.IASTvariable;

public interface IASTfilter extends IASTexpression {
	
	IASTexpression getSum();
	String getTag();
	IASTvariable[] getVars();
	IASTexpression getConsequence();
	IASTexpression getAlternant();
	
}
