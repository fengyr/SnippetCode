/**
 * @file       TransChannelServer.java
 *
 * @revision:  none 
 *
 * @version    0.0.01
 * @author:    Zenki (zhajun), zenki2001cn@163.com
 * @date:      2012-7-16 上午10:02:43 
 */

package com.innovation.nj.colorpickmobile.core;

import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.net.Socket;
import java.util.ArrayList;

import com.google.protobuf.InvalidProtocolBufferException;
import com.innovation.nj.colorpickmobile.model.ProtobufData;

import ColorSepServer.Image.ImagePackage;
import android.content.Context;
import android.util.Log;

/**
 *
 */
public class DataReceiver {
	static private final String TAG = "DataReceiver";

	private final int BUFFER_HEAD_SIZE = 4;
	private final String REQUEST_IMAGE_DATA = "type_mobile_data\r\n";

	private String mRemoteAddress;
	private int mRemotePort;
	private Socket mSocket;
	private OutputStream mOutputStream;
	private InputStream mInputStream;
	
	private Context mContext;
	private boolean mQuit = false;
	private ThreadGetData mThreadGetData;
	private ArrayList<DataRefreshHandler> mListDataRefresh;

	public DataReceiver(Context context, String remoteIpAddress, int port) {
		mContext = context;
		mRemoteAddress = remoteIpAddress;
		mRemotePort = port;
		mListDataRefresh = new ArrayList<DataRefreshHandler>();
		
		buildConnection();
	}

	public boolean isValidate() {
		if (mSocket != null) {
			return true;
		}

		return false;
	}

	// public void transformCommand(TransProtocol command) {
	// mCommand = command;
	// byte[] buffer;
	//
	// try {
	// if ((mOutputStream != null) && (command != null)) {
	// buffer = command.getEvent().getBytes();
	// mOutputStream.write(buffer);
	// mOutputStream.flush();
	// }
	// } catch (IOException e) {
	// e.printStackTrace();
	// }
	// }

	// public void transformData(byte[] data, int len) {
	// try {
	// if (mOutputStream != null) {
	// // Log.d(TAG, "transformData len = " + len);
	// mOutputStream.write(data, 0, len);
	// }
	// } catch (IOException e) {
	// Log.e(TAG, "transformData error = " + e);
	// mCommand.onTransform(TransProtocol.getEventFromType(
	// TransProtocol.COMMAND_RESPONSE_FAILED, ""));
	// }
	// }

	private byte[] toBytes(int n) {
		byte[] b = new byte[4];
		b[0] = (byte) (n & 0xff);
		b[1] = (byte) (n >> 8 & 0xff);
		b[2] = (byte) (n >> 16 & 0xff);
		b[3] = (byte) (n >> 24 & 0xff);
		return b;
	}

	private int toInt(byte[] b) {
		int iOutcome = 0;
		byte bLoop;

		for (int i = 0; i < b.length; i++) {
			bLoop = b[i];
			iOutcome += (bLoop & 0xFF) << (8 * i);
		}
		return iOutcome;
	}
	
	private class ThreadGetData extends Thread {
		public ThreadGetData() {
		}
		
		@Override
		public void run() {
			Log.d(TAG, "ThreadGetData Start.......");
			
			while(!mQuit) {
				boolean login = requestHeader();
				if (login) {
					waitRespond();
				}
				
				try {
					Thread.sleep(1000);
					// reconnect to server
					buildConnection();
				} catch (InterruptedException e) {
					e.printStackTrace();
				}
			}
			
			Log.d(TAG, "ThreadGetData Stop.......");
			super.run();
		}
	}
	
	private void buildConnection() {
		try {
			Log.d(TAG, "client setup address = " + mRemoteAddress + " port = "
					+ mRemotePort);

			mSocket = new Socket(mRemoteAddress, mRemotePort);
			mSocket.setSoTimeout(10000);
			mSocket.setReuseAddress(true);

			if (mSocket != null) {
				mOutputStream = mSocket.getOutputStream();
				mInputStream = mSocket.getInputStream();
			}
		} catch (Exception e) {
			Log.e(TAG, "setUp error " + e);
		}
	}

