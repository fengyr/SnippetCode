/**
 * @file       MyFragmentAdapter.java
 *
 * @revision:  none 
 *
 * @version    0.0.01
 * @author:    Zenki (zhajun), junzha@pateo.com.cn
 * @date:      2013-2-27 下午03:18:25 
 */

package com.example.viewpagetest;

import android.support.v4.app.Fragment;
import android.support.v4.app.FragmentManager;
import android.support.v4.app.FragmentPagerAdapter;

/**
 *
 */
public class MyFragmentAdapter extends FragmentPagerAdapter {

	String[] mPageTitles = new String[]{"Page1", "Page2", "Page3"};
	
	/**
	 * @param fm
	 */
	public MyFragmentAdapter(FragmentManager fm) {
		super(fm);
	}

	@Override
	public Fragment getItem(int arg0) {
		// 该处直接创建一个MyFragment对象，实际应该创建一个单例对象
		return new MyFragment();
	}

	@Override
	public int getCount() {
		// 返回Page的总数
		return 3;
	}
	
	@Override
	public CharSequence getPageTitle(int position) {
		return mPageTitles[position];
	}
}
