/**
 * @file       AttachmentView.java
 *
 * @revision:  none 
 *
 * @version    0.0.01
 * @author:    Zenki (zhajun), zenki2001cn@163.com
 * @date:      2012-3-6 上午09:16:48 
 */

package com.foxconn.supernote.email.util;

import java.util.ArrayList;
import java.util.List;

import android.content.Context;
import android.util.AttributeSet;
import android.util.Log;
import android.widget.ListView;

/**
 *
 */
public class AttachmentView extends ListView {

	static private final String TAG = "AttachmentView";
	private ArrayList<String> mAttachments;
	private Context mContext;

	/**
	 * @param context
	 * @param attrs
	 * @param defStyle
	 */
	public AttachmentView(Context context, AttributeSet attrs, int defStyle) {
		super(context, attrs, defStyle);
		
		mContext = context;
	}

	/**
	 * @param context
	 * @param attrs
	 */
	public AttachmentView(Context context, AttributeSet attrs) {
		super(context, attrs);
		
		mContext = context;
	}

	/**
	 * @param context
	 */
	public AttachmentView(Context context) {
		super(context);
		
		mContext = context;
	}

	public void setAttachment(List<String> attachs) {
		mAttachments = (ArrayList<String>) attachs;

		initView(mAttachments);
	}

	private void initView(List<String> listFiles) {
		ListViewAdapter adapter = new ListViewAdapter(mContext, listFiles);

		this.setAdapter(adapter);
	}
	
}
