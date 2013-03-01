package com.test;

import android.app.ActivityGroup;
import android.os.Bundle;
import android.view.View;
import android.view.ViewGroup;
import android.view.View.OnClickListener;
import android.widget.BaseAdapter;
import android.widget.ListView;
import android.widget.TextView;
import android.widget.Toast;

public class PlayListView extends ActivityGroup{
	private ListView listView;
	private ListAdpter listAdpter;
	private String[] items;
	@Override
	protected void onCreate(Bundle savedInstanceState) {
		// TODO Auto-generated method stub
		super.onCreate(savedInstanceState);
		 setContentView(R.layout.list);
		 items=new String[]{"111","222","333","444","555","666"};
		 listView=(ListView) findViewById(R.id.list);
		/* ArrayList<HashMap<String,Object>> listItem = new ArrayList<HashMap<String,Object>>();
		 for(int i=0;i<items.length;i++){
	        	HashMap<String,Object> map = new HashMap<String,Object>();
	        	map.put("sorts1", items[i]);
	        	listItem.add(map);
	        }
			SimpleAdapter sa=new SimpleAdapter(this,listItem , R.layout.listitem,new String[]{"sorts1"},new int[]{R.id.list_tv});
			listView.setAdapter(sa);*/
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
				view = getLayoutInflater().inflate(R.layout.listitem, null);
			}	
			TextView tv=(TextView) view.findViewById(R.id.list_tv);
			tv.setText(items[position]);
			tv.setOnClickListener(new OnClickListener() {
				
				public void onClick(View v) {
					// TODO Auto-generated method stub
					Toast.makeText(PlayListView.this, "TextViewµã»÷ÊÂ¼þ", Toast.LENGTH_SHORT).show();
				}
			});
			return view;
		}
		
	}
 
	
}
