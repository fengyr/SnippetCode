package com.pateo.as.photo.activity;

import com.pateo.as.photo.R;
import com.pateo.as.photo.model.PhotoAlbums.Album;
import com.pateo.as.photo.view.CoverGridView;
import com.pateo.as.photo.view.CoverGridViewAdapter;

import dls.widget.AsAlertDialog;
import dls.widget.AsOptionMenu;

import android.os.Bundle;
import android.content.Context;
import android.content.DialogInterface;
import android.content.Intent;
import android.util.Log;
import android.view.Menu;
import android.view.View;
import android.widget.AdapterView;
import android.widget.AdapterView.OnItemClickListener;
import android.widget.Toast;

public class MainActivity extends BaseActivity {
	static private final String TAG = "PateoAsPhoto";

	private CoverGridView mGridView;

	AsOptionMenu.MenuBodyAdapter bodyAdapter = new AsOptionMenu.MenuBodyAdapter(
			this, new int[] { R.drawable.ic_launcher },
			new int[] { R.string.delete_all_images });
	AsOptionMenu mOptionMenu;

	private class OptionMenuListener implements OnItemClickListener {
		@Override
		public void onItemClick(AdapterView<?> arg0, View arg1, int arg2,
				long arg3) {
			switch (arg2) {
			case 0:
				showAskDelAllDialog();
				break;

			default:
				break;
			}

			mOptionMenu.dismiss();
		}
	}

	private void showAskDelAllDialog() {
		String msg = getString(R.string.dlg_msg_del_all_img);
		String yes = getString(R.string.yes);
		String no = getString(R.string.no);

		AsAlertDialog.Builder customBuilder = new AsAlertDialog.Builder(
				MainActivity.this);
		AsAlertDialog dd = customBuilder
				.setType(AsAlertDialog.ERROR_TYPE_DIALOG).setMessage(msg)
				.setNegativeButton(no, new DialogInterface.OnClickListener() {
					public void onClick(DialogInterface dialog, int which) {
						dialog.dismiss();
					}
				})
				.setPositiveButton(yes, new DialogInterface.OnClickListener() {
					public void onClick(DialogInterface dialog, int which) {
						// TODO: not implement delete all image yet!!!
						dialog.dismiss();
					}
				}).create();

		dd.show();
	}

	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);

		setContentView(R.layout.main);

		initViews();
	}
	
	@Override
	protected void onResume() {
		if (mGridView != null) {
			mGridView.refresh();
		}
		
		super.onResume();
	}

	@Override
	public boolean onCreateOptionsMenu(Menu menu) {
		mOptionMenu = new AsOptionMenu(MainActivity.this, menu,
				new OptionMenuListener(), R.drawable.ic_launcher);
		mOptionMenu.SetBodyAdapter(bodyAdapter);

		return super.onCreateOptionsMenu(menu);
	}

	@Override
	public boolean onMenuOpened(int featureId, Menu menu) {
		return mOptionMenu.show();
	}

	private void initViews() {
		mGridView = (CoverGridView) findViewById(R.id.id_main_gridview);
		mGridView.setAdapter(new CoverGridViewAdapter(this));

		mGridView.setOnItemClickListener(mGridViewOnItemClickListener);
	}

	private OnItemClickListener mGridViewOnItemClickListener = new OnItemClickListener() {
		@Override
		public void onItemClick(AdapterView<?> arg0, View arg1, int arg2,
				long arg3) {
			Log.d(TAG, "arg1: " + arg1 + " arg2: " + arg2 + " arg3: " + arg3);
			Album album = (Album) arg0.getItemAtPosition(arg2);
			if (album != null) {
				Toast.makeText(MainActivity.this, album.toString(),
						Toast.LENGTH_SHORT).show();
				sendIntentWithData(album);
			}
		}
	};

	private void sendIntentWithData(Album album) {
		Intent it = new Intent(MainActivity.this,
				CoverFlowActivity.class);
		Bundle bundle = new Bundle();
		bundle.putParcelable(KEY_BUNDLE_ALBUM, album);
		it.putExtra(KEY_EXTRA_DATA, bundle);
		startActivity(it);
	}

	@Override
	protected void onReceiveBroadcast(Context context, Intent intent) {
		String action = intent.getAction();
		Log.d(TAG, "onReceive action = " + action);

		if (action.equals(Intent.ACTION_MEDIA_SCANNER_FINISHED)) {
			mGridView.refresh();
		} else if (action.equals(Intent.ACTION_MEDIA_UNMOUNTED)) {
			mGridView.refresh();
		}
	}
}
