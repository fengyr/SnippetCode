/**
 * @file       TestPaintView.java
 *
 * @revision:  none 
 *
 * @version    0.0.01
 * @author:    Zenki (zhajun), zenki2001cn@163.com
 * @date:      2010-11-27 下午12:58:57 
 */

package com.zenki.bitmapdraw;

import java.io.File;
import java.io.FileOutputStream;

import android.content.Context;
import android.graphics.Bitmap;
import android.graphics.Canvas;
import android.graphics.Paint;
import android.graphics.Path;
import android.util.AttributeSet;
import android.util.Log;
import android.view.MotionEvent;
import android.view.View;

/**
 *
 */
public class TestPaintView extends View {
	
	static private final String TAG = "TestPaintView";
	private int WIDTH = 640;
	private int HEIGHT = 480;
	
	private Bitmap mBitmap = null;
	private Canvas mCanvas = null;
	private Paint mPaint = null;
	private Path mPath = null;
	private float mX = -1;
	private float mY = -1;
	
	public TestPaintView(Context context, AttributeSet attrs) {
		super(context, attrs);
		
		mPath = new Path();
		
		mBitmap = Bitmap.createBitmap(WIDTH, HEIGHT, Bitmap.Config.ARGB_8888);
		mBitmap.eraseColor(0xffffffff);
		
		mPaint = new Paint();
		mPaint.setColor(0xff000000);
		mPaint.setDither(false);
		mPaint.setStyle(Paint.Style.STROKE);
		mPaint.setStrokeJoin(Paint.Join.ROUND);
		mPaint.setStrokeCap(Paint.Cap.ROUND);
		mPaint.setStrokeWidth(10);
		
		mCanvas = new Canvas(mBitmap);
	}
	
	public boolean saveBitmap() {
		boolean ret = false;
		
		if (mBitmap != null) {
//			File file = new File("/mnt/sdcard/test.png");
//			try {
//				FileOutputStream fos = new FileOutputStream(file);
//				ret = mBitmap.compress(Bitmap.CompressFormat.PNG, 100, fos);
//				fos.close();
//			} catch (Exception e) {
//				Log.v(TAG, "saveBitmap" + e);
//			}
			invalidate();
		}
		return ret;
	}
	
	/* (non-Javadoc)
	 * @see android.view.View#onDraw(android.graphics.Canvas)
	 */
	@Override
	protected void onDraw(Canvas canvas) {
		if (!mBitmap.isRecycled()) {
			canvas.drawBitmap(mBitmap, 0, 0, mPaint);
			canvas.drawPath(mPath, mPaint);
		}
//		super.onDraw(canvas);
	}
	
	/* (non-Javadoc)
	 * @see android.view.View#onTouchEvent(android.view.MotionEvent)
	 */
	@Override
	public boolean onTouchEvent(MotionEvent event) {
		float x = event.getX();
		float y = event.getY();
		
		switch (event.getAction()) {
		case MotionEvent.ACTION_DOWN:
			touchStart(x, y);
			break;
		case MotionEvent.ACTION_MOVE:
			touchMove(x, y);
			break;
		case MotionEvent.ACTION_UP:
			touchEnd(x, y);
			break;

		default:
			break;
		}
		return true;
	}
	
	private void touchStart(float x, float y) {
		Log.v(TAG, "touchStart: " + x + y);
		mPath.reset();
		mPath.moveTo(x, y);
    	mX = x;
    	mY = y;
	}
	
	private void touchMove(float x, float y) {
		Log.v(TAG, "touchMove" + x + y);
//    	float dx = Math.abs(x - mX);
//    	float dy = Math.abs(y - mY);
//    	if (dx >= 0 || dy >= 0) {
//    		mPath.quadTo(mX, mY, (x + mX)/2, (y + mY)/2);
//    	}
    	mX = x;
    	mY = y;
    	
    	mPath.lineTo(mX, mY);
    	mCanvas.drawPath(mPath, mPaint);
	}
	
	private void touchEnd(float x, float y) {
		Log.v(TAG, "touchEnd" + x + y);
		mPath.lineTo(x, y);
		mCanvas.drawPath(mPath, mPaint);
		mPath.reset();
	}
}
