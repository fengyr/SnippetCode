/**
 * @file       ICapturer.java
 *
 * @revision:  none 
 *
 * @version    0.0.01
 * @author:    Zenki (zhajun), zenki2001cn@163.com
 * @date:      2012-3-2 上午08:30:36 
 */

package com.foxconn.supernote.email.util;

import android.graphics.Bitmap;

/**
 *
 */
public interface ICapturer {
	/**
	 * 截取当前webView的画面，并保存为PNG格式的图片,保存路径为参数savePath
	 * @param savePath 保存路径
	 * @return 保存的文件名
	 */
	public String captureSave(String savePath);
	
	/**
	 * 
	 * 将参数的Bitmap保存为PNG格式的图片,保存路径为参数savePath
	 *
	 * @param srcBitmap 保存图片源
	 * @param savePath 保存图片路径
	 * @return 保存的文件名
	 */
	public String captureToFile(Bitmap srcBitmap, String savePath);
	
	/**
	 * 截取当前webView的画面
	 * @return Bitmap格式数据
	 */
	public Bitmap capture();
}
