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

import java.io.File;
import java.util.ArrayList;
import java.util.Arrays;

import com.foxconn.supernote.email.R;
import com.foxconn.supernote.email.database.*;
import com.foxconn.supernote.email.util.IMailNotify;
import com.foxconn.supernote.email.util.IMailSender;
import com.foxconn.supernote.email.util.JMailEngine;
import com.foxconn.supernote.email.util.SendMailInfo;
import com.foxconn.supernote.email.widget.AttachmentView;

import android.app.Activity;
import android.content.Context;
import android.content.Intent;
import android.os.Bundle;
import android.text.Editable;
import android.text.Selection;
import android.text.format.Time;
import android.util.AttributeSet;
import android.util.Log;
import android.view.KeyEvent;
import android.view.Menu;
import android.view.MenuItem;
import android.view.View;
import android.widget.Button;
import android.widget.EditText;
import android.widget.LinearLayout;
import android.widget.TextView;
import android.widget.Toast;

/**
 *
 */
public class MailSendContainer extends Activity {
	
	private static final String TAG = "MailSendContainer";
	//發送信息
	private SendMailInfo mSendInfo;
	//用戶最近一次使用的郵箱
	private AccountInfo mLastUser;
	//查詢數據庫
	private IAccountManagerHelper mIAccMagHel;
	
	private TextView mSendFlag;
	
	private Button mBtnCancel;
	private Button mSure;
	//發件人地址
	private TextView mTextSendAddr;
	//發件時間
	private TextView mTextSendTime;
	Time time;
	//收件人地址
	private EditText mAcceptAddr;
	//標題
	private EditText mEditSub;
	//郵件內容
	private EditText mEditRe;
	//文件路徑+文件名
	private static String mFilePath;
	private AttachmentView mAttachView;
	//全本文件名列表
	ArrayList<String> attachListBook;
	//單本文件名列表
	ArrayList<String> attachListPaper;
	//傳輸文件的標誌：false--單本; true--全本
	private boolean mIsBookFlag = false;
	
	private LinearLayout mMenu;
	private LinearLayout mMenuBook;
	private LinearLayout mMenuSwithcUser;
	private LinearLayout mMenuPaper;
	
	protected void onCreate(Bundle savedInstanceState)
	{
		super.onCreate(savedInstanceState);
		Log.i(TAG, "OnCreate Begin");
		
		//AttachmentViewContent顯示內容設置
		Intent intent = this.getIntent();
		Bundle bundle = intent.getExtras();
		mFilePath = bundle.getString("filepath");
		Log.i(TAG, "FilePath = " + mFilePath);
		
		//不顯示屏幕頂端的標題
    	//requestWindowFeature(Window.FEATURE_NO_TITLE);
		setContentView(R.layout.mail_send);

		mAttachView = (AttachmentView) findViewById(R.id.AttachmentViewContent);
		//默認是單頁發送
		attachListPaper = new ArrayList<String>();
		Log.i(TAG, "onCreate attachList add, filePaht = " + mFilePath);
		attachListPaper.add(mFilePath);
		mAttachView.setAttachment(attachListPaper);
		
		mBtnCancel = (Button)findViewById(R.id.mCancel);
		mBtnCancel.setOnClickListener(btnCancel);
		
		mSure = (Button)findViewById(R.id.mSure);
		mSure.setOnClickListener(btnSure);
		
		mMenu = (LinearLayout) findViewById(R.id.mMenuBG);
		mMenu.setVisibility(View.GONE);
		mMenuBook = (LinearLayout) findViewById(R.id.mMenuBook);
		mMenuBook.setOnClickListener(MenuBookOnClick);
		mMenuSwithcUser = (LinearLayout) findViewById(R.id.mMenuSwitchUser);
		mMenuSwithcUser.setOnClickListener(MenuSwithcUserOnClick);
		mMenuPaper = (LinearLayout) findViewById(R.id.mMenuPaper);
		mMenuPaper.setOnClickListener(MenuPaperOnClick);
			
		//數據庫接口類
		mIAccMagHel = new InterViewSQLite(MailSendContainer.this);
		mLastUser = mIAccMagHel.getLastUser();
		mTextSendAddr = (TextView)findViewById(R.id.mSendAddr);
		mTextSendTime = (TextView)findViewById(R.id.mSendTime);
		time = new Time();
		
		mSendInfo = new SendMailInfo();
		mSendFlag = (TextView) findViewById(R.id.mSuccess);	
		
		
		//需要在數據庫中獲取發件人信息
		if(null == mLastUser)
		{
			Log.i(TAG, "There is no data");
			mTextSendAddr.setText("");
			mTextSendTime.setText("");
		}
		else
		{
			Log.i(TAG, "UserName = " + mLastUser.strUserName);
			mTextSendAddr.setText(mLastUser.strUserName + "@" + mLastUser.strMailAddress);
			time.setToNow();
			int month = time.month + 1;
	        mTextSendTime.setText(time.year + "-" + month + "-" 
		              + time.monthDay + " " + time.hour 
		              + ":" + time.minute + ":" + time.second);
		}
		
		mAcceptAddr = (EditText)findViewById(R.id.mEditAddr);
		mEditSub = (EditText)findViewById(R.id.mEditSub);
		mEditRe = (EditText)findViewById(R.id.mEditRe);
		// 使光标始终在最后位置  
		Editable etable = mEditRe.getText();
		Selection.setSelection(etable, etable.length());
		
	}
	
