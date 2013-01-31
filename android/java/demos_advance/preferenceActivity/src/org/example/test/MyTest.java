package org.example.test;

import android.app.Activity;
import android.os.Bundle;
import android.preference.PreferenceActivity;
import org.example.test.R;

public class MyTest extends PreferenceActivity {

    @Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        addPreferencesFromResource(R.layout.main);
    }
}
