package com.paracamplus.ilp1.ilp1tme3.interpreter.primitive;

import com.paracamplus.ilp1.interpreter.interfaces.EvaluationException;
import com.paracamplus.ilp1.interpreter.primitive.UnaryPrimitive;

import java.math.BigInteger;


public class Sinus extends UnaryPrimitive {
    public Sinus() {
        super("sinus");
    }

    double pi = Math.PI;

    @Override
    public Object apply(Object value) throws EvaluationException {
        if(value instanceof Double){
            return (double)Math.sin((Double)value);
        }else if(value instanceof BigInteger){
            return (double)Math.sin( ((BigInteger)value).doubleValue());
        }else throw new EvaluationException("Invalid argument, number expected");

    }
}
