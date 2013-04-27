package com.example.testappwidget;

import android.appwidget.AppWidgetManager;
import android.appwidget.AppWidgetProvider;
import android.content.Context;
import android.content.Intent;
import android.util.Log;

public abstract class MainAppWidget extends AppWidgetProvider {
	static private final String TAG = "MainAppWidget";
	
	@Override
	public void onDeleted(Context context, int[] appWidgetIds) {
		super.onDeleted(context, appWidgetIds);
	}
	
	@Override
	public void onDisabled(Context context) {
		super.onDisabled(context);
	}
	
	@Override
	public void onEnabled(Context context) {
		super.onEnabled(context);
	}
	
	@Override
	public void onReceive(Context context, Intent intent) {
		Log.d(TAG, intent.getAction());
		super.onReceive(context, intent);
	}
	
	@Override
	public void onUpdate(Context context, AppWidgetManager appWidgetManager,
			int[] appWidgetIds) {
//		for (int i = 0; i < appWidgetIds.length; i++) {
//			Log.d(TAG, "appWidgetid = " + appWidgetIds[i]);
//		}
		
		loadWidget(context, appWidgetManager, appWidgetIds);
		super.onUpdate(context, appWidgetManager, appWidgetIds);
	}
	
	protected abstract void loadWidget(Context context, AppWidgetManager appWidgetManager, int[] appWidgetIds);
}
