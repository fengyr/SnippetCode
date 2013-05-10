/**
 * @file       CoverFlowAdapter.java
 *
 * @revision:  none 
 *
 * @version    0.0.01
 * @author:    Zenki (zhajun), junzha@pateo.com.cn
 * @date:      2013-4-22 下午01:17:04 
 */

package com.pateo.as.photo.view;

import com.pateo.as.photo.R;
import com.pateo.as.photo.model.Photo;
import com.pateo.as.photo.model.PhotoThumbnail;
import com.pateo.as.photo.model.PhotoAlbums.Album;

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
public class CoverFlowAdapter extends BaseAdapter {
	static private final String TAG = "CoverFlowAdapter";

	private Album mAlbum;
	private Context mContext;

	private class ViewHolder {
		ImageView coverViewBg;
		ImageView coverView;
		ImageView locationMarkView;
		ImageView textBgImgView;
		TextView textView;

		int pos;
	}

	private class ScanCoverTask extends AsyncTask<Object, Object, Boolean> {
		Context mContextS;
		ViewHolder mViewHolderS;
		Album mAlbumS;
		Bitmap mThumbBitmap;

		public ScanCoverTask(Context context, ViewHolder viewHolder, Album album) {
			mContextS = context;
			mViewHolderS = viewHolder;
			mAlbumS = album;
		}

		@Override
		protected Boolean doInBackground(Object... params) {
			if (null == mAlbumS) {
				return false;
			}

			return loadCoverImage();
		}

		@Override
		protected void onPreExecute() {
			super.onPreExecute();
		}

		@Override
		protected void onPostExecute(Boolean result) {
			super.onPostExecute(result);

			if (result) {
				final int pos = mViewHolderS.pos;
				Photo photo = mAlbumS.PhotoList.get(pos);
				final String path = photo.PATH;
				int latitude = photo.GPS_LATITUDE;
				int longitude = photo.GPS_LONGITUDE;

				Log.e(TAG, "onPostExecute  pos = " + pos + " path = " + path
						+ " GPS_LATITUDE = " + photo.GPS_LATITUDE_STR
						+ " GPS_LONGITUDE = " + photo.GPS_LONGITUDE_STR);
				mViewHolderS.coverView.setImageBitmap(mThumbBitmap);
				mViewHolderS.textView.setText(path);

				if (latitude != 0 || longitude != 0) {
					mViewHolderS.locationMarkView.setVisibility(View.VISIBLE);
				} else {
					mViewHolderS.locationMarkView.setVisibility(View.INVISIBLE);
				}
			}
		}

		private boolean loadCoverImage() {
			final ContentResolver cr = mContextS.getContentResolver();
			// Get the photo at the position.
			final int pos = mViewHolderS.pos;
			final int imageId = mAlbumS.PhotoList.get(pos).ID;

			String[] projection = { Thumbnails._ID, Thumbnails.IMAGE_ID,
					Thumbnails.DATA, Thumbnails.WIDTH, Thumbnails.HEIGHT };
			String selection = Thumbnails.IMAGE_ID + "=" + imageId;
			PhotoThumbnail thumb = new PhotoThumbnail();
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

					Log.i(TAG, _id + " photo_id:" + photo_id + " thumb_path:"
							+ thumb_path + " width:" + width + " height:"
							+ height + "---");
					thumb.THUMB_ID = _id;
					thumb.IMAGE_ID = photo_id;
					thumb.PATH = thumb_path;
					thumb.WIDTH = width;
					thumb.HEIGHT = height;

					loadThumbImage(cr, thumb);
				}
			} catch (Exception e) {
				e.printStackTrace();
				return false;
			}

			try {
				mAlbumS.ThumbnailList.add(pos, thumb);
			} catch (Exception e) {
				e.printStackTrace();
			}

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

	public CoverFlowAdapter(Context context, Album album) {
		mAlbum = album;
		mContext = context;
	}

	@Override
	public int getCount() {
		if (null == mAlbum) {
			return 0;
		}

		return mAlbum.PhotoList.size();
	}

	@Override
	public Object getItem(int position) {
		if (null == mAlbum) {
			return null;
		}

		return mAlbum.PhotoList.get(position);
	}

	@Override
	public long getItemId(int position) {
		return position;
	}

	@Override
	public View getView(int position, View convertView, ViewGroup parent) {
		View cfView = convertView;

		int pos = position;

		if (null == cfView) {
			cfView = new View(mContext);
			LayoutInflater inflater = (LayoutInflater) mContext
					.getSystemService(Context.LAYOUT_INFLATER_SERVICE);
			cfView = inflater.inflate(R.layout.coverflow_child, null);
		}

		ViewHolder viewHolder = (ViewHolder) cfView.getTag();
		if (null == viewHolder) {
			Log.d(TAG, "getView viewHolder == null");
			viewHolder = new ViewHolder();
			viewHolder.coverViewBg = (ImageView) cfView
					.findViewById(R.id.id_coverflow_item_image_bg);
			viewHolder.coverView = (ImageView) cfView
					.findViewById(R.id.id_coverflow_item_image);
			viewHolder.locationMarkView = (ImageView) cfView
					.findViewById(R.id.id_location_mark);
			viewHolder.textBgImgView = (ImageView) cfView
					.findViewById(R.id.id_coverflow_item_text_bg);
			viewHolder.textView = (TextView) cfView
					.findViewById(R.id.id_coverflow_item_text);
			viewHolder.pos = position;
			cfView.setTag(viewHolder);

			execScanTask(pos, viewHolder);
		}

		return cfView;
	}

	private void execScanTask(int pos, ViewHolder viewHolder) {
		final Album album = mAlbum;

		ScanCoverTask task = new ScanCoverTask(mContext, viewHolder, album);

		if (task.getStatus() == AsyncTask.Status.FINISHED
				|| task.getStatus() == AsyncTask.Status.PENDING) {
			task.execute();
		}
	}
}