	private final LinearLayout.OnClickListener MenuBookOnClick = new LinearLayout.OnClickListener()
	{
		public void onClick(View v) 
		{
			Log.i(TAG, "MenuBookOnClick onClick Begin");
			String filePath = MailRecvContainer.SAVEPATH;
			File file = new File(filePath);
			if(mMenu.getVisibility()== View.VISIBLE)
        	{
				mMenu.setVisibility(View.GONE);
        	}
			if(file.isDirectory())
			{
				mIsBookFlag = true;
				attachListBook = new ArrayList<String>();
				String[] fileList = file.list();
				for(int i = 0; i < fileList.length; i++)
				{
					Log.i(TAG, "FilePath = " + filePath + fileList[i]);
					attachListBook.add(filePath + fileList[i]);
				}
				mAttachView.setAttachment(attachListBook);
			}
			else
			{
				return ;
			}
		}
	};
	
	private final LinearLayout.OnClickListener MenuSwithcUserOnClick = new LinearLayout.OnClickListener()
	{
		public void onClick(View v) 
		{
			Log.i(TAG, "MenuSwithcUserOnClick onClick Begin");
			if(mMenu.getVisibility()== View.VISIBLE)
        	{
				mMenu.setVisibility(View.GONE);
        	}
			Intent intent = new Intent(MailSendContainer.this, MailSettings.class);
			startActivityForResult(intent, 0);
		}
	};
	
