/**
 * @file       MyView.java
 *
 * @revision:  none 
 *
 * @version    0.0.01
 * @author:    Zenki (zhajun), junzha@pateo.com.cn
 * @date:      2013-1-17 下午03:04:14 
 */

package com.example.testcustomattrbuite;

import android.content.Context;
import android.content.res.TypedArray;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.graphics.Canvas;
import android.graphics.Paint;
import android.graphics.PixelFormat;
import android.graphics.drawable.Drawable;
import android.util.AttributeSet;
import android.view.View;

/**
 *
 */
public class MyView extends View {

	/**
	 * @param context
	 * @param attrs
	 * @param defStyle
	 */
	public MyView(Context context, AttributeSet attrs, int defStyle) {
		super(context, attrs, defStyle);
		initResource(context, attrs, defStyle);
	}

	/**
	 * @param context
	 * @param attrs
	 */
	public MyView(Context context, AttributeSet attrs) {
		this(context, attrs, R.attr.myViewStyle);
	}

	/**
	 * @param context
	 */
	public MyView(Context context) {
		super(context);
	}

	static private final String TAG = "MyView";

	private String mText;
	private Paint mPaint;
	private int mOriental = -1;
	private Bitmap mBitmap;

	private void initResource(Context context, AttributeSet attrs, int defStyle) {
		getAttrMethod(attrs, defStyle);

		mPaint = new Paint();
		mPaint.setColor(R.color.text_color);
	}

	private void getAttrMethod(AttributeSet attrs, int defStyle) {
		int resourceId = -1;

		TypedArray typeArray = getContext().obtainStyledAttributes(attrs,
				R.styleable.MyView, defStyle, R.style.MyViewDefaultStyle);
		// Other method to get attributes
		// mText = typeArray.getText(R.styleable.MyView_TextAttrExt).toString();
		// mOriental = typeArray.getInt(R.styleable.MyView_OrientalAttrExt, 0);
		// mBitmap =
		// drawableToBitmap(typeArray.getDrawable(R.styleable.MyView_BackgroundAttrExt));
		int count = typeArray.getIndexCount();
		for (int i = 0; i < count; i++) {
			int attr = typeArray.getIndex(i);
			resourceId = typeArray.getResourceId(attr, 0);

			switch (attr) {
			case R.styleable.MyView_TextAttrExt:
				mText = typeArray.getResources().getText(resourceId).toString();
				break;
			case R.styleable.MyView_OrientalAttrExt:
				mOriental = typeArray.getInt(attr, 0);
				break;
			case R.styleable.MyView_BackgroundAttrExt:
				mBitmap = BitmapFactory.decodeResource(
						typeArray.getResources(), resourceId);
				break;

			default:
				break;
			}
		}

		typeArray.recycle();
	}

	private Bitmap drawableToBitmap(Drawable drawable) {

		Bitmap bitmap = Bitmap
				.createBitmap(
						drawable.getIntrinsicWidth(),
						drawable.getIntrinsicHeight(),
						drawable.getOpacity() != PixelFormat.OPAQUE ? Bitmap.Config.ARGB_8888
								: Bitmap.Config.RGB_565);
		Canvas canvas = new Canvas(bitmap);
		drawable.setBounds(0, 0, drawable.getIntrinsicWidth(),
				drawable.getIntrinsicHeight());
		drawable.draw(canvas);
		return bitmap;
	}

	@Override
	protected void onDraw(Canvas canvas) {
		super.onDraw(canvas);

		mText = mText + String.valueOf(mOriental);

		canvas.drawText(mText, 200, 200, mPaint);

		if (mBitmap != null) {
			canvas.drawBitmap(mBitmap, 0, 0, mPaint);
		}
	}
}
