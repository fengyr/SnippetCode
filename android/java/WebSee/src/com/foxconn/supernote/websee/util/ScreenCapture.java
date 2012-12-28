package com.foxconn.supernote.websee.util;

import java.io.File;
import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.IOException;
import java.text.SimpleDateFormat;

import android.graphics.Bitmap;
import android.graphics.Canvas;
import android.graphics.Picture;
import android.util.Log;
import android.webkit.WebView;

public class ScreenCapture implements ICapturer{
	
	public static final String TAG = "ScreenCapture";
	private WebView webView;
	
	public ScreenCapture(WebView webView){
		this.webView = webView;
	}

	/**
	 * 截取当前webView的画面，并保存为PNG格式的图片,保存路径为参数savePath
	 */
	@Override
	public String captureSave(String savePath) {
		Log.d(TAG, "captureSave path = "+savePath);
		// save the picture to a file
		Bitmap capBitmap = this.capture();
		if(null != capBitmap){
			return this.captureToFile(capBitmap, savePath);
		}else{
			return "";
		}
		
	}

	/**
	 * 截取当前webView的画面
	 */
	@Override
	public Bitmap capture() {
		// TODO Auto-generated method stub
		Picture capPic = webView.capturePicture();
		if(capPic.getWidth()>0 && capPic.getHeight()>0){
			Bitmap capBitmap = Bitmap.createBitmap(capPic.getWidth(),
					capPic.getHeight(), Bitmap.Config.RGB_565);
			Canvas capCanvas = new Canvas(capBitmap);
			capPic.draw(capCanvas);
			return capBitmap;
		}else{
			return null;
		}
		
	}

	/**
	 * 将参数的Bitmap保存为PNG格式的图片,保存路径为参数savePath
	 */
	@Override
	public String captureToFile(Bitmap srcBitmap, String savePath) {
		// TODO Auto-generated method stub
		//判断路径是否存在,不存在创建路径
		File saveFile = new File(savePath);
		if(!saveFile.exists()){
			if(!saveFile.mkdirs()){
				return "";
			}
		}
		//根据系统时间，获取文件名
		SimpleDateFormat dateFormat=new SimpleDateFormat("yyyyMMdd_hhmmss");     
		String fileName = dateFormat.format(new java.util.Date())+".png";
		FileOutputStream fos;
		try {
			fos = new FileOutputStream(savePath+fileName);
			srcBitmap.compress(Bitmap.CompressFormat.PNG, 100, fos);
			fos.flush();
			fos.close();
			Log.d(TAG, "captureSave OK!");
			return fileName;
		} catch (FileNotFoundException e) {
			e.printStackTrace();
			return "";
		} catch (IOException e) {
			e.printStackTrace();
			return "";
		}
	}
	
}
