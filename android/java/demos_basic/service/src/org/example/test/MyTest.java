package org.example.test;

import android.app.Activity;

import android.view.View;
import android.view.View.OnClickListener;

import android.widget.Button;

import android.content.ComponentName;
import android.content.Context;
import android.content.Intent;
import android.content.ServiceConnection;

import android.os.Binder;
import android.os.Bundle;
import android.os.IBinder;
import android.os.RemoteException;
import android.os.Process;

import android.util.Log;

public class MyTest extends Activity implements OnClickListener {

    private static final String TAG = "MyTest";
    private RemoteServiceDemo mService = null;

    @Override
    public void onCreate(Bundle savedInstanceState)
    {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.main);

        mStartServiceButton = (Button) findViewById(R.id.start_service_id);
        mStartServiceButton.setOnClickListener(this);
        mStopServiceButton = (Button) findViewById(R.id.stop_service_id);
        mStopServiceButton.setOnClickListener(this);
    }

    public void onClick(View view) {
        switch ( view.getId() ) {
        case R.id.start_service_id :
            Log.v(TAG, " start service click,activity process id = " + Process.myPid() + "thread id = " + Thread.currentThread().getId());
            //startService(new Intent(MyTest.this, RemoteServiceDemo.class));
            //startService(new Intent(MyTest.this, RemoteServiceDemo.class));
            bindService(new Intent(MyTest.this, TestService.class), mConnection, Context.BIND_AUTO_CREATE);
            break;
        case R.id.stop_service_id :
            Log.v(TAG, " stop service click ");
            //stopService(new Intent(MyTest.this, RemoteServiceDemo.class));
            //stopService(new Intent(MyTest.this, RemoteServiceDemo.class));
            unbindService(mConnection);
            break;
        default :
            break;
        }
    }

    private ServiceConnection mConnection = new ServiceConnection() {
        public void onServiceConnected(ComponentName className, IBinder service) {
            TestService.TestBinder testService = (TestService.TestBinder) service;
            String msg = testService.getMyName();
            Log.v(TAG, " service connected get message: " + msg);
        }

        public void onServiceDisconnected(ComponentName className) {
            Log.v(TAG, " service disconnected ");
        }
    };

    private Button mStartServiceButton;
    private Button mStopServiceButton;
}
