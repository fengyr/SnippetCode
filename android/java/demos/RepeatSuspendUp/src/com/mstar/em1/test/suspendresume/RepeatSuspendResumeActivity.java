package com.mstar.em1.test.suspendresume;

import com.mstar.em1.test.suspendresume.R;

import android.app.Activity;
import android.content.BroadcastReceiver;
import android.content.ComponentName;
import android.content.Context;
import android.content.Intent;
import android.content.IntentFilter;
import android.os.Bundle;
import android.util.Log;
import android.view.View;
import android.view.View.OnClickListener;
import android.widget.Button;
import android.widget.TextView;

public class RepeatSuspendResumeActivity extends Activity implements
		OnClickListener {

	public static final int SUSPEND_INTERVAL = 20 * 1000; // ms
	public static final int SCREEN_OFF_TIMEOUT = 0 * 1000; // ms
	public static final int GO_TO_SLEEP_DELAY = 0 * 1000; // ms
	public static final String ALARM_ACTION = "mstar.em1.action.alarm_wakeup";
	public static final String EXTRA_SCREEN_OFF_TIMEOUT = "mstar.em1.extra_screen_off_timeout";
	public static final String EXTRA_SUSPEND_INTERVAL = "mstar.em1.extra_suspend_interval";

	private static final String TAG = "RepeatSuspendUp";

	private volatile boolean mStarted = false;
	private int mScreenOff = SCREEN_OFF_TIMEOUT;
	private int mSuspendInterval = SUSPEND_INTERVAL;
	private Button mControlButton = null;
	private TextView mShowNumber = null;
	static private long mNumber = 0;

	public void onClick(View v) {
		switch (v.getId()) {
		case R.id.control_button:
			if (!mStarted) {
				Log.d(TAG, "@@@@@@@@@@ Start !!!!!!");
				mStarted = true;
				// start service
				Intent in = new Intent();
				in.putExtra(EXTRA_SCREEN_OFF_TIMEOUT, mScreenOff);
				in.putExtra(EXTRA_SUSPEND_INTERVAL, mSuspendInterval);
				in.setComponent(new ComponentName(
						"com.mstar.em1.test.suspendresume",
						"com.mstar.em1.test.suspendresume.RepeatSuspendResumeService"));
				startService(in);
				// change button text
				mControlButton.setText(R.string.stop);
			} else {
				Log.d(TAG, "@@@@@@@@@@ Stop !!!!!!!");
				Intent in = new Intent();
				in.setComponent(new ComponentName(
						"com.mstar.em1.test.suspendresume",
						"com.mstar.em1.test.suspendresume.RepeatSuspendResumeService"));
				stopService(in);
				mControlButton.setText(R.string.start);
				mStarted = false;
			}
			break;
		case R.id.exit_button:
			Intent in = new Intent();
			in.setComponent(new ComponentName(
					"com.mstar.em1.test.suspendresume",
					"com.mstar.em1.test.suspendresume.RepeatSuspendResumeService"));
			stopService(in);
			finish();
			break;
		case R.id.device_on_15_sec:
			setTimeOfDeviceOn(15 * 1000, ((Button) v).getText().toString());
			break;
		case R.id.device_on_30_sec:
			setTimeOfDeviceOn(30 * 1000, ((Button) v).getText().toString());
			break;
		case R.id.device_on_45_sec:
			setTimeOfDeviceOn(45 * 1000, ((Button) v).getText().toString());
			break;
		case R.id.device_on_1_min:
			setTimeOfDeviceOn(60 * 1000, ((Button) v).getText().toString());
			break;
		case R.id.suspend_30_sec:
			setTimeOfSuspend(30 * 1000, ((Button) v).getText().toString());
			break;
		case R.id.suspend_45_sec:
			setTimeOfSuspend(45 * 1000, ((Button) v).getText().toString());
			break;
		case R.id.suspend_1_min:
			setTimeOfSuspend(60 * 1000, ((Button) v).getText().toString());
			break;
		case R.id.suspend_2_min:
			setTimeOfSuspend(2 * 60 * 1000, ((Button) v).getText().toString());
			break;
		}
	}

	/** Called when the activity is first created. */
	@Override
	public void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setContentView(R.layout.main);
		// start/stop button
		mControlButton = (Button) findViewById(R.id.control_button);
		mShowNumber = (TextView) findViewById(R.id.show_number);
		// initial
		setTimeOfDeviceOn(SCREEN_OFF_TIMEOUT, "2 s");
		setTimeOfSuspend(SUSPEND_INTERVAL, "20 s");

		IntentFilter filter = new IntentFilter();
		filter.addAction(Intent.ACTION_SCREEN_ON);
		registerReceiver(new UpdateReceiver(), filter);
	}

	private void setTimeOfDeviceOn(int ms, String text) {
		mScreenOff = ms;
		((TextView) findViewById(R.id.device_on_text)).setText(getResources()
				.getString(R.string.on_text) + text);
	}

	private void setTimeOfSuspend(int ms, String text) {
		mSuspendInterval = ms;
		((TextView) findViewById(R.id.device_suspend_text))
				.setText(getResources().getString(R.string.suspend_text) + text);
	}

	public void updateShow() {
		mNumber++;
		String temp = String.valueOf(mNumber);
		mShowNumber.setText(temp);
	}

	class UpdateReceiver extends BroadcastReceiver {

		@Override
		public void onReceive(Context context, Intent intent) {
			Log.v(TAG, "updateReceiver " + intent.getAction());
			updateShow();
		}
	}
}
