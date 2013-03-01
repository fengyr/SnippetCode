/**
 * @file       MailSelector.java
 *
 * @revision:  none 
 *
 * @version    0.0.01
 * @author:    Zenki (zhajun), zenki2001cn@163.com
 * @date:      2012-3-2 上午08:11:17 
 */

package com.foxconn.supernote.email.activity;

import android.app.Activity;
import android.content.Context;
import android.content.Intent;
import android.content.SharedPreferences;
import android.content.SharedPreferences.Editor;
import android.os.Bundle;
import android.util.Log;
import android.view.View;
import android.widget.Button;
import android.widget.LinearLayout;

import com.foxconn.supernote.email.R;

/**
 * 郵件選擇類，判斷用戶是否第一次使用郵箱，若不是使用最近一次使用的郵箱登陸
 */
public class MailSelector extends Activity{
	
	//日誌標示
	private static final String TAG = "MailSelector";
	
	//取消
	private Button mBtnCancel;
	//郵箱按鈕
	private LinearLayout mEmailGoogle;
	private LinearLayout mEmailNetease;
	private LinearLayout mEmailSina;
	
	private SharedPreferences mSharPrefe;
	
	// 1--gmail; 2--163; 3--sina
	private static int mType = 0;
	
	protected void onCreate(Bundle savedInstanceState)
	{
	    super.onCreate(savedInstanceState);
		Log.i(TAG, "OnCreate Begin");
		//判斷是否第一次訪問,讀取數據
		mSharPrefe = getSharedPreferences("SupernoteEMail", Context.MODE_PRIVATE);
		//第一次登陸為0
		mType = mSharPrefe.getInt("type", 0);

		//不顯示屏幕頂端的標題
		//requestWindowFeature(Window.FEATURE_NO_TITLE);
	
		setContentView(R.layout.main);
		mEmailGoogle = (LinearLayout) findViewById(R.id.buttongoogle);
		mEmailGoogle.setOnClickListener(btnGmailOnClick);
		
		mEmailNetease = (LinearLayout) findViewById(R.id.buttonnetease);
		mEmailNetease.setOnClickListener(btnMeteaseOnClick);
		
		mEmailSina = (LinearLayout) findViewById(R.id.buttonsina);
		mEmailSina.setOnClickListener(btnSinaOnClick);
		
		mBtnCancel = (Button) findViewById(R.id.mCancel);
		mBtnCancel.setOnClickListener(btnCancel);
		
		//非第一次登陸
		if(mType != 0)
		{
			enterLoginEmail();
		}
		
		//檢查網絡
		
	}//end onCreate
	
	/**
	 * 進入郵件登陸介面
	 */
	private void enterLoginEmail()
	{   
		Intent intent = new Intent(MailSelector.this, MailRecvContainer.class);
		Editor editor = mSharPrefe.edit();
		editor.putInt("type", mType);
		editor.commit();
		
		Bundle bundle = new Bundle();
		bundle.putInt("type", mType);
		intent.putExtras(bundle);
		startActivity(intent);
	}
	
	private final LinearLayout.OnClickListener btnGmailOnClick = new LinearLayout.OnClickListener()
	{
		public void onClick(View v) 
		{
			Log.i(TAG, "btnGmailOnClick onClick Begin");
			//mEmailGoogle.setBackgroundDrawable(getResources().getDrawable(R.drawable.button01_hl));
			mType = 1;
			enterLoginEmail();
		}
	};
	
	private final LinearLayout.OnClickListener btnMeteaseOnClick = new LinearLayout.OnClickListener()
	{
		public void onClick(View v) 
		{
			Log.i(TAG, "btnMeteaseOnClick onClick Begin");
			//mEmailNetease.setBackgroundDrawable(getResources().getDrawable(R.drawable.button01_hl));
			mType = 2;
			enterLoginEmail();
			
		}
	};
	
	private final LinearLayout.OnClickListener btnSinaOnClick = new LinearLayout.OnClickListener()
	{
		public void onClick(View v) 
		{
			Log.i(TAG, "btnSinaOnClick onClick Begin");
			//mEmailSina.setBackgroundDrawable(getResources().getDrawable(R.drawable.button01_hl));
			mType = 3;
			enterLoginEmail();
		}
	};
	
	private final Button.OnClickListener btnCancel = new Button.OnClickListener()
	{
		public void onClick(View v) 
		{
			Log.i(TAG, "btnCancel finish()");
			finish();
		}
	};
	
}
