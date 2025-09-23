package com.paracamplus.ilp4.exam1920.ast;

import java.util.ArrayList;

import com.paracamplus.ilp1.ast.ASTvariable;
import com.paracamplus.ilp1.interfaces.IASTalternative;
import com.paracamplus.ilp1.interfaces.IASTbinaryOperation;
import com.paracamplus.ilp1.interfaces.IASTblock;
import com.paracamplus.ilp1.interfaces.IASTboolean;
import com.paracamplus.ilp1.interfaces.IASTexpression;
import com.paracamplus.ilp1.interfaces.IASTfloat;
import com.paracamplus.ilp1.interfaces.IASTinteger;
import com.paracamplus.ilp1.interfaces.IASTinvocation;
import com.paracamplus.ilp1.interfaces.IASTsequence;
import com.paracamplus.ilp1.interfaces.IASTstring;
import com.paracamplus.ilp1.interfaces.IASTunaryOperation;
import com.paracamplus.ilp1.interfaces.IASTvariable;
import com.paracamplus.ilp1.interpreter.interfaces.EvaluationException;
import com.paracamplus.ilp1.interpreter.interfaces.ILexicalEnvironment;
import com.paracamplus.ilp1.interpreter.operator.Inequal;
import com.paracamplus.ilp2.interfaces.IASTassignment;
import com.paracamplus.ilp2.interfaces.IASTloop;
import com.paracamplus.ilp3.interfaces.IASTcodefinitions;
import com.paracamplus.ilp3.interfaces.IASTlambda;
import com.paracamplus.ilp3.interfaces.IASTtry;
import com.paracamplus.ilp4.exam1920.interfaces.IASTfactory;
import com.paracamplus.ilp4.exam1920.interfaces.IASTiterable;
import com.paracamplus.ilp4.exam1920.interfaces.IASTvisitor;
import com.paracamplus.ilp4.interfaces.IASTfieldRead;
import com.paracamplus.ilp4.interfaces.IASTfieldWrite;
import com.paracamplus.ilp4.interfaces.IASTinstantiation;
import com.paracamplus.ilp4.interfaces.IASTself;
import com.paracamplus.ilp4.interfaces.IASTsend;
import com.paracamplus.ilp4.interfaces.IASTsuper;
import com.paracamplus.ilp4.interpreter.ILP9Instance;

public class ForeachTransform implements IASTvisitor<Object, ILexicalEnvironment, EvaluationException>  {

	 /* on doit transformer les methodes qui sont pertinonte par rapport a notre for dans il faut transformer
	  * le noeud iterable , 
	  */
	
	IASTfactory factory;
	public ForeachTransform(IASTfactory factory)
	{
		this.factory= factory;
	}
	@Override
	public Object visit(IASTiterable iast, ILexicalEnvironment data) throws EvaluationException {
		// TODO Auto-generated method stub
		// ici on doit evaluer si val contient bien la methode iterable
		
		if( iast.getValue() instanceof ILP9Instance)
		{
			// ici je transforme 
			ILP9Instance instance = (ILP9Instance)iast.getValue() ;
			// on ajoute créer une séquence pour intialiser i par iterator et change le for en while
			ArrayList<IASTexpression> sequence = new ArrayList<>()	;
			ArrayList<IASTexpression> sequenceBody = new ArrayList<>()	;
			// on instance notre iterator et on app en premier a next (comme l'exemple)
			Object iter = factory.newSend("iterator",iast.getValue() , new IASTexpression[0]).accept(this, data); // on visite le noeud
			sequence.add(factory.newAssignment(iast.getVariable(),factory.newSend("iterator",iter , new IASTexpression[0])));
			// ici on vas ajouter au body la sequence next pour a chaque fois que itére dans while on fait un.next
			sequenceBody.add(iast.getBody());
			sequenceBody.add(factory.newSend("next",iast.getValue(),new IASTexpression[0]));
			
			// on ajoute notre nouvelle loop avec les conditions i != false et le nouveau body (avec l'app de next) ( le body sera forme de plusieurs sequence avec le next en dernier)
			
			sequence.add(factory.newLoop(factory.newBinaryOperation(factory.newOperator("!="), iast.getVariable(), factory.newBooleanConstant("false")), factory.newSequence(sequenceBody.toArray())));
			return sequence;
		}
		// traiter si value n'est pas une instance

		throw new EvaluationException("");
	}
	
