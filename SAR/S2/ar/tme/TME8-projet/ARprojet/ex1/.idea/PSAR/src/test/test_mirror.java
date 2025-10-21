package test;

import machine.MirrorInitiator;
import machine.Server;
import utils.exception.ServerException;
import utils.tools.CountdownTimer;

import java.io.IOException;

public class test_mirror {
    public static void main(String[] args) {
        CountdownTimer timer = new CountdownTimer(10);  // 创建一个5秒的倒计时
        timer.start();

    }
}
