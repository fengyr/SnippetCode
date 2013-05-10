/**
 * @file       PhotoDisplayActivity.java
 *
 * @revision:  none 
 *
 * @version    0.0.01
 * @author:    Zenki (zhajun), junzha@pateo.com.cn
 * @date:      2013-4-25 上午10:47:49 
 */

package com.pateo.as.photo.activity;

import java.io.File;
import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.IOException;
import java.util.Timer;
import java.util.TimerTask;

import com.pateo.as.photo.R;
import com.pateo.as.photo.model.Photo;
import com.pateo.as.photo.model.PhotoAlbums.Album;
import com.pateo.as.photo.utils.MediaUpdater;
import com.pateo.as.photo.view.PhotoView;
import com.pateo.as.photo.view.ToolBar;
import com.pateo.as.photo.view.ToolBarAdapter;

import dls.widget.AsAlertDialog;

import android.content.Context;
import android.content.DialogInterface;
import android.content.Intent;
import android.graphics.Bitmap;
import android.net.Uri;
import android.os.Bundle;
import android.os.Environment;
import android.os.Handler;
import android.os.Message;
import android.util.Log;
import android.view.MotionEvent;
import android.view.View;
import android.view.ViewGroup;
import android.view.ViewGroup.LayoutParams;
import android.widget.FrameLayout;
import android.widget.RelativeLayout;
import android.widget.TextView;

/**
 *
 */
public class DisplayActivity extends BaseActivity implements ToolBarAdapter {
	static private final String TAG = "PhotoDisplayActivity";

	private Album mCurrentAlbum;
	private int mCurrentPosition;
	private PhotoView mPhotoView;
	private ToolBar mToolBar;
	private TextView mPhotoNameView;
	private boolean mSlideMode = false;
	private Timer mTimerSlide;
	private boolean mAlbumModified = false;

	private int mOldLeft;
	private int mOldTop;
	private int mOldRight;
	private int mOldBottom;

