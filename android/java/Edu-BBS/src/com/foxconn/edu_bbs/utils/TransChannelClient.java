/**
 * @file       TransChannelServer.java
 *
 * @revision:  none 
 *
 * @version    0.0.01
 * @author:    Zenki (zhajun), zenki2001cn@163.com
 * @date:      2012-7-16 上午10:02:43 
 */

package com.foxconn.edu_bbs.utils;

import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.net.Socket;

import android.util.Log;

/**
 *
 */
public class TransChannelClient {
	static private final String TAG = "TransChannelClient";

	private final int BUFFER_SIZE = 4096;
	private String mRemoteAddress;
	private int mRemotePort;
	private Socket mSocket;
	private TransProtocol mCommand;
	private OutputStream mOutputStream;

	public TransChannelClient(String remoteIpAddress, int port) {
		mRemoteAddress = remoteIpAddress;
		mRemotePort = port;
		setUp();
	}
	
	public boolean isValidate() {
		if (mSocket != null) {
			return true;
		}
		
		return false;
	}

	public void transformCommand(TransProtocol command) {
		mCommand = command;
		byte[] buffer;

		try {
			if ((mOutputStream != null) && (command != null)) {
				buffer = command.getEvent().getBytes();
				mOutputStream.write(buffer);
				mOutputStream.flush();
			}
		} catch (IOException e) {
			e.printStackTrace();
		}
	}

	public void transformData(byte[] data, int len) {
		try {
			if (mOutputStream != null) {
				// Log.d(TAG, "transformData len = " + len);
				mOutputStream.write(data, 0, len);
			}
		} catch (IOException e) {
			 Log.e(TAG, "transformData error = " + e);
			 mCommand.onTransform(TransProtocol.getEventFromType(
						TransProtocol.COMMAND_RESPONSE_FAILED, ""));
		}
	}

	public void waitRespond() {
		InputStream is = null;
		byte[] buffer = new byte[BUFFER_SIZE];
		String result = "";
		boolean quit = false;

		int count = 0;
		int start = 0;

		try {
			is = mSocket.getInputStream();

			while (!quit) {
				count = is.read(buffer, start, BUFFER_SIZE - start);
				if (count < 0) {
					break;
				}

				count += start;
				start = 0;

				for (int i = 0; i < count; i++) {
					if ((buffer[i] == '\r') && (i + 1 < count)
							&& (buffer[i + 1] == '\n')) {
						result = new String(buffer, start, i - start);
						Log.i(TAG, "waitRespond result = " + result + " len = "
								+ result.length());

						// do something
						mCommand.onTransform(result);

						quit = true;
					}
				}

				if (start != count) {
					final int remaining = BUFFER_SIZE - start;
					System.arraycopy(buffer, start, buffer, 0, remaining);
					start = remaining;
				} else {
					start = 0;
				}
			}
		} catch (Exception e) {
			e.printStackTrace();
			mCommand.onTransform(TransProtocol.getEventFromType(
					TransProtocol.COMMAND_RESPONSE_FAILED, ""));
		}
	}

	public void stop() {
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
		} catch (IOException e) {
			e.printStackTrace();
		}
	}

	private void setUp() {
		try {
			mSocket = new Socket(mRemoteAddress, mRemotePort);
			mSocket.setSoTimeout(10000);

			if (mSocket != null) {
				mOutputStream = mSocket.getOutputStream();
			}
		} catch (Exception e) {
			Log.e(TAG, "setUp error " + e);
		}
	}
}
