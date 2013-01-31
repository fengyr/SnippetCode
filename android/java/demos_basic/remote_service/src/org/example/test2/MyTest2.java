package org.example.test2;

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
import org.example.test.IRemoteService;

import android.util.Log;

public class MyTest2 extends Activity implements OnClickListener {

    private static final String TAG = "MyTest2";
    IRemoteService mService = null;

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
            //startService(new Intent(MyTest2.this, TestService.class));
            bindService(new Intent(IRemoteService.class.getName()), mConnection, Context.BIND_AUTO_CREATE);
            
            break;
        case R.id.stop_service_id :
            Log.v(TAG, " stop service click ");

            try {
                String msg = mService.getRemoteName();
                Log.v(TAG, " get message: " + msg);

                int pid = mService.getPid();
                if ( pid > 0 ) {
                    Process.sendSignal(pid, 9);
                    Log.v(TAG, " pid " + pid);
                }
            }
            catch(Exception e) {
                Log.v(TAG, " exception " + e);
            }
            //stopService(new Intent(MyTest2.this, TestService.class));
            unbindService(mConnection);
            finish();
            break;
        default :
            break;
        }
    }

    private ServiceConnection mConnection = new ServiceConnection() {
        public void onServiceConnected(ComponentName className, IBinder service) {
            String msg = "";
            mService = IRemoteService.Stub.asInterface(service);
            try {
                msg = mService.getRemoteName();
            }
            catch(Exception e) {
                Log.v(TAG, " exception " + e);
            }

            Log.v(TAG, " service connected get message: " + msg);
        }

        public void onServiceDisconnected(ComponentName className) {
            mService = null;
            Log.v(TAG, " service disconnected ");
        }
    };

    private Button mStartServiceButton;
    private Button mStopServiceButton;
}
