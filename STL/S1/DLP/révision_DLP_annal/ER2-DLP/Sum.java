package com.paracamplus.ilp4.exam2020.interpreter;

import com.paracamplus.ilp4.exam2020.interpreter.interfaces.ISum;

public class Sum implements ISum {
	
	private String discrim;
	private Object[] values;
	
	public Sum(String discrim, Object[] values) {
		this.discrim = discrim;
		this.values = values;
	}

	public String getDiscrim() {
		return discrim;
	}

	public Object[] getValues() {
		return values;
	}
	
}
