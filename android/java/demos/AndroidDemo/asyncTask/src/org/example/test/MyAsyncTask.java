package org.example.test;

import android.os.AsyncTask;

import android.util.Log;
import java.lang.Thread;

import android.os.Handler;
import android.os.Handler.Callback;
import android.os.Message;

public class MyAsyncTask extends AsyncTask<String, Integer, Integer> implements Callback {
    private static final String TAG = "MyAsyncTask";

    @Override
    protected void onPreExecute() {
        Log.v(TAG, " onPreExecute ");

        super.onPreExecute();
    }

    @Override
    protected Integer doInBackground(String...params) {
        Log.v(TAG, " doInBackground " + params[0] + " " + params[1] + " " + params[2]);

        int i = 0;
        try {
            while ( i<10 ) {
                i++;
                Thread.sleep(500);
                publishProgress(i);
            }
        }
        catch(Exception e) {
            Log.v(TAG, " debug  " + e.getMessage());
        }

        return i;
    }

    @Override
    protected void onProgressUpdate(Integer...values) {
        Log.v(TAG, " onProgressUpdate " + values[0]);

        super.onProgressUpdate(values);
    }

    @Override
    protected void onPostExecute(Integer result) {
        Log.v(TAG, " result: " + result);

        Message msg = new Message();
        msg.what = 1;
        handleMessage(msg);

        //final Handler handler = new Handler() {
            //public void handleMessage(Message msg) {
            //}
        //};
        //handler.sendMessage(msg);

        super.onPostExecute(result);
    }

    @Override
    protected void onCancelled() {
        Log.v(TAG, " onCancelled ");

        super.onCancelled();
    }

    public boolean handleMessage(Message msg) {
        return false;
    }
}
