package com.foxconn.supernote.email.widget;

import android.content.Context;
import android.net.http.SslError;
import android.util.AttributeSet;
import android.util.Log;
import android.view.MotionEvent;
import android.webkit.SslErrorHandler;
import android.webkit.WebSettings;
import android.webkit.WebView;
import android.webkit.WebViewClient;
import android.webkit.WebSettings.LayoutAlgorithm;

public class MailView extends WebView {

	public static final String TAG = "MailView";
	
	public MailView(Context context) {
		super(context);
		this.setMailView();
	}
	
	public MailView(Context context, AttributeSet attrs){
		super(context, attrs);
		this.setMailView();
	}
	
	@Override
	public boolean onTouchEvent(MotionEvent ev) {
		setVerticalScrollBarEnabled(true);
		setHorizontalScrollBarEnabled(true);
		return super.onTouchEvent(ev);
	}
	
	public void flingScroll(int vx, int vy) {
	}
	
	public boolean performLongClick(){
		return false;
	}
	
	protected void onScrollChanged(int l, int t, int oldl, int oldt) {
	}
	
	private void setMailView(){
		this.setWebViewClient(new MailWebViewClient());
        //SET SCRIPT
		this.getSettings().setJavaScriptEnabled(true);
		this.getSettings().setSupportZoom(true);
		this.getSettings().setDefaultZoom(WebSettings.ZoomDensity.FAR);
		this.getSettings().setBuiltInZoomControls(true);
		this.getSettings().setLayoutAlgorithm(LayoutAlgorithm.NORMAL);
		this.getSettings().setLoadsImagesAutomatically(true);
		this.getSettings().setPluginsEnabled(true);
		this.getSettings().setAllowFileAccess(true);
		this.getSettings().setAppCacheEnabled(true); 
		this.getSettings().setDomStorageEnabled(true);
	}
	
	private class MailWebViewClient extends WebViewClient {
		@Override
		public boolean shouldOverrideUrlLoading(WebView view, String url) {
			Log.d(TAG,"onReceivedSslError----");
			view.loadUrl(url);
			return true;
		}
		
		@Override
		public void onReceivedSslError(WebView view, SslErrorHandler handler,
				SslError error) {
			Log.d(TAG,"onReceivedSslError----");
			handler.proceed();
		}
		
	}
}
