package com.test;

import android.app.ActivityGroup;
import android.os.Bundle;
import android.view.View;
import android.view.ViewGroup;
import android.view.View.OnClickListener;
import android.widget.BaseAdapter;
import android.widget.ImageView;
import android.widget.ListView;
import android.widget.TextView;
import android.widget.Toast;


public class PlayerView extends ActivityGroup {
	private ListView listView;
	private ListAdpter listAdpter;
	private String[] items;
	@Override
	protected void onCreate(Bundle savedInstanceState) {
		// TODO Auto-generated method stub
		super.onCreate(savedInstanceState);
		 setContentView(R.layout.playlist);
		 items=new String[]{"111","222","333","444","555","666"};
		 listView=(ListView) findViewById(R.id.content_playlist);
		 listAdpter =new ListAdpter();
		 listView.setAdapter(listAdpter);
		 
	}
	class ListAdpter extends BaseAdapter
	{

		public int getCount() {
			// TODO Auto-generated method stub
			return items.length;
		}

		public Object getItem(int position) {
			// TODO Auto-generated method stub
			return position;
		}

		public long getItemId(int position) {
			// TODO Auto-generated method stub
			return position;
		}

		public View getView(int position, View view, ViewGroup parent) {
			// TODO Auto-generated method stub
			if (view == null) {
				view = getLayoutInflater().inflate(R.layout.playlistitem, null);
			}	
			ImageView iv=(ImageView)view.findViewById(R.id.playlistitem_iv);
			iv.setBackgroundResource(R.drawable.frame_player_normal);
			TextView tv1=(TextView) view.findViewById(R.id.playlistitem_tv1);
			TextView tv2=(TextView) view.findViewById(R.id.playlistitem_tv2);
			TextView tv3=(TextView) view.findViewById(R.id.playlistitem_tv3);
			TextView tv4=(TextView) view.findViewById(R.id.playlistitem_tv4);
			tv1.setText(items[position]);
			tv2.setText(items[position]);
			tv3.setText(R.string.download);
			tv4.setText(R.string.preview);
			iv.setOnClickListener(new OnClickListener() {
				
				public void onClick(View v) {
					// TODO Auto-generated method stub
					Toast.makeText(PlayerView.this, "ImageView点击事件", Toast.LENGTH_SHORT).show();
				}
			});
            //tv1点击事件
			tv1.setOnClickListener(new OnClickListener() {
				
				public void onClick(View v) {
					// TODO Auto-generated method stub
					Toast.makeText(PlayerView.this, "TextView1点击事件", Toast.LENGTH_SHORT).show();
				}
			});
			//tv2点击事件
			tv2.setOnClickListener(new OnClickListener() {
				
				public void onClick(View v) {
					// TODO Auto-generated method stub
					Toast.makeText(PlayerView.this, "TextView2点击事件", Toast.LENGTH_SHORT).show();
				}
			});
			 //tv3点击事件
			tv3.setOnClickListener(new OnClickListener() {
				
				public void onClick(View v) {
					// TODO Auto-generated method stub
					Toast.makeText(PlayerView.this, "download点击事件", Toast.LENGTH_SHORT).show();
				}
			});
			 //tv4点击事件
			tv4.setOnClickListener(new OnClickListener() {
				
				public void onClick(View v) {
					// TODO Auto-generated method stub
					Toast.makeText(PlayerView.this, "preview点击事件", Toast.LENGTH_SHORT).show();
				}
			});
			return view;
		}
		
	}
}
