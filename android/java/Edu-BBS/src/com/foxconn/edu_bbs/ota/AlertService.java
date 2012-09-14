package com.foxconn.edu_bbs.ota;

import com.foxconn.edu_bbs.display.DisplayOnePicActivity;
import com.foxconn.edu_bbs.service.ITransService;
import com.foxconn.edu_bbs.utils.BBSUtilities;

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
	private SharedPreferences sp;
	private final String SP_NAME = "configuration";	
	private long interval_time_1 = 30000;	//Interval time of request data
	private long interval_time_2 = 180000;	//Interval time of report status
	private long interval_time_3 = 60000;	//Interval time of reconnect network, if it's down
	private DownloadHelper dp;
	private Context context;
	
	private Handler handler = new Handler(Looper.getMainLooper()) {
		public void handleMessage(Message msg) {
			super.handleMessage(msg);
			switch (msg.what) {
			case 1:
				Intent intent = new Intent(AlertService.this, DisplayOnePicActivity.class);
				intent.setFlags(Intent.FLAG_ACTIVITY_NEW_TASK);
				AlertService.this.startActivity(intent);
//			case 2:
//				SharedPreferences.Editor editor = sp.edit();
//				editor.putLong("requestTime", msg.arg1);
//				editor.commit();
			}
		}
	};
	
	//////////////////////// TransService Block /////////////////////////////
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
	////////////////////////TransService Block /////////////////////////////
	
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
	
	public void init() {
		try {
//			DocumentBuilderFactory dbf = DocumentBuilderFactory.newInstance();   
//			DocumentBuilder db = dbf.newDocumentBuilder();  
//			Document doc = db.parse(new File(configFilePath));
//			THIRTY_MILLS = Long.parseLong(doc.getElementsByTagName("RequestDataFromServer").item(0).getFirstChild().getNodeValue());
//			THREE_MINUTES_MILLS = Long.parseLong(doc.getElementsByTagName("ResponseStatueToServer").item(0).getFirstChild().getNodeValue());
//			Log.i(TAG, "RequestDataFromServer in every " + interval_time_1/1000 + " seconds");
//			Log.i(TAG, "ResponseStatueToServer in every " + interval_time_2/1000 + " seconds");
			
			// Show old resource before new one has downloaded
			Message message = handler.obtainMessage();
			message.what = 1;
			handler.sendMessage(message);
		} catch (Exception e) {
			e.printStackTrace();
		}
	}
	
	private void transDataToSlave() {
		if (mITransService != null) {
			Log.e(TAG, "transDataToSlave start !!!!!!!!!!!!!!!!!!!");
			
			String zipFilePath = dp.getLastDownloadFile();
			
			try {
				mITransService.transData(zipFilePath);
			} catch (RemoteException e) {
				e.printStackTrace();
			}
		}
	}
	
	private String queryStatus() {
		String status = "00000000";
		
		if (mITransService != null) {
			Log.e(TAG, "queryStatus start !!!!!!!!!!!!!!!!!!!");
			
			try {
				status = mITransService.queryStatus("1");
				Log.e(TAG, "clickBtnQueryStatus result = " + status);
			} catch (RemoteException e) {
				e.printStackTrace();
			}
		}
		
		return status;
	}

	public void onStart(Intent intent, int startId) {
		super.onStart(intent, startId);
		
		// Slaver machine won't do any of master's work.
		String str = new BBSUtilities().getMasterID();
		if (str.equalsIgnoreCase("unknow")) return;
		
		// Master machine start to work
		init();
		context = this.getApplicationContext();
		sp = getSharedPreferences(this.SP_NAME, Context.MODE_WORLD_WRITEABLE);
		dp = new DownloadHelper();
		long currentTime = System.currentTimeMillis();
		SharedPreferences.Editor editor = sp.edit();
		editor.putLong("time", currentTime);
		editor.putLong("rTime", currentTime);
		editor.putString("version", "0");
		editor.putLong("requestTime", interval_time_1);
		editor.commit();
		
		//Request for data in every certain seconds
		new Thread(new Runnable() {
			public void run() {
				while (true) {
					long currentTime = System.currentTimeMillis();
					long lastTime = sp.getLong("time", -1L);
					
					long intervalTime = sp.getLong("requestTime", -1L);
					String str = dp.getTime();
					if (str != null && Integer.valueOf(str) * 1000 != intervalTime) {
						intervalTime = (long) (Integer.valueOf(str) * 1000);
						SharedPreferences.Editor editor = sp.edit();
						editor.putLong("requestTime", intervalTime);
						editor.commit();
					}
					
					if (isNetworkConnected(context)) {
						if (currentTime - lastTime > intervalTime) {
							String strVersion = sp.getString("version", "0");
							strVersion = dp.requestData(strVersion);
							if ("no".equals(strVersion) || strVersion == null) {
								currentTime = System.currentTimeMillis();
								SharedPreferences.Editor editor = sp.edit();
								editor.putLong("time", currentTime);
								editor.commit();
								Log.i(TAG, "Your data is the newest, no need to update!");
							} else {
								currentTime = System.currentTimeMillis();
								SharedPreferences.Editor editor = sp.edit();
								editor.putLong("time", currentTime);
								editor.putString("version", strVersion);
								editor.commit();
								Log.i(TAG, "Down the data and unzip it already, show the picture then!");
								Message message = handler.obtainMessage();
								message.what = 1;
								handler.sendMessage(message);
								
								// 向slave端传输数据
								transDataToSlave();
							}
						}
					} else {
						Log.e(TAG, "Network is down, retry again in 60 seconds!");
						try {
							Thread.sleep(interval_time_3);
						} catch (InterruptedException e) {
							e.printStackTrace();
						}
					}
					
					try {
						Thread.sleep(200);
					} catch (InterruptedException e) {
						e.printStackTrace();
					}
				}
			}
		}, "thread-alert").start();
		
		//Send devices' statues to the server in every 3 minutes
		new Thread(new Runnable() {
			public void run() {
				while (true) {
					long currentTime = System.currentTimeMillis();
					long lastTime = sp.getLong("rTime", -1L);
					if (isNetworkConnected(context)) {
						if (currentTime - lastTime > interval_time_2) {
							String status = queryStatus();
							dp.responseStatus(status);
							currentTime = System.currentTimeMillis();
							SharedPreferences.Editor editor = sp.edit();
							editor.putLong("rTime", currentTime);
							editor.commit();
							Log.e(TAG, "Send devices' statues to the server already!");
						}
					} else {
						Log.e(TAG, "Network is down, retry again in 60 seconds!");
						try {
							Thread.sleep(interval_time_3);
						} catch (InterruptedException e) {
							e.printStackTrace();
						}
					}
					
					try {
						Thread.sleep(200);
					} catch (InterruptedException e) {
						e.printStackTrace();
					}
				}
			}
		}, "thread-alert").start();
	}
		
	public boolean isNetworkConnected(Context context) {
		ConnectivityManager cm = (ConnectivityManager)context.getSystemService(
				Context.CONNECTIVITY_SERVICE);
		if (cm == null) {
			return false;
		} else {
			NetworkInfo[] info = cm.getAllNetworkInfo();
			if (info != null) {
				for (int i = 0; i < info.length; i++) {
					if (info[i].getState() == NetworkInfo.State.CONNECTED
							&& info[i].getTypeName().equalsIgnoreCase("WIFI")) {
						return true;
					}
				}
			}
		}
		return false;
	}
}
