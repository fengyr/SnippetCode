/**
 * @file       GridImageAdapter.java
 *
 * @revision:  none 
 *
 * @version    0.0.01
 * @author:    Zenki (zhajun), junzha@pateo.com.cn
 * @date:      2013-4-16 下午03:17:06 
 */

package com.pateo.as.photo.view;

import com.pateo.as.photo.R;
import com.pateo.as.photo.model.PhotoAlbums;
import com.pateo.as.photo.model.PhotoAlbums.Album;
import com.pateo.as.photo.model.PhotoThumbnail;

import android.content.ContentResolver;
import android.content.Context;
import android.database.Cursor;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.os.AsyncTask;
import android.provider.MediaStore.Images.Thumbnails;
import android.util.Log;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.BaseAdapter;
import android.widget.ImageView;
import android.widget.TextView;

/**
 *
 */
public class CoverGridViewAdapter extends BaseAdapter {
	static private final String TAG = "CoverGridViewAdapter";
	private Context mContext;
	private PhotoAlbums mAlbums;
	private ScanTask[] mScanTasks;

	private class ScanTask extends AsyncTask<Object, Object, Boolean> {
		Context mContextS;
		ViewHolder mViewHolderS;
		Album mAlbumS;
		Bitmap mThumbBitmap;
		boolean mScanFlag;
		CoverGridViewAdapter mAdapter;

		public ScanTask(Context context, CoverGridViewAdapter adapter,
				ViewHolder viewHolder, Album album) {
			mContextS = context;
			mViewHolderS = viewHolder;
			mAlbumS = album;
			mScanFlag = false;
			mAdapter = adapter;
		}

		@Override
		protected Boolean doInBackground(Object... params) {
			if (null == mAlbumS) {
				return false;
			}

			if (mScanFlag) {
				return true;
			}

			return fillCoverFrame();
		}

		@Override
		protected void onPreExecute() {
			super.onPreExecute();
		}

		@Override
		protected void onPostExecute(Boolean result) {
			super.onPostExecute(result);

			if (result) {
				mViewHolderS.bg = mThumbBitmap;
				mViewHolderS.text = String.format("%s (%d)",
						mAlbumS.BucketName, mAlbumS.PhotoList.size());

				mViewHolderS.coverImgView.setImageBitmap(mViewHolderS.bg);
				mViewHolderS.textView.setText(mViewHolderS.text);
				Log.d(TAG, "onPostExecute");
			}
		}

		private boolean fillCoverFrame() {
			final ContentResolver cr = mContextS.getContentResolver();
			// Get the first photo as a cover.
			final int imageId = mAlbumS.PhotoList.get(0).ID;

			String[] projection = { Thumbnails._ID, Thumbnails.IMAGE_ID,
					Thumbnails.DATA, Thumbnails.WIDTH, Thumbnails.HEIGHT };
			String selection = Thumbnails.IMAGE_ID + "=" + imageId;
			// Log.d(TAG, "getCoverImage selection=" + selection);

			try {
				Cursor cursor = cr.query(Thumbnails.EXTERNAL_CONTENT_URI,
						projection, selection, null, null);

				if (cursor.moveToFirst()) {
					// Log.d(TAG, "getCoverImage moveToFirst()");
					int _id;
					int photo_id;
					String thumb_path;
					int width;
					int height;

					int _idColumn = cursor.getColumnIndex(Thumbnails._ID);
					int image_idColumn = cursor
							.getColumnIndex(Thumbnails.IMAGE_ID);
					int dataColumn = cursor.getColumnIndex(Thumbnails.DATA);
					int widthColumn = cursor.getColumnIndex(Thumbnails.WIDTH);
					int heighColumn = cursor.getColumnIndex(Thumbnails.HEIGHT);

					_id = cursor.getInt(_idColumn);
					photo_id = cursor.getInt(image_idColumn);
					thumb_path = cursor.getString(dataColumn);
					width = cursor.getInt(widthColumn);
					height = cursor.getInt(heighColumn);

					// Log.i(TAG, _id + " photo_id:" + photo_id + " thumb_path:"
					// + thumb_path + " width:" + width + " height:"
					// + height + "---");
					PhotoThumbnail thumb = new PhotoThumbnail();
					thumb.THUMB_ID = _id;
					thumb.IMAGE_ID = photo_id;
					thumb.PATH = thumb_path;
					thumb.WIDTH = width;
					thumb.HEIGHT = height;

					loadThumbImage(cr, thumb);

					mAlbumS.ThumbnailList.add(0, thumb);
				}
			} catch (Exception e) {
				e.printStackTrace();
				return false;
			}

			mScanFlag = true;

			return true;
		}

