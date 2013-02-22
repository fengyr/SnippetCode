package com.foxconn.edu_bbs.ota;

import java.io.File;
import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.IOException;

import com.foxconn.edu_bbs.display.DisplayOnePicActivity;
import com.foxconn.edu_bbs.service.ITransService;
import com.foxconn.edu_bbs.utils.TransConfig;

import android.app.Service;
import android.content.ComponentName;
import android.content.Context;
import android.content.Intent;
import android.content.ServiceConnection;
import android.content.SharedPreferences;
import android.net.ConnectivityManager;
import android.net.NetworkInfo;
import android.os.Handler;
import android.os.IBinder;
import android.os.Looper;
import android.os.Message;
import android.os.RemoteException;
import android.util.Log;

public class AlertService extends Service {
	private static String TAG = "Edu-BBS AlertService";
	private SharedPreferences mTransDataSp;
	private SharedPreferences mQueryStatusSp;
	private final String TRANS_DATA_SP_NAME = "transDataConfig";
	private final String SP_NAME_QUERY_STATUS = "queryStatusConfig";
	private long interval_time_1 = TransConfig.REQUEST_DATA_INTERVAL; // Interval
																		// time
																		// of
																		// request
																		// data
	private long interval_time_2 = TransConfig.REQUEST_STATUS_INTERVAL; // Interval
																		// time
																		// of
																		// report
																		// status
	private long interval_time_3 = TransConfig.RECONNECT_NETWORK_INTERVAL; // Interval
																			// time
																			// of
																			// reconnect
																			// network,
	// if it's down
	private DownloadHelper downloadHelper;
	private Context context;

	private Handler handler = new Handler(Looper.getMainLooper()) {
		public void handleMessage(Message msg) {
			super.handleMessage(msg);
			switch (msg.what) {
			case 1: {
				Intent intent = new Intent(AlertService.this,
						DisplayOnePicActivity.class);
				intent.setFlags(Intent.FLAG_ACTIVITY_NEW_TASK);
				AlertService.this.startActivity(intent);
			}
			}
		}
	};

	// ////////////////////// TransService Block /////////////////////////////
	private ITransService mITransService = null;
	private ServiceConnection mServiceConnection = new ServiceConnection() {

		@Override
		public void onServiceDisconnected(ComponentName name) {
			mITransService = null;
		}

		@Override
		public void onServiceConnected(ComponentName name, IBinder service) {
			mITransService = ITransService.Stub.asInterface(service);
		}
	};

	private void initTransService() {
		bindService(new Intent(ITransService.class.getName()),
				mServiceConnection, Context.BIND_AUTO_CREATE);
	}

	private void delTransService() {
		unbindService(mServiceConnection);
	}

	// //////////////////////TransService Block /////////////////////////////

	public IBinder onBind(Intent intent) {
		return null;
	}

	public void onCreate() {
		initTransService();

		super.onCreate();
	}

	public void onDestroy() {
		delTransService();

		super.onDestroy();
	}

	private void launchDisplayActivity() {
		Message message = handler.obtainMessage();
		message.what = 1;
		handler.sendMessage(message);
	}

	private void sendUpdateIntent() {
		Intent intent = new Intent();
		intent.setAction(DisplayOnePicActivity.UPDATE_INTENT);
		sendBroadcast(intent);
		Log.i(TAG, "sendUpdateIntent !!!");
	}

	public void init() {
		try {
			// Show old resource before new one has downloaded
			launchDisplayActivity();
		} catch (Exception e) {
			e.printStackTrace();
		}
	}

	private void transDataToSlave() {
		if (mITransService != null) {
			Log.i(TAG, "transDataToSlave start >>>>>>>");

			String zipFilePath = downloadHelper.getLastDownloadFile();

			// FIXME: 伪装文件名
			// zipFilePath = TransConfig.getInstance().getDownloadDir() +
			// "/bbs-aa01-31-0.zip";

			try {
				Log.i(TAG, "transDataToSlave filename = " + zipFilePath);
				mITransService.transFile(zipFilePath);
			} catch (RemoteException e) {
				e.printStackTrace();
			}

			Log.i(TAG, "transDataToSlave end <<<<<<<");
		} else {
			Log.e(TAG, "transDataToSlave mITransService == null !!!!!!");
		}
	}

	private String queryStatus() {
		String status = "1";
		for (int i = 0; i < TransConfig.DEVICE_NUM_PER_GROUP - 1; i++) {
			status = status + "0";
		}

		if (mITransService != null) {
			Log.i(TAG, "queryStatus start >>>>>>>");

			try {
				status = mITransService.queryStatus("1");
				Log.i(TAG, "queryStatus result = " + status);
			} catch (RemoteException e) {
				e.printStackTrace();
			}

			Log.i(TAG, "queryStatus end <<<<<<<");
		} else {
			Log.e(TAG, "queryStatus mITransService == null !!!!!!");
		}

		return status;
	}

