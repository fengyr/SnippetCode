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
	
	// 默认host端的IP, usb1
	private String mServerIp = "0.0.0.0";
	
	// 默认slave端的IP, usb0
	private String mRemoteIp = "0.0.0.0";
	
	// 命令传输端口
	public static final int COMMAND_PORT = 9001;
	
	// 数据传输端口
	public static final int DATA_PORT = 9901;
	
	// 每台设备的屏幕数量
	public static final int DISPLAY_NUM_PER_DEVICE = 4;
	
	// 每一组中机器的数量，通常是8台机器一组，分别为1个master，7个slave
	public static final int DEVICE_NUM_PER_GROUP = 8;
	
	// 向服务器获取图片数据的时间间隔，单位毫秒
	public static final int REQUEST_DATA_INTERVAL = 30000;
	
	// 向slave查询状态的时间间隔，单位毫秒
	public static final int REQUEST_STATUS_INTERVAL = 180000;
	
	// 连接网络失败后，重新连接的时间间隔，单位毫秒
	public static final int RECONNECT_NETWORK_INTERVAL = 60000;
	
	// 切换显示四块屏幕的时间间隔，单位毫秒
	public static final int SWITCH_SCREEN_INTERVAL = 5000;
	
	// 向slave传输文件失败后，重新尝试的次数
	public static final int TRANS_RETRY_COUNT = 3;
	
	// 唤醒设备的间隔时间，单位毫秒
	public static final int WAKEUP_INTERVAL = 180000;
	
	// 唤醒设备的接口文件
	public static final String WAKEUP_INTERFACE = "/proc/chg_enable";
	
	// 屏幕切换的接口文件
	public static final String PANEL_INTERFACE = "/sys/devices/platform/mxc_epdc_fb/panel_en";
	
	// 配置文件的路径, 内容格式如下：
	//
	// <?xml version="1.0" encoding="UTF-8"?>
	// <uid>aa01</uid>
	// <server>192.168.1.100</server>
	// <remote>192.168.1.102</remote>
	public static final String CONFIG_FILE = "/data/config.xml";
	
	// 图片文件解压的目录
	private String mTargetFileDir = "/data/files";
	
	// zip文件保存目录
	private String mDownloadFileDir = "/data/download_zips";
	
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
	
	public String getRemoteIp() {
		return mRemoteIp;
	}
	
	public void setRemoteIp(String ip) {
		mRemoteIp = ip;
	}
	
	public String getTargetDir() {
		return mTargetFileDir;
	}
	
	public void setTargetDir(String dir) {
		mTargetFileDir = dir;
	}
	
	public String getDownloadDir() {
		return mDownloadFileDir;
	}
	
	public void setDownloadDir(String dir) {
		mDownloadFileDir = dir;
	}
}
