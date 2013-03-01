package com.example.optionmenu;

import android.os.Bundle;
import android.app.Activity;
import android.util.Log;
import android.view.Menu;
import android.view.View;
import android.view.View.OnClickListener;
import android.widget.AdapterView;
import android.widget.AdapterView.OnItemClickListener;
import android.widget.Button;
import android.widget.Toast;

public class MainActivity extends Activity {
	static private final String TAG = "MainActivity";

	OptionMenu.MenuBodyAdapter bodyAdapter = new OptionMenu.MenuBodyAdapter(
			this, new int[] { R.drawable.ic_launcher, R.drawable.ic_launcher, R.drawable.ic_launcher},
			new int[] { R.string.menu_1, R.string.menu_2, R.string.menu_3,
					R.string.menu_4, R.string.menu_5 });
	OptionMenu mOptionMenu;
	Button mButton;

	@Override
	public void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setContentView(R.layout.activity_main);

		mButton = (Button) findViewById(R.id.TextView01);
		mButton.setOnClickListener(new OnClickListener() {

			@Override
			public void onClick(View v) {
				Toast.makeText(MainActivity.this, "Test OptionMenu",
						Toast.LENGTH_SHORT).show();
			}
		});
	}

	class BodyClickEvent implements OnItemClickListener {
		@Override
		public void onItemClick(AdapterView<?> arg0, View arg1, int arg2,
				long arg3) {
			switch (arg2) {
			case 0:
				Log.d(TAG, "onItemClick 1");
				break;
			case 1:
				Log.d(TAG, "onItemClick 2");
				break;
			case 2:
				Log.d(TAG, "onItemClick 3");
				break;

			default:
				break;
			}
			
			mOptionMenu.dismiss();
		}
	}

	@Override
	public boolean onCreateOptionsMenu(Menu menu) {
		mOptionMenu = new OptionMenu(this, menu, new BodyClickEvent(),
				R.drawable.ic_launcher);
		mOptionMenu.SetBodyAdapter(bodyAdapter);
		mOptionMenu.setParentView(findViewById(R.id.LinearLayout01));

		return super.onCreateOptionsMenu(menu);
	}

	@Override
	public boolean onMenuOpened(int featureId, Menu menu) {
		return mOptionMenu.show();
	}
}
