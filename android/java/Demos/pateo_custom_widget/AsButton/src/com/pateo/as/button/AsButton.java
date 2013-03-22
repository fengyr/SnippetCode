/**
 * @file       Button.java
 *
 * @revision:  none 
 *
 * @version    0.0.01
 * @author:    Zenki (zhajun), junzha@pateo.com.cn
 * @date:      2013-3-5 下午01:39:38 
 */

package com.pateo.as.button;

import android.content.Context;
import android.util.AttributeSet;
import android.view.Gravity;
import android.widget.Button;

/**
 *
 */
public class AsButton extends Button {

	/**
	 * @param context
	 * @param attrs
	 * @param defStyle
	 */
	public AsButton(Context context) {
		super(context);
	}
	
	/**
	 * @param context
	 * @param attrs
	 * @param defStyle
	 */
	public AsButton(Context context, AttributeSet attrs) {
		super(context, attrs, 0);
		initWidget();
	}
	
	/**
	 * @param context
	 * @param attrs
	 * @param defStyle
	 */
	public AsButton(Context context, AttributeSet attrs, int defStyle) {
		super(context, attrs, defStyle);
		initWidget();
	}
	
	private void initWidget() {
		setFocusable(true);
		setBackgroundResource(R.drawable.button_selector);
		setGravity(Gravity.CENTER);
		setTextSize(24);
		setTextColor(0xffffffff);
	}
}
