package org.example.test;

import android.app.Activity;
import android.app.Notification;
import android.app.NotificationManager;
import android.app.PendingIntent;

import android.os.Bundle;

import android.content.Intent;

import android.widget.RemoteViews;

public class MyTest extends Activity {

    @Override
    public void onCreate(Bundle savedInstanceState)
    {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.main);

        mTestManager = (NotificationManager)getSystemService(NOTIFICATION_SERVICE);
        mNotifi = new Notification(R.drawable.stat_sample, getString(R.string.tickerText), 
                System.currentTimeMillis());
        //mNotifi = new Notification();

        makeNotification();
    }

    private void makeNotification() {
        mContentIntent = makeIntent();
        mNotifi.setLatestEventInfo(this, "hello", "zenki", mContentIntent);
        mNotifi.vibrate = new long[] { 100, 250, 100, 500};
        mTestManager.notify(R.string.tickerText, mNotifi);
    }

    private PendingIntent makeIntent() {
        return PendingIntent.getBroadcast(this, 0, new Intent(this, MyBroadcast.class), 0);
    }

    private NotificationManager mTestManager;
    private PendingIntent mContentIntent;
    private Notification mNotifi;
}
