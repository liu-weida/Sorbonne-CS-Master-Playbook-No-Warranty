package com.paracamplus.ilp4.exam2020.interpreter;

import com.paracamplus.ilp1.interfaces.IASTvariable;
import com.paracamplus.ilp1.interpreter.interfaces.EvaluationException;
import com.paracamplus.ilp1.interpreter.interfaces.IGlobalVariableEnvironment;
import com.paracamplus.ilp1.interpreter.interfaces.ILexicalEnvironment;
import com.paracamplus.ilp1.interpreter.interfaces.IOperatorEnvironment;
import com.paracamplus.ilp4.exam2020.interfaces.IASTfilter;
import com.paracamplus.ilp4.exam2020.interfaces.IASTsum;
import com.paracamplus.ilp4.exam2020.interfaces.IASTvisitor;
import com.paracamplus.ilp4.exam2020.interpreter.interfaces.ISum;
import com.paracamplus.ilp4.interpreter.interfaces.IClassEnvironment;

public class Interpreter extends com.paracamplus.ilp4.interpreter.Interpreter 
implements IASTvisitor<Object, ILexicalEnvironment, EvaluationException> {

	public Interpreter(IGlobalVariableEnvironment globalVariableEnvironment, IOperatorEnvironment operatorEnvironment,
			IClassEnvironment classEnvironment) {
		super(globalVariableEnvironment, operatorEnvironment, classEnvironment);
	}
	
	@Override
	public Object visit(IASTsum iast, ILexicalEnvironment data) throws EvaluationException {
		Object[] values = new Object[iast.getValues().length];
		for (int i = 0; i < values.length; i++) {
			values[i] = iast.getValues()[i].accept(this, data);
		}
		return new Sum(iast.getDiscrim(), values);
	}

	@Override
	public Object visit(IASTfilter iast, ILexicalEnvironment data) throws EvaluationException {
		// On vérfie que c'est bien une somme et qu'elle a le bon tag
		Object maybeSum = iast.getSum().accept(this, data);
		if(maybeSum instanceof ISum && ((ISum) maybeSum).getDiscrim().equals(iast.getTag())) {
			
			ISum sum = (ISum) maybeSum;
			
			// On vérifie que le nombre de variable est bien inferieur ou égal au nombre de valeurs
			if(iast.getVars().length > sum.getValues().length) {
				throw new EvaluationException("Too much variable to to bind");
			}
			
			ILexicalEnvironment newEnv = data;
			for(int i = 0; i < iast.getVars().length; i++) {
				IASTvariable var = iast.getVars()[i];
				newEnv = newEnv.extend(var, sum.getValues()[i]);
			}
			
			// On exécute la conséquence avec le nouvel env lex
			return iast.getConsequence().accept(this, newEnv);
			
		} else {
			
			// On exécute l'alternant sans changer l'environnement lexical
			return iast.getAlternant().accept(this, data);
			
		}
	}

}
