package com.example.surendramobiya.client;

import android.hardware.SensorManager;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.text.method.ScrollingMovementMethod;
import android.util.Log;
import android.view.MotionEvent;
import android.view.View;
import android.widget.Button;
import android.widget.CompoundButton;
import android.widget.EditText;
import android.widget.Switch;
import android.widget.TextView;

import java.io.BufferedWriter;
import java.io.IOException;
import java.io.OutputStreamWriter;
import java.io.PrintWriter;
import java.net.*;
import java.text.DecimalFormat;

import android.view.View.OnClickListener;
import android.hardware.Sensor;
import android.hardware.SensorEvent;
import android.hardware.SensorEventListener;
import android.widget.Toast;

public class MainActivity extends AppCompatActivity implements OnClickListener,SensorEventListener,View.OnTouchListener {

    private TextView tv1;
    private TextView tv2;
    private EditText ipbox;
    private EditText portbox;
    private EditText delay;
    private String xyz;
    private Socket socket;

    private SensorManager sManager;
    private static int serverport;
    private static String curr_ip= "cur";
    private static String pre_ip = "pre";
    private Button startbtn;
    private Button stopbtn;
    private Switch debugswitch;
    String strtosend;
    private PrintWriter out;
    float ogx, ogy,ogz;
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        ipbox = (EditText)findViewById(R.id.ip);
        portbox = (EditText)findViewById(R.id.port);
        delay = (EditText)findViewById(R.id.delay);
        startbtn = (Button) findViewById(R.id.startbtn) ;
        startbtn.setOnClickListener(this);
        stopbtn = (Button) findViewById(R.id.stopbtn) ;
        stopbtn.setOnClickListener(this);
        tv2 = (TextView) findViewById(R.id.t2);
        tv1 = (TextView) findViewById(R.id.t1);
        tv1.setOnTouchListener(this);
        tv1.setMovementMethod(new ScrollingMovementMethod());
        sManager = (SensorManager) getSystemService(SENSOR_SERVICE);

