package org.example.test;

import android.content.Context;
import android.content.Intent;
import android.content.BroadcastReceiver;

import android.widget.Toast;

public class MyBroadcast extends BroadcastReceiver {

    @Override
    public void onReceive(Context context, Intent intent)
    {
        Toast.makeText(context, "Broadcast received", Toast.LENGTH_SHORT).show();
    }
}

