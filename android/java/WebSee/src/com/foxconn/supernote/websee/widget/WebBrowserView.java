package com.foxconn.supernote.websee.widget;

import android.content.Context;
import android.util.AttributeSet;
import android.view.MotionEvent;
import android.webkit.WebSettings;
import android.webkit.WebView;
import android.webkit.WebSettings.LayoutAlgorithm;

public class WebBrowserView extends WebView {

	public static final String TAG = "WebBrowserView";
	
	public WebBrowserView(Context context) {
		super(context);
		this.setWebBrowserView();
	}
	
	public WebBrowserView(Context context, AttributeSet attrs){
		super(context, attrs);
		this.setWebBrowserView();
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
	
	private void setWebBrowserView(){
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
}
