package com.foxconn.edu_bbs.display;

import java.io.File;
import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.IOException;
import java.util.Timer;
import java.util.TimerTask;

import com.foxconn.edu_bbs.ota.R;
import com.foxconn.edu_bbs.utils.BBSUtilities;
import com.foxconn.edu_bbs.utils.TransConfig;

import android.app.Activity;
import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;
import android.content.IntentFilter;
import android.content.SharedPreferences;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.graphics.Matrix;
import android.graphics.drawable.Drawable;
import android.os.Bundle;
import android.os.Handler;
import android.os.Message;
import android.os.PowerManager;
import android.util.Log;
import android.view.Menu;
import android.view.MenuItem;
import android.view.View;
import android.view.Window;
import android.view.WindowManager;
import android.widget.ImageView;

public class DisplayOnePicActivity extends Activity {

	static private final String TAG = "DisplayOnePicActivity";

	static public int UPDATE_MODE_FULL = View.EINK_UPDATE_MODE_FULL
			| View.EINK_WAIT_MODE_WAIT | View.EINK_WAVEFORM_MODE_GC16;
	static public int UPDATE_MODE_CLEAN = View.EINK_WAVEFORM_MODE_INIT;
	static public String UPDATE_INTENT = "UpdateDisplay";
	private TransConfig mConfig = TransConfig.getInstance();
	private String targetFileDir = mConfig.getTargetDir();
	private int DISPLAY_SLEEP_TIME = TransConfig.SWITCH_SCREEN_INTERVAL;
	private String fileNamePart = "-bbs-%d-%d.jpg";
	private ImageView mImageView;
	private Bitmap mBackground;
	PowerManager mPowerManager;
	PowerManager.WakeLock mWakeLock;
	private SharedPreferences mSharedPreferences;
	private final String SPNAME = "deviceNumber";

	static private final int MSG_DISPLAY_PICTURE = 1;
	static private final int MSG_DISPLAY_BLANK = 2;

	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		requestWindowFeature(Window.FEATURE_NO_TITLE);
		setContentView(R.layout.main);

		mImageView = (ImageView) findViewById(R.id.loadPic);
		mImageView.setOnClickListener(new View.OnClickListener() {
			@Override
			public void onClick(View v) {
				// TODO Auto-generated method stub
				openOptionsMenu();
			}
		});

		mPowerManager = (PowerManager) getSystemService(Context.POWER_SERVICE);
		mWakeLock = mPowerManager.newWakeLock(
				PowerManager.SCREEN_BRIGHT_WAKE_LOCK, "BBS_DISPLAY");
		if (mWakeLock != null) {
			mWakeLock.acquire();
		}
		
		mSharedPreferences = getSharedPreferences(SPNAME,
				Context.MODE_WORLD_WRITEABLE);

		mBackground = BitmapFactory.decodeResource(getResources(),
				R.drawable.background);

		mDisplayThread.start();

