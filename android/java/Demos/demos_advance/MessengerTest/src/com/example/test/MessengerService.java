/**
 * @file       MessengerService.java
 *
 * @revision:  none 
 *
 * @version    0.0.01
 * @author:    Zenki (zhajun), zenki2001cn@163.com
 * @date:      2012-8-9 下午03:52:10 
 */

package com.example.test;

import android.app.Service;
import android.content.Intent;
import android.os.Handler;
import android.os.IBinder;
import android.os.Message;
import android.os.Messenger;
import android.os.RemoteException;
import android.util.Log;
import android.widget.Toast;

/**
 *
 */
public class MessengerService extends Service {

	static private final String TAG = "MessengerService";
	public static final int MSG_SAY_HELLO = 1;
	
	private final IMessengerTest.Stub mBinder = new IMessengerTest.Stub() {
		
		@Override
		public void sayHello() throws RemoteException {
			Toast.makeText(MessengerService.this, "hello world", Toast.LENGTH_SHORT).show();
		}
	};
	
	// 建立一个Handler来处理sayHello的功能，远程可通过Messenger来传递消息。
	class MessengerTestHandler extends Handler {
		@Override
		public void handleMessage(Message msg) {
			switch (msg.what) {
			case MSG_SAY_HELLO:
				Log.d(TAG, "handleMessage MSG_SAY_HELLO");
				
				try {
					mBinder.sayHello();
					
					// 回复客户端的消息
					Message message = Message.obtain(null, MessengerTestActivity.MSG_REPLY_HELLO);
					msg.replyTo.send(message);
				} catch (RemoteException e) {
					e.printStackTrace();
				}
				break;

			default:
				break;
			}
			super.handleMessage(msg);
		}
	}
	
	private MessengerTestHandler mMessengerHandler = new MessengerTestHandler();
	private Messenger mMessenger = new Messenger(mMessengerHandler);
	
	@Override
	public IBinder onBind(Intent arg0) {
		Log.d(TAG, "onBind intent = " + arg0);
		
		// 返回一个Messenger的IBinder
		return mMessenger.getBinder();
	}

}