	private final LinearLayout.OnClickListener MenuPaperOnClick = new LinearLayout.OnClickListener()
	{
		public void onClick(View v) 
		{
			Log.i(TAG, "MenuPaperOnClick onClick Begin");
			if(mMenu.getVisibility()== View.VISIBLE)
        	{
				mMenu.setVisibility(View.GONE);
        	}
			mIsBookFlag = false;
			attachListPaper = new ArrayList<String>();
			Log.i(TAG, " FilePath = " + mFilePath);
			attachListPaper.add(mFilePath);
			mAttachView.setAttachment(attachListPaper);
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
			//回調函數，發送成功后顯示success標誌的
			IMailNotify notify = new IMailNotify() 
			{
		 	    @Override
		 		public void onFinished(int code) 
		 	    {
		 			Log.d(TAG, "sendFinished");
		 			mSendFlag.setText("");
		 			switch (code) 
		 			{
					case 0:
						mSendFlag.setText(R.string.mess_sendsuccess);
		 				break;
		 			case 1:
		 				Log.d(TAG, "onFinished code 1");
						Toast.makeText(MailSendContainer.this, R.string.mess_sendfail,
		 						Toast.LENGTH_SHORT).show();
		 				break;
		 		    case 2:
		 				Toast.makeText(MailSendContainer.this, R.string.mess_sendtimeout,
		 						Toast.LENGTH_SHORT).show();
						break;
		 
		 			default:
		 				break;
		 			}
				 }

		 	    //3S中清除success顯示
				@Override
				public void onClean(int code) {
					// TODO Auto-generated method stub
					switch (code) 
		 			{
					case 0:
						mSendFlag.setText("");
		 				break;
		 				
					case 1:
						Log.d(TAG, "onClean code 1");
						break;
		 
		 			default:
		 				break;
		 			}
				}
			};

			//若發件者為null，則需要設置
			if(null == mLastUser)
			{
				Toast.makeText(MailSendContainer.this, 
						       R.string.mess_sendmail, 
						       Toast.LENGTH_SHORT).show();
				return;
			}
			else
			{
				//構造發件信息
				initSendInfo();	
				mSendFlag.setText(R.string.mess_sendingmail);
				
				//發送郵件
				IMailSender sender = new JMailEngine(notify);
				sender.sendMail(mSendInfo);
			}
			
		}
	};
	
	/**
	 * 需要查詢數據庫獲取發送信息的值并賦值給sendinfo 
	 */
	private void initSendInfo()
	{   
		ArrayList<String> mAccAddrList;
		
		String[] mArraySendAddr = (mAcceptAddr.getText().toString()).split(";");
		mAccAddrList =  new ArrayList<String>(Arrays.asList(mArraySendAddr));
		if(null == mAccAddrList || mAccAddrList.size() == 0)
		{
			Log.d(TAG, "initSendInfo there is no receiver info");
			Toast.makeText(MailSendContainer.this, R.string.mess_receivemail,
						Toast.LENGTH_SHORT).show();
			return;
		}
		
		mSendInfo.strRecvEmailAddr = mAccAddrList;
		Log.i(TAG, "receiver email address : ");
		for(int i = 0; i < mAccAddrList.size(); i++)
		{
			Log.i(TAG, ";  " + mAccAddrList.get(i));
		}
		mSendInfo.strSubject = mEditSub.getText().toString();
        mSendInfo.strTextContent = mEditRe.getText().toString();
        Log.i(TAG, "strSubject = " + mSendInfo.strSubject 
        	 + "; strTextContent = " + mSendInfo.strTextContent);
        
        mLastUser = mIAccMagHel.getLastUser();
        mSendInfo.nPop3Port = mLastUser.nPort2;
        mSendInfo.nSmtpPort = mLastUser.nPort1;
        mSendInfo.strPop3 = mLastUser.strPop3;
        mSendInfo.strSmtp = mLastUser.strSmtp;
        mSendInfo.strSendEmailAddr = mTextSendAddr.getText().toString();
        mSendInfo.strSendPassword = mLastUser.strPassword;
        mSendInfo.strSendUserName = mLastUser.strUserName;
        Log.i(TAG, "nPop3Port = " + mSendInfo.nPop3Port 
           	 + "; nSmtpPort = " + mSendInfo.nSmtpPort
           	 + "; strPop3 = " + mSendInfo.strPop3
           	 + "; strSmtp = " + mSendInfo.strSmtp
           	 + "; strSendEmailAddr = " + mSendInfo.strSendEmailAddr
           	 + "; strSendPassword = " + mSendInfo.strSendPassword
           	 + "; strSendUserName = " + mSendInfo.strSendUserName);
           	 
        Log.i(TAG, "mIsBookFlag = " + mIsBookFlag);
        //獲取發送的圖片的文件名
        //發送全本
        if(mIsBookFlag)
        {
        	mSendInfo.listAttachments = attachListBook;
        }
        //發送單本
        else
        {
        	mSendInfo.listAttachments = attachListPaper;
        }
        	
	}
	
