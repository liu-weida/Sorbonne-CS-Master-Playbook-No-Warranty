package com.paracamplus.ilp4.exam2020.interfaces;

import com.paracamplus.ilp1.interfaces.IASTexpression;

public interface IASTsum extends IASTexpression {

	String getDiscrim();
	IASTexpression[] getValues();
	
}
