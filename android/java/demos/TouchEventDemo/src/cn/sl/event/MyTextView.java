package cn.sl.event;

import android.content.Context;
import android.util.Log;
import android.view.Gravity;
import android.view.MotionEvent;
import android.widget.TextView;

public class MyTextView extends TextView {

	public MyTextView(Context context) {
		super(context);
		this.setGravity(Gravity.CENTER);
		this.setText("Touch Event Demo");
		// TODO Auto-generated constructor stub
	}

	@Override
	public boolean onTouchEvent(MotionEvent event) {
		// TODO Auto-generated method stub
		Log.d(Constant.LOGCAT, "MyTextView onTouchEvent "+Constant.getActionTAG(event.getAction()));
		return false;
	}

}
