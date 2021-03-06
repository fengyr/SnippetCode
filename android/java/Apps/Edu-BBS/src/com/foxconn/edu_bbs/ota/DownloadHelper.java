package com.foxconn.edu_bbs.ota;

import java.io.BufferedInputStream;
import java.io.BufferedReader;
import java.io.File;
import java.io.FileInputStream;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStreamReader;
import java.net.HttpURLConnection;
import java.net.URL;
import java.util.zip.ZipEntry;
import java.util.zip.ZipFile;
import java.util.zip.ZipInputStream;

import com.foxconn.edu_bbs.utils.BBSUtilities;
import com.foxconn.edu_bbs.utils.TransConfig;

import android.util.Log;

public class DownloadHelper {
	private String TAG = "DownloadHelper";
	private TransConfig mConfig = TransConfig.getInstance();
	private static int BUFFER_SIZE = 8192;
	private String dataAddress = "http://tst.skyonebook.com/dzggb/index.php?uid=";
	private String statusAddress = "http://tst.skyonebook.com/dzggb/status.php?uid=";
	private String targetFileDir = mConfig.getTargetDir();
	private String zipFileDir = mConfig.getDownloadDir();
	private String zipFileName;
	private String urlFileName;
	private String device_uid = null;
	private String latestVersion;
	private String dataUrl;
	private String requestTime = null;
	private String fileSize = null;
	private String showAll = null;
	
	private long firstTime = 0L;
	private long secondTime = 0L;
	private long firstSize = 0L;
	private long secondSize = 0L;
	private double downloadRate = 0.0;
	private int downloadPercent = 0;
	private String mLastDownloadFile = "";
	
//	Handler mServiceHandler;
//	int MESSAGE_WHAT;
//	int marg1;
//
//	public void sendMessage(Handler serviceHandler, int message_what, int arg1) {
//		mServiceHandler = serviceHandler;
//		MESSAGE_WHAT = message_what;
//		marg1 = arg1;
//    }
	
	public DownloadHelper() {
		assertDownloadDir();
	}
	
	public String getLastDownloadFile() {
		return mLastDownloadFile;
	}
	
	private void assertDownloadDir() {
		File zipdir = new File(zipFileDir);
		if (!zipdir.exists()) {
			zipdir.mkdirs();
			zipdir = null;
		}
	}
	
	/*
	 * Send URL like this
	 * http://tst.skyonebook.com/dzggb/index.php?uid=aa01&getdata=0
	 */
	public String requestData(String version) {
		device_uid = new BBSUtilities().getMasterID();
		if (device_uid.equalsIgnoreCase("unknown")) {
			Log.e(TAG, "Failed while requesting data, uid unknown!");
			return null;
		}
		String requestUrl = dataAddress + device_uid + "&getdata=" + version ;
		dataUrl = getUrl(requestUrl);
		
		Log.i(TAG, "requestData Request URL: " + requestUrl);
		Log.i(TAG, "requestData Return URL: " + dataUrl);
		
		if (dataUrl == null || dataUrl.equalsIgnoreCase("no")) {
			return dataUrl;
		} else {
			urlFileName = parseParams(dataUrl, "filename");
			zipFileName = urlFileName.substring(0, urlFileName.lastIndexOf(".")) + "-0.zip";
			latestVersion = parseParams(dataUrl, "version");
			requestTime = parseParams(dataUrl, "time");
			fileSize = parseParams(dataUrl, "size");
			showAll = parseParams(dataUrl, "showall");
			mLastDownloadFile = zipFileDir + "/" + zipFileName;
			Log.i(TAG, "FileName(" + zipFileName + ") Version(" + latestVersion + ") Time(" + requestTime + ") Size(" + fileSize + ") ShowAll(" + showAll + ")");
			
			deleteOldFile();
			
			if (downloadNewFile(dataUrl.substring(0, dataUrl.lastIndexOf('/') + 1) + urlFileName)) {
				if (unZip()) {
					return latestVersion;
				}
			}
		}
		return null;
	}

	public String getTime() {
			return requestTime;
	}
	
	public String getFileSize() {
		return fileSize;
	}
	
	private String parseParams(String url, String str) {
		String value = null;
		int j = 0;
		int i = url.indexOf("&" + str + "=") + str.length() + 2;
		if (str.equalsIgnoreCase("filename")) {
			i = url.lastIndexOf("&version");
			j = url.lastIndexOf('/');
			value = url.substring(j+1,i);	
		} else if (str.equalsIgnoreCase("version")) {
			j = url.indexOf("&time");
			value = url.substring(i, j);
		} else if (str.equalsIgnoreCase("time")) {
			j = url.indexOf("&size");
			value = url.substring(i, j);
		} else if (str.equalsIgnoreCase("size")) {
			j = url.indexOf("&showall");
			value = url.substring(i, j);
		} else if (str.equalsIgnoreCase("showall")) {
			value = url.substring(i);
		}
		return value;
	}
	
	/*
	 * Send URL like this
	 * http://tst.skyonebook.com/dzggb/status.php?uid=aa01&status=11111111
	 * 
	 * Response URL like this
	 * http://tst.skyonebook.com/dzggb/devicestatus.html
	 */
	public void responseStatus(String status) {
		device_uid = new BBSUtilities().getMasterID();
		if (device_uid.equalsIgnoreCase("unknown")) {
			Log.e(TAG, "Failed while responsing status, uid unknown!");
			return;
		}
		String requestUrl = statusAddress + device_uid + "&status=" + status;
		String rtn = getUrl(requestUrl);
		
		Log.i(TAG, "responseStatus Request URL: " + requestUrl);
		Log.i(TAG, "responseStatus Return URL: " + rtn);		
	}

