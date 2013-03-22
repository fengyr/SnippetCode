/**
 * @file       MyWidgetTest.java
 *
 * @revision:  none 
 *
 * @version    0.0.01
 * @author:    Zenki (zhajun), junzha@pateo.com.cn
 * @date:      2013-3-14 下午04:51:08 
 */

package com.example.commonwidgettest.test;

import com.example.commonwidgettest.MainActivity;
import com.example.commonwidgettest.R;
import com.pateo.as.button.AsButton;
import com.pateo.as.extendablelistview.AsExtendableListView;

import android.app.Activity;
import android.test.ActivityInstrumentationTestCase2;

/**
 * 获取Activity中控件的对象，并测试控件的方法
 */
public class CommonWidgetTest extends ActivityInstrumentationTestCase2<MainActivity> {

	private Activity mActivity;
	private AsExtendableListView mExtendableListView;
	private AsButton mAsButton;
	
	/**
	 * @param activityClass
	 */
	public CommonWidgetTest() {
		super(MainActivity.class);
	}

	@Override
	protected void setUp() throws Exception {
		super.setUp();
		
		mActivity = getActivity();
		mExtendableListView = (AsExtendableListView) mActivity.findViewById(R.id.id_ExpandableListView);
		mAsButton = (AsButton) mActivity.findViewById(R.id.asbutton);
	}
	
	public void testAsButtonClick() {
		mActivity.runOnUiThread(new Runnable() {
			
			@Override
			public void run() {
				mAsButton.performClick();
			}
		});
		
		getInstrumentation().waitForIdleSync();
	}
	
	public void testAsExtendableListViewClick() {
		mActivity.runOnUiThread(new Runnable() {
			
			@Override
			public void run() {
				mExtendableListView.performClick();
			}
		});
		
		getInstrumentation().waitForIdleSync();
	}
}
