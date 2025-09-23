/* *****************************************************************
 * ilp4 - Implantation d'un langage de programmation.
 * by Christian.Queinnec@paracamplus.com
 * See http://mooc.paracamplus.com/ilp4
 * GPL version 3
 ***************************************************************** */
package com.paracamplus.ilp4.exam1920.compiler.Normalizer;

import java.util.HashMap;
import java.util.List;
import java.util.Map;
import java.util.Vector;

import com.paracamplus.ilp1.compiler.interfaces.IASTCglobalFunctionVariable;
import com.paracamplus.ilp1.compiler.normalizer.INormalizationEnvironment;
import com.paracamplus.ilp1.compiler.normalizer.NormalizationEnvironment;
import com.paracamplus.ilp1.compiler.CompilationException;
import com.paracamplus.ilp4.compiler.interfaces.IASTCclassDefinition;
import com.paracamplus.ilp2.compiler.interfaces.IASTCfunctionDefinition;
import com.paracamplus.ilp2.compiler.normalizer.INormalizationFactory;
import com.paracamplus.ilp4.compiler.interfaces.IASTCmethodDefinition;
import com.paracamplus.ilp4.compiler.interfaces.IASTCprogram;
import com.paracamplus.ilp4.exam1920.interfaces.IASTiterable;
import com.paracamplus.ilp4.interfaces.IASTclassDefinition;
import com.paracamplus.ilp1.interfaces.IASTexpression;
import com.paracamplus.ilp4.interfaces.IASTfieldRead;
import com.paracamplus.ilp4.interfaces.IASTfieldWrite;
import com.paracamplus.ilp2.interfaces.IASTfunctionDefinition;
import com.paracamplus.ilp2.interfaces.IASTloop;
import com.paracamplus.ilp4.interfaces.IASTinstantiation;
import com.paracamplus.ilp4.interfaces.IASTmethodDefinition;
import com.paracamplus.ilp4.interfaces.IASTprogram;
import com.paracamplus.ilp4.interfaces.IASTself;
import com.paracamplus.ilp4.interfaces.IASTsend;
import com.paracamplus.ilp4.interfaces.IASTsuper;
import com.paracamplus.ilp1.interfaces.IASTvariable;
import com.paracamplus.ilp4.interfaces.IASTvisitor;

public class Normalizer 
extends com.paracamplus.ilp4.compiler.normalizer.Normalizer 
implements 
 IASTvisitor<IASTexpression, INormalizationEnvironment, CompilationException> {

	
	  @Override
		public IASTexpression visit(IASTiterable iast, INormalizationEnvironment env)
	            throws CompilationException {
	        IASTexpression value = iast.getValue().accept(this, env);
	        IASTexpression body = iast.getBody().accept(this, env);
	        IASTexpression variable = iast.getVariable().accept(this, env);//pas besoin

	        // ici on doit créer le INormalisation factory
	        //IASTexpression newIterable(IASTexpression value , IASTvariable variable, IASTexpression body );
	        return ((INormalizationFactory)factory).newIterable(value,variable,body);
	    }

 
}
