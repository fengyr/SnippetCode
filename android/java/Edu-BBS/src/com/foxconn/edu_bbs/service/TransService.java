/**
 * @file       TransService.java
 *
 * @revision:  none 
 *
 * @version    0.0.01
 * @author:    Zenki (zhajun), zenki2001cn@163.com
 * @date:      2012-7-23 下午03:53:24 
 */

package com.foxconn.edu_bbs.service;

import com.foxconn.edu_bbs.utils.TransChannelClient;
import com.foxconn.edu_bbs.utils.TransChannelServer;
import com.foxconn.edu_bbs.utils.TransConfig;
import com.foxconn.edu_bbs.utils.TransProtocol;
import com.foxconn.edu_bbs.utils.ZipHelper;

import android.app.Service;
import android.content.Intent;
import android.os.IBinder;
import android.os.Looper;
import android.os.RemoteException;
import android.util.Log;

/**
 *
 */
public class TransService extends Service {

	static private final String TAG = "TransService";
	
	private ITransService mBinder;
	private TransConfig mConfig = TransConfig.getInstance();
	
	@Override
	public void onCreate() {
		mBinder = createBinder();
		
		// TransService默认启动接收服务，用于监听来自其他设备的控制命令
		if (mBinder != null) {
			try {
				mBinder.runServer();
			} catch (RemoteException e) {
				e.printStackTrace();
			}
		}
		
		super.onCreate();
	}
	
	@Override
	public void onDestroy() {
		if (mBinder != null) {
			try {
				mBinder.stopServer();
			} catch (RemoteException e) {
				e.printStackTrace();
			}
			mBinder = null;
		}
		
		super.onDestroy();
	}
	
	@Override
	public IBinder onBind(Intent intent) {
		if (null == mBinder) {
			mBinder = createBinder();
		}
		
		return (IBinder) mBinder;
	}
	
	private ITransService createBinder() {
		ITransService binder = new ITransService.Stub() {
			
			private TransChannelServer mServer;
			
			@Override
			public boolean transData(String orgFile) throws RemoteException {
				new TransProtocol().transData(orgFile);
				
				return true;
			}
			
			@Override
			public String queryStatus(String arg) throws RemoteException {
				final String[] status = new String[]{"1"};
				
				TransChannelClient client = new TransChannelClient(mConfig.getServerIp(),
						TransConfig.COMMAND_PORT);
				TransProtocol command = new TransProtocol(
						TransProtocol.COMMAND_QUERY_STATUS, arg);
				command.setListener(new ITransCallback() {
					@Override
					public void action(String result) {
						Log.e(TAG, "!!!!! queryStatus result = " + result);
						
						int num = Integer.valueOf(result.split("\r\n")[0].split(" ")[1]);
						String temp = "";
						
						// 根据返回的在线设备数，组合成8位的字串，如"11110000"
						for (int i = 0, j = num; i < 8; i++, j--) {
							if (j > 0) {
								temp += "1";
							} else {
								temp += "0";
							}
						}
						
						status[0] = temp;
					}
				});

				client.transformCommand(command);
				client.waitRespond();

				client.stop();
				
				return status[0];
			}

			@Override
			public boolean runServer() throws RemoteException {
				new Thread() {
					public void run() {
						Looper.prepare();
						
						mServer = new TransChannelServer(
								mConfig.getServerIp(),
								TransConfig.COMMAND_PORT);
						mServer.startServer(null);
						
						Looper.loop();
					};
				}.start();
				
				return true;
			}

			@Override
			public boolean stopServer() throws RemoteException {
				if (mServer != null) {
					mServer.stop();
					mServer = null;
				}
				
				return true;
			}

			@Override
			public boolean zipFile(String srcDir, String desFile)
					throws RemoteException {
				try {
					new ZipHelper().zip(srcDir, desFile);
				} catch (Exception e) {
					e.printStackTrace();
				}
				
				return true;
			}

			@Override
			public boolean unzipFile(String srcFile, String desDir)
					throws RemoteException {
				try {
					new ZipHelper().unZip(srcFile, desDir);
				} catch (Exception e) {
					e.printStackTrace();
				}
				
				return true;
			}
		};
		
		return binder;
	}
}
