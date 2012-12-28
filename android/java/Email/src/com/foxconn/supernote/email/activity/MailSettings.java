/**
 * @file       MailSettings.java
 *
 * @revision:  none 
 *
 * @version    0.0.01
 * @author:    Zenki (zhajun), zenki2001cn@163.com
 * @date:      2012-3-2 上午08:13:15 
 */

package com.foxconn.supernote.email.activity;

import java.util.ArrayList;

import com.foxconn.supernote.email.R;
import com.foxconn.supernote.email.database.AccountInfo;
import com.foxconn.supernote.email.database.IAccountManagerHelper;
import com.foxconn.supernote.email.database.InterViewSQLite;
import com.foxconn.supernote.email.widget.SettingsListAdapter;

import android.app.Activity;
import android.content.Intent;
import android.graphics.drawable.Drawable;
import android.os.Bundle;
import android.text.Editable;
import android.text.InputType;
import android.text.Selection;
import android.util.Log;
import android.view.View;
import android.widget.AdapterView;
import android.widget.AdapterView.OnItemClickListener;
import android.widget.Button;
import android.widget.EditText;
import android.widget.LinearLayout;
import android.widget.ListView;
import android.widget.TextView;
import android.widget.Toast;

/**
 * 郵件設置
 */
public class MailSettings extends Activity 
{
	
	//日誌標示
	private static final String TAG = "MailSettings";

    // 1--gmail; 2--163; 3--sina
	private int mType = 1;
    
	private Button mBtnCancel;
	private Button mSure;
	private Button mAddUser;
	private Button mSelectEmail;
	//用戶名
	private EditText mUserName;
	//密碼
	private EditText mUserPassword;
	//已保存帳戶列表
	private ListView mUserListView;
	private Drawable mSelectedDraw;
	
	private LinearLayout mLayoutEmail;
	private TextView mBtnEmailAddr;
	private Button mBtnGmailEmail;
	private Button mBtnNetEaseEmail;
	private Button mBtnSinaEmail;
	
	//查詢數據庫
	private static IAccountManagerHelper mIAccMagHel;
	//所有已經設置的用戶列表
	private ArrayList<AccountInfo> mAccountInfoList;
	private AccountInfo mSelectedInfo;
	
	public ListView getListView() 
	{
		return mUserListView;
	}
	
	protected void onCreate(Bundle savedInstanceState)
	{
		super.onCreate(savedInstanceState);
		Log.i(TAG, "OnCreate Begin");

		setContentView(R.layout.mail_settings);
		
		mIAccMagHel = new InterViewSQLite(MailSettings.this);
		mAccountInfoList = (ArrayList<AccountInfo>)mIAccMagHel.queryAllUsers() ;
		if(null == mAccountInfoList)
		{
			mAccountInfoList = new ArrayList<AccountInfo>();
		}
		else
		{
			for(int i = 0; i < mAccountInfoList.size(); i++)
			{
				Log.i(TAG, "mAccountInfoList[" + i + "] = " 
						+ mAccountInfoList.get(i).strUserName + "@"
						+ mAccountInfoList.get(i).strMailAddress);
			}
		}
		
		mBtnCancel = (Button)findViewById(R.id.mCancel);
		mBtnCancel.setOnClickListener(btnCancel);
		
		mSure = (Button)findViewById(R.id.mSure);
		mSure.setOnClickListener(btnSure);
		
		mAddUser = (Button)findViewById(R.id.mAddUser);
		mAddUser.setOnClickListener(btnAddUser);
		
		mLayoutEmail = (LinearLayout)findViewById(R.id.mEditEmailAddr);
		mLayoutEmail.setVisibility(View.INVISIBLE);
		
		mSelectEmail = (Button)findViewById(R.id.mbtSelectEmail);
		mSelectEmail.setOnClickListener(btnSelectEmail);
		
		mBtnEmailAddr = (TextView)findViewById(R.id.mEditEmailAddrDefaul);
		
		mBtnGmailEmail = (Button)findViewById(R.id.mEditGoogleEmail);
		mBtnGmailEmail.setOnClickListener(btnGmailEmail);
		
		mBtnNetEaseEmail = (Button)findViewById(R.id.mEditNeteaseEmail);
		mBtnNetEaseEmail.setOnClickListener(btnNetEaseEmail);
		
		mBtnSinaEmail = (Button)findViewById(R.id.mEditSinaEmail);
		mBtnSinaEmail.setOnClickListener(btnSinaEmail);
		
		mUserName = (EditText)findViewById(R.id.mEditUserName);
		mUserPassword = (EditText)findViewById(R.id.mEditPassword);
		//設置顯示類型
		mUserPassword.setInputType(InputType.TYPE_CLASS_TEXT 
				                  | InputType.TYPE_TEXT_VARIATION_PASSWORD);
		// 使光标始终在最后位置  
		Editable etable = mUserPassword.getText();
		Selection.setSelection(etable, etable.length());
		
		mUserListView = (ListView)findViewById(R.id.mUserList);
		mUserListView.setOnItemClickListener(listItemClick);
		mSelectedDraw = getResources().getDrawable(R.drawable.hl02);
		
		if(null == mAccountInfoList)
		{
			Log.i(TAG, "mAccountInfoList = null");
			mUserListView.setAdapter(null);
		}
		else
		{
			Log.i(TAG, "mAccountInfoList != null");
			
			SettingsListAdapter adapter = new SettingsListAdapter(this, mAccountInfoList);
			mUserListView.setAdapter(adapter);
		}
		
	}//end onCreate
	
