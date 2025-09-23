package com.paracamplus.ilp3.ilp3tme7.interpreter;

import com.paracamplus.ilp1.interpreter.interfaces.EvaluationException;
import com.paracamplus.ilp1.interpreter.interfaces.Invocable;

import java.util.List;
import java.util.concurrent.Semaphore;

public class CoroutineInstance extends Thread {

    private final Invocable fun;
    private final List<Object> argument;
    private final Interpreter interpreter;
    private final Semaphore resumeCoroutine;
    private final Semaphore yieldCoroutine;
    private boolean finished = false;

    public CoroutineInstance(Invocable fun, List<Object> argument, Interpreter interpreter){
        this.fun = fun;
        this.argument = argument;
        this.interpreter = interpreter;
        resumeCoroutine = new Semaphore(0);
        yieldCoroutine = new Semaphore(0);
    }

    public void run(){
        try {
            resumeCoroutine.acquire();
            fun.apply(interpreter, argument.toArray());
            finished = true;
        } catch (InterruptedException | EvaluationException e) {
            e.printStackTrace();
        }
        yieldCoroutine.release();
    }

    //appelant
    public void resumeCoroutine(){
        resumeCoroutine.release();
        if(! finished){
            try {
                yieldCoroutine.acquire();
            } catch (InterruptedException e) {
                e.printStackTrace();
            }
        }
    }

    //coroutine
    public void yieldCoroutine(){
        yieldCoroutine.release();
        try {
            resumeCoroutine.acquire();
        } catch (InterruptedException e) {
            e.printStackTrace();
        }
    }


}
