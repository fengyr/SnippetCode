package org.example.test;

import android.content.Context;
import android.content.Intent;
import android.content.BroadcastReceiver;

import android.net.ConnectivityManager;
import android.net.NetworkInfo;

import android.widget.Toast;

import android.util.Log;

public class ConnectionStatus extends BroadcastReceiver {

    private static final String TAG = "ConnectionStatus";
    private Context mContext;

    @Override
    public void onReceive(Context context, Intent intent) {
        mContext = context;

        showStatus();
    }

    private void showStatus() {
        int i;
        ConnectivityManager connectManager = (ConnectivityManager) mContext.getSystemService(Context.CONNECTIVITY_SERVICE);
        NetworkInfo[] activeInfo = connectManager.getAllNetworkInfo();

        for (i=0; i<activeInfo.length; i++) {
            Log.v(TAG, " debug " + activeInfo[i]);
        }

    }
}