	private boolean requestHeader() {
		if ((mOutputStream == null) || (mInputStream == null)) {
			return false;
		}
		
		byte[] header = new byte[1028];
		byte[] response = new byte[1024];
		byte temp[] = toBytes(0);
		System.arraycopy(temp, 0, header, 0, temp.length);
		System.arraycopy(REQUEST_IMAGE_DATA.getBytes(), 0, header, 4,
				REQUEST_IMAGE_DATA.length());

		int rsize = 0;
		try {
			mOutputStream.write(header);
			Log.d(TAG, "requestHeader header=" + header.toString());
			rsize = mInputStream.read(response);
			Log.d(TAG, "response=" + response.toString() + " rsize=" + rsize);
		} catch (IOException e) {
			e.printStackTrace();
		}
		
		if (rsize <= 0) {
			return false;
		}
		
		return true;
	}

	private void waitRespond() {
		if ((mOutputStream == null) || (mInputStream == null)) {
			return;
		}
		
		int rsize = 0;
		int protoSize = 0;
		int rstart = 0;
		int rleft = 0;

		try {
			while (!mQuit) {
				byte[] buf_header = new byte[BUFFER_HEAD_SIZE];
				rsize = mInputStream.read(buf_header, 0, BUFFER_HEAD_SIZE);
				Log.d(TAG, "waitRespond header=" + rsize);
				if (rsize < 0) {
					break;
				}

				protoSize = toInt(buf_header);
				Log.d(TAG, "waitRespond protoSize=" + protoSize);

				byte[] buf_data = new byte[protoSize];

				rstart = 0;
				rleft = protoSize;
				while (rleft > 0) {
					rsize = mInputStream.read(buf_data, rstart, rleft);
					Log.d(TAG, "waitRespond rSize=" + rstart);
					rstart += rsize;
					rleft = protoSize - rstart;
				}
				Log.d(TAG, "waitRespond dataSize=====" + rstart);
				
				ProtobufData data = new ProtobufData();
				try {
					ImagePackage pack = ImagePackage.parseFrom(buf_data);
					data.time = pack.getTime();
					data.frame_count = pack.getFrameCount();
					data.image_class = pack.getImageClass();
					data.class_count = pack.getClassCount();
					data.image_width = pack.getImageWidth();
					data.image_height = pack.getImageHeight();
					data.process_time = pack.getProcessTime();
					data.process_status = pack.getProcessStatus();
					data.retinue_init = pack.getRetinueInit();
					data.add_sample_result = pack.getAddSampleResult();
					data.data_type = pack.getDataType();
					data.add_sample_counts = pack.getAddSampleCounts();
					data.image_data = pack.getImageData();
					
					for (DataRefreshHandler handler : mListDataRefresh) {
						handler.refresh(data);
					}
					
					Log.e(TAG, "getImageClass=" + pack.getImageClass() + 
							" getAddSampleCounts=" + pack.getAddSampleCounts() + 
							" getFrameCount=" + pack.getFrameCount());
				} catch (InvalidProtocolBufferException e) {
					e.printStackTrace();
				}
			}
		} catch (Exception e) {
			e.printStackTrace();
		}
	}

	public void run() {
		mThreadGetData = new ThreadGetData();
		mThreadGetData.start();
	}
	
	public void stop() {
		mQuit = true;
		
		try {
			mThreadGetData.join(3000);
		} catch (InterruptedException e1) {
			e1.printStackTrace();
		}
		
		try {
			if (mSocket != null) {
				mSocket.close();
				Log.d(TAG, "client stop address = " + mRemoteAddress
						+ " port = " + mRemotePort);
			}
			if (mOutputStream != null) {
				mOutputStream.close();
				mOutputStream = null;
			}
			if (mInputStream != null) {
				mInputStream.close();
				mInputStream = null;
			}
		} catch (IOException e) {
			e.printStackTrace();
		}
	}
	
	public void registerRefreshHandler(DataRefreshHandler handler) {
		if (mListDataRefresh != null) {
			mListDataRefresh.add(handler);
		}
	}
}
