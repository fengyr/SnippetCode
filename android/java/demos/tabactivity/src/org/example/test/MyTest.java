package org.example.test;

import android.app.Activity;
import android.app.TabActivity;

import android.os.Bundle;

import android.widget.TabHost;
import android.widget.TabHost.OnTabChangeListener;
import android.widget.TabHost.TabContentFactory;
import android.widget.TextView;

import android.view.LayoutInflater;
import android.view.View;

import android.util.Log;

public class MyTest extends TabActivity implements OnTabChangeListener {

    private static final String TAG = "MyTest";
    private TabHost mTabHost;

    @Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

        mTabHost = this.getTabHost();

        LayoutInflater factory = LayoutInflater.from(this);
        factory.inflate(R.layout.main, mTabHost.getTabContentView(), true);

        mTabHost.addTab(mTabHost.newTabSpec("Tab1")
                        .setIndicator("A", null)
                        .setContent(R.id.tabhost));
        mTabHost.addTab(mTabHost.newTabSpec("Tab2")
                        .setIndicator("B", null)
                        .setContent(R.id.tab2));

        mTabHost.setOnTabChangedListener(this);
    }

    public void onTabChanged(String tab) {
        Log.v(TAG, " tab changed to: " + tab);
    }
}
