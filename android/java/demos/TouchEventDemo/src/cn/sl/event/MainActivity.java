package cn.sl.event;

import android.app.Activity;
import android.os.Bundle;
import android.view.ViewGroup.LayoutParams;

public class MainActivity extends Activity {
	Group1 group1;
	Group2 group2;
	MyTextView myTv;

	/** Called when the activity is first created. */
	@Override
	public void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		
		
		//--group1
		//----|
		//-------group2
		//---------|
		//------------myTv
		
		group1 = new Group1(this);
		group2 = new Group2(this);
		myTv = new MyTextView(this);
		group2.addView(myTv, new LayoutParams(LayoutParams.FILL_PARENT,
				LayoutParams.FILL_PARENT));
		group1.addView(group2, new LayoutParams(LayoutParams.FILL_PARENT,
				LayoutParams.FILL_PARENT));
		setContentView(group1);
	}
}