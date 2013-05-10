/**
 * @file       MediaUpdater.java
 *
 * @revision:  none 
 *
 * @version    0.0.01
 * @author:    Zenki (zhajun), junzha@pateo.com.cn
 * @date:      2013-5-7 上午09:56:15 
 */

package com.pateo.as.photo.utils;

import android.content.Context;
import android.media.MediaScannerConnection;
import android.media.MediaScannerConnection.MediaScannerConnectionClient;
import android.net.Uri;
import android.util.Log;

/**
 *
 */
public class MediaUpdater {
	static private final String TAG = "MediaUpdater";

	private MediaScannerConnection mMediaScanConn = null;
	private MediaScannerClient mClient = null;
	private String filePath = null;
	private String fileType = null;
	private String[] filePaths = null;

	public MediaUpdater(Context context) {
		if (mClient == null) {
			mClient = new MediaScannerClient();
		}

		if (mMediaScanConn == null) {
			mMediaScanConn = new MediaScannerConnection(context, mClient);
		}
	}

	class MediaScannerClient implements MediaScannerConnectionClient {

		@Override
		public void onMediaScannerConnected() {
			if (filePath != null) {
				Log.d(TAG, "onMediaScannerConnected filePath = " + filePath);
				mMediaScanConn.scanFile(filePath, fileType);
			}

			if (filePaths != null) {
				for (String file : filePaths) {
					mMediaScanConn.scanFile(file, fileType);
				}
			}

			filePath = null;
			fileType = null;
			filePaths = null;
		}

		@Override
		public void onScanCompleted(String path, Uri uri) {
			Log.d(TAG, "onScanCompleted ");
			mMediaScanConn.disconnect();
		}
	}

	public void scanFile(String filepath, String fileType) {
		this.filePath = filepath;
		this.fileType = fileType;
		mMediaScanConn.connect();
	}

	public void scanFile(String[] filePaths, String fileType) {
		this.filePaths = filePaths;
		this.fileType = fileType;
		mMediaScanConn.connect();
	}

}
