package com.paracamplus.ilp4.exam2020.parser;

import org.antlr.v4.runtime.ANTLRInputStream;
import org.antlr.v4.runtime.CommonTokenStream;
import org.antlr.v4.runtime.tree.ParseTreeWalker;

import com.paracamplus.ilp1.parser.ParseException;
import com.paracamplus.ilp4.exam2020.interfaces.IASTfactory;
import com.paracamplus.ilp4.interfaces.IASTprogram;
import com.paracamplus.ilp4.exam2020.parser.ILPMLlistener;

import antlr4.ILPMLgrammarExamLexer;
import antlr4.ILPMLgrammarExamParser;

public class ILPMLparser extends com.paracamplus.ilp4.parser.ilpml.ILPMLParser {

	public ILPMLparser(IASTfactory factory) {
		super(factory);
	}
	
	@Override
    public IASTprogram getProgram() throws ParseException {
		try {
			ANTLRInputStream in = new ANTLRInputStream(input.getText());
			// flux de caractères -> analyseur lexical
			ILPMLgrammarExamLexer lexer = new ILPMLgrammarExamLexer(in);
			// analyseur lexical -> flux de tokens
			CommonTokenStream tokens =	new CommonTokenStream(lexer);
			// flux tokens -> analyseur syntaxique
			ILPMLgrammarExamParser parser = new ILPMLgrammarExamParser(tokens);
			// démarage de l'analyse syntaxique
			ILPMLgrammarExamParser.ProgContext tree = parser.prog();		
			// parcours de l'arbre syntaxique et appels du Listener
			ParseTreeWalker walker = new ParseTreeWalker();
			ILPMLlistener extractor = new ILPMLlistener((IASTfactory)factory);
			walker.walk(extractor, tree);	
			return tree.node;
		} catch (Exception e) {
			throw new ParseException(e);
		}
    }

}
