package com.foxconn.edu_bbs.utils;

import java.io.BufferedReader;
import java.io.File;
import java.io.FileReader;
import java.io.IOException;
import java.util.regex.Matcher;
import java.util.regex.Pattern;

import android.util.Log;

public class BBSUtilities {
	private static String TAG = "[Edu-BBS]BBSUtilities";

	/*
	 * Return like this: aa01, or aa02
	 */
	public String getMasterID() {
		File xmlFile = new File("/sdcard/config.xml");
		String uid = null;
		
		try {
			BufferedReader in = new BufferedReader(new FileReader(xmlFile));
			String str;

			Pattern p = Pattern.compile("<[uU][iI][dD]>([^<]*)");
			while ((str = in.readLine()) != null) {
				Matcher m = p.matcher(str);
				if(m.find()) {
					uid = m.group(1);
					uid.replaceAll(" ", "");
					break;
				}
			}
			in.close();
		} catch (IOException e) {
			e.printStackTrace();
		}
		
		if (uid != null) {
			return uid;
		} else {
			Log.e(TAG, "Get uid failed!!!!");
			return "unknown";
		}
	}
	
	/*
	 * 
	 */
	public String getDeviceStatus() {
		;//Get it for somewhere and save it
		return null;
	}
}
