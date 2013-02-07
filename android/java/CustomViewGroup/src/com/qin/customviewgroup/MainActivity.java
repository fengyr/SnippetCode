package com.qin.customviewgroup;

import android.app.Activity;
import android.os.Bundle;
import android.view.View;
import android.widget.Button;
import android.widget.TextView;

public class MainActivity extends Activity {

	private Button btn;
	private TextView txt;
	private MyViewGroup myViewGroup;

	/** Called when the activity is first created. */
	@Override
	public void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setContentView(R.layout.main);

		btn = (Button) findViewById(R.id.btn);
		txt = (TextView) findViewById(R.id.txt);
		myViewGroup = (MyViewGroup) findViewById(R.id.custemViewGroup);

		btn.setOnClickListener(new View.OnClickListener() {

			@Override
			public void onClick(View v) {
				myViewGroup.requestFocus();
			}
		});

	}
}