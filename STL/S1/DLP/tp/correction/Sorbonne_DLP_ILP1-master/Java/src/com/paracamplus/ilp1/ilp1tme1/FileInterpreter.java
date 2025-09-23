package com.paracamplus.ilp1.ilp1tme1;

import com.paracamplus.ilp1.interpreter.interfaces.EvaluationException;
import com.paracamplus.ilp1.interpreter.test.InterpreterTest;
import com.paracamplus.ilp1.parser.ParseException;

import java.io.File;
import java.io.IOException;

public class FileInterpreter extends InterpreterTest {
    public FileInterpreter(File file) {
        super(file);
    }


    public static void main (String[] args) throws ParseException, IOException, EvaluationException {
         //String fileName = args[0];
         File file = new File("C:\\Users\\14993\\OneDrive\\桌面\\学习\\dlp\\Sorbonne_DLP_ILP1-master\\SamplesILP1\\u01-1.ilpml");
         FileInterpreter fileInterpreter = new FileInterpreter(file);
         fileInterpreter.processFile();
    }
}

