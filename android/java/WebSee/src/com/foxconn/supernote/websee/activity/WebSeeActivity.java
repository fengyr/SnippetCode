package com.foxconn.supernote.websee.activity;

import android.app.Activity;
import android.graphics.Bitmap;
import android.net.http.SslError;
import android.os.Bundle;
import android.util.Log;
import android.view.KeyEvent;
import android.view.View;
import android.webkit.SslErrorHandler;
import android.webkit.WebView;
import android.webkit.WebViewClient;
import android.widget.Button;
import android.widget.EditText;
import android.widget.Toast;

import com.foxconn.supernote.websee.R;
import com.foxconn.supernote.websee.util.ICapturer;
import com.foxconn.supernote.websee.util.ISaveFile;
import com.foxconn.supernote.websee.util.SaveWebFile;
import com.foxconn.supernote.websee.util.ScreenCapture;
import com.foxconn.supernote.websee.widget.WebBrowserView;

public class WebSeeActivity extends Activity 
{
	private static final String TAG = "WebSeeActivity";
	public static final String WEB_SAVEPNGPATH = "/sdcard/SuperNote/Web/Png/";
	public static final String WEB_SAVEHTMLPATH = "/sdcard/SuperNote/Web/Html/";
	//取消
	private Button mBtnCancel;
	
	private Button mExportHtml;
	private Button mExportPng;
	private Button mGoUrl;
	
	private EditText mUrlAddr;
	private ICapturer mCapturer;
	private ISaveFile mSaveFile;
	private WebBrowserView mWebView;
	
    /** Called when the activity is first created. */
    @Override
    public void onCreate(Bundle savedInstanceState) 
    {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.main);
        
        mBtnCancel = (Button) findViewById(R.id.mLogout);
		mBtnCancel.setOnClickListener(btnCancel);
		
		mExportHtml = (Button) findViewById(R.id.mExhtml);
		mExportHtml.setOnClickListener(btnExportHtml);
		
		mExportPng = (Button) findViewById(R.id.mExpng);
		mExportPng.setOnClickListener(btnExportPng);
		
		mGoUrl = (Button) findViewById(R.id.mGo);
		mGoUrl.setOnClickListener(btnGoUrl);
		
		mUrlAddr = (EditText)findViewById(R.id.mEditUrl);
		mWebView = (WebBrowserView)findViewById(R.id.WebViewContent);
		mWebView.setWebViewClient(new WebBrowserViewClient());
		mWebView.setOnKeyListener(viewOnKeyListener);
		
		mCapturer = new ScreenCapture(mWebView);
		mSaveFile = new SaveWebFile();
    }
    
    private final Button.OnClickListener btnExportHtml = new Button.OnClickListener()
	{
		public void onClick(View v) 
		{
			String str = mWebView.getUrl();
			Log.i(TAG," web url: " + str);
			if(null == str || str.length() == 0)
			{
				Toast.makeText(WebSeeActivity.this,
						R.string.mess_geturlfail,
 						Toast.LENGTH_SHORT).show();
				return;
			}
			else
			{
				String filename = mSaveFile.saveHtml(str, WEB_SAVEHTMLPATH);
				if(null == filename || filename.length() == 0)
				{
					Toast.makeText(WebSeeActivity.this, 
							R.string.mess_screencapturefail,
	 						Toast.LENGTH_SHORT).show();
					return;
				}
				else
				{
				    String mess = WEB_SAVEHTMLPATH + filename;
				    Log.i(TAG,"save html path : " + mess);
					Toast.makeText(WebSeeActivity.this, 
							getString(R.string.mess_screencapturesuccess) + mess,
	 						Toast.LENGTH_SHORT).show();
					return;
				}
				
			}
		}
	};
	
    private final Button.OnClickListener btnExportPng = new Button.OnClickListener()
	{
		public void onClick(View v) 
		{
			String fileName = mCapturer.captureSave(WEB_SAVEPNGPATH);
			if(null == fileName || fileName.length() == 0)
			{
				Toast.makeText(WebSeeActivity.this, 
						R.string.mess_screencapturefail,
 						Toast.LENGTH_SHORT).show();
				return;
			}
			else
			{
				String mess = WEB_SAVEPNGPATH + fileName;
				Log.i(TAG,"save png path : " + mess);
				Toast.makeText(WebSeeActivity.this, 
						getString(R.string.mess_screencapturesuccess) + mess,
 						Toast.LENGTH_SHORT).show();
				return;
			}
		}
	};
	
	private final Button.OnClickListener btnGoUrl = new Button.OnClickListener()
	{
		public void onClick(View v) 
		{
			String str = mUrlAddr.getText().toString();
			Log.i(TAG,"url address: " + str);
			if(null == str || str.length() == 0)
			{
				Toast.makeText(WebSeeActivity.this, R.string.mess_urlisnull,
 						Toast.LENGTH_SHORT).show();
			}
			else
			{
				if(!str.startsWith("http://"))
				{
					str = "http://" + str;
				}
				Log.i(TAG,"locdurl url: " + str);
				mWebView.loadUrl(str);
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
	
    public boolean onKeyDown(int keyCode, KeyEvent event) 
    {
    	Log.i(TAG, "onKeyDown keyCode = " + keyCode 
    		 + "; event.getAction() = " + event.getAction());
    	//硬件返回鍵，返回網頁的上一個網頁
		if(event.getAction() == KeyEvent.ACTION_DOWN 
		  && keyCode == KeyEvent.KEYCODE_BACK
		  && mWebView.canGoBack())
		{
			mWebView.goBack();
			return true;
		}
		return false;
    	
    };
	
	private final View.OnKeyListener  viewOnKeyListener = new View.OnKeyListener() 
	{
		@Override
		public boolean onKey(View v, int keyCode, KeyEvent event) 
		{
			Log.i(TAG, "viewOnKeyListener onKey");
			//硬件返回鍵，返回網頁的上一個網頁
			if(event.getAction() == KeyEvent.ACTION_DOWN 
			  && keyCode == KeyEvent.KEYCODE_BACK
			  && mWebView.canGoBack())
			{
				mWebView.goBack();
				return true;
			}
			return false;
		}
		
	};
	
	// [Begin]add by jerry.wei
	private class WebBrowserViewClient extends WebViewClient {
		@Override
		public boolean shouldOverrideUrlLoading(WebView view, String url) {
			Log.d(TAG,"onReceivedSslError----");
			view.loadUrl(url);
			mUrlAddr.setText(url);
			return true;
		}
		
		@Override
		public void onReceivedSslError(WebView view, SslErrorHandler handler,
				SslError error) {
			Log.d(TAG,"onReceivedSslError----");
			handler.proceed();
		}
		
		@Override
		public void onPageStarted(WebView view, String url, Bitmap favicon){
			mUrlAddr.setText(url);
		}
		
		@Override
		public void onPageFinished(WebView view, String url){
			mUrlAddr.setText(url);
		}
		
	}
	// [End]add by jerry.wei

}