	public class PhotoViewHolder {
		public PhotoView PhotoView;
		public TextView PhotoNameView;
	}

	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);

		setContentView(R.layout.display);

		initBundle();
		initViews();

		loadPhoto();
	}

	private void initBundle() {
		Bundle bd = getIntent().getBundleExtra(KEY_EXTRA_DATA);

		if (bd != null) {
			mCurrentAlbum = bd.getParcelable(KEY_BUNDLE_ALBUM);
			mCurrentPosition = bd.getInt(KEY_BUNDLE_POS);
		}
	}

	private void initViews() {
		mPhotoView = (PhotoView) findViewById(R.id.id_photo_image);
		mToolBar = (ToolBar) findViewById(R.id.id_toolbar);
		mPhotoNameView = (TextView) findViewById(R.id.id_photo_name);

		PhotoViewHolder pvh = new PhotoViewHolder();
		pvh.PhotoView = mPhotoView;
		pvh.PhotoNameView = mPhotoNameView;

		mToolBar.initViews();
		mToolBar.setPhotoViewHolder(pvh);
		mToolBar.setController(this);
	}

	private void loadPhoto() {
		if (mCurrentAlbum != null) {
			if (mCurrentAlbum.PhotoList.size() == 0) {
				Log.d(TAG, "loadPhoto size = 0");
				return;
			}
			
			Photo photo = mCurrentAlbum.PhotoList.get(mCurrentPosition);
			mPhotoView.setPhoto(photo);
			mPhotoNameView.setText(photo.PATH);

			// update navigate button status
			int latitude = photo.GPS_LATITUDE;
			int longitude = photo.GPS_LONGITUDE;
			if (latitude != 0 || longitude != 0) {
				mToolBar.setNavigateStatus(true);
			} else {
				mToolBar.setNavigateStatus(false);
			}
		}
	}
	
	private void setResultForModify() {
		if (mAlbumModified) {
			Log.d(TAG, "onPause");
			Intent it = new Intent();
			Bundle bundle = new Bundle();
			bundle.putParcelable(KEY_BUNDLE_ALBUM, mCurrentAlbum);
			it.putExtra(KEY_EXTRA_DATA, bundle);
			setResult(101, it);
			mAlbumModified = false;
			
			finish();
		}
	}
	
	@Override
	protected void onPause() {
		super.onPause();
	}
	
	@Override
	public void onBackPressed() {
		setResultForModify();
		
		super.onBackPressed();
	}
	
	private void rescanMedia(String filePath) {
		MediaUpdater updater = new MediaUpdater(DisplayActivity.this);
		updater.scanFile(filePath, "image/*");
	}

	private void showAskRotateDialog() {
		String msg = getString(R.string.cover_origin_image);
		String yes = getString(R.string.yes);
		String no = getString(R.string.no);

		AsAlertDialog.Builder customBuilder = new AsAlertDialog.Builder(
				DisplayActivity.this);
		AsAlertDialog dd = customBuilder
				.setType(AsAlertDialog.ERROR_TYPE_DIALOG).setMessage(msg)
				.setNegativeButton(no, new DialogInterface.OnClickListener() {
					public void onClick(DialogInterface dialog, int which) {
						mPhotoView.restorePhoto();
						dialog.dismiss();
						loadPhoto();
					}
				})
				.setPositiveButton(yes, new DialogInterface.OnClickListener() {
					public void onClick(DialogInterface dialog, int which) {
						mPhotoView.savePhoto();
						dialog.dismiss();
						loadPhoto();
						
						mAlbumModified = true;

						Photo photo = mPhotoView.getPhoto();
						rescanMedia(photo.PATH);
					}
				}).create();

		dd.show();
	}
	
	private void doDeletePhoto() {
		Photo photo = mCurrentAlbum.PhotoList.get(mCurrentPosition);
		File f = new File(photo.PATH);
		f.delete();
		mCurrentAlbum.PhotoList.remove(mCurrentPosition);
		mCurrentAlbum.ThumbnailList.remove(mCurrentPosition);
		
		int pos = mCurrentPosition;
		if (pos >= mCurrentAlbum.getPhotoSize()) {
			pos = 0;
		}

		mCurrentPosition = pos;
		loadPhoto();
	}
	
	private void showAskDeleteDialog() {
		Photo photo = mCurrentAlbum.PhotoList.get(mCurrentPosition);
		String msg = String.format(getString(R.string.cover_origin_image), photo.PATH);
		String yes = getString(R.string.yes);
		String no = getString(R.string.no);

		AsAlertDialog.Builder customBuilder = new AsAlertDialog.Builder(
				DisplayActivity.this);
		AsAlertDialog dd = customBuilder
				.setType(AsAlertDialog.ERROR_TYPE_DIALOG).setMessage(msg)
				.setNegativeButton(no, new DialogInterface.OnClickListener() {
					public void onClick(DialogInterface dialog, int which) {
						dialog.dismiss();
					}
				})
				.setPositiveButton(yes, new DialogInterface.OnClickListener() {
					public void onClick(DialogInterface dialog, int which) {
						doDeletePhoto();
						rescanSdcard();
						
						mAlbumModified = true;
						
						dialog.dismiss();
					}
				}).create();

		dd.show();
	}

	private void rescanSdcard() {
		sendBroadcast(new Intent(Intent.ACTION_MEDIA_MOUNTED,
				Uri.parse("file://"
						+ Environment.getExternalStorageDirectory()
								.getAbsolutePath())));
	}

	private void updateFullScreenUI() {
		if (isFullScreen()) {
			mToolBar.setVisibility(View.GONE);
			mPhotoNameView.setVisibility(View.GONE);

//			mOldLeft = mPhotoView.getLeft();
//			mOldTop = mPhotoView.getTop();
//			mOldRight = mPhotoView.getRight();
//			mOldBottom = mPhotoView.getBottom();
			
			RelativeLayout.LayoutParams linearParams = (RelativeLayout.LayoutParams) mPhotoView.getLayoutParams();

			linearParams.height = 480;

			Log.d(TAG, "updateFullScreenUI");
			mPhotoView.setLayoutParams(linearParams);
			mPhotoView.layout(0, 0, 800, 480);
			mPhotoView.measure(800, 480);
		} else {
			mToolBar.removeCallbacks(mHideToolBar);
			mToolBar.setVisibility(View.VISIBLE);
			mPhotoNameView.setVisibility(View.VISIBLE);
		}
	}

	private Runnable mHideToolBar = new Runnable() {
		@Override
		public void run() {
			mToolBar.setVisibility(View.GONE);
			mPhotoNameView.setVisibility(View.GONE);
		}
	};

	private void showToolBarDelayed() {
		mToolBar.setVisibility(View.VISIBLE);
		mPhotoNameView.setVisibility(View.VISIBLE);
		mToolBar.postDelayed(mHideToolBar, 3000);
	}

	@Override
	public boolean onTouchEvent(MotionEvent event) {
		int action = event.getAction();

		switch (action) {
		case MotionEvent.ACTION_DOWN:
			if (isFullScreen() || isSlideMode()) {
				showToolBarDelayed();
			}
			break;

		default:
			break;
		}
		return super.onTouchEvent(event);
	}

	@Override
	protected void onReceiveBroadcast(Context context, Intent intent) {

	}

	@Override
	public void displayPrePhoto() {
		int pos = mCurrentPosition - 1;
		if (pos < 0) {
			pos = mCurrentAlbum.getPhotoSize() - 1;
		}

		mCurrentPosition = pos;
		if (mPhotoView.isModified()) {
			showAskRotateDialog();
		} else {
			loadPhoto();
		}
	}

	@Override
	public void displayNextPhoto() {
		int pos = mCurrentPosition + 1;
		if (pos >= mCurrentAlbum.getPhotoSize()) {
			pos = 0;
		}

		mCurrentPosition = pos;
		if (mPhotoView.isModified()) {
			showAskRotateDialog();
		} else {
			loadPhoto();
		}
	}

	@Override
	public void rotateLeftPhoto() {
		if (mPhotoView != null) {
			mPhotoView.rotateLeft();
		}
	}

	@Override
	public void rotateRightPhoto() {
		if (mPhotoView != null) {
			mPhotoView.rotateRight();
		}
	}

	@Override
	public void navigate() {
		// TODO: not implement
	}

	@Override
	public boolean fullScreen() {
		if (isFullScreen()) {
			exitFullScreen();
		} else {
			enterFullScreen();
		}

		updateFullScreenUI();

		return isFullScreen();
	}
	
	public static final int SLIDE_MODE_LOAD_PHOTO = 1;
	public static final int SLIDE_MODE_EXIT = 2;
	public Handler H = new Handler() {
		public void handleMessage(Message msg) {
			switch (msg.what) {
			case SLIDE_MODE_LOAD_PHOTO:
				loadPhoto();
				break;
			case SLIDE_MODE_EXIT:
				exitSlideMode();
				break;

			default:
				break;
			}
		};
	};

	private void enterSlideMode() {
		if (!isFullScreen()) {
			fullScreen();
		}

		TimerTask task = new TimerTask() {
			public void run() {
				int pos = mCurrentPosition + 1;
				if (pos >= mCurrentAlbum.getPhotoSize()) {
					pos = mCurrentPosition;
					H.sendEmptyMessage(SLIDE_MODE_EXIT);
					return;
				}

				mCurrentPosition = pos;
				H.sendEmptyMessage(SLIDE_MODE_LOAD_PHOTO);
			}
		};

		mTimerSlide = new Timer(true);
		mTimerSlide.schedule(task, 1000, 3000);

		mSlideMode = true;
	}

	private void exitSlideMode() {
		if (mTimerSlide != null) {
			mTimerSlide.cancel();
			mTimerSlide = null;
		}
		
		fullScreen();

		mSlideMode = false;
	}

	public boolean isSlideMode() {
		return mSlideMode;
	}

	private void updateSlideModeUI() {
		if (isSlideMode()) {
			mToolBar.enterSlideMode();
		} else {
			mToolBar.exitSlideMode();
		}
	}

	@Override
	public boolean slideMode() {
		if (isSlideMode()) {
			exitSlideMode();
		} else {
			enterSlideMode();
		}

		updateSlideModeUI();

		return isSlideMode();
	}

	@Override
	public void deletePhoto() {
		showAskDeleteDialog();
	}
}
