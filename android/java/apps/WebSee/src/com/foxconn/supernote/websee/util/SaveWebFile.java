package com.foxconn.supernote.websee.util;

import java.io.BufferedInputStream;
import java.io.ByteArrayOutputStream;
import java.io.File;
import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStreamWriter;
import java.io.PrintWriter;
import java.net.MalformedURLException;
import java.net.URL;
import java.net.URLConnection;
import java.text.SimpleDateFormat;
import java.util.regex.Matcher;
import java.util.regex.Pattern;

import android.util.Log;

public class SaveWebFile implements ISaveFile {

	public static final String TAG = "SaveWebFile";
	public String fileName = "";
	public byte[] contentByte = null;

	@Override
	public String saveHtml(String urlStr, String savePath) {
		// 判断路径是否存在,不存在创建路径
		File saveFile = new File(savePath);
		if (!saveFile.exists()) {
			if (!saveFile.mkdirs()) {
				return "";
			}
		}
		fileName = "";
		this.saveWebFile(urlStr, savePath);
		return fileName;
	}

	/**
	 * 根据URL保存网页内容
	 * @param urlStr
	 * @param savePath
	 * @return
	 */
	private String saveWebFile(String urlStr, String savePath) {
		String result = "";
		String enCode = "UTF-8";
		String content = "";
		InputStream is = null;
		try {
			URL url = new URL(urlStr);
			URLConnection uc = url.openConnection();
			uc.connect();
            is = uc.getInputStream();
            enCode = this.getCharset(is);
            if("".equals(enCode)){
            	enCode = "UTF-8";
            }
            content = new String(contentByte,enCode);
			// 匹配Title
			String titileReg = "<(title|TITLE)>(.*?)</(title|TITLE)>";
			Pattern titleP = Pattern.compile(titileReg);
			Matcher mTitle = titleP.matcher(content);
			if (mTitle.find()) {
				fileName = mTitle.group(2);
			}
			is.close();
		} catch (MalformedURLException e) {
			e.printStackTrace();
		} catch (IOException e) {
			e.printStackTrace();
		}
		if ("".equals(fileName)) {
			// 根据系统时间，获取文件名
			SimpleDateFormat dateFormat = new SimpleDateFormat(
					"yyyyMMdd_hhmmss");
			fileName = dateFormat.format(new java.util.Date())+"";
		} 
		
		File folder = new File(savePath + fileName);
		if(!folder.exists()){
			folder.mkdirs();
		}
		// 匹配Picture资源
		String imgReg = "(?x)(src|SRC|background|BACKGROUND)=('|\")(http://.*?/)(.*?.(jpg|JPG|png|PNG|gif|GIF))('|\")";
		Pattern imgP = Pattern.compile(imgReg);
		Matcher mImg = imgP.matcher(content);
		StringBuffer replaceImgSb = new StringBuffer();
		//根据系统时间，获取文件名
		String saveImgName = "";
		File saveImgFile;
		while(mImg.find()){
			String fileType = mImg.group(4).substring(mImg.group(4).lastIndexOf(".")-1);
			saveImgName = System.currentTimeMillis()+fileType;
			saveImgFile = new File(savePath + fileName+"/"+saveImgName);
			savePictureAddCss(saveImgFile, mImg.group(3)+mImg.group(4));
			mImg.appendReplacement(replaceImgSb, mImg.group(1) + "='"+fileName+"/"+ saveImgName+ "'");
		}
		mImg.appendTail(replaceImgSb);
		
		// 匹配CSS资源
		String cssReg = "(href|Href|HREF)=\"(http://[\\w\\d\\/\\.*]+\\.css)\"";
		Pattern cssP = Pattern.compile(cssReg);
		Matcher mCss = cssP.matcher(replaceImgSb.toString());
		StringBuffer replaceCssSb = new StringBuffer();
		//根据系统时间，获取文件名
		String saveCssName = "";
		File saveCssFile;
		while(mCss.find()){
			saveCssName = System.currentTimeMillis()+".css";
			saveCssFile = new File(savePath + fileName+"/"+saveCssName);
			savePictureAddCss(saveCssFile, mCss.group(2));
			mCss.appendReplacement(replaceCssSb, mCss.group(1) + "='"+ fileName+"/"+ saveCssName+ "'");
		}
		mCss.appendTail(replaceCssSb);
		File file = new File(savePath + fileName+".html");
		if (null != replaceCssSb.toString() && !"".equals(replaceCssSb.toString())) {
			if (saveHtml(file, replaceCssSb.toString(), enCode)) {
				result = file.getName();
			}
		}
		return result;
	}

