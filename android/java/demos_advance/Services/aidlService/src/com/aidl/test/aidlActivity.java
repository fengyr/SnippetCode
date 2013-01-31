package com.aidl.test;

import android.app.Activity;
import android.app.Service;
import android.media.AudioManager;
import android.os.Bundle;

public class aidlActivity extends Activity {
    /** Called when the activity is first created. */
    @Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.main);
        
        AudioManager am = (AudioManager) getSystemService(Service.AUDIO_SERVICE);
    }
}