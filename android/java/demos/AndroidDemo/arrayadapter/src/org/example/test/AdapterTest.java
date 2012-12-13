package org.example.test;

import android.app.Activity;
import android.os.Bundle;

import android.widget.ListView;

import java.util.ArrayList;
import java.util.List;

public class AdapterTest extends Activity
{
    @Override
    public void onCreate(Bundle savedInstanceState)
    {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.main);

        ListView listView = (ListView) findViewById(R.id.listview);

        List<MyListItem> list = new ArrayList<MyListItem>();
        MyListItem item1 = new MyListItem("item1");
        MyListItem item2 = new MyListItem("item2");
        MyListItem item3 = new MyListItem("item3");

        list.add(item1);
        list.add(item2);
        list.add(item3);

        MyArrayAdapter adapter = new MyArrayAdapter(this, list);
        listView.setAdapter(adapter);
    }
}
