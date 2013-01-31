package org.example.test;

import android.app.Activity;
import android.os.Bundle;
import android.os.Parcel;
import android.util.Log;
import android.widget.Button;
import android.view.View.OnClickListener;
import android.view.View;

public class ParcelTest extends Activity implements OnClickListener {

    private static final String TAG = "ParcelTest";
    Parcel parcel = Parcel.obtain();

    @Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.main);

        Button saveBtn = (Button) findViewById(R.id.save_id);
        Button loadBtn = (Button) findViewById(R.id.load_id);

        saveBtn.setOnClickListener(this);
        loadBtn.setOnClickListener(this);
    }

    private void saveData() {
        parcel.writeFloat(10);
        parcel.writeLong(10020202);

        //Log.v(TAG, " saveData ");
    }

    private void loadData() {
        int dataSize = parcel.dataSize();
        parcel.setDataPosition(0);
        float i = parcel.readFloat();
        int pos = parcel.dataPosition();
        Log.v(TAG, " pos =  " + pos);
        long s = parcel.readLong();
        pos = parcel.dataPosition();
        Log.v(TAG, " pos =  " + pos);

        Log.v(TAG, "dataSize = " + dataSize + " i = " + i + " s = " + s);
    }

    public void onClick(View view) {
        switch ( view.getId() ) {
        case R.id.save_id :
            saveData();
            break;
        case R.id.load_id :
            loadData();
            break;
        default :
            break;
        }
    }
}
