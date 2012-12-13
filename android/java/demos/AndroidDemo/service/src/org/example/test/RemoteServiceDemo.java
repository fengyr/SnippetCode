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

import android.util.Log;

public class RemoteServiceDemo extends Service {

    private static final String TAG = "RemoteServiceDemo";

    private final IRemoteService.Stub mBinder = new IRemoteService.Stub() {
        public String getRemoteName() {
            String name = "RemoteServiceDemo";
            return name;
        }

        public int getPid() {
            return Process.myPid();
        }
    };

    @Override
    public void onCreate() {
        /*code here*/
        Log.v(TAG, " onCreate,activity process id = " + Process.myPid() + "thread id = " + Thread.currentThread().getId());
    }

    @Override
    public void onStart(Intent  intent, int startId) {
        /*code here*/
    }

    @Override
    public IBinder onBind(Intent intent) {
        /*code here*/

        return mBinder;
    }

    @Override
    public boolean onUnbind(Intent intent) {
        /*code here*/

        return true;
    }

    @Override
    public void onDestroy() {
        /*code here*/
        Log.v(TAG, " destroy ");
        super.onDestroy();
    }

}


