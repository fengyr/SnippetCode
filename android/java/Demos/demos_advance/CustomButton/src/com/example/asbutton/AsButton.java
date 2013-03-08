/**
 * @file       AsMenu.java
 *
 * @revision:  none 
 *
 * @version    0.0.01
 * @author:    Zenki (zhajun), junzha@pateo.com.cn
 * @date:      2013-3-4 上午10:50:35 
 */

package com.example.asbutton;

import android.content.Context;
import android.content.res.TypedArray;
import android.graphics.Canvas;
import android.graphics.Paint;
import android.graphics.Paint.FontMetrics;
import android.util.AttributeSet;
import android.util.Log;
import android.widget.Button;

/**
 *
 */
public class AsButton extends Button {

	static private final String TAG = "AsButton";
	private Paint mPaint;
	private int mBackgroundColor = 0xff444446;
	private int mTextColor = 0xffffffff;
	private int mStroke = 1;
	private int mHorPadding = 0;
	private int mVerPadding = 0;
	private float[] mLinesPoint;
	private final int mDefaultWidth = 100;
	private final int mDefaultHeight = 50;

	/**
	 * @param context
	 * @param attrs
	 * @param defStyle
	 */
	public AsButton(Context context) {
		super(context);
		Log.d(TAG, "AsButton(Context context)");
	}

	/**
	 * @param context
	 * @param attrs
	 * @param defStyle
	 */
	public AsButton(Context context, AttributeSet attrs) {
		this(context, attrs, 0);
		Log.d(TAG, "AsButton(Context context, AttributeSet attrs)");
	}

	/**
	 * @param context
	 * @param attrs
	 * @param defStyle
	 */
	public AsButton(Context context, AttributeSet attrs, int defStyle) {
		super(context, attrs, defStyle);
		
		TypedArray a = context.obtainStyledAttributes(attrs,  
                R.styleable.AsButton);  
          
        mBackgroundColor = a.getColor(R.styleable.AsButton_BackGround,  
        		mBackgroundColor);
        mTextColor = a.getColor(R.styleable.AsButton_TextColor, mTextColor);
        Log.d(TAG, "mBackgroundColor="+mBackgroundColor + " mTextColor="+mTextColor);
          
        a.recycle();
	}

	@Override
	protected void onAttachedToWindow() {
		super.onAttachedToWindow();

		initDefaultAttr();
		initDrawer();
	}

	private void initDefaultAttr() {
		setBackgroundColor(mBackgroundColor);
		setTextColor(mTextColor);
	}

	@Override
	protected void onMeasure(int widthMeasureSpec, int heightMeasureSpec) {
		super.onMeasure(widthMeasureSpec, heightMeasureSpec);
		int widthMode = MeasureSpec.getMode(widthMeasureSpec);
		int heightMode = MeasureSpec.getMode(heightMeasureSpec);

		int widthSize = MeasureSpec.getSize(widthMeasureSpec);
		int heightSize = MeasureSpec.getSize(heightMeasureSpec);

		if (widthMode == MeasureSpec.EXACTLY ) {
			Paint paint = getPaint();
			float textWidth = paint.measureText((String) getText());
			FontMetrics fm = paint.getFontMetrics();
			float textHeight = (float) Math.ceil(fm.descent - fm.ascent);

			mHorPadding = (int) ((widthSize - textWidth) / 2);
			mVerPadding = (int) ((heightSize - textHeight) / 2);
		}
		
		if (widthMode == MeasureSpec.AT_MOST || widthMode == MeasureSpec.UNSPECIFIED) {
			widthSize = mDefaultWidth;
			heightSize = mDefaultHeight;
			
			Paint paint = getPaint();
			float textWidth = paint.measureText((String) getText());
			FontMetrics fm = paint.getFontMetrics();
			float textHeight = (float) Math.ceil(fm.descent - fm.ascent);

			mHorPadding = (int) ((widthSize - textWidth) / 2);
			mVerPadding = (int) ((heightSize - textHeight) / 2);
		}
		
		setPadding(mHorPadding, mVerPadding, mHorPadding, mVerPadding);

		mLinesPoint = new float[] { mStroke, mStroke, mStroke,
				heightSize - mStroke, mStroke, heightSize - mStroke,
				widthSize - mStroke, heightSize - mStroke, widthSize - mStroke,
				heightSize - mStroke, widthSize - mStroke, mStroke,
				widthSize - mStroke, mStroke, mStroke, mStroke };

		setMeasuredDimension(widthSize, heightSize);
	}

	@Override
	protected void onDraw(Canvas canvas) {
		canvas.drawLines(mLinesPoint, mPaint);

		super.onDraw(canvas);
	}

	private void initDrawer() {
		mPaint = getPaint();
		mPaint.setColor(0xffffffff);
		mPaint.setAntiAlias(true);
		mPaint.setStrokeWidth(mStroke);
	}
}
