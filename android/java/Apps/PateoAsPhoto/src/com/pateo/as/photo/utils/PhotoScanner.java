/**
 * @file       PhotoScanner.java
 *
 * @revision:  none 
 *
 * @version    0.0.01
 * @author:    Zenki (zhajun), junzha@pateo.com.cn
 * @date:      2013-4-16 下午03:51:43 
 */

package com.pateo.as.photo.utils;

import java.io.File;
import java.io.FileFilter;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.Set;

import com.pateo.as.photo.model.Photo;
import com.pateo.as.photo.model.PhotoAlbums;
import com.pateo.as.photo.model.PhotoAlbums.Album;

import android.content.ContentResolver;
import android.content.Context;
import android.database.Cursor;
import android.media.ExifInterface;
import android.os.Environment;
import android.os.Handler;
import android.os.Message;
import android.provider.MediaStore.Images.Media;
import android.util.Log;

/**
 *
 */
public class PhotoScanner {

	static private final String TAG = "PhotoScanner";

	private String mRoot;
	private Context mContext;
	private PhotoAlbums mPhotoAlbums;
	private PhotoScannerListener mListener;
	private H mHanlder;

	class H extends Handler {
		static final int SCAN_COMPLETED = 0;
		static final int NO_AVALIABLE_IMG = 1;

		@Override
		public void handleMessage(Message msg) {
			switch (msg.what) {
			case SCAN_COMPLETED:
				if (mListener != null) {
					mListener.onScannerFinished(
							PhotoScannerListener.FLAG_SCAN_COMPLETED,
							mPhotoAlbums);
				}
				break;

			case NO_AVALIABLE_IMG:
				if (mListener != null) {
					mListener.onScannerFinished(
							PhotoScannerListener.FLAG_NO_AVAILABLE_IMG, null);
				}
				break;

			default:
				break;
			}

			super.handleMessage(msg);
		}
	}

	static class ScanPhotoThread extends Thread {
		private PhotoScanner mScanner;
		private ContentResolver mCR;
		private HashMap<String, ArrayList<Photo>> mPhotoMap;

		public ScanPhotoThread(PhotoScanner scanner) {
			mScanner = scanner;
			mCR = scanner.getContext().getContentResolver();
			mPhotoMap = new HashMap<String, ArrayList<Photo>>();
		}

		@Override
		public void run() {
			String[] projection = { Media._ID, Media.DATA, Media.MIME_TYPE,
					Media.LATITUDE, Media.LONGITUDE, Media.BUCKET_DISPLAY_NAME };
			Log.d(TAG, "scanDirs uri=" + Media.EXTERNAL_CONTENT_URI.toString());
			Cursor cursor = mCR.query(Media.EXTERNAL_CONTENT_URI, projection,
					null, null, null);

			getColumnData(cursor);

			if (0 == mPhotoMap.size()) {
				mScanner.sendMessageEvent(H.NO_AVALIABLE_IMG);
			} else {
				mergeData();
				mScanner.sendMessageEvent(H.SCAN_COMPLETED);
			}
		}

		private void getColumnData(Cursor cursor) {
			if (cursor.moveToFirst()) {
				Log.d(TAG, "ScanPhotoThread getColumnData enter");

				int _id;
				String image_path;
				String mime_path;
				String bucket_name;
				int latitude;
				int longitude;

				int _idColumn = cursor.getColumnIndex(Media._ID);
				int image_pathColumn = cursor.getColumnIndex(Media.DATA);
				int mime_typeColumn = cursor.getColumnIndex(Media.MIME_TYPE);
				int latitudeColumn = cursor.getColumnIndex(Media.LATITUDE);
				int longitudeColumn = cursor.getColumnIndex(Media.LONGITUDE);
				int bucketColumn = cursor
						.getColumnIndex(Media.BUCKET_DISPLAY_NAME);

				do {
					_id = cursor.getInt(_idColumn);
					image_path = cursor.getString(image_pathColumn);
					mime_path = cursor.getString(mime_typeColumn);
					bucket_name = cursor.getString(bucketColumn);
					latitude = cursor.getInt(latitudeColumn);
					longitude = cursor.getInt(longitudeColumn);

					// Log.i(TAG, "_id:" + _id + " image_path:" + image_path
					// + " mime_path:" + mime_path + " bucket_name:"
					// + bucket_name);
					Photo photo = new Photo();
					photo.ID = _id;
					photo.PATH = image_path;
					photo.MIME = mime_path;
					photo.GPS_LATITUDE = latitude;
					photo.GPS_LONGITUDE = longitude;
					photo.BUCKET_NAME = bucket_name;

					try {
						ExifInterface exif = new ExifInterface(image_path);
						photo.GPS_LATITUDE_STR = exif
								.getAttribute(ExifInterface.TAG_GPS_ALTITUDE);
						photo.GPS_LONGITUDE_STR = exif
								.getAttribute(ExifInterface.TAG_GPS_ALTITUDE);
					} catch (Exception e) {
						e.printStackTrace();
					}

					ArrayList<Photo> photoList = mPhotoMap.get(bucket_name);
					if (null == photoList) {
						photoList = new ArrayList<Photo>();
					}
					photoList.add(photo);
					mPhotoMap.put(bucket_name, photoList);
				} while (cursor.moveToNext());
			}
		}

		private void mergeData() {
			Set<String> keySet = mPhotoMap.keySet();
			for (String key : keySet) {
				Album album = new Album();
				ArrayList<Photo> photos = mPhotoMap.get(key);
				album.BucketName = key;
				album.PhotoList = photos;
				mScanner.addAlbum(album);
			}

			// TODO: here, not implement for sorting directory
		}
	}

	Context getContext() {
		return mContext;
	}

	public PhotoScanner(Context context, String rootDir) {
		mRoot = rootDir;
		mContext = context;
		mHanlder = new H();
		mPhotoAlbums = new PhotoAlbums();
	}

	public void setPhotoScannerListener(PhotoScannerListener listener) {
		mListener = listener;
	}

	public boolean isSdcardExsit() {
		return Environment.getExternalStorageState().equals(
				Environment.MEDIA_MOUNTED);
	}

	public void scan() {
		// new ScanThumbsThread(this).start();

		mPhotoAlbums.clear();
		new ScanPhotoThread(this).start();
	}

	private File[] getDirs() {
		File root = new File(mRoot);
		if (!root.exists()) {
			return null;
		}

		File[] dirs = root.listFiles(new FileFilter() {

			@Override
			public boolean accept(File pathname) {
				if (pathname.isDirectory()) {
					return true;
				}
				return false;
			}
		});

		for (int i = 0; i < dirs.length; i++) {
			Log.d(TAG, "dir" + i + ": " + dirs[i].getName());
		}

		return dirs;
	}

	void sendMessageEvent(int what) {
		Message msg = Message.obtain();
		msg.what = what;

		switch (what) {
		case H.SCAN_COMPLETED:
			msg.obj = mPhotoAlbums;
			break;

		default:
			break;
		}

		mHanlder.sendMessage(msg);
	}

	void addAlbum(Album album) {
		try {
			mPhotoAlbums.addAlbum(album);
		} catch (Exception e) {
			Log.e(TAG, "PhotoScanner addAlbum " + e);
		}
	}

	PhotoAlbums getAlbums() {
		return mPhotoAlbums;
	}
}
