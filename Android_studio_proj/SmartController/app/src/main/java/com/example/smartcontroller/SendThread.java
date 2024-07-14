package com.example.smartcontroller;

import java.io.BufferedReader;
import java.io.OutputStream;
import java.io.PrintWriter;
import java.net.Socket;

public class SendThread extends Thread {

    private MainActivity mContext;
    private Socket mSocket;

    private boolean startSend;
    private char order;

    public SendThread(MainActivity context, Socket socket) {
        this.mContext = context;
        this.mSocket = socket;
    }

    public void press_A()
    {
        order = 'A';
        startSend = true; // 开始发送
        mContext.isPressedButtonA = true;
        mContext.ledImageView.setImageResource(R.drawable.amark);
        mContext.runOnUiThread(new Runnable() {
            @Override
            public void run() {
                mContext.pressButtonA.setText("Stop");
            }
        });
    }

    public void press_B()
    {
        order = 'B';
        startSend = true; // 开始发送
        mContext.isPressedButtonB = true;
        mContext.ledImageView.setImageResource(R.drawable.bmark);
        mContext.runOnUiThread(new Runnable() {
            @Override
            public void run() {
                mContext.pressButtonB.setText("Stop");
            }
        });
    }

    public void press_C()
    {
        order = 'C';
        startSend = true; // 开始发送
        mContext.isPressedButtonC = true;
        mContext.ledImageView.setImageResource(R.drawable.cmark);
        mContext.runOnUiThread(new Runnable() {
            @Override
            public void run() {
                mContext.pressButtonC.setText("Stop");
            }
        });
    }

    public void press_D()
    {
        order = 'D';
        startSend = true; // 开始发送
        mContext.isPressedButtonD = true;
        mContext.ledImageView.setImageResource(R.drawable.dmark);
        mContext.runOnUiThread(new Runnable() {
            @Override
            public void run() {
                mContext.pressButtonD.setText("Stop");
            }
        });
    }

    public void car_stopa()
    {
        order = 'E';
        startSend = true; // 开始发送
        mContext.isPressedButtonA = false;
        mContext.ledImageView.setImageResource(R.drawable.online);
        mContext.runOnUiThread(new Runnable() {
            @Override
            public void run() {
                mContext.pressButtonA.setText("A: Turn Left");
            }
        });
    }

    public void car_stopb()
    {
        order = 'F';
        startSend = true; // 开始发送
        mContext.isPressedButtonB = false;
        mContext.ledImageView.setImageResource(R.drawable.online);
        mContext.runOnUiThread(new Runnable() {
            @Override
            public void run() {
                mContext.pressButtonB.setText("B: Turn Right");
            }
        });
    }

    public void car_stopc()
    {
        order = 'G';
        startSend = true; // 开始发送
        mContext.isPressedButtonC = false;
        mContext.ledImageView.setImageResource(R.drawable.online);
        mContext.runOnUiThread(new Runnable() {
            @Override
            public void run() {
                mContext.pressButtonC.setText("C: Go Straight");
            }
        });
    }

    public void car_stopd()
    {
        order = 'H';
        startSend = true; // 开始发送
        mContext.isPressedButtonD = false;
        mContext.ledImageView.setImageResource(R.drawable.online);
        mContext.runOnUiThread(new Runnable() {
            @Override
            public void run() {
                mContext.pressButtonD.setText("D: Go Back");
            }
        });
    }

    @Override
    public void run()
    {
        try {
            OutputStream os = mSocket.getOutputStream();
            PrintWriter pw = new PrintWriter(os);
            while (true)
            {
                // 如果还没有发送指令
                if (startSend)
                {
                    startSend = false;
                    pw.write(order);
                    pw.flush();
                }
            }
        } catch (Exception e) {
            e.printStackTrace();
        }
    }


}
