/**
 * @file       MyWidgetTest.java
 *
 * @revision:  none 
 *
 * @version    0.0.01
 * @author:    Zenki (zhajun), junzha@pateo.com.cn
 * @date:      2013-3-14 下午04:51:08 
 */

package com.zenki.bitmapdraw.test;

import com.zenki.bitmapdraw.R;
import com.zenki.bitmapdraw.TestBitmapDraw;
import com.zenki.bitmapdraw.TestPaintView;

import android.app.Activity;
import android.test.ActivityInstrumentationTestCase2;

/**
 * 获取Activity中控件的对象，并测试控件的方法
 */
public class MyWidgetTest extends ActivityInstrumentationTestCase2<TestBitmapDraw> {

	private Activity mActivity;
	private TestPaintView mTestPaintView;
	
	/**
	 * @param activityClass
	 */
	public MyWidgetTest() {
		super(TestBitmapDraw.class);
	}

	@Override
	protected void setUp() throws Exception {
		super.setUp();
		
		mActivity = getActivity();
		mTestPaintView = (TestPaintView) mActivity.findViewById(R.id.paint);
	}
	
	public void testPaintViewtouchStart() {
		mActivity.runOnUiThread(new Runnable() {
			
			@Override
			public void run() {
				mTestPaintView.saveBitmap();
			}
		});
		
		getInstrumentation().waitForIdleSync();
	}
}
