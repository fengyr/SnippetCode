/**
 * @file       AsExtendableListView.java
 *
 * @revision:  none 
 *
 * @version    0.0.01
 * @author:    Zenki (zhajun), junzha@pateo.com.cn
 * @date:      2013-3-6 下午02:04:35 
 */

package com.pateo.as.extendablelistview;

import com.example.asextendablelistview.R;

import android.content.Context;
import android.util.AttributeSet;
import android.util.Log;
import android.view.View;
import android.view.ViewGroup;
import android.widget.ExpandableListAdapter;
import android.widget.ExpandableListView;

/**
 *
 */
public class AsExtendableListView extends ExpandableListView {

	static private final String TAG = "AsExtendableListView";
	private ExpandableListAdapter mAdapter;

	/**
	 * @param context
	 * @param attrs
	 * @param defStyle
	 */
	public AsExtendableListView(Context context) {
		super(context);
	}

	/**
	 * @param context
	 * @param attrs
	 * @param defStyle
	 */
	public AsExtendableListView(Context context, AttributeSet attrs) {
		super(context, attrs);

		init();
	}

	/**
	 * @param context
	 * @param attrs
	 * @param defStyle
	 */
	public AsExtendableListView(Context context, AttributeSet attrs,
			int defStyle) {
		super(context, attrs, defStyle);

		init();
	}

	private void init() {
		this.setGroupIndicator(null);
		this.setChildIndicator(null);
		this.setCacheColorHint(0);
		this.setChildIndicatorBounds(1, 1);
//		this.setSelector(R.drawable.extendablelistview_selector);
		this.setBackgroundColor(0xffdcdcdc);
		
		AsExtendableListView.this
				.setOnGroupExpandListener(mGroupExpandListener);
		AsExtendableListView.this
				.setOnGroupCollapseListener(mGroupCollapseListener);
	}

	public void setAdapter(ExpandableListAdapter adapter) {
		mAdapter = adapter;

		super.setAdapter(adapter);
	}

	protected void onDraw(android.graphics.Canvas canvas) {

		super.onDraw(canvas);
	};

	OnGroupExpandListener mGroupExpandListener = new OnGroupExpandListener() {

		@Override
		public void onGroupExpand(int groupPosition) {

			final ExpandableListAdapter adapter = mAdapter;
			final int groupCount = adapter.getGroupCount();

			for (int i = 0; i < groupCount; i++) {
				if (groupPosition != i) {
					AsExtendableListView.this.collapseGroup(i);
				}
			}
		}
	};

	OnGroupCollapseListener mGroupCollapseListener = new OnGroupCollapseListener() {

		@Override
		public void onGroupCollapse(int groupPosition) {
		}
	};

	private void setListViewHeightBasedOnChildren(int groupPosition) {
		final ExpandableListAdapter adapter = mAdapter;
		if (adapter == null) {
			return;
		}

		int totalHeight = 0;
		for (int j = 0; j < adapter.getChildrenCount(groupPosition); j++) {
			View listItem = adapter.getChildView(groupPosition, j, false, null,
					this);
			listItem.measure(0, 0);
			totalHeight += listItem.getMeasuredHeight();
		}

		ViewGroup.LayoutParams params = this.getLayoutParams();
		params.height = totalHeight
				+ (this.getDividerHeight() * (this.getCount() - 1))
				+ params.height;
		this.setLayoutParams(params);
	}
}
