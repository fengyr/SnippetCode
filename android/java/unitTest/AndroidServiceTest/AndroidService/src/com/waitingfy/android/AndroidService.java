package com.waitingfy.android;

import android.app.Service;
import android.content.Intent;
import android.os.IBinder;
import android.util.Log;

public class AndroidService extends Service{
 
	private final static String TAG = "AndroidService";
	@Override
	public void onCreate() {
		super.onCreate();
		Log.v(TAG, "service: onCreate()");
	}
	@Override
	public void onStart(Intent intent, int startId) {
		super.onStart(intent, startId);
		Log.v(TAG, "service: onStart()");
	}
	@Override
	public void onDestroy() {
		super.onDestroy();
		Log.v(TAG, "service: onDestroy()");
	}
	@Override
	public IBinder onBind(Intent intent) {
		// TODO Auto-generated method stub
		return null;
	}

}
