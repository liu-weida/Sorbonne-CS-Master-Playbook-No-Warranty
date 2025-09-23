package com.paracamplus.ilp4.exam2020.interfaces;

public interface IASTvisitor<Result, Data, Anomaly extends Throwable>
extends com.paracamplus.ilp4.interfaces.IASTvisitor<Result, Data, Anomaly>{
	
	Result visit(IASTfilter iast, Data data) throws Anomaly;
	Result visit(IASTsum iast, Data data) throws Anomaly;
	
}
