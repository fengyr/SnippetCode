package com.foxconn.supernote.websee.util;

public interface ISaveFile {
	
	/*
	 * 根据将要保存的URL，保存网页为html
	 * 
	 */
	public String saveHtml(String url,String savePath);

}
