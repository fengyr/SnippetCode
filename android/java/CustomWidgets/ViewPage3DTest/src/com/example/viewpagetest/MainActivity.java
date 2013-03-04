package com.example.viewpagetest;

import de.inovex.android.widgets.ViewPager3D;
import android.os.Bundle;
import android.support.v4.app.FragmentActivity;
import android.support.v4.view.PagerAdapter;
import android.view.Menu;

public class MainActivity extends FragmentActivity {

	ViewPager3D mPager;
	PagerAdapter mAdapter;
	
	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setContentView(R.layout.activity_main);
		
		mAdapter = new MyFragmentAdapter(getSupportFragmentManager());

        mPager = (ViewPager3D)findViewById(R.id.pager);
        mPager.setAdapter(mAdapter);
	}

	@Override
	public boolean onCreateOptionsMenu(Menu menu) {
		// Inflate the menu; this adds items to the action bar if it is present.
		getMenuInflater().inflate(R.menu.main, menu);
		return true;
	}

}