	public void onStart(Intent intent, int startId) {
		super.onStart(intent, startId);

		// Master machine start to work
		init();
		context = this.getApplicationContext();
		mTransDataSp = getSharedPreferences(this.TRANS_DATA_SP_NAME,
				Context.MODE_WORLD_WRITEABLE);
		mQueryStatusSp = getSharedPreferences(this.SP_NAME_QUERY_STATUS,
				Context.MODE_WORLD_WRITEABLE);
		downloadHelper = new DownloadHelper();
		long currentTime = System.currentTimeMillis();
		SharedPreferences.Editor editor = mTransDataSp.edit();
		editor.putLong("time", currentTime);
		editor.putString("version", "0");
		editor.putLong("requestTime", interval_time_1);
		editor.commit();

		editor = mQueryStatusSp.edit();
		editor.putLong("rTime", currentTime);
		editor.commit();

		// Slaver machine won't do any of master's work.
		// String str = new BBSUtilities().getMasterID();
		// if (str.equalsIgnoreCase("unknow")) return;

		// Request for data in every certain seconds
		new Thread(new Runnable() {
			public void run() {
				while (true) {
					long currentTime = System.currentTimeMillis();
					long lastTime = mTransDataSp.getLong("time", -1L);

					long intervalTime = mTransDataSp.getLong("requestTime", -1L);
					String str = downloadHelper.getTime();
					if (str != null
							&& Integer.valueOf(str) * 1000 != intervalTime) {
						intervalTime = (long) (Integer.valueOf(str) * 1000);
						SharedPreferences.Editor editor = mTransDataSp.edit();
						editor.putLong("requestTime", intervalTime);
						editor.commit();
					}

					if (isNetworkConnected(context)) {
						if (currentTime - lastTime > intervalTime) {
							synchronized (downloadHelper) {
								String strVersion = mTransDataSp.getString(
										"version", "0");
								strVersion = downloadHelper
										.requestData(strVersion);
								if ("no".equals(strVersion)
										|| strVersion == null) {
									currentTime = System.currentTimeMillis();
									SharedPreferences.Editor editor = mTransDataSp
											.edit();
									editor.putLong("time", currentTime);
									editor.commit();
									Log.i(TAG,
											"Your data is the newest, no need to update!");
								} else {
									currentTime = System.currentTimeMillis();
									SharedPreferences.Editor editor = mTransDataSp
											.edit();
									editor.putLong("time", currentTime);
									editor.putString("version", strVersion);
									editor.commit();
									Log.i(TAG,
											"Down the data and unzip it already, show the picture then!");
									sendUpdateIntent();

									// 有数据更新后向slave端传输数据
									transDataToSlave();
								}
							}
						}
					} else {
						Log.e(TAG,
								"Network is down, retry again in 60 seconds! [TransData]");

						// FIXME: 调试失败情况下，向slave端传输数据
//						synchronized (downloadHelper) {
//							transDataToSlave();
//						}

						try {
							Thread.sleep(interval_time_3);
						} catch (InterruptedException e) {
							e.printStackTrace();
						}
					}

					try {
						Thread.sleep(500);
					} catch (InterruptedException e) {
						e.printStackTrace();
					}
				}
			}
		}, "thread-trans-data").start();

		// Send devices' statues to the server in every 3 minutes
		new Thread(new Runnable() {
			public void run() {
				while (true) {
					long currentTime = System.currentTimeMillis();
					long lastTime = mQueryStatusSp.getLong("rTime", -1L);
					if (isNetworkConnected(context)) {
						if (currentTime - lastTime > interval_time_2) {
							synchronized (downloadHelper) {
								String status = queryStatus();
								downloadHelper.responseStatus(status);
								currentTime = System.currentTimeMillis();
								SharedPreferences.Editor editor = mQueryStatusSp.edit();
								editor.putLong("rTime", currentTime);
								editor.commit();
							}
							Log.e(TAG,
									"Send devices' statues to the server already!");
						}
					} else {
						Log.e(TAG,
								"Network is down, retry again in 60 seconds! [QueryStatus]");

						// FIXME: 调试失败情况下，向slave端查询状态
//						synchronized (downloadHelper) {
//							String status = queryStatus();
//							downloadHelper.responseStatus(status);
//						}

						try {
							Thread.sleep(interval_time_3);
						} catch (InterruptedException e) {
							e.printStackTrace();
						}
					}

					try {
						Thread.sleep(500);
					} catch (InterruptedException e) {
						e.printStackTrace();
					}
				}
			}
		}, "thread-query-status").start();
		
		new Thread(new Runnable() {
			public void run() {
				while (true) {
					try {
						wakeupDevice();
						
						Thread.sleep(TransConfig.WAKEUP_INTERVAL);
					} catch (InterruptedException e) {
						e.printStackTrace();
					}
				}
			}
		}, "thread-wake-up").start();
	}

	private boolean wakeupDevice() {
		File wakeupFile = new File(TransConfig.WAKEUP_INTERFACE);
		if (!wakeupFile.exists()) {
			Log.e(TAG,
					"Can not find /proc/chg_enable !!!");
			return false;
		}

		FileOutputStream os = null;
		String msg = String.format("%d", 1);
		try {
			os = new FileOutputStream(wakeupFile);
			os.write(msg.getBytes());
			os.flush();
			os.close();
		} catch (FileNotFoundException e) {
			e.printStackTrace();
		} catch (IOException e) {
			e.printStackTrace();
		}

		Log.i(TAG, "Service wakeupDevice");

		return true;
	}
	
	public boolean isNetworkConnected(Context context) {
		ConnectivityManager cm = (ConnectivityManager) context
				.getSystemService(Context.CONNECTIVITY_SERVICE);
		if (cm == null) {
			return false;
		} else {
			NetworkInfo[] info = cm.getAllNetworkInfo();
			if (info != null) {
				for (int i = 0; i < info.length; i++) {
					if (info[i].getState() == NetworkInfo.State.CONNECTED
							&& info[i].getTypeName().equalsIgnoreCase("MOBILE")) {
						return true;
					}
				}
			}
		}
		return false;
	}
}
