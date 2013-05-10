/**
 * @file       ToolBar.java
 *
 * @revision:  none 
 *
 * @version    0.0.01
 * @author:    Zenki (zhajun), junzha@pateo.com.cn
 * @date:      2013-4-25 上午10:56:44 
 */

package com.pateo.as.photo.view;

import com.pateo.as.photo.R;
import com.pateo.as.photo.activity.DisplayActivity.PhotoViewHolder;

import android.content.Context;
import android.util.AttributeSet;
import android.view.View;
import android.widget.ImageButton;
import android.widget.LinearLayout;

/**
 *
 */
public class ToolBar extends LinearLayout {

	private ImageButton mBtnPreImage;
	private ImageButton mBtnNextImage;
	private ImageButton mBtnRotateLeft;
	private ImageButton mBtnRotateRight;
	private ImageButton mBtnNavigate;
	private ImageButton mBtnFullscreen;
	private ImageButton mBtnSlide;
	private ImageButton mBtnDelete;
	private PhotoViewHolder mPhotoViewHolder;
	private ToolBarAdapter mToolBarAdapter;

	/**
	 * @param context
	 * @param attrs
	 * @param defStyle
	 */
	public ToolBar(Context context) {
		super(context);
	}

	/**
	 * @param context
	 * @param attrs
	 * @param defStyle
	 */
	public ToolBar(Context context, AttributeSet attrs) {
		super(context, attrs);
	}

	/**
	 * @param context
	 * @param attrs
	 * @param defStyle
	 */
	public ToolBar(Context context, AttributeSet attrs, int defStyle) {
		super(context, attrs, defStyle);
	}

	public void setPhotoViewHolder(PhotoViewHolder holder) {
		mPhotoViewHolder = holder;
	}
	
	public void setController(ToolBarAdapter controller) {
		mToolBarAdapter = controller;
	}

	public void initViews() {
		mBtnPreImage = (ImageButton) findViewById(R.id.id_toolbar_btn_pre);
		mBtnNextImage = (ImageButton) findViewById(R.id.id_toolbar_btn_next);
		mBtnRotateLeft = (ImageButton) findViewById(R.id.id_toolbar_btn_rotate_left);
		mBtnRotateRight = (ImageButton) findViewById(R.id.id_toolbar_btn_rotate_right);
		mBtnNavigate = (ImageButton) findViewById(R.id.id_toolbar_btn_navigate);
		mBtnFullscreen = (ImageButton) findViewById(R.id.id_toolbar_btn_fullscreen);
		mBtnSlide = (ImageButton) findViewById(R.id.id_toolbar_btn_slide);
		mBtnDelete = (ImageButton) findViewById(R.id.id_toolbar_btn_delete);
		
		mBtnPreImage.setOnClickListener(listener);
		mBtnNextImage.setOnClickListener(listener);
		mBtnRotateLeft.setOnClickListener(listener);
		mBtnRotateRight.setOnClickListener(listener);
		mBtnNavigate.setOnClickListener(listener);
		mBtnFullscreen.setOnClickListener(listener);
		mBtnSlide.setOnClickListener(listener);
		mBtnDelete.setOnClickListener(listener);
	}
	
	public void setNavigateStatus(boolean enabled) {
		if (mBtnNavigate != null) {
			mBtnNavigate.setEnabled(enabled);
		}
	}
	
	public void exitSlideMode() {
		mBtnSlide.setImageResource(R.drawable.toolbar_btn_slide_selector);
		mBtnFullscreen.setEnabled(true);
		mBtnDelete.setEnabled(true);
	}
	
	public void enterSlideMode() {
		mBtnFullscreen.setEnabled(false);
		mBtnDelete.setEnabled(false);
	}
	
	private OnClickListener listener = new OnClickListener() {

		@Override
		public void onClick(View v) {
			int id = v.getId();
			switch (id) {
			case R.id.id_toolbar_btn_pre:
				if (mToolBarAdapter != null) {
					mToolBarAdapter.displayPrePhoto();
				}
				break;
			case R.id.id_toolbar_btn_next:
				if (mToolBarAdapter != null) {
					mToolBarAdapter.displayNextPhoto();
				}
				break;
			case R.id.id_toolbar_btn_rotate_left:
				if (mToolBarAdapter != null) {
					mToolBarAdapter.rotateLeftPhoto();
				}
				break;
			case R.id.id_toolbar_btn_rotate_right:
				if (mToolBarAdapter != null) {
					mToolBarAdapter.rotateRightPhoto();
				}
				break;
			case R.id.id_toolbar_btn_navigate:
				if (mToolBarAdapter != null) {
					mToolBarAdapter.navigate();
				}
				break;
			case R.id.id_toolbar_btn_fullscreen:
				if (mToolBarAdapter != null) {
					boolean isFullScreen = mToolBarAdapter.fullScreen();
					if (isFullScreen) {
						mBtnFullscreen.setImageResource(R.drawable.toolbar_btn_fullscreen_pressed);
					} else {
						mBtnFullscreen.setImageResource(R.drawable.toolbar_btn_fullscreen_selector);
					}
				}
				break;
			case R.id.id_toolbar_btn_slide:
				if (mToolBarAdapter != null) {
					boolean isSlideMode = mToolBarAdapter.slideMode();
					if (isSlideMode) {
						mBtnSlide.setImageResource(R.drawable.toolbar_btn_slide_pressed);
					} else {
						mBtnSlide.setImageResource(R.drawable.toolbar_btn_slide_selector);
						mBtnFullscreen.setImageResource(R.drawable.toolbar_btn_fullscreen_selector);
					}
				}
				break;
			case R.id.id_toolbar_btn_delete:
				if (mToolBarAdapter != null) {
					mToolBarAdapter.deletePhoto();
				}
				break;

			default:
				break;
			}
		}
	};
}
