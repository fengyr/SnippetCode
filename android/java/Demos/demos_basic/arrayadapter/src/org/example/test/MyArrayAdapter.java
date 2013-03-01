package org.example.test;

import android.widget.ArrayAdapter;
import android.widget.TextView;

import android.content.Context;

import android.view.View;
import android.view.ViewGroup;
import android.view.LayoutInflater;

import android.util.Log;

import java.util.List;

public class MyArrayAdapter extends ArrayAdapter<MyListItem> {

    private static final String TAG = "MyArrayAdapter";

    LayoutInflater factory; 
    List<MyListItem> mListItem;
    Context mContext;

    public MyArrayAdapter(Context context, List<MyListItem> listItems) {
        super(context, R.id.itemtext, listItems);

        mListItem = listItems;
        mContext = context;
        factory = (LayoutInflater) mContext.getSystemService(Context.LAYOUT_INFLATER_SERVICE);
    }

    @Override
    public View getView(int position, View convertView, ViewGroup parent) {
        View view = convertView;

        if ( view == null ) {
            view = factory.inflate(R.layout.item, null);
            Log.v(TAG, " view == null ");
        }

        MyListItem item = getItem(position);
        TextView textView = (TextView) view.findViewById(R.id.itemtext);

        textView.setText(item.getName());

        return view;
    }
}
