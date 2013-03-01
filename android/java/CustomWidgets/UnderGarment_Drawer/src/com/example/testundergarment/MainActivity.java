package com.example.testundergarment;

import android.os.Bundle;
import android.app.Activity;
import android.graphics.Color;
import android.util.Log;
import android.view.Menu;
import android.view.View;
import android.view.View.OnClickListener;
import android.widget.Button;

public class MainActivity extends Activity {
	static private final String TAG = "MainActivity";
	
	DrawerGarment mDrawerGarment;
	Button mButton;
	
	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setContentView(R.layout.activity_main);
		
		mDrawerGarment = new DrawerGarment(this, R.layout.dashboard);
		mDrawerGarment.setSlideTarget(DrawerGarment.SLIDE_TARGET_CONTENT);
		mDrawerGarment.setDecorContentBackgroundColor(Color.WHITE);
		mDrawerGarment.reconfigureViewHierarchy();
		mDrawerGarment.setDrawerEnabled(true);
		mDrawerGarment.setDrawerCallbacks(new DrawerGarment.IDrawerCallbacks() {
            @Override
            public void onDrawerOpened() {
            	Log.d(TAG, "onDrawerOpened");
            }

            @Override
            public void onDrawerClosed() {
            	Log.d(TAG, "onDrawerClosed");
            }
        });
		
		mButton = (Button) findViewById(R.id.button);
		mButton.setOnClickListener(new OnClickListener() {
			
			@Override
			public void onClick(View v) {
				mDrawerGarment.openDrawer(mDrawerGarment.isDrawerOpened() ? false : true);
			}
		});
	}
	
	@Override
	public boolean onCreateOptionsMenu(Menu menu) {
		// Inflate the menu; this adds items to the action bar if it is present.
		getMenuInflater().inflate(R.menu.main, menu);
		return true;
	}

}
