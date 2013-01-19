/**
 * @file       MyApplicationTest.java
 *
 * @revision:  none 
 *
 * @version    0.0.01
 * @author:    Zenki (zhajun), zenki2001cn@163.com
 * @date:      2013-1-8 上午10:05:52 
 */

package com.zenki.bitmapdraw.test;

import com.zenki.bitmapdraw.TestBitmapDrawApplication;

import android.test.ApplicationTestCase;
import android.test.suitebuilder.annotation.MediumTest;
import android.util.Log;

/**
 *
 */
public class MyApplicationTest extends ApplicationTestCase<TestBitmapDrawApplication>{

	static private final String TAG = "MyApplicationTest";
	
	/**
	 * @param applicationClass
	 */
//	public MyApplicationTest(Class<TestBitmapDrawApplication> applicationClass) {
//		super(TestBitmapDrawApplication.class);
//	}
	
	public MyApplicationTest() {
		super(TestBitmapDrawApplication.class);
	}
	
	protected void setUp() throws Exception{
		super.setUp();
		
		Log.d(TAG, "MyApplicationTest setUp");
	}
	
	@MediumTest
	public void testApplicationCreate() {
		Log.d(TAG, "testApplicationCreate");
		createApplication();
	}
}
