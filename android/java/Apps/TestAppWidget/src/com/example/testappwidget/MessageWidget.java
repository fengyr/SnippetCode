/**
 * @file       OneCallWidget.java
 *
 * @revision:  none 
 *
 * @version    0.0.01
 * @author:    Zenki (zhajun), junzha@pateo.com.cn
 * @date:      2013-4-26 下午05:43:28 
 */

package com.example.testappwidget;

import android.app.PendingIntent;
import android.appwidget.AppWidgetManager;
import android.content.ComponentName;
import android.content.Context;
import android.content.Intent;
import android.widget.RemoteViews;

/**
 *
 */
public class MessageWidget extends MainAppWidget {
	static final String PACKAGE_NAME = "com.android.mms";
	static final String CLASS_NAME = "com.android.mms.ui.MessageMainActivity";
	private int layout = R.layout.message_layout;
	private int view_id = R.id.message_view;
	private int drawable_id = R.drawable.message;
	
	@Override
	protected void loadWidget(Context context, AppWidgetManager appWidgetManager, int[] appWidgetIds) {
		RemoteViews remoteViews  = new RemoteViews(context.getPackageName(), layout);
		
		Intent intent = new Intent(Intent.ACTION_MAIN);
		intent.addCategory(Intent.CATEGORY_LAUNCHER);
		intent.setAction(Intent.ACTION_MAIN);
		ComponentName cn = new ComponentName(PACKAGE_NAME, CLASS_NAME);           
		intent.setComponent(cn);
		PendingIntent pi = PendingIntent.getActivity(context, 0, intent, 0);
		
		remoteViews.setOnClickPendingIntent(view_id, pi);
		appWidgetManager.updateAppWidget(appWidgetIds, remoteViews);
	}

}
