/**
 * @file       MailSendContainer.java
 *
 * @revision:  none 
 *
 * @version    0.0.01
 * @author:    Zenki (zhajun), zenki2001cn@163.com
 * @date:      2012-3-2 上午08:14:40 
 */

package com.foxconn.supernote.email.activity;

import com.foxconn.supernote.email.R;
import com.foxconn.supernote.email.util.ICapturer;
import com.foxconn.supernote.email.util.ScreenCapture;
import com.foxconn.supernote.email.widget.MailView;

import android.app.Activity;
import android.content.Context;
import android.content.Intent;
import android.content.SharedPreferences;
import android.content.SharedPreferences.Editor;
import android.os.Bundle;
import android.util.Log;
import android.view.KeyEvent;
import android.view.View;
import android.widget.Button;
import android.widget.Toast;

/**
 *
 */
public class MailRecvContainer extends Activity {

	private static final String TAG = "MailRecvContainer";
	
	//三個郵箱的地址
	private static final String GOOGLEADDR = "http://gmail.com/";
	private static final String METEASEADDR = "http://email.163.com/";
	private static final String SINAADDR = "http://mail.sina.com.cn/";
	//截屏后文件保存地址
	public static final String SAVEPATH = "/sdcard/SuperNote/Email/";
	
	private static int mType = 0;
	
	//登出
	private Button mLogout;
	//截屏
	private Button mBtnCapture;
	private ICapturer mCapturer;
	private MailView mEmailView;

	protected void onCreate(Bundle savedInstanceState)
	{
		super.onCreate(savedInstanceState);
		Log.i(TAG, "OnCreate Begin");
		//不顯示屏幕頂端的標題
		//requestWindowFeature(Window.FEATURE_NO_TITLE);
		
		Intent intent = this.getIntent();
		Bundle bundle = intent.getExtras();
        mType = bundle.getInt("type");
        if(mType == 0 )
        {
        	finish();
        	return;
        }
        
        
        setContentView(R.layout.mail_recv);
        mEmailView = (MailView)findViewById(R.id.MailViewContent);
        //[Begin]Add by jerry
        mEmailView.setInitialScale(150);
        //[End]Add by jerry
        mEmailView.loadUrl(getEmailAddress(mType));
        mEmailView.setOnKeyListener(viewOnKeyListener);
        
        mLogout = (Button)findViewById(R.id.mLogout);
        mLogout.setOnClickListener(btnLogout);
        mBtnCapture = (Button)findViewById(R.id.mCapture);
		mBtnCapture.setOnClickListener(btnCapture);
	}
	
	private String getEmailAddress(int type)
	{
		switch(type)
		{
		case 1:
			return GOOGLEADDR;
			
		case 2:
			return METEASEADDR;
			
		case 3:
			return SINAADDR;
		default:
			return "";
		}
	}
	
	private final Button.OnClickListener btnCapture = new Button.OnClickListener()
	{
		public void onClick(View v) 
		{
			mCapturer = new ScreenCapture(mEmailView);
			String fileName = mCapturer.captureSave(SAVEPATH);
			if(null == fileName || fileName.length() == 0)
			{
				Toast.makeText(MailRecvContainer.this, R.string.mess_screencapture,
 						Toast.LENGTH_SHORT).show();
				return;
			}
			String filePath = SAVEPATH + fileName;
			Log.i(TAG, "btnCapture filePath = " + filePath);
			
			Intent intent = new Intent(MailRecvContainer.this, MailSendContainer.class);
			Bundle bundle = new Bundle();
			bundle.putString("filepath", filePath);
			intent.putExtras(bundle);
			startActivity(intent);
		}
	};
	
	private final View.OnKeyListener  viewOnKeyListener = new View.OnKeyListener() 
	{
		@Override
		public boolean onKey(View v, int keyCode, KeyEvent event) 
		{
			Log.i(TAG, "viewOnKeyListener onKey");
			//硬件返回鍵，返回網頁的上一個網頁
			//[Begin]Mod by jerry
			if(event.getAction() == KeyEvent.ACTION_DOWN 
			  && keyCode == KeyEvent.KEYCODE_BACK){
				if(mEmailView.canGoBack()){
					mEmailView.goBack();
				}
				return true;
			}
			//[End]Mod by jerry
			return false;
		}
		
	};
	
	private final Button.OnClickListener btnLogout = new Button.OnClickListener()
	{
		public void onClick(View v) 
		{
			//登出時將data/data/SupernoteEMail.xml文件中的值設置為0
			SharedPreferences mSharPrefe = 
				              getSharedPreferences("SupernoteEMail", Context.MODE_PRIVATE);
			Editor editor = mSharPrefe.edit();
			editor.putInt("type", 0);
			editor.commit();
			finish();
		}
	};
}
