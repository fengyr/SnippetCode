/**
 * @file       ProtobufData.java
 *
 * @revision:  none 
 *
 * @version    0.0.01
 * @author:    Zenki (zhajun), zenki2001cn@163.com
 * @date:      2013-9-30 下午02:56:43 
 */

package com.innovation.nj.colorpickmobile.model;

import org.opencv.android.Utils;
import org.opencv.core.CvType;
import org.opencv.core.Mat;

import android.graphics.Bitmap;
import android.graphics.Bitmap.Config;
import android.util.Log;

import com.google.protobuf.ByteString;

/**
 *
 */
public class ProtobufData {
	static private final String TAG = "ProtobufData";
	
	public int time;
	public long frame_count;
	public int image_class;
	public int class_count;
	public int image_width;
	public int image_height;
	public int process_time;
	public int process_status;
	public int retinue_init;
	public int add_sample_result;
	public int data_type;
	public String add_sample_counts;
	public ByteString image_data;

	public Bitmap decodeImageData() {
		byte[] bmpBytes = image_data.toByteArray();

		Log.e(TAG, "----------width=" + image_width + " --------height="
				+ image_height + " bytes len=" + bmpBytes.length);

		final int rows = image_height;
		final int cols = image_width;

		Mat mat = new Mat(rows, cols, CvType.CV_8UC3);
		mat.put(0, 0, bmpBytes);

		// Log.i(TAG, "==============" + mat.dump());

		Bitmap bmp = Bitmap.createBitmap(cols, rows, Config.RGB_565);
		Utils.matToBitmap(mat, bmp);
		
		return bmp;
	}
}
