/**
 * @file       TransConfig.java
 *
 * @revision:  none 
 *
 * @version    0.0.01
 * @author:    Zenki (zhajun), zenki2001cn@163.com
 * @date:      2012-7-20 上午11:41:17 
 */

package com.foxconn.edu_bbs.utils;

/**
 *
 */
public class TransConfig {

	static private TransConfig mConfig;
	
	private String mServerIp = "127.0.0.1";
	private String mClientIp = "127.0.0.1";
	public static final int COMMAND_PORT = 9001;
	public static final int DATA_PORT = 9901;
	private String mTargetFileDir = "/sdcard/files";
	private int mDeviceNumber = 1;
	
	static public TransConfig getInstance() {
		if (null == mConfig) {
			mConfig = new TransConfig();
		}
		
		return mConfig;
	}
	
	public String getServerIp() {
		return mServerIp;
	}
	
	public void setServerIp(String ip) {
		mServerIp = ip;
	}
	
	public String getClientIp() {
		return mClientIp;
	}
	
	public void setClientIp(String ip) {
		mClientIp = ip;
	}
	
	public String getTargetDir() {
		return mTargetFileDir;
	}
	
	public void setTargetDir(String dir) {
		mTargetFileDir = dir;
	}
	
	public int getDeviceNumber() {
		return mDeviceNumber;
	}
	
	public void setDeviceNumber(int number) {
		mDeviceNumber = number;
	}
	
//	public int getCommandPort() {
//		return mCommandPort;
//	}
//	
//	public void setCommandPort(int port) {
//		mCommandPort = port;
//	}
//	
//	public int getDataPort() {
//		return mDataPort;
//	}
//	
//	public void setDataPort(int port) {
//		mDataPort = port;
//	}
}
