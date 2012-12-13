package org.example.listactivity;

import android.app.Activity;
import android.app.ListActivity;

import android.os.Bundle;

import android.view.View;

import android.widget.ListView;
import android.widget.ArrayAdapter;

import android.util.Log;

public class MyListTest extends ListActivity {
    private static final String TAG = "MyListTest";
    @Override
    public void onCreate(Bundle savedInstanceState)
    {
        super.onCreate(savedInstanceState);
        //setContentView(R.layout.main);

        fillListView();
    }

    private void fillListView() {
        ArrayAdapter<String> adapter = new ArrayAdapter<String>(this, R.layout.listview, R.id.list_item);

        adapter.add("item1");
        adapter.add("item2");
        adapter.add("item3");

        this.setListAdapter(adapter);
    }

    @Override
    protected void onListItemClick(ListView listview, View v, int position, long id) {
        String item = listview.getItemAtPosition(position).toString();
        Log.v(TAG, " debug  " + item);
    }
}