	//pour tous les autres methodes ont doit visiter a chaque fois tous les noeuds
	@Override
	public Object visit(IASTinstantiation iast, ILexicalEnvironment data) throws EvaluationException {
		// TODO Auto-generated method stub
		return null;
	}

	@Override
	public Object visit(IASTfieldRead iast, ILexicalEnvironment data) throws EvaluationException {
		// TODO Auto-generated method stub
		return null;
	}

	@Override
	public Object visit(IASTself iast, ILexicalEnvironment data) throws EvaluationException {
		// TODO Auto-generated method stub
		return null;
	}

	@Override
	public Object visit(IASTsend iast, ILexicalEnvironment data) throws EvaluationException {
		// TODO Auto-generated method stub
		return null;
	}

	@Override
	public Object visit(IASTsuper iast, ILexicalEnvironment data) throws EvaluationException {
		// TODO Auto-generated method stub
		return null;
	}

	@Override
	public Object visit(IASTfieldWrite iast, ILexicalEnvironment data) throws EvaluationException {
		// TODO Auto-generated method stub
		return null;
	}

	@Override
	public Object visit(IASTcodefinitions iast, ILexicalEnvironment data) throws EvaluationException {
		// TODO Auto-generated method stub
		return null;
	}

	@Override
	public Object visit(IASTlambda iast, ILexicalEnvironment data) throws EvaluationException {
		// TODO Auto-generated method stub
		return null;
	}

	@Override
	public Object visit(IASTtry iast, ILexicalEnvironment data) throws EvaluationException {
		// TODO Auto-generated method stub
		return null;
	}

	@Override
	public Object visit(IASTassignment iast, ILexicalEnvironment data) throws EvaluationException {
		// TODO Auto-generated method stub
		return null;
	}

	@Override
	public Object visit(IASTloop iast, ILexicalEnvironment data) throws EvaluationException {
		// TODO Auto-generated method stub
		return null;
	}

	@Override
	public Object visit(IASTalternative iast, ILexicalEnvironment data) throws EvaluationException {
		// TODO Auto-generated method stub
		return null;
	}

	@Override
	public Object visit(IASTbinaryOperation iast, ILexicalEnvironment data) throws EvaluationException {
		// TODO Auto-generated method stub
		return null;
	}

	@Override
	public Object visit(IASTblock iast, ILexicalEnvironment data) throws EvaluationException {
		// TODO Auto-generated method stub
		return null;
	}

	@Override
	public Object visit(IASTboolean iast, ILexicalEnvironment data) throws EvaluationException {
		// TODO Auto-generated method stub
		return null;
	}

	@Override
	public Object visit(IASTfloat iast, ILexicalEnvironment data) throws EvaluationException {
		// TODO Auto-generated method stub
		return null;
	}

	@Override
	public Object visit(IASTinteger iast, ILexicalEnvironment data) throws EvaluationException {
		// TODO Auto-generated method stub
		return null;
	}

	@Override
	public Object visit(IASTinvocation iast, ILexicalEnvironment data) throws EvaluationException {
		// TODO Auto-generated method stub
		return null;
	}

	@Override
	public Object visit(IASTsequence iast, ILexicalEnvironment data) throws EvaluationException {
		// TODO Auto-generated method stub
		return null;
	}

	@Override
	public Object visit(IASTstring iast, ILexicalEnvironment data) throws EvaluationException {
		// TODO Auto-generated method stub
		return null;
	}

	@Override
	public Object visit(IASTunaryOperation iast, ILexicalEnvironment data) throws EvaluationException {
		// TODO Auto-generated method stub
		return null;
	}

	@Override
	public Object visit(IASTvariable iast, ILexicalEnvironment data) throws EvaluationException {
		// TODO Auto-generated method stub
		return null;
	}

	

}