	/**
	 * 根据内容保存html文件
	 * @param file
	 * @param contentStr
	 * @param encode
	 * @return
	 */
	public boolean saveHtml(File file, String contentStr, String encode) {
		boolean result = false;
		FileOutputStream fos = null;
		OutputStreamWriter osw = null;
		PrintWriter outFile = null;
		try {
			file.createNewFile();
			fos = new FileOutputStream(file);
			osw = new OutputStreamWriter(fos, encode);
			outFile = new PrintWriter(osw);
			outFile.write(contentStr);
			result = true;
		} catch (FileNotFoundException e) {
			result = false;
			e.printStackTrace();
		} catch (IOException e) {
			result = false;
			e.printStackTrace();
		} finally {
			try {
				if(null != osw){
					osw.close();
				}
			} catch (IOException e) {
				result = false;
				e.printStackTrace();
			}
			try {
				if(null != fos){
					fos.close();
				}
			} catch (IOException e) {
				result = false;
				e.printStackTrace();
			}
		}
		return result;
	}
	
	public boolean savePictureAddCss(File file, String picUrl) {
		boolean result = false;
		URL url = null;
		BufferedInputStream in = null;
		FileOutputStream fos = null;
		try {
			if(!file.exists()){
				file.createNewFile();
			}
			url = new URL(picUrl);
			in = new BufferedInputStream(url.openStream());
			fos = new FileOutputStream(file);

			int t;
			while ((t = in.read()) != -1) {
				fos.write(t);
			}
			result = true;
		} catch (MalformedURLException e) {
			result = false;
			e.printStackTrace();
		} catch (FileNotFoundException e) {
			result = false;
			e.printStackTrace();
		} catch (IOException e) {
			result = false;
			e.printStackTrace();
		}finally{
			try {
				if(null != fos){
					fos.close();
				}
			} catch (IOException e) {
				// TODO Auto-generated catch block
				result = false;
				e.printStackTrace();
			}
			try {
				if(null != in){
					in.close();
				}
			} catch (IOException e) {
				// TODO Auto-generated catch block
				result = false;
				e.printStackTrace();
			}
		}
		return result;
	}

	/**
	 * 获取网页字默认编码字符集，并将输入流保存在byte数组
	 * @param is
	 * @return
	 */
	public String getCharset(InputStream is) {
		String enCode = "";
		BufferedInputStream bis = new BufferedInputStream(is);
		ByteArrayOutputStream baos = new ByteArrayOutputStream();
		
		StringBuffer sb = new StringBuffer();
		byte[] byArray = new byte[1024];
		try {
			while ((bis.read(byArray)) != -1) {
				baos.write(byArray);
				sb.append(new String(byArray));
				if ("".equals(enCode)) {
					enCode = marchCharset(sb);
				}
			}
			contentByte = baos.toByteArray();
			baos.close();
			bis.close();
		} catch (IOException e1) {
			// TODO Auto-generated catch block
			e1.printStackTrace();
		}
		return enCode;
	}
	
	/**
	 * 匹配Buffer,找出对应的Charset
	 */
	public String marchCharset(StringBuffer sb){
		String encode = "";
		// 匹配enCode
		String enCodeReg = "(meta|META|Meta) (http-equiv|HTTP-EQUIV|Http-Equiv|Http-equiv)=(\"Content-Type\"|\"Content-type\"|\"content-type\"|content-type|Content-type|Content-Type) (content|Content|CONTENT)=\".*?(charset|Charset|CHARSET)=(.*?)\"";
		Pattern enCodeP = Pattern.compile(enCodeReg);
		
		Matcher mEncode = enCodeP.matcher(sb.toString());
		if (mEncode.find()) {
			encode = mEncode.group(6);
		}
		return encode;
	}
	
}
