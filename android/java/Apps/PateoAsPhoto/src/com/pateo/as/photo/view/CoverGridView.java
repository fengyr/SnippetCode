/**
 * @file       CoverGridView.java
 *
 * @revision:  none 
 *
 * @version    0.0.01
 * @author:    Zenki (zhajun), junzha@pateo.com.cn
 * @date:      2013-4-16 下午04:27:42 
 */

package com.pateo.as.photo.view;

import com.pateo.as.photo.R;
import com.pateo.as.photo.model.PhotoAlbums;
import com.pateo.as.photo.utils.PhotoScanner;
import com.pateo.as.photo.utils.PhotoScannerListener;

import dls.widget.AsProgressDialog;

import android.app.Dialog;
import android.content.Context;
import android.os.Environment;
import android.util.AttributeSet;
import android.util.Log;
import android.widget.GridView;

/**
 *
 */
public class CoverGridView extends GridView {
	static private final String TAG = "CoverGridView";

	private PhotoScanner mPhotoScanner;
	private AsProgressDialog mWaitingDialog;
	private Context mContext;

	PhotoScannerListener photoScannerListener = new PhotoScannerListener() {

		@Override
		public void onScannerFinished(int flag, PhotoAlbums albums) {

			dissmissWaitingDialog();

			Log.d(TAG, "onScannerFinished = " + flag);
			switch (flag) {
			case PhotoScannerListener.FLAG_SCAN_COMPLETED:
				// albums.dump();
				setAdapter(new CoverGridViewAdapter(getContext(), albums));
				break;
			case PhotoScannerListener.FLAG_NO_AVAILABLE_IMG:
				showDialogDelayed(
						getContext().getResources().getString(
								R.string.dlg_no_images), 2000);
				break;

			default:
				break;
			}
		}
	};

	/**
	 * @param context
	 * @param attrs
	 * @param defStyle
	 */
	public CoverGridView(Context context) {
		super(context);
		
		mContext = context;
	}

	/**
	 * @param context
	 * @param attrs
	 * @param defStyle
	 */
	public CoverGridView(Context context, AttributeSet attrs) {
		this(context, attrs, 0);
		
		mContext = context;
	}

	/**
	 * @param context
	 * @param attrs
	 * @param defStyle
	 */
	public CoverGridView(Context context, AttributeSet attrs, int defStyle) {
		super(context, attrs, defStyle);
		
		mContext = context;

		init();
	}

	private void init() {
		mPhotoScanner = new PhotoScanner(getContext(), Environment
				.getExternalStorageDirectory().getAbsolutePath());
		mPhotoScanner.setPhotoScannerListener(photoScannerListener);

		refresh();
	}

	private void showDialogDelayed(String title, int delayed) {
		final Dialog dlg = new Dialog(getContext());
		dlg.setTitle(title);
		dlg.show();

		postDelayed(new Runnable() {
			@Override
			public void run() {
				dlg.dismiss();
			}
		}, delayed);
	}

	private void showWaitingDialog() {
		if (mWaitingDialog != null) {
			return;
		} else {
			mWaitingDialog = AsProgressDialog.show(mContext, null,
					mContext.getString(R.string.dlg_wait_scan), false, true);
			mPhotoScanner.scan();
		}
	} 

	private void dissmissWaitingDialog() {
		if (mWaitingDialog != null) {
			mWaitingDialog.dismiss();
			mWaitingDialog = null;
		}
	}

	public void refresh() {
		if (!mPhotoScanner.isSdcardExsit()) {
			showDialogDelayed(
					getContext().getResources().getString(
							R.string.dlg_no_sdcard), 2000);
		} else {
			showWaitingDialog();
		}
	}

}
