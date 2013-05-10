/**
 * @file       PhotoBaseActivity.java
 *
 * @revision:  none 
 *
 * @version    0.0.01
 * @author:    Zenki (zhajun), junzha@pateo.com.cn
 * @date:      2013-4-22 下午01:22:37 
 */

package com.pateo.as.photo.activity;

import android.app.Activity;
import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;
import android.content.IntentFilter;
import android.os.Bundle;
import android.util.Log;
import android.view.Window;
import android.view.WindowManager;

/**
 *
 */
public abstract class BaseActivity extends Activity {
	static private final String TAG = "PhotoBaseActivity";

	static public final String KEY_EXTRA_DATA = "extra_data";
	static public final String KEY_BUNDLE_ALBUM = "album";
	static public final String KEY_BUNDLE_PHOTO = "photo";
	static public final String KEY_BUNDLE_POS = "position";
	
	private boolean mIsFullScreen;

	public static final int SYSTEM_UI_FLAG_HIDE_NAVIGATION = 0x00000002;

	private UsbReceiver mUsbReceiver;

	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);

		requestWindowFeature(Window.FEATURE_NO_TITLE);
		mIsFullScreen = false;
	}

	@Override
	protected void onResume() {
		super.onResume();

		registerUsbReceiver();
	}

	@Override
	protected void onStop() {
		super.onStop();

		unregisterUsbReceiver();
	}

	protected void registerUsbReceiver() {
		if (null == mUsbReceiver) {
			mUsbReceiver = new UsbReceiver();
		}

		IntentFilter filter = new IntentFilter();
		filter.addAction(Intent.ACTION_MEDIA_MOUNTED);
		filter.addAction(Intent.ACTION_MEDIA_UNMOUNTED);
		filter.addAction(Intent.ACTION_MEDIA_EJECT);
		filter.addAction(Intent.ACTION_MEDIA_REMOVED);
		filter.addAction(Intent.ACTION_MEDIA_SCANNER_FINISHED);
		filter.addDataScheme("file");
		registerReceiver(mUsbReceiver, filter);
	}

	protected void unregisterUsbReceiver() {
		if (mUsbReceiver != null) {
			unregisterReceiver(mUsbReceiver);
		}
	}

	protected class UsbReceiver extends BroadcastReceiver {
		@Override
		public void onReceive(Context context, Intent intent) {
			onReceiveBroadcast(context, intent);
		}
	}

	protected void enterFullScreen() {
		getWindow().addFlags(WindowManager.LayoutParams.FLAG_FULLSCREEN);
//		getWindow().getDecorView().setSystemUiVisibility(
//				SYSTEM_UI_FLAG_HIDE_NAVIGATION);
		mIsFullScreen = true;
	}
	
	protected void exitFullScreen() {
		final WindowManager.LayoutParams attrs = getWindow().getAttributes();
		attrs.flags &= (~WindowManager.LayoutParams.FLAG_FULLSCREEN);
		getWindow().setAttributes(attrs);
		getWindow().clearFlags(WindowManager.LayoutParams.FLAG_LAYOUT_NO_LIMITS);
		
		mIsFullScreen = false;
	}
	
	protected boolean isFullScreen() {
		return mIsFullScreen;
	}

	abstract protected void onReceiveBroadcast(Context context, Intent intent);
}
