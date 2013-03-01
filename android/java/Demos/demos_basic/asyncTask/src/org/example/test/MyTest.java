package org.example.test;

import android.app.Activity;

import android.os.Bundle;
import android.os.Message;

import android.widget.Button;
import android.view.View;
import android.view.View.OnClickListener;

import android.util.Log;

public class MyTest extends Activity implements OnClickListener {
    private static final String TAG = "MyTest";

    MyAsyncTask mTask;

    @Override
    public void onCreate(Bundle savedInstanceState)
    {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.main);


        Button btnStart = (Button) findViewById(R.id.start_id);
        Button btnStop = (Button) findViewById(R.id.stop_id);

        btnStart.setOnClickListener(this);
        btnStop.setOnClickListener(this);
    }

    public void onClick(View view) {
        switch ( view.getId() ) {
        case R.id.start_id :
            mTask = new MyAsyncTask() {
                @Override
                public boolean handleMessage(Message msg) {
                    switch ( msg.what ) {
                    case 0 :
                        // todo
                        break;
                    case 1 :
                        Log.v(TAG, " Task has finished ");
                        break;
                    default :
                        break;
                    }

                    return super.handleMessage(msg);
                }
            };
            mTask.execute(TAG, "hello", "world");
            break;
        case R.id.stop_id :
            mTask.cancel(true);
            break;
        default :
            break;
        }
    }
}
