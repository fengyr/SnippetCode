/**
 * @file       AsEditText.java
 *
 * @revision:  none 
 *
 * @version    0.0.01
 * @author:    Zenki (zhajun), junzha@pateo.com.cn
 * @date:      2013-3-12 上午10:12:45 
 */

package com.pateo.as.edittext;

import android.content.Context;
import android.content.res.TypedArray;
import android.graphics.Rect;
import android.util.AttributeSet;
import android.util.Log;
import android.view.MotionEvent;
import android.widget.EditText;

/**
 *
 */
public class AsEditText extends EditText {

	static private final String TAG = "AsEditText";

	/**
	 * @param context
	 * @param attrs
	 * @param defStyle
	 */
	public AsEditText(Context context) {
		super(context);
	}

	/**
	 * @param context
	 * @param attrs
	 * @param defStyle
	 */
	public AsEditText(Context context, AttributeSet attrs) {
		this(context, attrs, R.attr.AsEditTextStyle);
		initWidget();
	}

	/**
	 * @param context
	 * @param attrs
	 * @param defStyle
	 */
	public AsEditText(Context context, AttributeSet attrs, int defStyle) {
		super(context, attrs, defStyle);

		initWidget();
	}

	private void initWidget() {
		// this.setBackgroundResource(R.drawable.edit_text_bg);
		// this.setTextColor(FOCUS_COLOR);
	}
}