	/*
	 * Get URL from HTML resource file
	 */
	public String getUrl(String mUrl) {
		String dataUrl = null;
		URL url = null;
		HttpURLConnection httpUrl = null;
		BufferedReader br = null;
		try {
			url = new URL(mUrl);
			httpUrl =(HttpURLConnection) url.openConnection();
			httpUrl.connect();
			br = new BufferedReader(new InputStreamReader(httpUrl.getInputStream()));
			dataUrl = br.readLine();
		} catch (Exception e) {
			e.printStackTrace();
		} finally {
			try {
				if (br != null) {
					br.close();	
				}
				if (httpUrl != null) {
					httpUrl.disconnect();
				}				
			} catch (IOException e) {
				e.printStackTrace();
			}			
		}
		return dataUrl;
	}
	
	/*
	 * Delete overdue files, like bbs-aa01-6.zip
	 */
	public boolean deleteOldFile() {
		File file = new File(zipFileDir + "/" + zipFileName);
		if (file.exists()) {
			file.delete();
			Log.i(TAG, "Delete the expirement ZIP file!");
		}
		
		return true;
	}
	
	/*
	 * 
	 */
	public boolean downloadNewFile(String mUrl) {
		URL url = null;
		HttpURLConnection httpUrl = null;
		BufferedInputStream bis = null;
		FileOutputStream fos = null;
		byte[] buf = new byte[BUFFER_SIZE];  
		int size = 0;
		long hasDownload = 0L;
		
		assertDownloadDir();
		
		try {
			url = new URL(mUrl);
			httpUrl =(HttpURLConnection) url.openConnection();
			httpUrl.connect();
			bis = new BufferedInputStream(httpUrl.getInputStream());
			fos = new FileOutputStream (zipFileDir + "/" + zipFileName);
			while ((size = bis.read(buf)) != -1) {
				fos.write(buf, 0, size);
				
				// Should be disabled in the future
				// Show "wifi speed:2kbps,finished percentange:60%"
				hasDownload = hasDownload + size;
				secondTime = System.currentTimeMillis();
				secondSize = hasDownload;
				
				if (secondTime - firstTime > 1000) {
					downloadRate = ((secondSize - firstSize) / 1024.0f) / ((secondTime - firstTime) / 1000.0f);
					firstSize = secondSize;
					firstTime = secondTime;
					String str = getFileSize();
					downloadPercent = (int) (secondSize / (long) Integer.valueOf(str));
					//Toast.makeText(this, "WIFI speed: " + downloadRate + "kb/s, Finished percentange: " + downloadPercent + "%.", Toast.LENGTH_SHORT).show();
					Log.i(TAG, downloadRate + " % " + downloadPercent);
				}				
				
			}
		} catch (IOException e) {
			e.printStackTrace();
			return false;
		} finally {
			try {
				if (bis != null) {
					bis.close();
				}
				if (fos != null) {
					fos.close();
				}
				if (fos != null) {
					httpUrl.disconnect();
				}
			} catch (IOException e) {
				e.printStackTrace();
				return false;
			}			
		}
		
		Log.i(TAG, "Finish downloading the ZIP file!");
		return true;
	}
	
	/*
	 * Unpack pictures
	 */
	public boolean unZip() {
		boolean flag = false;
		File file = null;
		ZipFile zipFile = null;
		file = new File(zipFileDir + "/" + zipFileName);
		if (false == file.exists()) {
			return false;
		} else if (0 == file.length()) {
			return false;
		} else {
			byte[] buf = new byte[BUFFER_SIZE];
			int readSize = -1;
			ZipInputStream zis = null;
			FileOutputStream fos = null;
			try {
				zipFile = new ZipFile(file);
				zipFile.close();
				File newdir = new File(targetFileDir);
				if (false == newdir.exists()) {
					newdir.mkdirs();
					newdir = null;
				}
				zis = new ZipInputStream(new FileInputStream(file));
				ZipEntry zipEntry = zis.getNextEntry();
				while (null != zipEntry) {
					String zipEntryName = zipEntry.getName().replace('\\', '/');
					if (zipEntry.isDirectory()) {
						int indexNumber = zipEntryName.lastIndexOf('/');
						File entryDirs = new File(targetFileDir + "/" + zipEntryName.substring(0, indexNumber));
						entryDirs.mkdirs();
						entryDirs = null;
					} else {
						fos = new FileOutputStream(targetFileDir + "/" + zipEntryName);
						while ((readSize = zis.read(buf, 0, BUFFER_SIZE)) != -1) {
							fos.write(buf, 0, readSize);
						}
					}
					zipEntry = zis.getNextEntry();
				}
				flag = true;

			} catch (Exception e) {
				e.printStackTrace();
			} finally {
				try {
					if (null != zis) {
						zis.close();
					}
					if (null != fos) {
						fos.close();
					}
				} catch (IOException e) {
					e.printStackTrace();
				}
			}
		}
		Log.i(TAG, "unzip was excuted: " + flag);
		return flag;		
	}
}
