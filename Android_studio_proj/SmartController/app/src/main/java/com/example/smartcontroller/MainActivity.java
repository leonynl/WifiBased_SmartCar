package com.example.smartcontroller;

import androidx.appcompat.app.AppCompatActivity;
import android.app.Activity;
import android.os.Bundle;
import android.view.View;
import android.widget.Button;
import android.widget.ImageView;
import android.widget.Toast;

public class MainActivity extends Activity {

    public ImageView ledImageView;
    public Button connectButton; // 连接esp8266
    public Button pressButtonA; // press A
    public Button pressButtonB; // press B
    public Button pressButtonC; // press C
    public Button pressButtonD; // press D

    private ConnectThread connectThread;
    public boolean isConnected;

    public boolean isPressedButtonA;
    public boolean isPressedButtonB;
    public boolean isPressedButtonC;
    public boolean isPressedButtonD;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        initView();

        connectThread = new ConnectThread(this, "192.168.77.1", 77);
        // 开启连接线程
        connectThread.start();
    }

    private void initView() {
        // LED灯图标
        ledImageView = (ImageView) findViewById(R.id.image);
        connectButton = findViewById(R.id.button_connect);
        pressButtonA = findViewById(R.id.pressdButtonA);
        pressButtonB = findViewById(R.id.pressdButtonB);
        pressButtonC = findViewById(R.id.pressdButtonC);
        pressButtonD = findViewById(R.id.pressdButtonD);
    }

    // 连接esp8266
    public void connect(View view)
    {
        // 如果没有连接
        if (!isConnected)
        {
            connectThread.startConnect = true;
        } else {
            try {
                connectThread.closeConnect();
            } catch (Exception e) {
                e.printStackTrace();
            }
        }
    }

    // press A
    public void press_ButtonA(View view)
    {
        // 如果已经连接到esp8266
        if (isConnected)
        {
            // 如果已经打开LED
            if (isPressedButtonA)
            {
                // 关闭LED
                connectThread.car_stopa();
            } else {
                // 开启LED
                connectThread.press_A();
            }

        } else {
            Toast.makeText(this, "未连接到服务器！", Toast.LENGTH_SHORT).show();
        }
    }

    // press B
    public void press_ButtonB(View view)
    {
        // 如果已经连接到esp8266
        if (isConnected)
        {
            // 如果已经打开LED
            if (isPressedButtonB)
            {
                // 关闭LED
                connectThread.car_stopb();
            } else {
                // 开启LED
                connectThread.press_B();
            }

        } else {
            Toast.makeText(this, "未连接到服务器！", Toast.LENGTH_SHORT).show();
        }
    }

    // press C
    public void press_ButtonC(View view)
    {
        // 如果已经连接到esp8266
        if (isConnected)
        {
            // 如果已经打开LED
            if (isPressedButtonC)
            {
                // 关闭LED
                connectThread.car_stopc();
            } else {
                // 开启LED
                connectThread.press_C();
            }

        } else {
            Toast.makeText(this, "未连接到服务器！", Toast.LENGTH_SHORT).show();
        }
    }

    // press D
    public void press_ButtonD(View view)
    {
        // 如果已经连接到esp8266
        if (isConnected)
        {
            // 如果已经打开LED
            if (isPressedButtonD)
            {
                // 关闭LED
                connectThread.car_stopd();
            } else {
                // 开启LED
                connectThread.press_D();
            }

        } else {
            Toast.makeText(this, "未连接到服务器！", Toast.LENGTH_SHORT).show();
        }
    }

}