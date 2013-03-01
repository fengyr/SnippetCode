package org.example.test;

import android.view.View;
import android.view.KeyEvent;
import android.view.MotionEvent;

import android.content.Context;

import android.graphics.Canvas;
import android.graphics.Rect;
import android.graphics.Paint;

import android.util.Log;

public class MyView extends View {
    
    private static final String TAG = "";

    private MyTest activity;

    public MyView(Context context) {
        super(context);
        this.activity = (MyTest) context;

        setFocusable(true);
        setFocusableInTouchMode(true);
    }

    @Override
    public void onDraw(Canvas canvas) {

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
           /*code here*/
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

