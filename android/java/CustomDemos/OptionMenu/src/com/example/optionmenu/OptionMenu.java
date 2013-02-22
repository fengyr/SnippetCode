/**
 * @file       OptionMenu.java
 *
 * @revision:  none 
 *
 * @version    0.0.01
 * @author:    Zenki (zhajun), junzha@pateo.com.cn
 * @date:      2013-2-20 上午10:47:08 
 */

package com.example.optionmenu;

import android.content.Context;
import android.graphics.Color;
import android.graphics.drawable.ColorDrawable;
import android.view.Gravity;
import android.view.Menu;
import android.view.View;
import android.view.ViewGroup;
import android.widget.BaseAdapter;
import android.widget.GridView;
import android.widget.ImageView;
import android.widget.LinearLayout;
import android.widget.PopupWindow;
import android.widget.AdapterView.OnItemClickListener;
import android.widget.LinearLayout.LayoutParams;
import android.widget.TextView;

public class OptionMenu extends PopupWindow {
	private GridView mGridView;
	private LinearLayout mLayout;
	private Context mContext;
	private View mParentView;

	public OptionMenu(Context context, Menu menu,
			OnItemClickListener bodyClick, int colorBgTabMenu) {
		super(context);

		mContext = context;

		mLayout = new LinearLayout(context);
		mLayout.setOrientation(LinearLayout.VERTICAL);

		mGridView = new GridView(context);
		mGridView.setLayoutParams(new LayoutParams(LayoutParams.FILL_PARENT,
				LayoutParams.WRAP_CONTENT));
		mGridView.setSelector(new ColorDrawable(Color.TRANSPARENT));
		mGridView.setStretchMode(GridView.STRETCH_COLUMN_WIDTH);
		mGridView.setVerticalSpacing(0);
		mGridView.setHorizontalSpacing(0);
		mGridView.setPadding(0, 0, 0, 0);
		mGridView.setGravity(Gravity.CENTER);
		mGridView.setOnItemClickListener(bodyClick);
		mLayout.addView(mGridView);

		this.setContentView(mLayout);
		this.setWidth(LayoutParams.FILL_PARENT);
		this.setHeight(LayoutParams.WRAP_CONTENT);
		this.setBackgroundDrawable(new ColorDrawable(colorBgTabMenu));
		this.setFocusable(true);

		menu.add(OptionMenu.class.getName());
	}

	public void SetBodySelect(int index, int colorSelBody) {
		int count = mGridView.getChildCount();
		for (int i = 0; i < count; i++) {
			if (i != index)
				((LinearLayout) mGridView.getChildAt(i))
						.setBackgroundColor(Color.TRANSPARENT);
		}
		((LinearLayout) mGridView.getChildAt(index))
				.setBackgroundColor(colorSelBody);
	}

	public void SetBodyAdapter(MenuBodyAdapter bodyAdapter) {
		mGridView.setNumColumns(bodyAdapter.getCount());
		mGridView.setAdapter(bodyAdapter);
	}

	public void setParentView(View parent) {
		mParentView = parent;
	}

	public boolean show() {
		showAtLocation(mParentView, Gravity.BOTTOM, 0, 0);
		update();
		return false;
	}

	public void dismiss() {
		super.dismiss();
		update();
	}

	static public class MenuBodyAdapter extends BaseAdapter {
		private Context mContext;
		private int[] mDrawableResId;
		private int[] mStringResId;

		public MenuBodyAdapter(Context context, int[] drawableId, int[] stringId) {
			this.mContext = context;
			this.mDrawableResId = drawableId;
			this.mStringResId = stringId;
		}

		@Override
		public int getCount() {
			return mDrawableResId.length;
		}

		public Object getItem(int position) {
			return makeMenuBody(position);
		}

		public long getItemId(int position) {
			return position;
		}

		public View getView(int position, View convertView, ViewGroup parent) {
			return makeMenuBody(position);
		}

		private LinearLayout makeMenuBody(int position) {
			LinearLayout result = new LinearLayout(this.mContext);
			result.setOrientation(LinearLayout.VERTICAL);
			result.setGravity(Gravity.CENTER_HORIZONTAL
					| Gravity.CENTER_VERTICAL);
			result.setPadding(20, 20, 20, 20);

			ImageView img = new ImageView(this.mContext);
			img.setBackgroundResource(mDrawableResId[position]);
			result.addView(img, new LinearLayout.LayoutParams(new LayoutParams(
					LayoutParams.WRAP_CONTENT, LayoutParams.WRAP_CONTENT)));

			TextView text = new TextView(this.mContext);
			text.setText(mStringResId[position]);
			result.addView(text, new LinearLayout.LayoutParams(
					new LayoutParams(LayoutParams.WRAP_CONTENT,
							LayoutParams.WRAP_CONTENT)));

			return result;
		}
	}
}