	public static IAccountManagerHelper getIAccManHel()
	{
		return mIAccMagHel;
	}
	public void setIAccManHel(IAccountManagerHelper value)
	{
		mIAccMagHel = value;
	}
	
	private final ListView.OnItemClickListener listItemClick = new OnItemClickListener()
	{
		@Override
		public void onItemClick(AdapterView<?> arg0, View arg1, int arg2, long arg3) 
		{
			Log.i(TAG, "onItemClick Begin");		
			if(arg2 > mAccountInfoList.size() - 1)
			{
				return;
			}
			mSelectedInfo = mAccountInfoList.get(arg2);	
			SettingsListAdapter adapter = new SettingsListAdapter(MailSettings.this, mAccountInfoList);
			adapter.setSelectItem(arg2);
			mUserListView.setAdapter(adapter);
			
		}
	};
	
	private final Button.OnClickListener btnGmailEmail = new Button.OnClickListener()
	{
		public void onClick(View v) 
		{
			mBtnEmailAddr.setText(R.string.gmailaddress);
			mLayoutEmail.setVisibility(View.INVISIBLE);
			mType = 1;
		}
	};
	
	private final Button.OnClickListener btnNetEaseEmail = new Button.OnClickListener()
	{
		public void onClick(View v) 
		{
			mBtnEmailAddr.setText(R.string.neteaseaddress);
			mLayoutEmail.setVisibility(View.INVISIBLE);
			mType = 2;
		}
	};
	
	private final Button.OnClickListener btnSinaEmail = new Button.OnClickListener()
	{
		public void onClick(View v) 
		{
			mBtnEmailAddr.setText(R.string.sinaaddress);
			mLayoutEmail.setVisibility(View.INVISIBLE);
			mType = 3;
		}
	};
	
	private final Button.OnClickListener btnSelectEmail = new Button.OnClickListener()
	{
		public void onClick(View v) 
		{
			if(mLayoutEmail.getVisibility() == View.VISIBLE)
			{
				mLayoutEmail.setVisibility(View.INVISIBLE);
			}
			else
			{
				mLayoutEmail.setVisibility(View.VISIBLE);
			}
		}
	};
	
	private final Button.OnClickListener btnCancel = new Button.OnClickListener()
	{
		public void onClick(View v) 
		{
			finish();
		}
	};
	
	private final Button.OnClickListener btnSure = new Button.OnClickListener()
	{
		public void onClick(View v) 
		{
			Log.i(TAG, "btnSure onClick");
			if(null != mSelectedInfo)
			{
				Log.i(TAG, "mSelectedInfo != null");
				String str = mSelectedInfo.strUserName + "@" + mSelectedInfo.strMailAddress;
				Log.i(TAG, "btnSure sendadd = " + str);
				mSelectedInfo.nLastLogin = System.currentTimeMillis();
				mIAccMagHel.setLastUser(mSelectedInfo);
				//mIAccMagHel.addUser(mSelectedInfo);
				
				Intent intent = new Intent();
				Bundle bundle = new Bundle();
				bundle.putString("sendUserAddr", str);
				intent.putExtras(bundle);
				MailSettings.this.setResult(RESULT_OK,intent); 
			}
			else
			{
				Log.i(TAG, "mSelectedInfo == null");
			}
			finish();

		}
	};
	
	private final Button.OnClickListener btnAddUser = new Button.OnClickListener()
	{
		public void onClick(View v) 
		{
			Log.i(TAG," btnAddUser onClick Begin");
			if(null == mUserName.getText().toString()  
			  || mUserName.getText().toString().length() == 0
			  || null == mUserPassword.getText().toString()
			  || mUserPassword.getText().toString().length() == 0)
			{
				Toast.makeText(MailSettings.this, R.string.mess_settingsuserinfo,
 						Toast.LENGTH_SHORT).show();
				return;
			}
			AccountInfo mAccountInfo = new AccountInfo();
			mAccountInfo.strMailAddress = mBtnEmailAddr.getText().toString();
			mAccountInfo.nType = mType;
			mAccountInfo.strUserName = mUserName.getText().toString();
			mAccountInfo.strPassword = mUserPassword.getText().toString();
			mAccountInfo.nLastLogin = System.currentTimeMillis();
			Log.i(TAG, "add user info : strMailAddress = " 
				 +  mAccountInfo.strMailAddress
			     + "; nType = " + mAccountInfo.nType
			     + "; strUserName = " + mAccountInfo.strUserName
			     + "; strPassword = " + mAccountInfo.strPassword
			     + "; nLastLogin = " + mAccountInfo.nLastLogin);
			mIAccMagHel.addUser(mAccountInfo);
			mAccountInfoList = (ArrayList<AccountInfo>)mIAccMagHel.queryAllUsers() ;
			//mAccountInfoList.add(mAccountInfo);
			if(null == mAccountInfoList || mAccountInfoList.size() == 0)
			{
				mUserListView.setAdapter(null);
			}
			else
			{
				for(int i = 0; i < mAccountInfoList.size(); i++)
				{
					Log.i(TAG, "mAccountInfoList[" + i + "] = " 
							+ mAccountInfoList.get(i).strUserName + "@"
							+ mAccountInfoList.get(i).strMailAddress);
				}
				
				SettingsListAdapter adapter = new SettingsListAdapter(MailSettings.this, mAccountInfoList);
				mUserListView.setAdapter(adapter);
			}
		}
	};

}
