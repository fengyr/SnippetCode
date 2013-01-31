package com.test;

import android.app.ActivityGroup;
import android.content.Intent;
import android.os.Bundle;
import android.util.DisplayMetrics;
import android.view.View;
import android.view.View.OnClickListener;
import android.widget.LinearLayout;
import android.widget.LinearLayout.LayoutParams;

public class MainView extends ActivityGroup {
	@SuppressWarnings("unused")
	private LinearLayout bodyView,headview;
	private LinearLayout player, localList, onlineList, menu;
	private int flag = 0; // 通过标记跳转不同的页面，显示不同的菜单项
    /** Called when the activity is first created. */
    @Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.view_main);
        initMainView();
		// 默认显示 播放界面
		showView(flag);
		player.setOnClickListener(new OnClickListener() {
			
		
			public void onClick(View v) {
				// TODO Auto-generated method stub
				flag = 0;
				showView(flag);
				player.setBackgroundResource(R.drawable.frame_button_background);
				localList.setBackgroundResource(R.drawable.frame_button_nopressbg);
				onlineList.setBackgroundResource(R.drawable.frame_button_nopressbg);
			}
		});
		localList.setOnClickListener(new OnClickListener() {
			
		
			public void onClick(View v) {
				// TODO Auto-generated method stub
				flag = 1;
				showView(flag);
				localList.setBackgroundResource(R.drawable.frame_button_background);
				player.setBackgroundResource(R.drawable.frame_button_nopressbg);
				onlineList.setBackgroundResource(R.drawable.frame_button_nopressbg);
			}
		});
		onlineList.setOnClickListener(new OnClickListener() {
			
	
			public void onClick(View v) {
				// TODO Auto-generated method stub
				flag = 2;
				showView(flag);
				onlineList.setBackgroundResource(R.drawable.frame_button_background);
				player.setBackgroundResource(R.drawable.frame_button_nopressbg);
				localList.setBackgroundResource(R.drawable.frame_button_nopressbg);
			}
		});
		menu.setOnClickListener(new OnClickListener() {
			
				public void onClick(View v) {
				// TODO Auto-generated method stub
				
			}
		});

    }
   
    /*
	 * 初始化主界面底部的功能菜单：player，localList，onlineList,menu;
	 */
    public void initMainView() {
		headview=(LinearLayout) findViewById(R.id.head);
		bodyView=(LinearLayout) findViewById(R.id.body);
		player=(LinearLayout) findViewById(R.id.player);
		localList=(LinearLayout) findViewById(R.id.localList);
		onlineList=(LinearLayout) findViewById(R.id.onlineList);
		menu=(LinearLayout) findViewById(R.id.menu);
	}
    
   // 在主界面中显示其他界面
	public void showView(int flag) {
		switch (flag) {
		case 0:
			bodyView.removeAllViews();
			View v = getLocalActivityManager().startActivity("player",
					new Intent(MainView.this, PlayerView.class)).getDecorView();

			DisplayMetrics dm = new DisplayMetrics();
			getWindowManager().getDefaultDisplay().getMetrics(dm);
			//int dmwidth = dm.widthPixels;
			int dmheight = dm.heightPixels;
			LayoutParams param = new LayoutParams(LayoutParams.FILL_PARENT,
					dmheight -85);
			v.setLayoutParams(param);
			bodyView.addView(v);
			break;
		case 1:
			bodyView.removeAllViews();
			bodyView.addView(getLocalActivityManager().startActivity("locallist",
					new Intent(MainView.this, PlayListView.class))
					.getDecorView());
			break;
		case 2:			
			bodyView.removeAllViews();
			bodyView.addView(getLocalActivityManager().startActivity(
					"onlineList", new Intent(MainView.this, OnlineView.class))
					.getDecorView());
			break;
		default:
			break;
		}
	}
}