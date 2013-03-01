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

import java.io.File;
import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.net.InetAddress;
import java.net.ServerSocket;
import java.net.Socket;

import com.foxconn.edu_bbs.service.ITransCallback;

import android.content.Context;
import android.util.Log;

/**
 *
 */
public class TransChannelServer {
	static private final String TAG = "TransChannelServer";

	private final int BUFFER_SIZE = 4096;

	private String mLocalAddress;
	private int mLocalPort;
	private ServerSocket mSocket;
	private TransProtocol mTransProtocol;
	private Context mContext;
	volatile private boolean mQuit = false;

	public TransChannelServer(Context context, String localIpAddress, int port) {
		mContext = context;
		mLocalAddress = localIpAddress;
		mLocalPort = port;
		setUp();
	}

	public void startServer(TransProtocol protocol) {
		switch (mLocalPort) {
		case TransConfig.COMMAND_PORT:
			listenToCommand(protocol);
			break;
		case TransConfig.DATA_PORT:
			listenToData(protocol);
			break;
		default:
			break;
		}
	}

	public void listenToCommand(TransProtocol protocol) {
		Socket client = null;
		InputStream is = null;

		byte buffer[];
		String event = "";

		Log.d(TAG, "server listenToCommand address = " + mLocalAddress
				+ " port = " + mLocalPort);

		try {
			while (!mQuit) {
				if (mSocket != null) {
					client = mSocket.accept();
					is = client.getInputStream();
					buffer = new byte[BUFFER_SIZE];

					int count = 0;
					int start = 0;
					while (true) {
						count = is.read(buffer, start, BUFFER_SIZE - start);
						if (count < 0) {
							break;
						}

						count += start;
						start = 0;

						for (int i = 0; i < count; i++) {
							if ((buffer[i] == '\r') && (i + 1 < count)
									&& (buffer[i + 1] == '\n')) {
								event = new String(buffer, start, i - start);
								Log.i(TAG, "receive msg = " + event + " len = "
										+ event.length());

								if (mTransProtocol != null) {
									final OutputStream os = client
											.getOutputStream();
									mTransProtocol.setListener(new ITransCallback() {
										@Override
										public void action(String result) {
											try {
												Log.i(TAG,
														"response to client result = "
																+ result);
												os.write(result.getBytes());
											} catch (IOException e) {
												e.printStackTrace();
											}
										}
									});

									mTransProtocol.onTransform(event);
								}

								start = i + 1;
							}
						}

						if (start != count) {
							final int remaining = BUFFER_SIZE - start;
							System.arraycopy(buffer, start, buffer, 0,
									remaining);
							start = remaining;
						} else {
							start = 0;
						}
					}
				}
			}
		} catch (Exception e) {
			e.printStackTrace();
		} finally {
			synchronized (this) {
				if (is != null) {
					try {
						is.close();
					} catch (IOException e) {
					}
					is = null;
				}
			}

			try {
				if (client != null) {
					client.close();
				}
			} catch (IOException ex) {
			}

			Log.d(TAG, "server stop listenToCommand address = " + mLocalAddress
					+ " port = " + mLocalPort);
		}
	}

	public void listenToData(TransProtocol protocol) {
		Socket client = null;
		InputStream is = null;
		String cmdArg = protocol.getCommandArg();

		String[] cmdList = cmdArg.split(TransProtocol.COMMAND_ARG_SPLIT);
		File saveFile = new File(cmdList[0]);
		try {
			if (saveFile.exists()) {
				saveFile.delete();
			}
			saveFile.createNewFile();
		} catch (IOException e) {
			e.printStackTrace();
		}

		OutputStream os = null;
		try {
			os = new FileOutputStream(saveFile);
		} catch (FileNotFoundException e1) {
			e1.printStackTrace();
		}

		Log.d(TAG, "server listenToData address = " + mLocalAddress
				+ " port = " + mLocalPort + " saveFile = " + cmdList[0]);

		try {
			if (mSocket != null) {
				client = mSocket.accept();
				is = client.getInputStream();
				int count = 0;
				byte buffer[] = new byte[BUFFER_SIZE];

				while (true) {
					count = is.read(buffer);
					if (count < 0) {
						break;
					}

//					Log.d(TAG, "listenToData count = " + count);
					os.write(buffer, 0, count);
				}
			}
		} catch (Exception e) {
			e.printStackTrace();
		} finally {
			synchronized (this) {
				if (is != null) {
					try {
						is.close();
					} catch (IOException e) {
					}
					is = null;
				}

				if (os != null) {
					try {
						os.flush();
						os.close();
					} catch (Exception e2) {
					}
					os = null;
				}
			}

			try {
				if (client != null) {
					client.close();
					client = null;
				}
			} catch (IOException ex) {
			}

			Log.d(TAG, "server stop listenToData address = " + mLocalAddress
					+ " port = " + mLocalPort);
		}
	}

	public void stop() {
		mQuit = true;
		
		if (mSocket != null) {
			try {
				mSocket.close();
			} catch (IOException e) {
				e.printStackTrace();
			}
			mSocket = null;
		}
	}

	private void setUp() {
		try {
			Log.d(TAG, "server setup address = " + mLocalAddress
					+ " port = " + mLocalPort);
			
			InetAddress address = InetAddress.getByName(mLocalAddress);
			mSocket = new ServerSocket(mLocalPort, 10, address);
			mSocket.setReuseAddress(true);

			mTransProtocol = new TransProtocol(mContext);
		} catch (Exception e) {
			Log.e(TAG, "setUp error " + e);
		}
	}
}
