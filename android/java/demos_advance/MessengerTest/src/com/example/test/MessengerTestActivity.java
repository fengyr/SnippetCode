package com.example.test;

import android.app.Activity;
import android.content.ComponentName;
import android.content.Context;
import android.content.Intent;
import android.content.ServiceConnection;
import android.os.Bundle;
import android.os.Handler;
import android.os.IBinder;
import android.os.Message;
import android.os.Messenger;
import android.os.RemoteException;
import android.util.Log;
import android.view.View;
import android.view.View.OnClickListener;
import android.widget.Button;
import android.widget.Toast;

public class MessengerTestActivity extends Activity {
	
	static private final String TAG = "MessengerTestActivity";
	private Button mButton;
	
	static public final int MSG_REPLY_HELLO = 1;
	
	private Handler mHandler = new Handler() {
		@Override
		public void handleMessage(Message msg) {
			switch (msg.what) {
			case MSG_REPLY_HELLO:
				Toast.makeText(MessengerTestActivity.this, "hello world yeah !!!!", Toast.LENGTH_SHORT).show();
				break;

			default:
				break;
			}
			super.handleMessage(msg);
		}
	};
	
    /** Called when the activity is first created. */
    @Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.main);
        
        mButton = (Button) findViewById(R.id.id_button_hello);
        mButton.setOnClickListener(new OnClickListener() {
			
			@Override
			public void onClick(View arg0) {
				sendMessage();
			}
		});
        
        bindService(new Intent(IMessengerTest.class.getName()),
                mConnection, Context.BIND_AUTO_CREATE);
    }
    
    private Messenger mServerMessenger;
    
    private ServiceConnection mConnection = new ServiceConnection() {
		
		@Override
		public void onServiceDisconnected(ComponentName arg0) {
			
		}
		
		@Override
		public void onServiceConnected(ComponentName arg0, IBinder arg1) {
			// 通过BindService的方式获取到远端Messenger的对象
			mServerMessenger = new Messenger(arg1);
		}
	};
	
	private void sendMessage() {
		// 通过Messenger发送消息给远端，同时将自己的Messenger传递给远端，以便远端可以回复
	    Message message = Message.obtain(null, MessengerService.MSG_SAY_HELLO);
	    message.replyTo = new Messenger(mHandler);
	    
	    try {
	    	Log.d(TAG, "sendMessage MSG_SAY_HELLO");
	    	mServerMessenger.send(message);    
	    } catch (RemoteException e) {    
	        e.printStackTrace();    
	    }    
	} 
}