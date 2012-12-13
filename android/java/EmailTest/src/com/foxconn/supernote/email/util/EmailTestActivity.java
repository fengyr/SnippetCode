package com.foxconn.supernote.email.util;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

import com.chilkatsoft.CkZip;

import android.app.Activity;
import android.app.ListActivity;
import android.graphics.drawable.BitmapDrawable;
import android.os.Bundle;
import android.util.Log;
import android.view.LayoutInflater;
import android.view.View;
import android.view.View.OnClickListener;
import android.widget.Button;
import android.widget.EditText;
import android.widget.ImageView;
import android.widget.SimpleAdapter;
import android.widget.Toast;

public class EmailTestActivity extends Activity {

	static private final String TAG = "EmailTestActivity";
	Button btn163;
	Button btnGmail;
	Button btnSina;
	AttachmentView attachView;
	ArrayList<String> mAttachments;
	View view;
	
	String reg = "href=\"(http://[\\w\\d\\/\\.*]+\\.css)\"";

//	static {
//	    try {
////	    	System.load("/sdcard/libchilkat.so");
//	    	System.loadLibrary("chilkat");
//	    	
//	    } catch (UnsatisfiedLinkError e) {
//	      System.err.println("Native code library failed to load.\n" + e);
//	    }
//	  }
	
	IMailNotify notify = new IMailNotify() {

		@Override
		public void onFinished(int code) {
			Log.d(TAG, "sendFinished");
			
			switch (code) {
			case 0:
				Toast.makeText(EmailTestActivity.this, "success",
						Toast.LENGTH_SHORT).show();
				break;
			case 1:
				Toast.makeText(EmailTestActivity.this, "failed",
						Toast.LENGTH_SHORT).show();
				break;
			case 2:
				Toast.makeText(EmailTestActivity.this, "timeout",
						Toast.LENGTH_SHORT).show();
				break;

			default:
				break;
			}
		}
	};
	
	/** Called when the activity is first created. */
	@Override
	public void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setContentView(R.layout.main);
		
		Log.d(TAG, "reg = " + reg);

		btnGmail = (Button) findViewById(R.id.gmail);
		btnGmail.setOnClickListener(new OnClickListener() {

			@Override
			public void onClick(View v) {
//				SendMailInfo info = new SendMailInfo();
//				info.strSendEmailAddr = "zenki2001cn@gmail.com";
//				info.strRecvEmailAddr = new ArrayList<String>();
//				info.strRecvEmailAddr.add("zenki2001cn@163.com");
//				info.strSendUserName = "zenki2001cn";
//				info.strSendPassword = "008zenki";
//				info.strSmtp = "smtp.gmail.com";
//				info.nSmtpPort = 465;
//				info.strPop3 = "pop3.163.com";
//				info.nPop3Port = 995;
//				info.strSubject = "hello, test javamail";
//				info.strTextContent = "hello zenki2001cn";
//				
//				ArrayList<String> attachs = new ArrayList<String>();
//				attachs.add("/sdcard/a.jpg");
//				attachs.add("/sdcard/b.jpg");
//				attachs.add("/sdcard/c.jpg");
//				info.listAttachments = attachs;
//				
//				IMailSender sender = new JMailEngine(notify);
//				sender.sendMail(info);
				
//				CkZip zip = new CkZip();
//				boolean opened = zip.OpenFromWeb("http://www.sina.com.cn");
//				Log.d(TAG, "opened = " + opened);
			}
		});
		
		btnSina = (Button) findViewById(R.id.sina);
		btnSina.setOnClickListener(new OnClickListener() {

			@Override
			public void onClick(View v) {
				SendMailInfo info = new SendMailInfo();
				info.strSendEmailAddr = "zenki2001cn@sina.com.cn";
				info.strRecvEmailAddr = new ArrayList<String>();
				info.strRecvEmailAddr.add("zenki2001cn@163.com");
				info.strRecvEmailAddr.add("zenki2001cn@gmail.com");
				info.strSendUserName = "zenki2001cn";
				info.strSendPassword = "zenki007";
				info.strSmtp = "smtp.sina.com.cn";
				info.nSmtpPort = 25;
				info.strPop3 = "pop3.163.com";
				info.nPop3Port = 995;
				info.strSubject = "hello, test javamail";
				info.strTextContent = "hello zenki2001cn";
				
				ArrayList<String> attachs = new ArrayList<String>();
				attachs.add("/sdcard/a.jpg");
				attachs.add("/sdcard/b.jpg");
				attachs.add("/sdcard/c.jpg");
				info.listAttachments = attachs;
				
				IMailSender sender = new JMailEngine(notify);
				sender.sendMail(info);
			}
		});
		
		btn163 = (Button) findViewById(R.id.neteasy);
		btn163.setOnClickListener(new OnClickListener() {

			@Override
			public void onClick(View v) {
				SendMailInfo info = new SendMailInfo();
				info.strSendEmailAddr = "zenki2001cn@163.com";
				info.strRecvEmailAddr = new ArrayList<String>();
				info.strRecvEmailAddr.add("zenki2001cn@163.com");
				info.strSendUserName = "zenki2001cn";
				info.strSendPassword = "008zenki";
				info.strSmtp = "smtp.163.com";
				info.nSmtpPort = 25;
				info.strPop3 = "pop3.163.com";
				info.nPop3Port = 995;
				info.strSubject = "hello, test javamail";
				info.strTextContent = "hello zenki2001cn";
				
				ArrayList<String> attachs = new ArrayList<String>();
//				attachs.add("/sdcard/a.jpg");
//				attachs.add("/sdcard/b.jpg");
//				attachs.add("/sdcard/c.jpg");
				info.listAttachments = attachs;
				
				IMailSender sender = new JMailEngine(notify);
				sender.sendMail(info);
			}
		});
		
		attachView = (AttachmentView) findViewById(R.id.listView);
		mAttachments = new ArrayList<String>();
//		mAttachments.add("/sdcard/a.jpg");
//		mAttachments.add("/sdcard/a.jpg");
//		mAttachments.add("/sdcard/a.jpg");
		attachView.setAttachment(null);
	}
}