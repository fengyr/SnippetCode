/**
 * @file       PhotoView.java
 *
 * @revision:  none 
 *
 * @version    0.0.01
 * @author:    Zenki (zhajun), junzha@pateo.com.cn
 * @date:      2013-4-25 上午10:53:53 
 */

package com.pateo.as.photo.view;

import java.io.File;
import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.IOException;

import com.pateo.as.photo.model.Photo;

import android.content.Context;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.graphics.Matrix;
import android.util.AttributeSet;
import android.util.Log;
import android.widget.ImageView;

/**
 *
 */
public class PhotoView extends ImageView {

	static private final String TAG = "PhotoView";
	private Bitmap mPhotoBmp;
	private Photo mPhoto;
	private boolean mModified;

	/**
	 * @param context
	 * @param attrs
	 * @param defStyle
	 */
	public PhotoView(Context context) {
		super(context);
	}

	/**
	 * @param context
	 * @param attrs
	 * @param defStyle
	 */
	public PhotoView(Context context, AttributeSet attrs) {
		this(context, attrs, 0);
	}

	/**
	 * @param context
	 * @param attrs
	 * @param defStyle
	 */
	public PhotoView(Context context, AttributeSet attrs, int defStyle) {
		super(context, attrs, defStyle);
	}

	public void setPhoto(Photo photo) {
		mPhoto = photo;
		mModified = false;
		loadPhoto(photo);
	}
	
	@Override
	protected void onMeasure(int widthMeasureSpec, int heightMeasureSpec) {
		super.onMeasure(widthMeasureSpec, heightMeasureSpec);  
        int widthMode = MeasureSpec.getMode(widthMeasureSpec);  
        int heightMode = MeasureSpec.getMode(heightMeasureSpec);  
          
        int widthSize = MeasureSpec.getSize(widthMeasureSpec);  
        int heightSize = MeasureSpec.getSize(heightMeasureSpec);
        
        if(widthMode == MeasureSpec.AT_MOST){  
            Log.e(TAG,"LayoutA onMeasure() called,widthMode = MeasureSpec.AT_MOST,widthSize = " + widthSize + " ,heightSize = " + heightSize);  
        }  
          
        if(widthMode == MeasureSpec.EXACTLY){  
            Log.e(TAG,"LayoutA onMeasure() called,widthMode = MeasureSpec.EXACTLY,widthSize = " + widthSize + " ,heightSize = " + heightSize);  
        }  
          
        if(widthMode == MeasureSpec.UNSPECIFIED){  
            Log.e(TAG,"LayoutA onMeasure() called,widthMode = MeasureSpec.UNSPECIFIED,widthSize = " + widthSize + " ,heightSize = " + heightSize);  
        }  
          
        if(heightMode == MeasureSpec.AT_MOST){  
        	Log.e(TAG,"LayoutA onMeasure() called,heightMode = MeasureSpec.UNSPECIFIED,widthSize = " + widthSize + " ,heightSize = " + heightSize);
        }
        
        if(heightMode == MeasureSpec.EXACTLY){  
        	Log.e(TAG,"LayoutA onMeasure() called,heightMode = MeasureSpec.EXACTLY,widthSize = " + widthSize + " ,heightSize = " + heightSize);
        }
        
        if(heightMode == MeasureSpec.UNSPECIFIED){  
        	Log.e(TAG,"LayoutA onMeasure() called,heightMode = MeasureSpec.UNSPECIFIED,widthSize = " + widthSize + " ,heightSize = " + heightSize);
        }
	}

	private void loadPhoto(Photo photo) {
		BitmapFactory.Options options = new BitmapFactory.Options();
		options.inPreferredConfig = Bitmap.Config.RGB_565;
		options.inInputShareable = true;
		options.inPurgeable = true;

		mPhotoBmp = BitmapFactory.decodeFile(photo.PATH, options);
		setImageBitmap(mPhotoBmp);
	}

	public boolean isModified() {
		return mModified;
	}
	
	public void restorePhoto() {
		loadPhoto(mPhoto);
	}
	
	public Photo getPhoto() {
		return mPhoto;
	}

	public void savePhoto() {
		try {
			File f = new File(mPhoto.PATH);
			f.createNewFile();
			FileOutputStream fOut = null;
			fOut = new FileOutputStream(f);

			mPhotoBmp.compress(Bitmap.CompressFormat.PNG, 100, fOut);
			fOut.flush();
			fOut.close();
			
			mModified = false;
		} catch (FileNotFoundException e) {
			e.printStackTrace();
		} catch (IOException e) {
			e.printStackTrace();
		}
	}

	public void rotateLeft() {
		Matrix matrix = new Matrix();
		matrix.postRotate(-90);
		
		try {
			mPhotoBmp = Bitmap.createBitmap(mPhotoBmp, 0, 0, mPhotoBmp.getWidth(),
					mPhotoBmp.getHeight(), matrix, true);
			setImageBitmap(mPhotoBmp);
		} catch (Exception e) {
			e.printStackTrace();
		}
		
		mModified = true;
	}

	public void rotateRight() {
		Matrix matrix = new Matrix();
		matrix.postRotate(90);
		
		try {
			mPhotoBmp = Bitmap.createBitmap(mPhotoBmp, 0, 0, mPhotoBmp.getWidth(),
					mPhotoBmp.getHeight(), matrix, true);
			setImageBitmap(mPhotoBmp);
		} catch (Exception e) {
			e.printStackTrace();
		}
		
		mModified = true;
	}
	
	public void fullScreen() {
	}
}
