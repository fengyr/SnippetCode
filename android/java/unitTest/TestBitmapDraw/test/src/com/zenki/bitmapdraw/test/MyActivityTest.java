/**
 * @file       MyActivityTest.java
 *
 * @revision:  none 
 *
 * @version    0.0.01
 * @author:    Zenki (zhajun), zenki2001cn@163.com
 * @date:      2013-1-8 上午11:45:06 
 */

package com.zenki.bitmapdraw.test;

import com.zenki.bitmapdraw.TestBitmapDraw;

import android.content.Intent;
import android.test.ActivityUnitTestCase;
import android.test.suitebuilder.annotation.MediumTest;
import android.util.Log;

/**
 *
 */
public class MyActivityTest extends ActivityUnitTestCase<TestBitmapDraw> {

	static private final String TAG = "MyActivityTest";
	
	private Intent mStartIntent;
	
//	 默认的构造方法会出错
//	public MyActivityTest(Class<TestBitmapDraw> activityClass) {
//		super(activityClass);
//	}
	
	public MyActivityTest() {
		super(TestBitmapDraw.class);
	}
	
	@Override  
	protected void setUp() throws Exception {  
		super.setUp();  
		mStartIntent = new Intent(Intent.ACTION_MAIN);  
	}

	@MediumTest
	public void testActivitySetUp() {
		Log.d(TAG, "testActivitySetUp");
	}
	
	public void testOnStart() {
		TestBitmapDraw activity = startActivity(mStartIntent, null, null);
		getInstrumentation().callActivityOnStart(activity);
		getInstrumentation().callActivityOnResume(activity);  
		getInstrumentation().callActivityOnPause(activity);  
		getInstrumentation().callActivityOnStop(activity); 
	}
	
	public void testOnResume() {
		TestBitmapDraw activity = startActivity(mStartIntent, null, null);
		getInstrumentation().callActivityOnResume(activity);
	}
}
