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

import android.util.Log;
import android.widget.Toast;

public class DownloadHelper {
	private String TAG = "DownloadHelper";
	private static int BUFFER_SIZE = 8096;
	private String address = "http://tst.skyonebook.com/dzggb/index.php?uid=";
//	private String zipFilePath = "/data/data/com.foxconn.edu_bbs.ota/files/";
	private String zipFilePath = "/sdcard";
	private String zipFileName;
	private String urlFileName;
//	private String targetFileDir = "/data/data/com.foxconn.edu_bbs.ota/files/unzip_files" ;
	private String targetFileDir = "/sdcard/files" ;
	private String device_uid = null;
//	private String device_uid = "aa01";			//dummy
	private String device_status = "11111111";	//dummy
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
	
	public String getLastDownloadFile() {
		return mLastDownloadFile;
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
		String requestUrl = address + device_uid + "&getdata=" + version ;
		dataUrl = getUrl(requestUrl);
		
		Log.i(TAG, "Request URL: " + requestUrl);
		Log.i(TAG, "Return URL: " + dataUrl);
		
		if (dataUrl == null || dataUrl.equalsIgnoreCase("no")) {
			return dataUrl;
		} else {
			urlFileName = parseParams(dataUrl, "filename");
			zipFileName = urlFileName.substring(0, urlFileName.lastIndexOf(".")) + "-1.zip";
			latestVersion = parseParams(dataUrl, "version");
			requestTime = parseParams(dataUrl, "time");
			fileSize = parseParams(dataUrl, "size");
			showAll = parseParams(dataUrl, "showall");
			mLastDownloadFile = zipFilePath + "/" + zipFileName;
			Log.i(TAG, "FileName(" + zipFileName + ") Version(" + latestVersion + ") Time(" + requestTime + ") Size(" + fileSize + ") ShowAll(" + showAll + ")");
			
//			if (!requestTime.equalsIgnoreCase("30")) {
//				SharedPreferences.Editor editor = sp.edit();
//				editor.putLong("requestTime", Integer.valueOf(requestTime) * 1000);
//				editor.commit();
//				Message mMessage = mServiceHandler.obtainMessage();
//				mMessage.what = 2;
//				mMessage.arg1 = Integer.valueOf(requestTime) * 1000;
//				mServiceHandler.sendMessage(mMessage);	
//			}
			
			if (downloadNewFile(dataUrl.substring(0, dataUrl.lastIndexOf('/') + 1) + urlFileName)) {
				if (unZip()) {
					// FIXME: do not delete file, until trans it to slave
//					if (deleteOldFile()) {
						return latestVersion;
//					}				
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
	 * http://tst.skyonebook.com/dzggb/index.php?uid=aa01&status=11111111
	 */
	public void responseStatus(String status) {
		device_uid = new BBSUtilities().getMasterID();
		if (device_uid.equalsIgnoreCase("unknown")) {
			Log.e(TAG, "Failed while responsing status, uid unknown!");
			return;
		}
		String responseUrl = address + device_uid + "&status=" + status;
		Log.i(TAG, "Response URL: " + responseUrl);
		HttpURLConnection httpUrl = null;
		try {
			URL url = new URL(responseUrl);
			httpUrl =(HttpURLConnection) url.openConnection();
			httpUrl.connect();
		} catch (Exception e) {
			e.printStackTrace();
		} finally {
			try {
				if (httpUrl != null) {
					httpUrl.disconnect();
				}
			} catch (Exception e) {
				e.printStackTrace();
			}
		}				
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
		File file = new File(zipFilePath + "/" + zipFileName);
		file.delete();
		Log.i(TAG, "Delete the expirement ZIP file!");
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
		
		try {
			url = new URL(mUrl);
			httpUrl =(HttpURLConnection) url.openConnection();
			httpUrl.connect();
			bis = new BufferedInputStream(httpUrl.getInputStream());
			fos = new FileOutputStream (zipFilePath + "/" + zipFileName);
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
					Log.i(TAG, downloadRate + "  asdf " + downloadPercent);
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
		file = new File(zipFilePath + "/" + zipFileName);
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