        curr_ip = ipbox.getText().toString();
        serverport = Integer.parseInt(portbox.getText().toString());
        debugswitch = (Switch) findViewById(R.id.debugging);




    }

    public  void showToast(final String str)
    {
        runOnUiThread(new Runnable() {
            public void run() {
                Toast.makeText(getBaseContext(),str,Toast.LENGTH_SHORT).show();
            }
        });
    }

    public void senddata(String strtosend)
    {
        new Thread(new ClientThread(strtosend)).start();
    }

    @Override
    public boolean onTouch(View view, MotionEvent motionEvent) {
        switch (view.getId()) {
            case R.id.t1:
                //showToast("touch ");
                senddata("LC");
        }
        return false;
    }

    @Override
    public void onClick(View view) {
        try {
            switch (view.getId()) {

                case R.id.startbtn:
                    curr_ip = ipbox.getText().toString();
                    Log.d("cllient", "trying to be connected with "+ curr_ip);
                    serverport = Integer.parseInt(portbox.getText().toString());
                    int sensordelay = Integer.parseInt(delay.getText().toString());
                    if (sensordelay == 1)
                        sensordelay = SensorManager.SENSOR_DELAY_FASTEST;
                    else
                    if (sensordelay == 2)
                        sensordelay = SensorManager.SENSOR_DELAY_GAME;
                    else
                    if (sensordelay == 3)
                        sensordelay = SensorManager.SENSOR_DELAY_UI;
                    else
                    if (sensordelay == 4)
                        sensordelay = SensorManager.SENSOR_DELAY_NORMAL;
                    else
                    {
                        showToast("Wrong delay Input");
                        return;
                    }
                    sManager.registerListener(this, sManager.getDefaultSensor(Sensor.TYPE_GYROSCOPE), sensordelay);
                    //new Thread(new ClientThread(textbox.getText().toString())).start();
                    //Toast toast = Toast.makeText(getApplicationContext(), "Started sending data", Toast.LENGTH_SHORT);
                    //toast.show();
                    showToast("Starting sending data");
                    tv2.setText("Sending data to "+curr_ip);
                    break;

                case R.id.stopbtn:
                    sManager.unregisterListener(this);
                    //toast = Toast.makeText(getApplicationContext(), "Stopped sending data", Toast.LENGTH_SHORT);
                   // toast.show();
                    showToast("Stopped sending data");
                    tv2.setText("");
                    //socket.close();
                    break;


                default:
                    break;
            }
        }
        catch (Exception e)
        {
            Log.d("cllient", "failed"+e.getMessage());
            //Toast toast = Toast.makeText(getApplicationContext(), e.getMessage(), Toast.LENGTH_SHORT);
            //toast.show();
            showToast("Not connected " + e.getMessage() );
        }

    }

    @Override
    public void onSensorChanged(SensorEvent event) {

        float axisX = event.values[0];
        float axisY= event.values[1];
        float axisZ= event.values[2];
        String str1 = null;
        String str2 = null;
        String str3 = null;
        if (Math.abs(axisX-ogx) > 0.04) {
            str1 =  "" + new DecimalFormat("##.####").format(axisX) ;
               /* "Gy= " + new DecimalFormat("##.##").format(axisY) + "\n" +
                "Gz= " + new DecimalFormat("##.##").format(axisZ)+ "\n\n";*/

            //tv1.setText(str);


            ogx = axisX;
        }
        else
        {
            str1 = "0" ;
               /* "Gy= " + new DecimalFormat("##.##").format(axisY) + "\n" +
                "Gz= " + new DecimalFormat("##.##").format(axisZ)+ "\n\n";*/

            //tv1.setText(str);

            ogx = 0;

        }


        if (Math.abs(axisY-ogy) > 0.04) {
            str2 = "" + new DecimalFormat("##.####").format(axisY);
            //tv2.setText(str);
            ogy = axisY;
        }
        else
        {
            str2 = "0";
            // tv2.setText(str);
            ogy = 0;

        }

        if (Math.abs(axisZ - ogz) > 0.04) {
            str3 = "" + new DecimalFormat("##.####").format(axisZ);
            //tv3.setText(str);
            ogz = axisZ;
        }
        else
        {
            str3 = "0" ;
            //tv3.setText(str);
            ogz=0;
        }

        String xyz = str1+","+str2+","+str3;
        //tv1.setText(xyz);
        if (!xyz.equals("0,0,0")) {
            //Log.d("client",xyz);
            if (debugswitch.isChecked()) {
                String tmp = tv1.getText().toString();
                tmp = xyz + "\n" + tmp;
                tv1.setText(tmp);
            }
            //new Thread(new ClientThread(xyz)).start();
            senddata(xyz);
        }

    }

    @Override
    public void onAccuracyChanged(Sensor sensor, int i) {

    }

    class ClientThread implements Runnable {
        private String strtosend;
        ClientThread(String str)
        {
            strtosend = str;
        }
        @Override
        public void run() {

            try {
              // if (!curr_ip.equals(pre_ip))
               // {


                   // tv1.setText("changed ip");
                    //pre_ip = curr_ip;

                    InetAddress serverAddr = InetAddress.getByName(curr_ip);

                    socket = new Socket(serverAddr, serverport);
                    out = new PrintWriter(new BufferedWriter(
                          new OutputStreamWriter(socket.getOutputStream())),
                        true);

                //}
                    out.println(strtosend);
                   socket.close();

            } catch (UnknownHostException e1) {
                //tv1.setText(e1.getMessage());
                //e1.printStackTrace();
                Log.d("cllient", "Unknown Host " + e1.getMessage());
                //Toast toast = Toast.makeText(getApplicationContext(), e1.getMessage(), Toast.LENGTH_SHORT);
                //toast.show();
                showToast("Unknown host "+ e1.getMessage());

            } catch (IOException e1) {
                //e1.printStackTrace();
                //tv1.setText();
                Log.d("cllient", "wrong input " + e1.getMessage());
                //Toast toast = Toast.makeText(getApplicationContext(), e1.getMessage(), Toast.LENGTH_SHORT);
                //toast.show();
                showToast("Wrong input"+e1.getMessage());
            }
            catch (IllegalArgumentException e1) {
                Log.d("cllient", "not getting connected " + e1.getMessage());
                //Toast toast = Toast.makeText(getApplicationContext(), "jhhhh", Toast.LENGTH_SHORT);
                //toast.show();
                //Toast.makeText(getApplicationContext(),"Hello Javatpoint",Toast.LENGTH_SHORT).show();

                showToast("May be Server is down" + e1.getMessage());
            }



        }

    }




}
