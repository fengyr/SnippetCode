/**
 * @file       OptionMenu.java
 *
 * @revision:  none 
 *
 * @version    0.0.01
 * @author:    Zenki (zhajun), junzha@pateo.com.cn
 * @date:      2013-2-20 上午10:47:08 
 */

package com.pateo.as.optionmenu;

import com.example.optionmenu.R;

import android.app.Activity;
import android.content.Context;
import android.graphics.Color;
import android.graphics.Rect;
import android.graphics.drawable.ColorDrawable;
import android.util.Log;
import android.view.Gravity;
import android.view.Menu;
import android.view.MotionEvent;
import android.view.View;
import android.view.View.OnTouchListener;
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
	static private final String TAG = "OptionMenu";
	private GridView mGridView;
	private LinearLayout mLayout;
	private Context mContext;
	private View mParentView;
	private MenuBodyAdapter mAdapter;
	
	private static int[] mDrawableNormalIds = new int[]{R.drawable.button_one_normal, R.drawable.button_one_normal, R.drawable.button_one_normal,
												 R.drawable.button_two_normal_left, R.drawable.button_two_normal_left, R.drawable.button_two_normal_right, 
												 R.drawable.button_three_normal_left, R.drawable.button_three_normal_mid, R.drawable.button_three_normal_right,
												 R.drawable.button_four_normal_left, R.drawable.button_four_normal_mid, R.drawable.button_four_normal_right,
												 R.drawable.button_five_normal_left, R.drawable.button_five_normal_mid, R.drawable.button_five_normal_right};
	private int[] mDrawablePressedIds = new int[]{R.drawable.button_one_pressed, R.drawable.button_two_pressed, R.drawable.button_three_pressed, 
												  R.drawable.button_four_pressed, R.drawable.button_five_pressed };
	
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
		this.setTouchInterceptor(mTouchListener);

		menu.add(OptionMenu.class.getName());
		
		if (mContext instanceof Activity) {
			mParentView = ((Activity) mContext).getWindow().getDecorView();
		}
	}
	
	OnTouchListener mTouchListener = new OnTouchListener() {
		
		@Override
		public boolean onTouch(View v, MotionEvent event) {
			int count = mGridView.getChildCount();
			for (int i = 0; i < count; i++) {
				LinearLayout layout = (LinearLayout) mGridView.getChildAt(i);
				Rect rect = new Rect(layout.getLeft(), layout.getTop(), layout.getRight(), layout.getBottom());
				if (rect.contains((int)event.getX(), (int)event.getY())) {
					((LinearLayout) mGridView.getChildAt(i))
					.setBackgroundResource(mDrawablePressedIds[count -1]);
				} else {
					mAdapter.setButtonBackground(((LinearLayout) mGridView.getChildAt(i)), count, i);
				}
			}
			
			return false;
		}
	};

	public void SetBodySelect(int index) {
		int count = mGridView.getChildCount();
		for (int i = 0; i < count; i++) {
			if (i != index)
				mAdapter.setButtonBackground(((LinearLayout) mGridView.getChildAt(i)), count, i);
		}
		((LinearLayout) mGridView.getChildAt(index))
				.setBackgroundResource(mDrawablePressedIds[count -1]);
	}

	public void SetBodyAdapter(MenuBodyAdapter bodyAdapter) {
		mAdapter = bodyAdapter;
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
			setButtonBackground(result, getCount(), position);

			ImageView img = new ImageView(this.mContext);
			img.setBackgroundResource(mDrawableResId[position]);
			result.addView(img, new LinearLayout.LayoutParams(new LayoutParams(
					LayoutParams.WRAP_CONTENT, LayoutParams.WRAP_CONTENT)));

			TextView text = new TextView(this.mContext);
			text.setText(mStringResId[position]);
			text.setTextSize(16);
			text.setTextColor(0xffffffff);
			result.addView(text, new LinearLayout.LayoutParams(
					new LayoutParams(LayoutParams.WRAP_CONTENT,
							LayoutParams.WRAP_CONTENT)));

			return result;
		}
		
		private void setButtonBackground(View view, int count, int position) {
			int id = 0;
			int pos = 0;
			if (count < 1) {
				id = 1;
			} else if (count > 5) {
				id = 5;
			} else {
				id = count - 1;
			}
			
			if (count > 3) {
				if (position != (count -1)) {
					pos = 1;
				} else {
					pos = 2;
				}
			} else {
				pos = position;
			}
			
			view.setBackgroundResource(mDrawableNormalIds[id*3+pos]);
		}
	}
}
