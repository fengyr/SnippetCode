
package com.mstar.em1.test.suspendresume;

import android.app.AlarmManager;
import android.app.PendingIntent;
import android.app.Service;
import android.content.ComponentName;
import android.content.Context;
import android.content.Intent;
import android.os.IBinder;
import android.os.PowerManager;
import android.os.SystemClock;
import android.provider.Settings;
import android.util.Log;

public class RepeatSuspendResumeService extends Service {

    private static final String TAG = "RepeatSuspendResumeService";

    private int mScreenOffTimeout = 0;
    private int mSuspendInterval = 0;

    public IBinder onBind(Intent arg0) {
        return null;
    }

    @Override
    public void onCreate() {
        super.onCreate();
    }

    @Override
    public void onDestroy() {
        Log.d(TAG, "@@@@@@@@@@ onDestroy()");
        super.onDestroy();
        // cancel alarm
        Intent in = new Intent();
        in.putExtra(RepeatSuspendResumeActivity.EXTRA_SCREEN_OFF_TIMEOUT, mScreenOffTimeout);
        in.putExtra(RepeatSuspendResumeActivity.EXTRA_SUSPEND_INTERVAL, mSuspendInterval);
        in.setComponent(new ComponentName("com.mstar.em1.test.suspendresume",
                "com.mstar.em1.test.suspendresume.AlarmReceiver"));
        PendingIntent pendingIntent = PendingIntent.getBroadcast(this, 0, in,
                PendingIntent.FLAG_ONE_SHOT);
        AlarmManager alarm = (AlarmManager) getSystemService(Context.ALARM_SERVICE);
        alarm.cancel(pendingIntent);
        // disable screen off timeout
        setScreenOffTimeout(-1);
    }

    @Override
    public void onStart(Intent intent, int startId) {
        mScreenOffTimeout = intent.getIntExtra(RepeatSuspendResumeActivity.EXTRA_SCREEN_OFF_TIMEOUT, RepeatSuspendResumeActivity.SCREEN_OFF_TIMEOUT);
        mSuspendInterval = intent.getIntExtra(RepeatSuspendResumeActivity.EXTRA_SUSPEND_INTERVAL, RepeatSuspendResumeActivity.SUSPEND_INTERVAL);
        Log.d(TAG, "@@@@@@@@@@ onStart()");
        Log.d(TAG, "@@@@@@@@@@ Screen off timeout : " + mScreenOffTimeout + " Suspend interval : "
                + mSuspendInterval);
        super.onStart(intent, startId);
        try {
            // broadcast intent
            Intent in = new Intent();
            in.putExtra(RepeatSuspendResumeActivity.EXTRA_SCREEN_OFF_TIMEOUT, mScreenOffTimeout);
            in.putExtra(RepeatSuspendResumeActivity.EXTRA_SUSPEND_INTERVAL, mSuspendInterval);
            in.setComponent(new ComponentName("com.mstar.em1.test.suspendresume",
                    "com.mstar.em1.test.suspendresume.AlarmReceiver"));
            PendingIntent pendingIntent = PendingIntent.getBroadcast(this, 0, in,
                    PendingIntent.FLAG_ONE_SHOT | PendingIntent.FLAG_UPDATE_CURRENT);
            AlarmManager alarm = (AlarmManager) getSystemService(Context.ALARM_SERVICE);
            alarm.set(AlarmManager.ELAPSED_REALTIME_WAKEUP, SystemClock.elapsedRealtime()
                    + mSuspendInterval + mScreenOffTimeout, pendingIntent);
        } catch (Exception e) {
            e.printStackTrace();
        }
        // sleep
        PowerManager pm = (PowerManager) getSystemService(Context.POWER_SERVICE);
        pm.goToSleep(SystemClock.elapsedRealtime() + RepeatSuspendResumeActivity.GO_TO_SLEEP_DELAY);
    }

    private void setScreenOffTimeout(int ms) {
        Log.d(TAG, "@@@@@@@@@@ setScreenOffTimeout(" + ms + ")");
        Settings.System.putInt(getContentResolver(), Settings.System.SCREEN_OFF_TIMEOUT, ms);
    }
}
