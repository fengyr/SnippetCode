/**
 * @file       AsCheckBox.java
 *
 * @revision:  none 
 *
 * @version    0.0.01
 * @author:    Zenki (zhajun), junzha@pateo.com.cn
 * @date:      2013-3-5 下午03:18:40 
 */

package com.pateo.as.ascheckbox;

import android.content.Context;
import android.graphics.Color;
import android.util.AttributeSet;
import android.util.Log;
import android.view.Gravity;
import android.widget.CompoundButton.OnCheckedChangeListener;
import android.widget.LinearLayout;
import android.widget.TextView;

/**
 *
 */
public class AsCheckBox extends LinearLayout {

	static private final String TAG = "AsCheckBox";
	private TextView mTextView;
	private AsCheckBoxInner mCheckBox;
	private String mTextContent;

	/**
	 * @param context
	 * @param attrs
	 * @param defStyle
	 */
	public AsCheckBox(Context context) {
		super(context);
	}

	/**
	 * @param context
	 * @param attrs
	 * @param defStyle
	 */
	public AsCheckBox(Context context, AttributeSet attrs) {
		super(context, attrs);
		
		initAttributeSetValue(context, attrs);
		initView(context, attrs);
	}

	/**
	 * @param context
	 * @param attrs
	 * @param defStyle
	 */
	public AsCheckBox(Context context, AttributeSet attrs, int defStyle) {
		super(context, attrs);
		
		initAttributeSetValue(context, attrs);
		initView(context, attrs);
	}
	
	private void initAttributeSetValue(Context context, AttributeSet attrs) {
		int count = attrs.getAttributeCount();
		String name;
		for (int i = 0; i < count; i++) {
			name = attrs.getAttributeName(i);
			if (name.equals("text")) {
				int resouceId = Integer.valueOf(attrs.getAttributeValue(i).substring(1));
		        if (resouceId > 0) {
		        	mTextContent = context.getResources().getText(resouceId).toString();
		        }
			}
		}
	}

	private void initView(Context context, AttributeSet attrs) {
		mTextView = new TextView(context);
		LayoutParams lp = new LayoutParams(LinearLayout.LayoutParams.WRAP_CONTENT, LinearLayout.LayoutParams.WRAP_CONTENT);
		mTextView.setText(mTextContent);
		mTextView.setTextSize(26);
		mTextView.setTextColor(0xffafafaf);
		mTextView.setBackgroundColor(Color.TRANSPARENT);
		addView(mTextView, lp);
		
		mCheckBox = new AsCheckBoxInner(context, null, R.attr.AsCheckBoxStyle);
		lp = new LayoutParams(LinearLayout.LayoutParams.WRAP_CONTENT, LinearLayout.LayoutParams.WRAP_CONTENT);
		lp.leftMargin = 20;
		addView(mCheckBox, lp);

		this.setBackgroundResource(R.drawable.checkbox_text_background);
		this.setGravity(Gravity.CENTER);
	}

	public boolean isChecked() {
		return mCheckBox.isChecked();
	}
	
	public void setChecked(boolean checked) {
		mCheckBox.setChecked(checked);
	}
	
	public void setText(CharSequence text) {
		mTextView.setText(text);
	}
	
	public CharSequence getText() {
		return mTextView.getText();
	}
	
	public void setOnClickListener(OnClickListener l) {
		mCheckBox.setOnClickListener(l);
	}
	
	public void setOnCheckedChangeListener(OnCheckedChangeListener l) {
		mCheckBox.setOnCheckedChangeListener(l);
	}
}
