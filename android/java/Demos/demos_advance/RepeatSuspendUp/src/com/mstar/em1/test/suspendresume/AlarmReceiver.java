
package com.mstar.em1.test.suspendresume;

import com.mstar.em1.test.suspendresume.RepeatSuspendResumeActivity;

import android.app.AlarmManager;
import android.app.PendingIntent;
import android.content.BroadcastReceiver;
import android.content.ComponentName;
import android.content.Context;
import android.content.Intent;
import android.os.PowerManager;
import android.os.SystemClock;
import android.os.PowerManager.WakeLock;
import android.util.Log;

public class AlarmReceiver extends BroadcastReceiver {

    private static final String TAG = "AlarmReceiver";

    public void onReceive(Context context, Intent intent) {
        final Context c = context;
        String action = intent.getAction();
        int timeOfScreenOff = intent.getIntExtra(
                RepeatSuspendResumeActivity.EXTRA_SCREEN_OFF_TIMEOUT,
                RepeatSuspendResumeActivity.SCREEN_OFF_TIMEOUT);
        int timeOfSuspendInterval = intent.getIntExtra(
                RepeatSuspendResumeActivity.EXTRA_SUSPEND_INTERVAL,
                RepeatSuspendResumeActivity.SUSPEND_INTERVAL);
        Log.d(TAG, "@@@@@@@@@@ onReceive() : Action = " + action);
        Log.d(TAG, "@@@@@@@@@@ Screen off timeout : " + timeOfScreenOff + " Suspend interval : "
                + timeOfSuspendInterval);
        // power on the device
        PowerManager pm = (PowerManager) context.getSystemService(Context.POWER_SERVICE);
        WakeLock wl = pm.newWakeLock(PowerManager.FULL_WAKE_LOCK
                | PowerManager.ACQUIRE_CAUSES_WAKEUP, TAG);
        wl.acquire();
        // set alarm after a time interval
        Log.d(TAG, "@@@@@@@@@@ Set alarm");
        // set alarm
        Intent in = new Intent();
        in.putExtra(RepeatSuspendResumeActivity.EXTRA_SCREEN_OFF_TIMEOUT, timeOfScreenOff);
        in.putExtra(RepeatSuspendResumeActivity.EXTRA_SUSPEND_INTERVAL, timeOfSuspendInterval);
        in.setComponent(new ComponentName("com.mstar.em1.test.suspendresume",
                "com.mstar.em1.test.suspendresume.AlarmReceiver"));
        PendingIntent pendingIntent = PendingIntent.getBroadcast(c, 0, in,
                PendingIntent.FLAG_ONE_SHOT | PendingIntent.FLAG_UPDATE_CURRENT);
        AlarmManager alarm = (AlarmManager) c.getSystemService(Context.ALARM_SERVICE);
        alarm.set(AlarmManager.ELAPSED_REALTIME_WAKEUP, SystemClock.elapsedRealtime()
                + timeOfScreenOff + timeOfSuspendInterval, pendingIntent);
        try {
            Thread.sleep(RepeatSuspendResumeActivity.SCREEN_OFF_TIMEOUT);
        } catch (InterruptedException e) {
            e.printStackTrace();
        }
        Log.d(TAG, "@@@@@@@@@@ onReceive() end");
        // release wake lock
        wl.release();
    }
}
