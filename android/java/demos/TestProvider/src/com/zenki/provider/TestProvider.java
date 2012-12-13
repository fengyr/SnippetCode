package com.zenki.provider;

import android.app.Activity;
import android.content.ContentValues;
import android.database.Cursor;
import android.os.Bundle;
import android.os.Debug;
import android.util.Log;
import android.view.View;
import android.view.View.OnClickListener;
import android.widget.Button;

public class TestProvider extends Activity {

	static private final String TAG = "TestProvider";

	/** Called when the activity is first created. */
	@Override
	public void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setContentView(R.layout.main);

		Button mBtnInsert = (Button) findViewById(R.id.insert);
		Button mBtnDelete = (Button) findViewById(R.id.delete);
		Button mBtnQuery = (Button) findViewById(R.id.query);

		mBtnInsert.setOnClickListener(new OnClickListener() {

			@Override
			public void onClick(View arg0) {
				ContentValues values = new ContentValues();
				values.put(MyDatabase.MyColumn.NAME, "zenki");
				values.put(MyDatabase.MyColumn.AGE, "28");
				getContentResolver().insert(MyDatabase.MyColumn.CONTENT_URI,
						values);
			}
		});

		mBtnDelete.setOnClickListener(new OnClickListener() {

			@Override
			public void onClick(View arg0) {
				getContentResolver().delete(MyDatabase.MyColumn.CONTENT_URI,
						"( name = 'zenki' )", null);
			}
		});

		mBtnQuery.setOnClickListener(new OnClickListener() {

			@Override
			public void onClick(View arg0) {
				// Cursor cursor =
				// getContentResolver().query(MyDatabase.MyColumn.CONTENT_URI,
				// new String[] {MyDatabase.MyColumn.NAME,
				// MyDatabase.MyColumn.AGE},
				// null, null, null);
				int indexName = 0;
				int indexAge = 0;

				Cursor cursor = managedQuery(MyDatabase.MyColumn.CONTENT_URI,
						new String[] { MyDatabase.MyColumn.NAME,
								MyDatabase.MyColumn.AGE }, null, null, null);
//				if (cursor.getCount() == 0) {
//					Log.v(TAG, "cursor getCount 0");
//					return;
//				}

				cursor.moveToFirst();

				try {
					indexName = cursor
							.getColumnIndexOrThrow(MyDatabase.MyColumn.NAME);
					indexAge = cursor
							.getColumnIndexOrThrow(MyDatabase.MyColumn.AGE);
				} catch (Exception e) {
					// TODO: handle exception
					Log.v(TAG, "getColumn error");
					return;
				}

				while (!cursor.isAfterLast()) {
					String name = cursor.getString(indexName);
					String age = cursor.getString(indexAge);
					cursor.moveToNext();
					Log.v(TAG, "name: " + name + " age: " + age);
				}
				// String name = cursor.getString(0);
				// String age = cursor.getString(1);
			}
		});
	}

	/*
	 * (non-Javadoc)
	 * 
	 * @see android.app.Activity#onUserInteraction()
	 */
	@Override
	public void onUserInteraction() {
		Log.v(TAG, "user entry");
		super.onUserInteraction();
	}

	/*
	 * (non-Javadoc)
	 * 
	 * @see android.app.Activity#onUserLeaveHint()
	 */
	@Override
	protected void onUserLeaveHint() {
		Log.v(TAG, "home press");
		super.onUserLeaveHint();
	}
}