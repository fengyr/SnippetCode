/**
 * @file       TestBitmapDrawApplication.java
 *
 * @revision:  none 
 *
 * @version    0.0.01
 * @author:    Zenki (zhajun), zenki2001cn@163.com
 * @date:      2013-1-8 上午10:01:26 
 */

package com.zenki.bitmapdraw;

import android.app.Application;
import android.util.Log;

/**
 *
 */
public class TestBitmapDrawApplication extends Application {
	static private final String TAG = "TestBitmapDrawApplication";
	
	/**
	 * 
	 */
	public TestBitmapDrawApplication() {
		Log.d(TAG, testGetApplicationName());
	}
	
	public String testGetApplicationName() {
		return "My Name is TestBitmapDrawApplication edition 2";
	}
}
