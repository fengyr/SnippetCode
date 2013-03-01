package org.example.test; 

import android.view.View;
import android.view.KeyEvent;
import android.view.MotionEvent;
import android.view.View.OnClickListener;

import android.content.Context;

import android.graphics.Canvas;
import android.graphics.Rect;
import android.graphics.Paint;

import android.util.Log;
import android.util.AttributeSet;
import android.widget.LinearLayout;

import android.widget.Toast;
import android.widget.TextView;
import android.widget.Button;

public final class  MyRelativeLayout extends LinearLayout implements OnClickListener {
    
    private static final String TAG = "MyRelativeLayout";

    private MyTest activity;
    private TextView tv;
    private Button btn;

    public MyRelativeLayout(Context context) {
        super(context);
        this.activity = (MyTest) context;

        /*code here*/

        setFocusable(true);
        setFocusableInTouchMode(true);
    }

    public MyRelativeLayout(Context context, AttributeSet attrs) {
		super(context, attrs);
	}


    @Override
    protected void onFinishInflate() {
        super.onFinishInflate();

        tv = (TextView) findViewById(R.id.tv_id);
        btn = (Button) findViewById(R.id.button_id);

        tv.setOnClickListener(this);
        btn.setOnClickListener(this);

        Log.v(TAG, " finish inflate ");
    }

    public void onClick(View view) {
        Log.v(TAG, " on click ");

        switch ( view.getId() ) {
        case R.id.tv_id: 
            Log.v(TAG, " view press ");
            Toast.makeText(activity, "Test", Toast.LENGTH_LONG).show();
            break;
        case R.id.button_id: 
            Log.v(TAG, " button press ");
            Toast.makeText(activity, "Test", Toast.LENGTH_LONG).show();
            break;
        default :
            break;
        }
    }

    @Override
    public void onDraw(Canvas canvas) {
        /*code here*/

    }

    @Override
    protected void onSizeChanged(int w, int h, int oldw, int oldh) {
        /*code here*/

        super.onSizeChanged(w, h, oldw, oldh);
    }

    @Override
    public boolean onKeyDown(int keyCode, KeyEvent event) {

        switch ( keyCode ) {
        case KeyEvent.KEYCODE_DPAD_UP : 
            Log.e(TAG, " key up ");
           break;
        case KeyEvent.KEYCODE_1 :
           /*code here*/
           break;
        case KeyEvent.KEYCODE_A :
           /*code here*/
           break;
        case KeyEvent.KEYCODE_ALT_LEFT :
           /*code here*/
           break;
        case KeyEvent.KEYCODE_AT :
           /*code here*/
           break;
        case KeyEvent.KEYCODE_BACK :
           /*code here*/
           break;
        case KeyEvent.KEYCODE_CALL :
           /*code here*/
           break;
        default :
            return super.onKeyDown(keyCode, event);
        }

        return true;
    }

    @Override
    public boolean onTouchEvent(MotionEvent event) {
        if ( event.getAction() != MotionEvent.ACTION_DOWN ) {
            return super.onTouchEvent(event);
        }

        /*code here*/

        return true;
    }
}

