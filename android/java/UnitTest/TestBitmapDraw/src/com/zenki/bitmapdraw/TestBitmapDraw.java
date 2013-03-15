package com.zenki.bitmapdraw;

import android.app.Activity;
import android.os.Bundle;
import android.util.Log;
import android.view.LayoutInflater;
import android.view.View;
import android.view.View.OnClickListener;
import android.widget.Button;

public class TestBitmapDraw extends Activity {
    
	static private final String TAG = "TestBitmapDraw";
	
	private Button mBtnSave;
	private TestPaintView mPaintView;
	
    @Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.main);
        
        mPaintView = (TestPaintView) findViewById(R.id.paint);
        
        mBtnSave = (Button) findViewById(R.id.save);
        mBtnSave.setOnClickListener(new OnClickListener() {
			@Override
			public void onClick(View v) {
				mPaintView.displayAngle();
			}
		});
    }
    
    @Override
    protected void onResume() {
    	Log.d(TAG, "onResume");
    	super.onResume();
    }
    
    @Override
    protected void onRestart() {
    	Log.d(TAG, "onRestart");
    	super.onRestart();
    }
    
    @Override
    protected void onPause() {
    	Log.d(TAG, "onPause");
    	super.onPause();
    }
}