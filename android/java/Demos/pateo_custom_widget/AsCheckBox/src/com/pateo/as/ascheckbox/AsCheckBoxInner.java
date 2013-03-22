/**
 * @file       AsCheckBoxInner.java
 *
 * @revision:  none 
 *
 * @version    0.0.01
 * @author:    Zenki (zhajun), junzha@pateo.com.cn
 * @date:      2013-3-22 上午09:11:15 
 */

package com.pateo.as.ascheckbox;

import android.content.Context;
import android.util.AttributeSet;
import android.widget.CheckBox;

/**
 *
 */
public class AsCheckBoxInner extends CheckBox {

	/**
	 * @param context
	 * @param attrs
	 * @param defStyle
	 */
	public AsCheckBoxInner(Context context) {
		super(context);
	}
	
	/**
	 * @param context
	 * @param attrs
	 * @param defStyle
	 */
	public AsCheckBoxInner(Context context, AttributeSet attrs) {
		this(context, attrs, R.attr.AsCheckBoxStyle);
	}
	
	/**
	 * @param context
	 * @param attrs
	 * @param defStyle
	 */
	public AsCheckBoxInner(Context context, AttributeSet attrs, int defStyle) {
		super(context, attrs, defStyle);
	}

}
