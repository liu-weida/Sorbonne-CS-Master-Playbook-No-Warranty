/* *****************************************************************
 * ILP9 - Implantation d'un langage de programmation.
 * by Christian.Queinnec@paracamplus.com
 * See http://mooc.paracamplus.com/ilp9
 * GPL version 3
 ***************************************************************** */
package com.paracamplus.ilp4.exam1920.interpreter;

import java.util.List;
import java.util.Vector;

import com.paracamplus.ilp4.interfaces.IASTclassDefinition;
import com.paracamplus.ilp1.interfaces.IASTexpression;
import com.paracamplus.ilp4.interfaces.IASTfieldRead;
import com.paracamplus.ilp4.interfaces.IASTfieldWrite;
import com.paracamplus.ilp2.interfaces.IASTfunctionDefinition;
import com.paracamplus.ilp4.interfaces.IASTinstantiation;
import com.paracamplus.ilp4.interfaces.IASTmethodDefinition;
import com.paracamplus.ilp4.interfaces.IASTprogram;
import com.paracamplus.ilp4.interfaces.IASTself;
import com.paracamplus.ilp4.interfaces.IASTsend;
import com.paracamplus.ilp4.interfaces.IASTsuper;
import com.paracamplus.ilp4.exam1920.interfaces.IASTiterable;
import com.paracamplus.ilp4.exam1920.interfaces.IASTvisitor;
import com.paracamplus.ilp1.interpreter.interfaces.EvaluationException;
import com.paracamplus.ilp4.interpreter.ILP9Instance;
import com.paracamplus.ilp4.interpreter.interfaces.IClass;
import com.paracamplus.ilp4.interpreter.interfaces.IClassEnvironment;
import com.paracamplus.ilp1.interpreter.interfaces.IGlobalVariableEnvironment;
import com.paracamplus.ilp1.interpreter.interfaces.ILexicalEnvironment;
import com.paracamplus.ilp4.interpreter.interfaces.IMethod;
import com.paracamplus.ilp1.interpreter.interfaces.IOperatorEnvironment;
import com.paracamplus.ilp4.interpreter.interfaces.ISuperCallInformation;
import com.paracamplus.ilp3.interpreter.primitive.Throw.ThrownException;

public class Interpreter extends com.paracamplus.ilp4.interpreter.Interpreter
implements IASTvisitor<Object, ILexicalEnvironment, EvaluationException> 
{

	public Interpreter(IGlobalVariableEnvironment globalVariableEnvironment, IOperatorEnvironment operatorEnvironment,
			IClassEnvironment classEnvironment) {
		super(globalVariableEnvironment, operatorEnvironment, classEnvironment);
		// TODO Auto-generated constructor stub
	}

	@Override
	public Object visit(IASTiterable iast, ILexicalEnvironment data) throws EvaluationException {
		// TODO Auto-generated method stub
		// on evalue expr1
		Object obj = iast.getValue().accept(this, data);
		if(obj instanceof ILP9Instance) // si c'est bien une instance
		{
			ILP9Instance instance = (ILP9Instance)obj;
			//en assigne a notre variable resultat de iterator dans notre environement lexical
			Object ite = instance.send(this, "iterator", new Object[0]);
			if(ite instanceof ILP9Instance)
			{
				throw new EvaluationException("ce n'est pas une instance");
			}
			data.update(iast.getVariable(), ite);
			while(true)
			{
	            ite = ((ILP9Instance)ite).send(this, "next", new Object[0]);
				data.update(iast.getVariable(),ite);
				// si la valeur de notre variable est false on sort de la boucle
				if(data.getValue(iast.getVariable()).equals(Boolean.FALSE)) {
					break;
				}
	            iast.getBody().accept(this, data);

			}
		}else
		{
			throw new EvaluationException("expr 1 n'est pas une instance");
		}
		return null;
	}
    
   
}
