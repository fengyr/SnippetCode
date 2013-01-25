package com.test;

import android.app.Activity;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.graphics.Matrix;
import android.graphics.drawable.Drawable;
import android.os.Bundle;
import android.view.View;
import android.widget.Button;
import android.widget.ImageView;

public class ImageViewActivity extends Activity {

	private ImageView mImageView01;
	private ImageView mImageView02;
	private Button mButton01;
	private Button mButton02;

	public void onCreate(Bundle savedInstanceState) {

		super.onCreate(savedInstanceState);
		setContentView(R.layout.main);

		mImageView01 = (ImageView) findViewById(R.id.myImageView1);
		mImageView02 = (ImageView) findViewById(R.id.myImageView2);
		mButton01 = (Button) findViewById(R.id.myButton1);
		mButton02 = (Button) findViewById(R.id.myButton2);

		mImageView01.setImageDrawable(getResources().getDrawable(
				R.drawable.right));
		mImageView02
				.setImageDrawable(getResources().getDrawable(R.drawable.oa));

		mButton01.setOnClickListener(new Button.OnClickListener() {
			public void onClick(View v) {
				Matrix m = new Matrix();
				final float f4[] = {1.0f, 0.0f, 0.0f,
									0.0f, -1.0f, 0.0f,
									0.0f, 0.0f, 1.0f};
				m.setValues(f4);
//				m.preTranslate(120, 180);
//				matrix.preRotate(90);
//				matrix.preSkew(2, 2);
//				mImageView01.setImageMatrix(matrix);
//
//				mImageView01.setImageDrawable(getResources().getDrawable(
//						R.drawable.right));
				Bitmap tmp = BitmapFactory.decodeResource(getResources(), R.drawable.right);
				Bitmap bmp = Bitmap.createBitmap(tmp, 0, 0, tmp.getWidth(), tmp.getHeight(), m, true);
				mImageView01.setImageBitmap(bmp);
			}
		});

		mButton02.setOnClickListener(new Button.OnClickListener() {
			@Override
			public void onClick(View v) {
				mImageView01.setImageDrawable(getResources().getDrawable(
						R.drawable.left));
			}
		});
	}
}