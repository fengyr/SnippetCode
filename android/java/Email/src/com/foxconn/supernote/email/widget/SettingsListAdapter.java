package com.foxconn.supernote.email.widget;

import java.util.List;

import com.foxconn.supernote.email.R;
import com.foxconn.supernote.email.activity.MailSettings;
import com.foxconn.supernote.email.database.AccountInfo;
import com.foxconn.supernote.email.database.IAccountManagerHelper;
import com.foxconn.supernote.email.database.InterViewSQLite;

import android.app.AlertDialog;
import android.content.Context;
import android.content.DialogInterface;
import android.graphics.Color;
import android.graphics.drawable.Drawable;
import android.util.Log;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.view.View.OnClickListener;
import android.widget.BaseAdapter;
import android.widget.ImageView;
import android.widget.ListView;
import android.widget.TextView;

public class SettingsListAdapter  extends BaseAdapter 
{
	private static final String TAG = "SettingsListAdapter";
	
	private LayoutInflater factory;
	private List<AccountInfo> mListItem;
	private Context mContext;
	private ListView mConvertView;
	private int  selectItem = -1;
	private Drawable mSelectDraw;

	public SettingsListAdapter(Context context, List<AccountInfo> listItems) 
	{
		mListItem = listItems;
		mContext = context;
		factory = (LayoutInflater) mContext
				.getSystemService(Context.LAYOUT_INFLATER_SERVICE);
		mConvertView = ((MailSettings)context).getListView();
		mSelectDraw = mContext.getResources().getDrawable(R.drawable.hl02);
	}
	
	@Override
	public int getCount() 
	{
		return mListItem.size();
	}

	@Override
	public Object getItem(int position) 
	{
		return mListItem.get(position);
	}
	
	public  void setSelectItem(int selectItem) 
	{  
        this.selectItem = selectItem;         
    }  


	@Override
	public long getItemId(int position) 
	{
		return position;
	}
	
	@Override
	public View getView(int position, View convertView, ViewGroup parent) 
	{
		Log.i(TAG, "getView Begin");
		ViewHolder holder;
		int index = position;
		AccountInfo accountInfo;
        if (convertView == null) 
        {
        	Log.i(TAG, "getView convertView == null");
			convertView = factory.inflate(R.layout.settings_list_row, null);
		}
        Log.i(TAG, "getView convertView != null");
        holder = new ViewHolder();
		holder.position = index;
		holder.sendUserInfo = (TextView) convertView.findViewById(R.id.mCountUserAddr);
		holder.iconDelete = (ImageView) convertView.findViewById(R.id.mDeleteImg);
		
		
		accountInfo = mListItem.get(index);
		holder.sendUserInfo.setText((index + 1) + ".  " + accountInfo.strUserName 
				                   + "@" + accountInfo.strMailAddress);	
		holder.iconDelete.setVisibility(View.VISIBLE);
		holder.iconDelete.setOnClickListener(deleteItemListener);
		if(position == selectItem)
		{
			convertView.setBackgroundDrawable(mSelectDraw);
			//convertView.setBackgroundColor(Color.GRAY);
		}
		else
		{
			convertView.setBackgroundColor(Color.WHITE);
		}
		convertView.setTag(holder);

		return convertView;
	}
	
	// delete userinfo
	OnClickListener deleteItemListener = new OnClickListener() 
	{
		@Override
		public void onClick(View v) 
		{
			Log.i(TAG, "deleteItemListener onClick");
			View vItem = (View) v.getParent();
			ViewHolder holder = (ViewHolder) vItem.getTag();
			
			final int point = holder.position;
			Log.i(TAG, "deleteItemListener ");
			new AlertDialog.Builder(mContext)
			.setMessage(R.string.ask_delete_info)
			.setPositiveButton(R.string.sure,
					new DialogInterface.OnClickListener() 
			{
						@Override
						public void onClick(DialogInterface dialog,
								int which)
						{
							if(null != MailSettings.getIAccManHel())
							{
								//刪除數據庫中的數據
								MailSettings.getIAccManHel().deleteUser(mListItem.get(point));
								mListItem.remove(point);
							}
							else
							{
								IAccountManagerHelper mIAccMagHel = new InterViewSQLite(mContext);
								mIAccMagHel.deleteUser(mListItem.get(point));
								mListItem.remove(point);
							}
							
							//刷新listview列表顯示
							if (mConvertView != null) 
							{
								mConvertView.setAdapter(SettingsListAdapter.this);
							}
						}
			})
			.setNegativeButton(R.string.cancel,
					new DialogInterface.OnClickListener() 
			{
						public void onClick(DialogInterface dialog,
								int which) 
						{
							dialog.cancel();
						}
			}).show();
			
		}
	};

	
	private class ViewHolder 
	{
		int position;
		TextView sendUserInfo;
		ImageView iconDelete;
	}

}
