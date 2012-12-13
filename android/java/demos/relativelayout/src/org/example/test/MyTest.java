package org.example.test;

import android.app.Activity;

import android.os.Bundle;

import android.view.LayoutInflater;
import android.view.View;

import android.widget.RelativeLayout;

import android.util.Log;

public class MyTest extends Activity {

    private static final String TAG = "MyTest";

    @Override
    public void onCreate(Bundle savedInstanceState)
    {
        super.onCreate(savedInstanceState);

        Log.v(TAG, " debug 1 ");
        mMyLayout = new MyRelativeLayout(this);
        try {
            //mInflater = getLayoutInflater();
            mInflater = LayoutInflater.from(this);
            Log.v(TAG, " debug 2 ");
            mMyLayout = (MyRelativeLayout) mInflater.inflate(R.layout.main, mMyLayout);
        }
        catch(Exception e) {
            Log.v(TAG, " exception inflate ");
        }


        Log.v(TAG, " debug 3 ");
        setContentView(mMyLayout);
    }

    private LayoutInflater mInflater;
    private MyRelativeLayout mMyLayout;
}
