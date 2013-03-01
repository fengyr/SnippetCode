package org.example.test;

import android.app.Activity;
import android.os.Bundle;
import android.util.Log;

import org.json.JSONObject;
import org.json.JSONArray;
import org.json.JSONException;


public class MyTest extends Activity {

    private static final String TAG = "MyTest";
    JSONObject jsonObject = null;
    JSONArray jsonArray = null;

    @Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.main);

        try {
            jsonObject = new JSONObject();
            jsonArray = new JSONArray();

            jsonObject.put("name1", "zenki1");
            jsonObject.put("name2", "zenki2");
            jsonObject.put("name3", "zenki3");

            jsonArray.put(new JSONObject().put("name1", "zenki1"));
            jsonArray.put(new JSONObject().put("name2", "zenki2"));
            jsonArray.put(new JSONObject().put("name3", "zenki3"));
        }
        catch(Exception e) {
            Log.v(TAG, " json excepiton " + e);
        }

        Log.v(TAG, " jsonObject " + jsonObject);
        Log.v(TAG, " jsonArray " + jsonArray);
    }
}
