package org.example.test;

import android.app.Activity;

import android.os.Bundle;

import android.content.SharedPreferences;

import android.preference.Preference;
import android.preference.PreferenceActivity;
import android.preference.CheckBoxPreference;
import android.preference.EditTextPreference;
import android.preference.ListPreference;
import android.preference.PreferenceCategory;
import android.preference.PreferenceScreen;

import android.view.Window;

public class MyTest extends Activity
{
    /** Called when the activity is first created. */
    @Override
    public void onCreate(Bundle savedInstanceState)
    {
        super.onCreate(savedInstanceState);

        requestWindowFeature(Window.FEATURE_CUSTOM_TITLE);

        MyView myview = new MyView(this);
        myview.inflate(this, R.layout.myview, null);
        setContentView(myview);
        //addPreferencesFromResource(R.xml.preference);

        getWindow().setFeatureInt(Window.FEATURE_CUSTOM_TITLE, R.layout.custom_title_1);
    }
}