		registerDisplayReceiver();
	}

	@Override
	protected void onResume() {
		super.onResume();
	}

	protected void onDestroy() {
		if (mWakeLock != null) {
			mWakeLock.release();
		}

		unregisterDisplayReceiver();

		super.onDestroy();
	};

	private final BroadcastReceiver mDisplayReceiver = new BroadcastReceiver() {
		@Override
		public void onReceive(Context context, Intent intent) {
			String action = intent.getAction();
			if (action.equals(UPDATE_INTENT)) {
				synchronized (mDisplayThread) {
					Log.i(TAG, "mDisplayReceiver notify !!!");
					mDisplayThread.notify();
				}
			}
		}
	};

	private void registerDisplayReceiver() {
		IntentFilter filter = new IntentFilter();
		filter.addAction(DisplayOnePicActivity.UPDATE_INTENT);
		registerReceiver(mDisplayReceiver, filter);
	}

	private void unregisterDisplayReceiver() {
		unregisterReceiver(mDisplayReceiver);
	}

	Handler mHandler = new Handler() {
		public void handleMessage(android.os.Message msg) {
			switch (msg.what) {
			case MSG_DISPLAY_PICTURE:
				Bitmap bmp = (Bitmap) msg.obj;
				showPicture(bmp);
				break;
			case MSG_DISPLAY_BLANK:
				showBlankBackground();
				break;

			default:
				break;
			}
		};
	};

	private boolean switchPanel(int id) {
		File panelFile = new File(TransConfig.PANEL_INTERFACE);
		if (!panelFile.exists()) {
			Log.e(TAG,
					"Can not find /sys/devices/platform/mxc_epdc_fb/panel_en !!!");
			return false;
		}

		FileOutputStream os = null;
		String msg = String.format("%d", id);
		try {
			os = new FileOutputStream(panelFile);
			os.write(msg.getBytes());
			os.flush();
			os.close();
		} catch (FileNotFoundException e) {
			e.printStackTrace();
		} catch (IOException e) {
			e.printStackTrace();
		}

		Log.i(TAG, "switchPanel id = " + id);

		return true;
	}

	/*
	* Scale the image to fit for the screen
	*/
	private Bitmap scaleImage(Bitmap bmp, int degree) {
		float scale_x = (float)mImageView.getWidth()/bmp.getWidth();
		float scale_y = (float)mImageView.getHeight()/bmp.getHeight();

		Matrix matrix = new Matrix();
		matrix.postRotate(degree);
		matrix.postScale(scale_x, scale_y);

		Bitmap bitmap = Bitmap.createBitmap(bmp, 0, 0, bmp.getWidth(), bmp.getHeight(), matrix,
				true);
		return bitmap;
	}

	private void showBlankBackground() {
		if (mBackground != null) {
			Log.i(TAG, "showBlankBackground update");
			mImageView.invalidate(UPDATE_MODE_CLEAN | UPDATE_MODE_FULL);
			mImageView.setImageBitmap(mBackground);
			mImageView.invalidate(UPDATE_MODE_CLEAN | UPDATE_MODE_FULL);
		}
	}

	private void showPicture(Bitmap bitmap) {
		showBlankBackground();

		Log.i(TAG, "showPicture update double");
		mImageView.setImageBitmap(bitmap);
		mImageView.invalidate(UPDATE_MODE_CLEAN | UPDATE_MODE_FULL);
		
		showBlankBackground();
		mImageView.setImageBitmap(bitmap);
		mImageView.invalidate(UPDATE_MODE_CLEAN | UPDATE_MODE_FULL);
	}

	private int getDeviceNumber() {
		if (null == mSharedPreferences) {
			mSharedPreferences = getSharedPreferences(SPNAME,
					Context.MODE_WORLD_WRITEABLE);
		}
		
		int deviceNumber = mSharedPreferences.getInt("DeviceNumber", 0);
		
		return deviceNumber;
	}
	
	private Thread mDisplayThread = new Thread(new Runnable() {
		@Override
		public void run() {
			for (int i = 0; i < TransConfig.DISPLAY_NUM_PER_DEVICE; i++) {
				switchPanel(i + 1);

				Message msg = Message.obtain();
				msg.what = MSG_DISPLAY_BLANK;
				mHandler.sendMessage(msg);

				try {
					Thread.sleep(2000);
				} catch (InterruptedException e) {
					e.printStackTrace();
				}
			}

			synchronized (mDisplayThread) {
				while (true) {
					String device_uid = new BBSUtilities().getMasterID();
					int device_num = getDeviceNumber();

					for (int i = 0; i < TransConfig.DISPLAY_NUM_PER_DEVICE; i++) {
						// XXX:切换屏幕显示，i从0~3，因此调用switchPanel时需要加1
						switchPanel(i + 1);

						String picFile = targetFileDir
								+ "/"
								+ device_uid
								+ String.format(fileNamePart,
										device_num, i);

						File mFile = null;
						mFile = new File(picFile);
						if (!mFile.exists()) {
							Log.e(TAG,
									"refresh targetFile = null !!!!!!!!   name = "
											+ picFile);
						} else {
							Bitmap bitmap = BitmapFactory.decodeFile(picFile);

							if (bitmap != null) {
								Log.i(TAG,
										"refresh targetFile = " + picFile
												+ " picWidth="
												+ bitmap.getWidth()
												+ " picHeight="
												+ bitmap.getHeight()
												+ " viewWidth="
												+ mImageView.getWidth()
												+ " viewHeight="
												+ mImageView.getHeight());

								if (i == 0 || i ==2) {
									bitmap = scaleImage(bitmap, 180);
								} else {
									bitmap = scaleImage(bitmap, 0);
								}
								Message msg = Message.obtain();
								msg.what = MSG_DISPLAY_PICTURE;
								msg.obj = bitmap;
								mHandler.sendMessage(msg);
							} else {
								Log.e(TAG,
										"decode bitmap = null !!!!!!!!  file = "
												+ picFile);
							}
						}

						try {
							Thread.sleep(DISPLAY_SLEEP_TIME);
						} catch (InterruptedException e) {
							e.printStackTrace();
						}
					}

					try {
						Log.i(TAG, "mDisplayReceiver wait !!!");
						mDisplayThread.wait();
					} catch (InterruptedException e) {
						e.printStackTrace();
					}
				}
			}
		}
	});

	public boolean onCreateOptionsMenu(Menu menu) {
		menu.add(0,0,0,"Exit");
		return true;
	}

	public boolean onOptionsItemSelected(MenuItem item) {
		int item_id = item.getItemId();
		switch (item_id) {
		case 0:
			DisplayOnePicActivity.this.finish();
			break;
		}
		return true;
	}
}
