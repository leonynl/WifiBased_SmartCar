package com.example.smartcontroller;

import android.widget.Toast;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.io.OutputStream;
import java.io.PrintStream;
import java.io.PrintWriter;
import java.net.Socket;

public class ConnectThread extends Thread {

    private MainActivity mContext;
    private String ip;
    private int port;

    private Socket mSocket;
    private BufferedReader br;

    private SendThread mSendThread;

    public ConnectThread(MainActivity context, String ip, int port) {
        this.mContext = context;
        this.ip = ip;
        this.port = port;
    }

    private void connect()
    {
        try {
            // 和服务器创建连接
            mSocket = new Socket("192.168.77.1",77);

            mContext.isConnected = true;

            // 提示用户连接成功
            mContext.runOnUiThread(new Runnable() {
                @Override
                public void run() {
                    mContext.connectButton.setText("断开");
                    mContext.ledImageView.setImageResource(R.drawable.online);

                    Toast.makeText(mContext, "连接成功", Toast.LENGTH_SHORT).show();
                }
            });


            // 创建发送消息的线程
            mSendThread = new SendThread(mContext, mSocket);
            mSendThread.start();

            // 从服务器接收的信息
           InputStream is = mSocket.getInputStream();
            br = new BufferedReader(new InputStreamReader(is));
            String info = null;
            while((info = br.readLine())!=null){
                System.out.println("我是客户端，服务器返回信息："+info);
            }

            br.close();
            is.close();

            mSocket.close();
        } catch (Exception e) {
            e.printStackTrace();

            mContext.isConnected = false;

            mContext.runOnUiThread(new Runnable() {
                @Override
                public void run() {
                    Toast.makeText(mContext, "连接失败", Toast.LENGTH_SHORT).show();
                }
            });
        }
    }

    // 关闭连接
    public void closeConnect() throws IOException
    {
        if (mSocket == null) return;

        mSocket.shutdownInput();
        mSocket.shutdownOutput();
        while (!mSocket.isInputShutdown() || !mSocket.isOutputShutdown());
        br.close();
        mSocket.close();

        // 修改标志位
        mContext.isConnected = false;
        // 修改界面
        mContext.runOnUiThread(new Runnable() {
            @Override
            public void run() {
                mContext.connectButton.setText("连接");
                mContext.ledImageView.setImageResource(R.drawable.offline);
                Toast.makeText(mContext, "已断开连接", Toast.LENGTH_SHORT).show();
            }
        });
    }

    public void press_A()
    {
        mSendThread.press_A();
    }
    public void press_B()
    {
        mSendThread.press_B();
    }
    public void press_C()
    {
        mSendThread.press_C();
    }
    public void press_D()
    {
        mSendThread.press_D();
    }
    public void car_stopa()
    {
        mSendThread.car_stopa();
    }
    public void car_stopb()
    {
        mSendThread.car_stopb();
    }
    public void car_stopc()
    {
        mSendThread.car_stopc();
    }
    public void car_stopd()
    {
        mSendThread.car_stopd();
    }
    public boolean startConnect;

    @Override
    public void run()
    {
        while (true)
        {
            if (startConnect)
            {
                startConnect = false;
                connect();
            }
        }
    }

}