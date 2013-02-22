package com.foxconn.edu_bbs.utils;

import java.io.BufferedReader;
import java.io.DataOutputStream;
import java.io.File;
import java.io.FileReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.util.regex.Matcher;
import java.util.regex.Pattern;

import android.util.Log;

public class BBSUtilities {
	private static String TAG = "[Edu-BBS]BBSUtilities";

	static public String runRootCommand(String command) {
		Process process = null;
		DataOutputStream os = null;
		InputStreamReader osRes = null;
		String rtn = "";

		Log.i(TAG, "runRootCommand command = " + command);
		
		try {
			process = Runtime.getRuntime().exec("su root");
//			process = Runtime.getRuntime().exec(command);
			os = new DataOutputStream(process.getOutputStream());
			osRes = new InputStreamReader(process.getInputStream());
			BufferedReader reader = new BufferedReader(osRes);

			os.writeBytes(command + "\n");
			os.flush();
			os.writeBytes("exit\n");
			os.flush();
			
			rtn += reader.readLine();
//		    while (rtn != null) {
		    	Log.i(TAG, "runRootCommand result = " + rtn);
		        rtn += reader.readLine();
//		    }
		} catch (Exception ex) {
			ex.printStackTrace();
			Log.e(TAG,
			"runRootCommand error !!!");
		} finally {
			try {
				process.waitFor();
			} catch (InterruptedException e1) {
				e1.printStackTrace();
			}

			try {
				if (os != null) {
					os.close();
				}
				if (osRes != null) {
					osRes.close();
				}
				process.destroy();
			} catch (Exception e) {
			}
		}
		
		return rtn;
	}
	
	/*
	 * Return like this: aa01, or aa02
	 */
	public String getMasterID() {
		File xmlFile = new File(TransConfig.CONFIG_FILE);
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
	
	public String getServerIp() {
		File xmlFile = new File(TransConfig.CONFIG_FILE);
		String uid = null;
		
		try {
			BufferedReader in = new BufferedReader(new FileReader(xmlFile));
			String str;

			Pattern p = Pattern.compile("<server>([^<]*)");
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
			Log.e(TAG, "Get ServerIp failed!!!!");
			return "unknown";
		}
	}
	
	public String getRemoteIp() {
		File xmlFile = new File(TransConfig.CONFIG_FILE);
		String uid = null;
		
		try {
			BufferedReader in = new BufferedReader(new FileReader(xmlFile));
			String str;

			Pattern p = Pattern.compile("<remote>([^<]*)");
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
			Log.e(TAG, "Get RemoteIp failed!!!!");
			return "unknown";
		}
	}
}
