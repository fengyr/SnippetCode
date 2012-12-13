package com.ecrock.imageviewer;

import android.app.Activity;
import android.os.Bundle;
import android.util.Log;
import android.view.View;
import android.view.View.OnClickListener;
import android.view.animation.AnimationUtils;
import android.widget.Button;
import android.widget.ImageSwitcher;
import android.widget.ImageView;
import android.widget.ViewSwitcher;
import android.widget.Gallery.LayoutParams;

public class ImageViewer extends Activity implements ViewSwitcher.ViewFactory,
		OnClickListener {

	private ImageSwitcher mSwitcher;

	private Integer[] mImageIds = { R.drawable.image1, R.drawable.image2,
			R.drawable.image3, R.drawable.image4, R.drawable.image5,
			R.drawable.image6, R.drawable.image7, R.drawable.image8 };

	int ii = 1;

	/** Called when the activity is first created. */
	@Override
	public void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setContentView(R.layout.main);

		mSwitcher = (ImageSwitcher) findViewById(R.id.switcher);
		mSwitcher.setFactory(this);

		prev = (Button) findViewById(R.id.prev);
		next = (Button) findViewById(R.id.next);

		prev.setOnClickListener(this);
		next.setOnClickListener(this);
	}

	Button prev, next;

	// 

	public View makeView() {
		ImageView i = new ImageView(this);
		i.setBackgroundColor(0xFF000000);
		i.setScaleType(ImageView.ScaleType.FIT_CENTER);
		i.setLayoutParams(new ImageSwitcher.LayoutParams(
				LayoutParams.FILL_PARENT, LayoutParams.FILL_PARENT));
		return i;
	}

	public void onClick(View v) {
		// TODO Auto-generated method stub
		switch (v.getId()) {
		case R.id.prev:
			Log.i("", "R.id.prev");
			if (ii == 0) {
				ii = 7;
			} else {
				ii--;
			}
			mSwitcher.setInAnimation(AnimationUtils.loadAnimation(this,
					R.anim.push_left_in));
			mSwitcher.setOutAnimation(AnimationUtils.loadAnimation(this,
					R.anim.push_right_out));
			mSwitcher.setImageResource(mImageIds[ii]);
			break;
		case R.id.next:
			Log.i("", "R.id.next");
			if (ii == 7) {
				ii = 0;
			} else {
				ii++;
			}
			mSwitcher.setInAnimation(AnimationUtils.loadAnimation(this,
					R.anim.push_right_in));
			mSwitcher.setOutAnimation(AnimationUtils.loadAnimation(this,
					R.anim.push_left_out));
			mSwitcher.setImageResource(mImageIds[ii]);
			break;
		}
	}

}
