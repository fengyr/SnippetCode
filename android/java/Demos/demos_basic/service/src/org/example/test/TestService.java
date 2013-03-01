package org.example.test;

import android.app.Activity;
import android.app.Service;

import android.content.ComponentName;
import android.content.Context;
import android.content.Intent;
import android.content.ServiceConnection;

import android.os.Binder;
import android.os.Bundle;
import android.os.IBinder;
import android.os.RemoteException;
import android.os.Process;

import android.widget.Toast;

import android.util.Log;

public class TestService extends Service {

    public class TestBinder extends Binder {
        String getMyName() {
            Log.v(TAG, " get name is zenki  ");
            return "get name is zenki";
        }
    }

    @Override
    public void onCreate() {
        Log.v(TAG, " onCreate,activity process id = " + Process.myPid() + "thread id = " + Thread.currentThread().getId());
        Toast.makeText(TestService.this, "service start", Toast.LENGTH_LONG).show();
    }

    @Override
    public void onStart(Intent  intent, int startId) {
        Log.v(TAG, " onStart ");
    }

    @Override
    public IBinder onBind(Intent intent) {
        Log.v(TAG, " onBind ");
        return mTestBinder;
    }

    @Override
    public boolean onUnbind(Intent intent) {
        Log.v(TAG, " onUnbind ");

        return true;
    }

    @Override
    public void onDestroy() {
        Log.v(TAG, " service onDestroy ");
        Toast.makeText(TestService.this, "service stop", Toast.LENGTH_LONG).show();
    }

    private final TestBinder mTestBinder = new TestBinder();
    private static final String TAG = "TestService";
}
