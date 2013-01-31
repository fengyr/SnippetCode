/**
 * @file       MyProvider.java
 *
 * @revision:  none 
 *
 * @version    0.0.01
 * @author:    Zenki (zhajun), zenki2001cn@163.com
 * @date:      2010-11-24 上午08:54:43 
 */

package com.zenki.provider;

import android.content.ContentProvider;
import android.content.ContentUris;
import android.content.ContentValues;
import android.content.Context;
import android.content.UriMatcher;
import android.database.Cursor;
import android.database.SQLException;
import android.database.sqlite.SQLiteDatabase;
import android.database.sqlite.SQLiteDatabase.CursorFactory;
import android.database.sqlite.SQLiteOpenHelper;
import android.database.sqlite.SQLiteQueryBuilder;
import android.net.Uri;
import android.util.Log;

/**
 *
 */
public class MyProvider extends ContentProvider {

	static private final String TAG = "MyProvider";
	private static final String DATABASE_NAME = "myprovider";
	private static final int DATABASE_VERSION = 1;
	private static final String TABLE_NAME = "mytable";

	private static final UriMatcher mUriMatcher = new UriMatcher(
			UriMatcher.NO_MATCH);
	private static final int MYDATABASE = 1;
	private static final int MYDATABASE_ID = 2;
	private static final String AUTHORITY = "com.zenki.myprovider";

	private DatabaseHelper mDbHelper = null;

	static {
		mUriMatcher.addURI(AUTHORITY, "myprovider", 1);
		mUriMatcher.addURI(AUTHORITY, "myprovider/#", 2);
	}

	private class DatabaseHelper extends SQLiteOpenHelper {

		public DatabaseHelper(Context context) {
			super(context, DATABASE_NAME, null, DATABASE_VERSION);
		}

		// 创建数据库
		@Override
		public void onCreate(SQLiteDatabase db) {
			try {
				db.execSQL("create table " + TABLE_NAME + " ("
						+ MyDatabase.MyColumn._ID
						+ " INTEGER PRIMARY KEY AUTOINCREMENT, "
						+ MyDatabase.MyColumn.NAME + " TEXT, "
						+ MyDatabase.MyColumn.AGE + " TEXT);");
			} catch (SQLException e) {
				Log.v(TAG, "execSQL error");
			}
		}

		// 数据库升级
		@Override
		public void onUpgrade(SQLiteDatabase db, int oldVersion, int newVersion) {
			try {
				db.execSQL("DROP TABLE IF EXISTS " + TABLE_NAME);
			} catch (SQLException e) {
				Log.v(TAG, "onUpgrade error");
			}
			onCreate(db);
		}
	}

	public MyProvider() {

	}

	/*
	 * (non-Javadoc)
	 * 
	 * @see android.content.ContentProvider#onCreate()
	 */
	@Override
	public boolean onCreate() {
		mDbHelper = new DatabaseHelper(getContext());
		return false;
	}

	/*
	 * (non-Javadoc)
	 * 
	 * @see android.content.ContentProvider#insert(android.net.Uri,
	 * android.content.ContentValues)
	 */
	@Override
	public Uri insert(Uri arg0, ContentValues arg1) {
		Uri uri = null;
		ContentValues value;
		
		if (arg1 != null) {
			value = new ContentValues(arg1);
		} else {
			value = new ContentValues();
			value.put(MyDatabase.MyColumn.NAME, "jacky");
			value.put(MyDatabase.MyColumn.AGE, "20");
		}
		
		SQLiteDatabase db = mDbHelper.getWritableDatabase();
		
		long rowId = db.insert(TABLE_NAME, null, value);
		if (rowId != -1) {
			uri = ContentUris.withAppendedId(MyDatabase.MyColumn.CONTENT_URI, rowId);
		}
		

		return uri;
	}

	/*
	 * (non-Javadoc)
	 * 
	 * @see android.content.ContentProvider#delete(android.net.Uri,
	 * java.lang.String, java.lang.String[])
	 */
	@Override
	public int delete(Uri arg0, String arg1, String[] arg2) {
		SQLiteDatabase db = mDbHelper.getWritableDatabase();
		
//		String rowId = arg0.getPathSegments().get(1);
		return db.delete(TABLE_NAME, arg1, arg2);
	}

	/*
	 * (non-Javadoc)
	 * 
	 * @see android.content.ContentProvider#query(android.net.Uri,
	 * java.lang.String[], java.lang.String, java.lang.String[],
	 * java.lang.String)
	 */
	@Override
	public Cursor query(Uri arg0, String[] arg1, String arg2, String[] arg3,
			String arg4) {
		SQLiteQueryBuilder qb = new SQLiteQueryBuilder();
		
		switch (mUriMatcher.match(arg0)) {
		case MYDATABASE:
			qb.setTables(TABLE_NAME);
			break;
		case MYDATABASE_ID:
			qb.setTables(TABLE_NAME);
			qb.appendWhere(MyDatabase.MyColumn._ID + "=" + arg0.getPathSegments().get(1));
			break;

		default:
			break;
		}
		
		SQLiteDatabase db = mDbHelper.getReadableDatabase();
		Cursor c = qb.query(db, arg1, arg2, arg3, null, null, arg4);
		
		return c;
	}

	/*
	 * (non-Javadoc)
	 * 
	 * @see android.content.ContentProvider#update(android.net.Uri,
	 * android.content.ContentValues, java.lang.String, java.lang.String[])
	 */
	@Override
	public int update(Uri arg0, ContentValues arg1, String arg2, String[] arg3) {
		// TODO Auto-generated method stub
		return 0;
	}

	/*
	 * (non-Javadoc)
	 * 
	 * @see android.content.ContentProvider#getType(android.net.Uri)
	 */
	@Override
	public String getType(Uri arg0) {
		switch (mUriMatcher.match(arg0)) {
		case MYDATABASE:
			return MyDatabase.MyColumn.CONTENT_TYPE;
		case MYDATABASE_ID:
			return MyDatabase.MyColumn.CONTENT_ITEM_TYPE;

		default:
			throw new IllegalArgumentException("Error URI: " + arg0);
		}
	}
}
