package org.example.test;

import android.app.Activity;
import android.app.PendingIntent;
import android.os.Bundle;
import android.widget.Button;
import android.view.View;
import android.view.View.OnClickListener;
import android.util.Log;
import android.content.Intent;
import android.content.IntentFilter;

public class MyTest extends Activity implements OnClickListener {

    private static final String TAG = "MyTest";

    @Override
    public void onCreate(Bundle savedInstanceState)
    {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.main);

        Button button = (Button) findViewById(R.id.getStatusId);
        button.setOnClickListener(this);
    }

    public void onClick(View view) {
        switch ( view.getId() ) {
        case R.id.getStatusId :
            getStatus();
            break;
        default :
            break;
        }
    }

    private void getStatus() {
        Log.v(TAG, " get status ");

        //registerReceiver(new ConnectionStatus(), new IntentFilter("org.example.test.ConnectionStatus"));

        Intent intent = new Intent(this, ConnectionStatus.class);
        //intent.setFlags(Intent.FLAG_ACTIVITY_NEW_TASK);
        sendBroadcast(intent);
    }
}
