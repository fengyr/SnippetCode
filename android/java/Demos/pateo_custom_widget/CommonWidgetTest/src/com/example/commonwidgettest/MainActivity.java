package com.example.commonwidgettest;

import com.pateo.as.button.AsButton;
import com.pateo.as.extendablelistview.AsExtendableListView;
import com.pateo.as.optionmenu.OptionMenu;

import android.os.Bundle;
import android.app.Activity;
import android.content.Context;
import android.util.Log;
import android.view.LayoutInflater;
import android.view.Menu;
import android.view.View;
import android.view.ViewGroup;
import android.view.View.OnClickListener;
import android.widget.AdapterView;
import android.widget.BaseExpandableListAdapter;
import android.widget.ImageView;
import android.widget.RelativeLayout;
import android.widget.TextView;
import android.widget.AdapterView.OnItemClickListener;

public class MainActivity extends Activity {
	static private final String TAG = "MainActivity";

	AsExtendableListView mExpandListView;
	public String[] mStrGroupTitles = { "第一组", "第二组" };
	public String[] mStrSubGroupTitles = { "子分组标题" };
	public String[] mStrChildContentOne = { "child1" };
	public String[] mStrChildContentTwo = { "child6", "child7", "child8",
			"child9" };

	OptionMenu.MenuBodyAdapter bodyAdapter = new OptionMenu.MenuBodyAdapter(
			this, new int[] { R.drawable.ic_launcher, R.drawable.ic_launcher,
					R.drawable.ic_launcher }, new int[] { R.string.menu_1,
					R.string.menu_2, R.string.menu_3, R.string.menu_4,
					R.string.menu_5 });
	OptionMenu mOptionMenu;

	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setContentView(R.layout.activity_main);

		mExpandListView = (AsExtendableListView) MainActivity.this
				.findViewById(R.id.id_ExpandableListView);
		mExpandListView.setAdapter(new TreeViewAdapter(this));
		
		AsButton button = (AsButton) findViewById(R.id.asbutton);
		button.setOnClickListener(new OnClickListener() {
			
			@Override
			public void onClick(View v) {
				Log.d(TAG, "OnClickListener");
			}
		});
	}

	class BodyClickEvent implements OnItemClickListener {
		@Override
		public void onItemClick(AdapterView<?> arg0, View arg1, int arg2,
				long arg3) {
			switch (arg2) {
			case 0:
				Log.d(TAG, "onItemClick 1");
				break;
			case 1:
				Log.d(TAG, "onItemClick 2");
				break;
			case 2:
				Log.d(TAG, "onItemClick 3");
				break;

			default:
				break;
			}

			mOptionMenu.dismiss();
		}
	}

	@Override
	public boolean onCreateOptionsMenu(Menu menu) {
		mOptionMenu = new OptionMenu(MainActivity.this, menu,
				new BodyClickEvent(), R.drawable.ic_launcher);
		mOptionMenu.SetBodyAdapter(bodyAdapter);

		return super.onCreateOptionsMenu(menu);
	}

	@Override
	public boolean onMenuOpened(int featureId, Menu menu) {
		return mOptionMenu.show();
	}

	public class TreeViewAdapter extends BaseExpandableListAdapter {
		private LayoutInflater inflater;
		private Context mContext;
		private TextView textView;
		private boolean mShow = true;

		class ViewHolder {
			ImageView imageView;
			TextView textView;
		}

		public TreeViewAdapter(Context context) {
			this.inflater = LayoutInflater.from(context);
			this.mContext = context;
		}

		@Override
		public Object getChild(int groupPosition, int childPosition) {
			return childPosition;
		}

		@Override
		public long getChildId(int groupPosition, int childPosition) {
			return 0;
		}

		@Override
		public View getChildView(int groupPosition, int childPosition,
				boolean isLastChild, View convertView, ViewGroup parent) {
			final View myView = inflater.inflate(R.layout.subchild, null);
			final int groupPos = groupPosition;
			final int childPos = childPosition;

			textView = (TextView) myView.findViewById(R.id.id_child_TextView);
			myView.setBackgroundResource(R.drawable.button_selector);
			// textView.setBackgroundResource(R.drawable.child_background);

			final RelativeLayout relativeLayout = (RelativeLayout) myView
					.findViewById(R.id.id_hide_layout);
			TextView itemView = (TextView) relativeLayout
					.findViewById(R.id.id_child_TextView2);
			itemView.setOnClickListener(new OnClickListener() {

				@Override
				public void onClick(View v) {
					Log.d(TAG, "itemView.setOnClickListener");
				}
			});

			textView.setOnClickListener(new OnClickListener() {

				@Override
				public void onClick(View arg0) {
					Log.i("getChildView", "groupPosition= " + groupPos);
					Log.i("getChildView", "childPosition= " + childPos);
					if (mShow) {
						relativeLayout.setVisibility(View.VISIBLE);
					} else {
						relativeLayout.setVisibility(View.GONE);
					}

					mShow = !mShow;
				}
			});
			if (groupPosition == 0) {
				textView.setText(mStrChildContentOne[childPosition]);
			} else if (groupPosition == 1) {
				textView.setText(mStrChildContentTwo[childPosition]);
			}

			return myView;
		}

		@Override
		public int getChildrenCount(int groupPosition) {
			if (groupPosition == 0) {
				return mStrChildContentOne.length;
			} else if (groupPosition == 1) {
				return mStrChildContentTwo.length;
			}

			return 0;
		}

		@Override
		public Object getGroup(int groupPosition) {
			return "getGroup=" + groupPosition;
		}

		@Override
		public int getGroupCount() {
			return mStrGroupTitles.length;
		}

		@Override
		public long getGroupId(int groupPosition) {
			return groupPosition;
		}

		@Override
		public View getGroupView(int groupPosition, boolean isExpanded,
				View convertView, ViewGroup parent) {
			View myView = convertView;
			if (null == myView) {
				myView = inflater.inflate(R.layout.group, null);
				myView.setBackgroundResource(R.drawable.button_selector);
			}

			ViewHolder viewHolder = (ViewHolder) myView.getTag();

			if (null == viewHolder) {
				viewHolder = new ViewHolder();
				viewHolder.imageView = (ImageView) myView
						.findViewById(R.id.id_group_ImageView);
				viewHolder.textView = (TextView) myView
						.findViewById(R.id.id_group_TextView);
				myView.setTag(viewHolder);
			}

			viewHolder.textView.setText(mStrGroupTitles[groupPosition]);

			Log.d(TAG, "getGroupView isExpanded=" + isExpanded);
			return myView;
		}

		@Override
		public boolean hasStableIds() {
			return false;
		}

		@Override
		public boolean isChildSelectable(int groupPosition, int childPosition) {
			return false;
		}

		@Override
		public void onGroupCollapsed(int groupPosition) {
			Log.d(TAG, "getGroupView onGroupCollapsed=" + groupPosition);
			super.onGroupCollapsed(groupPosition);
		}

		@Override
		public void onGroupExpanded(int groupPosition) {
			Log.d(TAG, "getGroupView onGroupExpanded=" + groupPosition);
			super.onGroupExpanded(groupPosition);
		}
	}
}