	@Override
	public boolean onKeyDown(int keyCode, KeyEvent event) 
	{
		Log.v(TAG, "event.getKeyCode()= " + event.getKeyCode());
		switch(event.getKeyCode()) 
		{
        case KeyEvent.KEYCODE_MENU:
        	Log.v(TAG, "KeyEvent.KEYCODE_MENU");
        	if(mMenu.getVisibility()!= View.VISIBLE)
        	{
        		mMenu.setVisibility(View.VISIBLE);
        	}
        	else
        	{
        		mMenu.setVisibility(View.GONE);
        	}
            break;
        
		}
		return super.onKeyDown(keyCode, event);
	}
	
//	 public boolean onCreateOptionsMenu(Menu menu)
//	 {
//		 menu.add(0,Menu.FIRST, 0, R.string.menubook).setIcon(R.drawable.book_nor);    
//		 menu.add(0,Menu.FIRST+1, 1 ,R.string.menuswitchuser).setIcon(R.drawable.switch_nor);    
//		 menu.add(0,Menu.FIRST+2, 2, R.string.menupaper).setIcon(R.drawable.paper_nor);
//
//		 return true;
//	 }
	 
	 /** 
	 * 处理点击菜单 
	 *  
	 * @param item 
	 * @return 
     */  
	 public boolean onOptionsItemSelected(MenuItem item)
	 {    
		 switch(item.getItemId())
		 {
		 //全本
		 case Menu.FIRST:
			 String filePath = MailRecvContainer.SAVEPATH;
			 File file = new File(filePath); 
			 if(file.isDirectory())
			 {
				 mIsBookFlag = true;
				 attachListBook = new ArrayList<String>();
				 String[] fileList = file.list();
				 for(int i = 0; i < fileList.length; i++)
				 {
					 Log.i(TAG, "FilePath = " + filePath + fileList[i]);
					 attachListBook.add(filePath + fileList[i]);
				 }
				 mAttachView.setAttachment(attachListBook);
			 }
			 else
			 {
				 return false;
			 }
			 break;
		//帳戶設置	 
		 case Menu.FIRST + 1:    
			 Intent intent = new Intent(MailSendContainer.this, MailSettings.class);
			 //startActivity(intent);
			 startActivityForResult(intent, 0);
			 break;
		//單本	 
		 case Menu.FIRST + 2:   
			 mIsBookFlag = false;
			 attachListPaper = new ArrayList<String>();
			 Log.i(TAG, " FilePath = " + mFilePath);
			 attachListPaper.add(mFilePath);
			 mAttachView.setAttachment(attachListPaper);
			 
			 break;
	     }
		 return true;
	 }
	 /**
	  * startActivityForResult調用的Activity結束后調用該函數处理返回的数据 
	  */
	 protected void onActivityResult(int requestCode, int resultCode, Intent data) 
	 {
		 super.onActivityResult(requestCode, resultCode, data);
		 Log.i(TAG, "onActivityResult Begin, requestCode = " + requestCode
				 + "; resultCode = " + resultCode);
		 switch (resultCode) 
		 { 
		 case RESULT_OK:
			 Bundle bundle = data.getExtras();
			 String str = bundle.getString("sendUserAddr");
			 Log.i(TAG, "onActivityResult, sendUserAddr = " + str);
			 mLastUser = mIAccMagHel.getLastUser();
			 if(mLastUser != null)
			 {
				 mTextSendAddr.setText(mLastUser.strUserName + "@" + mLastUser.strMailAddress);
				 time.setToNow();
				 int month = time.month + 1;
			     mTextSendTime.setText(time.year + "-" + month + "-" 
				              + time.monthDay + " " + time.hour 
				              + ":" + time.minute + ":" + time.second);
			 }
			 else
			 {
				 mTextSendAddr.setText("");
			 }
			 break;
		 default:
		           break;
		 }
	}
	 
	 public class BottomMenuView extends View
	 {
	 	public BottomMenuView(Context context,AttributeSet paramAttributeSet) 
	 	{
	 		super(context, paramAttributeSet);
	 		Log.i(TAG, "BottomMenuView Begin");
	 	}
	 }
}


