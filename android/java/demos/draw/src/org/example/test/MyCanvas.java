package org.example.test;

import android.view.View;
import android.view.KeyEvent;
import android.view.MotionEvent;
import android.view.View.OnClickListener;

import android.content.Context;

import android.graphics.Canvas;
import android.graphics.Rect;
import android.graphics.Paint;
import android.graphics.Color;

import android.util.Log;
import android.util.AttributeSet;

public final class  MyCanvas extends View implements OnClickListener {
    
    private static final String TAG = "";

    private int mWidthElement;
    private int mHeightElement;
    private int mWidthTotal;
    private int mHeightTotal;
    private Paint mPaint;
    private Rect[][] mRectElement;
    //private Rect mSelectedRect;
    private int mSelectedX;
    private int mSelectedY;

    /*private FIXME activity;*/

    public MyCanvas (Context context) {
        super(context);
        /*this.activity = (FIXME) context;*/

        mPaint = new Paint();
        mRectElement = new Rect[4][6];
        mSelectedX = -1;
        mSelectedY = -1;
        mPaint.setColor(Color.argb(255, 255, 0, 0));

        setFocusable(true);
        setFocusableInTouchMode(true);
    }

    public MyCanvas (Context context, AttributeSet attrs) {
		super(context, attrs);

        mPaint = new Paint();
        mRectElement = new Rect[4][6];
        mSelectedX = 0;
        mSelectedY = 0;
        mPaint.setColor(Color.argb(255, 255, 0, 0));

        setFocusable(true);
        setFocusableInTouchMode(true);
	}


    @Override
    protected void onFinishInflate() {
        super.onFinishInflate();

        /*code here*/
    }

    public void onClick(View view) {
        switch ( view.getId() ) {
        //case : 
        //    Intent intent = new Intent(this, // FIXME.class);
        //    startActivity(intent);
        //    break;
        default :
            break;
        }
    }

    private void fillRectElement() {
        int i;
        int j;

        for (i=0; i<4; i++) {
            for (j=0; j<6; j++) {
                mRectElement[i][j] = new Rect(i*mWidthElement, j*mHeightElement, 
                                i*mWidthElement+mWidthElement, j*mHeightElement+mHeightElement);
            }
        }
    }

    private void drawRect(Canvas canvas) {
        if ( (mSelectedX == -1) || (mSelectedY == -1) ) {
            Log.v(TAG, " not selected ");
            return;
        }

        Paint paint = new Paint();

        paint.setColor(Color.argb(255, 0, 255, 0));
        canvas.drawRect(mRectElement[mSelectedX][mSelectedY], paint);
    }

    private void drawWidth(Canvas canvas) {
        int i;
        for (i=0; i<7; i++) {
            canvas.drawLine(0, i*mHeightElement, mWidthTotal, i*mHeightElement, mPaint);
        }
    }

    private void drawHeight(Canvas canvas) {
        int i;
        for (i=0; i<5; i++) {
            canvas.drawLine(i*mWidthElement, 0, i*mWidthElement, mHeightTotal, mPaint);
        }
    }

    @Override
    public void onDraw(Canvas canvas) {
        drawWidth(canvas);
        drawHeight(canvas);
        drawRect(canvas);
    }

    @Override
    protected void onSizeChanged(int w, int h, int oldw, int oldh) {
        mWidthElement = w / 4;
        mHeightElement = h / 6;
        mWidthTotal = w;
        mHeightTotal = h;

        fillRectElement();

        super.onSizeChanged(w, h, oldw, oldh);
    }

    private void moveLeft() {
        Log.v(TAG, " move up ");
        if ( mSelectedX == 0 ) {
            return;
        }
        else {
            mSelectedX--;
        }
        invalidate();
    }

    private void moveRight() {
        Log.v(TAG, " move down ");
        if ( mSelectedX == 3 ) {
            return;
        }
        else {
            mSelectedX++;
        }
        invalidate();
    }

    private void moveUp() {
        Log.v(TAG, " move left ");
        if ( mSelectedY == 0 ) {
            return;
        }
        else {
            mSelectedY--;
        }
        invalidate();
    }

    private void moveDown() {
        Log.v(TAG, " move right ");
        if ( mSelectedY == 5 ) {
            return;
        }
        else {
            mSelectedY++;
        }
        invalidate();
    }

    @Override
    public boolean onKeyDown(int keyCode, KeyEvent event) {
        switch ( keyCode ) {
        case KeyEvent.KEYCODE_DPAD_UP : 
            moveUp();
            break;
        case KeyEvent.KEYCODE_DPAD_DOWN :
            moveDown();
            break;
        case KeyEvent.KEYCODE_DPAD_LEFT :
            moveLeft();
            break;
        case KeyEvent.KEYCODE_DPAD_RIGHT :
            moveRight();
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

        int pointX = (int) event.getX();
        int pointY = (int) event.getY();
        /*for (Rect[] rectX : mRectElement) {*/
            //for (Rect rectY : rectX) {
                //if ( rectY.contains(pointX, pointY) ) {
                    //Log.v(TAG, " X: " + pointX);
                    //Log.v(TAG, " Y: " + pointY);
                    //mSelectedRect = new Rect(rectY);
                    //invalidate();
                //}

                //if ( rectY.equals(mSelectedRect) ) {
                    //Log.v(TAG, " selected equals ");
                //}
            //}
        /*}*/

        int i, j;
        for (i=0; i<4; i++) {
            for (j=0; j<6; j++) {
                if ( mRectElement[i][j].contains(pointX, pointY) ) {
                    Log.v(TAG, " X: " + pointX + " Y: " + pointY);
                    mSelectedX = i;
                    mSelectedY = j;
                    invalidate();
                }
            }
        }

        return true;
    }
}