		private void loadThumbImage(ContentResolver cr, PhotoThumbnail thumb)
				throws Exception {
			BitmapFactory.Options options = new BitmapFactory.Options();
			options.inPreferQualityOverSpeed = false;
			options.inJustDecodeBounds = false;
			mThumbBitmap = BitmapFactory.decodeFile(thumb.PATH, options);
		}
	}

	private class ViewHolder {
		ImageView coverImgViewBg;
		ImageView coverImgView;
		ImageView textBgImgView;
		TextView textView;

		int pos;
		Bitmap bg;
		String text;
	}

	public CoverGridViewAdapter(Context context) {
		this(context, null);
	}

	public CoverGridViewAdapter(Context context, PhotoAlbums albums) {
		mContext = context;
		mAlbums = albums;

		if (mAlbums != null) {
			final int size = mAlbums.getAlbumSize();
			mScanTasks = new ScanTask[size];
		}
	}

	@Override
	public View getView(int position, View convertView, ViewGroup parent) {
		View gridView = convertView;

		int pos = position;

		if (null == gridView) {
			Log.e(TAG, "getView == null, pos = " + pos + " childCount = "
					+ parent.getChildCount());
			gridView = new View(mContext);
			LayoutInflater inflater = (LayoutInflater) mContext
					.getSystemService(Context.LAYOUT_INFLATER_SERVICE);
			gridView = inflater.inflate(R.layout.grid_child, null);
		}

		ViewHolder viewHolder = (ViewHolder) gridView.getTag();
		if (null == viewHolder) {
			Log.d(TAG, "getView viewHolder == null");
			viewHolder = new ViewHolder();
			viewHolder.coverImgViewBg = (ImageView) gridView
					.findViewById(R.id.id_grid_item_image_bg);
			viewHolder.coverImgView = (ImageView) gridView
					.findViewById(R.id.id_grid_item_image);
			viewHolder.textBgImgView = (ImageView) gridView
					.findViewById(R.id.id_grid_item_text_bg);
			viewHolder.textView = (TextView) gridView
					.findViewById(R.id.id_grid_item_text);
			viewHolder.pos = pos;
			gridView.setTag(viewHolder);

			execScanTask(pos, viewHolder);
		}

		return gridView;
	}

	@Override
	public int getCount() {
		if (null == mAlbums) {
			return 0;
		}
		return mAlbums.getAlbumSize();
	}

	@Override
	public Object getItem(int position) {
		if (null == mAlbums) {
			return null;
		}

		final Album album = mAlbums.getAlbumList().get(position);
		return album;
	}

	@Override
	public long getItemId(int position) {
		return position;
	}

	private void execScanTask(int pos, ViewHolder viewHolder) {
		final Album album = mAlbums.getAlbumList().get(pos);

		ScanTask task = mScanTasks[pos];
		if (null == task) {
			task = new ScanTask(mContext, this, viewHolder, album);
		}

		if (task.getStatus() == AsyncTask.Status.FINISHED
				|| task.getStatus() == AsyncTask.Status.PENDING) {
			task.execute();
		}
	}
}
