/**
 * @file       ListViewAdapter.java
 *
 * @revision:  none 
 *
 * @version    0.0.01
 * @author:    Zenki (zhajun), zenki2001cn@163.com
 * @date:      2012-3-6 上午10:33:14 
 */

package com.foxconn.supernote.email.util;

import java.util.List;

import android.content.Context;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.graphics.Matrix;
import android.graphics.drawable.BitmapDrawable;
import android.graphics.drawable.Drawable;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.BaseAdapter;
import android.widget.ImageView;

/**
 *
 */
public class ListViewAdapter extends BaseAdapter {

	LayoutInflater factory;
	List<String> mListItem;
	Context mContext;

	public ListViewAdapter(Context context, List<String> listItems) {
		mListItem = listItems;
		mContext = context;
		factory = (LayoutInflater) mContext
				.getSystemService(Context.LAYOUT_INFLATER_SERVICE);
	}

	@Override
	public int getCount() {
		if (mListItem != null) {
			return mListItem.size();
		}
		return 1;
	}

	@Override
	public Object getItem(int position) {
		if (mListItem != null) {
			return mListItem.get(position);
		}
		
		return "";
	}

	@Override
	public long getItemId(int position) {
		return position;
	}

	@Override
	public View getView(int position, View convertView, ViewGroup parent) {
		View view = convertView;

		if (view == null) {
			view = factory.inflate(R.layout.list_item, null);
		}

		String file = null;
		Bitmap temp = null;
		if (null == mListItem) {
			temp = BitmapFactory.decodeResource(mContext.getResources(), R.drawable.white_back);
		} else {
			file = mListItem.get(position);
			BitmapDrawable drawable = new BitmapDrawable(mContext.getResources(),
					file);
			temp = drawable.getBitmap();
		}
		
		Matrix matrix = new Matrix();
		float scaleWidth = 0.2f;
		float scaleHeight = 0.2f;
		matrix.postScale(scaleWidth, scaleHeight);
		
		if (temp != null) {
			Bitmap bitmap = Bitmap.createBitmap(temp, 0, 0, temp.getWidth(),
					temp.getHeight(), matrix, true);

			ImageView imageView = (ImageView) view.findViewById(R.id.img);
			imageView.setImageBitmap(bitmap);
		}

		return view;
	}
}
