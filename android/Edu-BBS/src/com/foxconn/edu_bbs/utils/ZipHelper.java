/**
 * @file       ZipHelper.java
 *
 * @revision:  none 
 *
 * @version    0.0.01
 * @author:    Zenki (zhajun), zenki2001cn@163.com
 * @date:      2012-7-18 上午09:20:39 
 */

package com.foxconn.edu_bbs.utils;

import java.io.File;
import java.io.FileInputStream;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.util.Enumeration;
import java.util.zip.ZipEntry;
import java.util.zip.ZipFile;
import java.util.zip.ZipOutputStream;

import android.util.Log;

/**
 * Usage:
 *  	new ZipHelper().zip("/sdcard/test_dir", "/sdcard/mytest.zip");
 *  	new ZipHelper().unZip("/sdcard/mytest.zip", "/sdcard/");
 */
public class ZipHelper {
	static private final String TAG = "ZipHelper";

	public ZipHelper() {
	}

	public void zip(String inputFileName, String zipFileName) throws Exception {
		zip(zipFileName, new File(inputFileName));
	}

	private void zip(String zipFileName, File inputFile) throws Exception {
		ZipOutputStream out = new ZipOutputStream(new FileOutputStream(
				zipFileName));

		String base = "";
		String fileName = new File(zipFileName).getName();
		int index = fileName.lastIndexOf(".");
		if (index == -1) {
			base = fileName;
		} else {
			base = fileName.substring(0, index);
		}

		Log.e(TAG, "zip base = " + base);
		zip(out, inputFile, base);
		out.close();
	}

	private void zip(ZipOutputStream out, File f, String base) throws Exception {
		if (f.isDirectory()) {
			File[] fl = f.listFiles();
			out.putNextEntry(new ZipEntry(base + "/"));
			base = base.length() == 0 ? "" : base + "/";
			for (int i = 0; i < fl.length; i++) {
				Log.d(TAG, "file [" + i + "] " + fl[i].getAbsolutePath());
				zip(out, fl[i], base + fl[i].getName());
			}
		} else {
			out.putNextEntry(new ZipEntry(base));
			FileInputStream in = new FileInputStream(f);
			int len = 0;
			byte[] buf = new byte[1024];
			while ((len = in.read(buf)) != -1) {
				out.write(buf, 0, len);
//				Log.d(TAG, "write zip len =" + len);
			}
			in.close();
		}
	}

	public void unZip(String zipfile, String destDir) {
		destDir = destDir.endsWith(File.separator) ? destDir : destDir
				+ File.separator;
		byte b[] = new byte[1024];
		int length;
		ZipFile zipFile;

		try {
			zipFile = new ZipFile(new File(zipfile));
			Enumeration<? extends ZipEntry> enumeration = zipFile.entries();
			ZipEntry zipEntry = null;
			
			while (enumeration.hasMoreElements()) {
				zipEntry = (ZipEntry) enumeration.nextElement();
				File loadFile = new File(destDir + zipEntry.getName());
				
				if (zipEntry.isDirectory()) {
					loadFile.mkdirs();
				} else {
					if (!loadFile.getParentFile().exists()) {
						loadFile.getParentFile().mkdirs();
					}
					
					OutputStream outputStream = new FileOutputStream(loadFile);
					InputStream inputStream = zipFile.getInputStream(zipEntry);
					
					while ((length = inputStream.read(b)) > 0) {
						outputStream.write(b, 0, length);
					}
				}
			}
		} catch (IOException e) {
			e.printStackTrace();
		}
	}
}
