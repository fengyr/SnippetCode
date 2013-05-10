/**
 * @file       PhotosCoverFlowActivity.java
 *
 * @revision:  none 
 *
 * @version    0.0.01
 * @author:    Zenki (zhajun), junzha@pateo.com.cn
 * @date:      2013-4-22 上午11:29:33 
 */

package com.pateo.as.photo.activity;

import com.pateo.as.photo.R;
import com.pateo.as.photo.model.Photo;
import com.pateo.as.photo.model.PhotoAlbums.Album;
import com.pateo.as.photo.view.CoverFlowAdapter;

import dls.widget.AsAdapterView;
import dls.widget.AsAdapterView.OnItemClickListener;
import dls.widget.AsCoverFlow;
import android.content.Context;
import android.content.Intent;
import android.os.Bundle;
import android.util.Log;
import android.view.View;
import android.widget.Toast;

/**
 *
 */
public class CoverFlowActivity extends BaseActivity {
	static private final String TAG = "PhotosCoverFlowActivity";

	private AsCoverFlow mPhotoCoverFlow;
	private Album mCurrentAlbum;

	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);

		setContentView(R.layout.coverflow_layout);

		initBundle();
		initViews();
	}

	private void initBundle() {
		Bundle bd = getIntent().getBundleExtra(KEY_EXTRA_DATA);

		if (bd != null) {
			mCurrentAlbum = bd.getParcelable(KEY_BUNDLE_ALBUM);
			Log.d(TAG, "initBundle album = " + mCurrentAlbum);
		}
	}

	private void initViews() {
		mPhotoCoverFlow = (AsCoverFlow) findViewById(R.id.id_coverflow);
		mPhotoCoverFlow.setAdapter(new CoverFlowAdapter(CoverFlowActivity.this,
				mCurrentAlbum));
		mPhotoCoverFlow.setOnItemClickListener(mCoverFlowClickListener);
	}

	private void sendIntentWithData(Album album, int curPos) {
		Intent it = new Intent(CoverFlowActivity.this, DisplayActivity.class);
		Bundle bundle = new Bundle();
		bundle.putParcelable(KEY_BUNDLE_ALBUM, album);
		bundle.putInt(KEY_BUNDLE_POS, curPos);
		it.putExtra(KEY_EXTRA_DATA, bundle);
		startActivityForResult(it, 100);
	}

	@Override
	protected void onActivityResult(int requestCode, int resultCode, Intent data) {
		Log.d(TAG, "onActivityResult requestCode = " + requestCode
				+ " resultCode = " + resultCode + " data = " + data);
		if (resultCode == 101) {
			Bundle bd = data.getBundleExtra(KEY_EXTRA_DATA);

			if (bd != null) {
				mCurrentAlbum = bd.getParcelable(KEY_BUNDLE_ALBUM);
			}
			mPhotoCoverFlow.setAdapter(new CoverFlowAdapter(
					CoverFlowActivity.this, mCurrentAlbum));
		}
		super.onActivityResult(requestCode, resultCode, data);
	}

	private OnItemClickListener mCoverFlowClickListener = new OnItemClickListener() {
		@Override
		public void onItemClick(AsAdapterView<?> parent, View view,
				int position, long id) {
			Log.d(TAG, "position: " + position + " id: " + id + " view: "
					+ view);
			sendIntentWithData(mCurrentAlbum, position);
		}
	};

	@Override
	protected void onReceiveBroadcast(Context context, Intent intent) {
	}
}
