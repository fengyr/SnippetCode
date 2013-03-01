/**
 * @file       TransProtocol.java
 *
 * @revision:  none 
 *
 * @version    0.0.01
 * @author:    Zenki (zhajun), zenki2001cn@163.com
 * @date:      2012-7-17 上午08:41:33 
 */

package com.foxconn.edu_bbs.utils;

import java.io.File;
import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStream;

import com.foxconn.edu_bbs.display.DisplayOnePicActivity;
import com.foxconn.edu_bbs.service.ITransCallback;

import android.content.Context;
import android.content.Intent;
import android.content.SharedPreferences;
import android.util.Log;

/**
 *
 */
public class TransProtocol {
	static private final String TAG = "TransProtocol";

	static public final int COMMAND_QUERY_STATUS = 1;
	static public final int COMMAND_QUERY_RESULT = 2;
	static public final int COMMAND_TRANS_DATA = 3;
	static public final int COMMAND_RESPONSE_OK = 10;
	static public final int COMMAND_RESPONSE_FAILED = 11;
	static public final int DATA_RESPONSE_OK = 12;
	static public final int DATA_RESPONSE_FAILED = 13;

	static public final String COMMAND_PREFIX = "BBS:";
	static public final String COMMAND_ARG_SPLIT = ":";

	static private final String MSG_QUERY_STATUS = "QUERY_STATUS";
	static private final String MSG_QUERY_RESULT = "QUERY_RESULT";
	static private final String MSG_TRANS_DATA = "TRANS_DATA";
	static private final String MSG_COMMAND_OK = "COMMAND_OK";
	static private final String MSG_COMMAND_FAILED = "COMMAND_FAILED";
	static private final String MSG_DATA_OK = "DATA_OK";
	static private final String MSG_DATA_FAILED = "DATA_FAILED";

	private static final String TRANS_FILE_FORMAT = "%s/%s-%s-%s-%d.zip";
	private static final String COMMAND_ARG_FORMAT = "%s:%d";

	private int mCommandType = 0;
	private String mCommandArg = "";
	private ITransCallback mListener = null;
	private TransConfig mConfig = TransConfig.getInstance();
	private Context mContext;
	private SharedPreferences mSharedPreferences;
	private final String SPNAME = "deviceNumber";

	private int mRetryCount = TransConfig.TRANS_RETRY_COUNT;

	private final int MSG_SEND_DATA = 1;

	// private Handler mHandler = new Handler() {
	// public void handleMessage(Message msg) {
	// switch (msg.what) {
	// case MSG_SEND_DATA:
	// File file = (File) msg.obj;
	//
	// mRetryCount--;
	// transDataToNext(file);
	// if (mRetryCount <= 0) {
	// mRetryCount = 3;
	// }
	// break;
	//
	// default:
	// break;
	// }
	// };
	// };

	// public interface TransListener {
	// public void action(String result);
	// }

	public TransProtocol(Context context, int type, String arg) {
		mContext = context;
		mCommandType = type;
		mCommandArg = arg;

		mSharedPreferences = mContext.getSharedPreferences(SPNAME,
				Context.MODE_WORLD_WRITEABLE);
	}

	public TransProtocol(Context context, int type) {
		mContext = context;
		mCommandType = type;

		mSharedPreferences = mContext.getSharedPreferences(SPNAME,
				Context.MODE_WORLD_WRITEABLE);
	}

	public TransProtocol(Context context) {
		mContext = context;

		mSharedPreferences = mContext.getSharedPreferences(SPNAME,
				Context.MODE_WORLD_WRITEABLE);
	}

	public void setListener(ITransCallback listener) {
		mListener = listener;
	}

	public String getCommandArg() {
		return mCommandArg;
	}

	public void setCommandArg(String arg) {
		mCommandArg = arg;
	}

	static public String getEventFromType(int type, String mArg) {
		String msg = new String();
		byte[] tail = { '\r', '\n' };

		switch (type) {
		// BBS:QUERY_STATUS num
		case COMMAND_QUERY_STATUS:
			msg += COMMAND_PREFIX + MSG_QUERY_STATUS + " " + mArg;
			break;

		// BBS:QUERY_RESULT num
		case COMMAND_QUERY_RESULT:
			msg += COMMAND_PREFIX + MSG_QUERY_RESULT + " " + mArg;
			break;

		// BBS:TRANS_DATA name:size
		case COMMAND_TRANS_DATA:
			msg += COMMAND_PREFIX + MSG_TRANS_DATA + " " + mArg;
			break;

		// BBS:COMMAND_RESPONSE_OK
		case COMMAND_RESPONSE_OK:
			msg += COMMAND_PREFIX + MSG_COMMAND_OK;
			break;

		// BBS:COMMAND_RESPONSE_FAILED
		case COMMAND_RESPONSE_FAILED:
			msg += COMMAND_PREFIX + MSG_COMMAND_FAILED;
			break;

		// BBS:DATA_RESPONSE_OK
		case DATA_RESPONSE_OK:
			msg += COMMAND_PREFIX + MSG_DATA_OK;
			break;

		// BBS:DATA_RESPONSE_FAILED
		case DATA_RESPONSE_FAILED:
			msg += COMMAND_PREFIX + MSG_DATA_FAILED;
			break;
		default:
			break;
		}

		msg = msg + new String(tail);

		return msg;
	}

	// 将Client端传来的数值加1后，继续向下一个服务器传递，
	// 直到遇到连接错误，则将该值返回给上一个Client端
	private void handleQueryStatus(String cmdArg) {
		Log.d(TAG, "handleQueryStatus MSG_QUERY_STATUS arg = " + cmdArg);

		TransChannelClient client = null;
		int response_num = Integer.valueOf(cmdArg);
		final String nextArg = String.format("%d", response_num + 1);

		// FIXME: 本地测试多个服务端时，使用端口号 + 1，测试另一个服务器
		// 实际使用将替换固定的IP和端口
		client = new TransChannelClient(mContext, mConfig.getRemoteIp(),
				TransConfig.COMMAND_PORT);

		if (!client.isValidate()) {
			Log.d(TAG, "handleQueryStatus !client.isValidate() = ");

			handleQueryResult(nextArg);
			return;
		}

		TransProtocol command = new TransProtocol(mContext,
				TransProtocol.COMMAND_QUERY_STATUS, nextArg);
		command.setListener(new ITransCallback() {
			@Override
			public void action(String result) {
				Log.i(TAG, "handleQueryStatus action result = " + result);
				if (result.equals(MSG_COMMAND_FAILED)) {
					handleQueryResult(nextArg);
				} else {
					handleQueryResult(result.split(" ")[1]);
				}
			}
		});

		client.transformCommand(command);
		client.waitRespond();

		client.stop();
	}

	private void handleQueryResult(String cmdArg) {
		Log.d(TAG, "handleQueryResult MSG_QUERY_RESULT arg = " + cmdArg);
		if (mListener != null) {
			mListener.action(getEventFromType(COMMAND_QUERY_RESULT, cmdArg));
		}
	}

	private void handleTransData(String cmdArg) {
		Log.d(TAG, "handleTransData MSG_TRANS_DATA arg = " + cmdArg);
		// 向Client端返回命令状态，与Client端同步，接下来开启数据接收服务
		if (mListener != null) {
			mListener.action(getEventFromType(COMMAND_RESPONSE_OK, ""));
		}

		TransChannelServer server = new TransChannelServer(mContext,
				mConfig.getServerIp(), TransConfig.DATA_PORT);
		server.startServer(this);

		Log.d(TAG, "handleTransData !!!! MSG_TRANS_DATA response");
		String[] cmdList = cmdArg.split(COMMAND_ARG_SPLIT);
		File file = new File(cmdList[0]);
		long size = Integer.valueOf(cmdList[1]);
		boolean res = false;

		if ((file != null) && (file.exists())) {
			res = (size == file.length()) ? true : false;
		} else {
			res = false;
		}

		// FIXME:
		// 1.如果验证文件大小正确，则返回DATA_RESPONSE_OK的状态
		// 通知Client端数据接收完成。否则Client端会自动重新发送数据
		// 2.更新DeviceNumber，显示模块会根据该值查找相应的图片。
		if (mListener != null) {
			if (res) {
				String shortName = file.getName();
				int deviceNum = Integer.valueOf(shortName.substring(0,
						shortName.lastIndexOf(".")).split("-")[3]);
				saveDeviceNumber(deviceNum);

				ZipHelper ziphelp = new ZipHelper();
				ziphelp.unZip(file.getAbsolutePath(), mConfig.getTargetDir());
				mListener.action(getEventFromType(DATA_RESPONSE_OK, ""));
			} else {
				mListener.action(getEventFromType(DATA_RESPONSE_FAILED, ""));
			}
		}

		server.stop();

		// 向下一个服务器传输数据
		if (res) {
			sendUpdateIntent();
			transDataToNext(file);
		}
	}

	public void transFile(String srcFile) {
		File file = new File(srcFile);

		if (file.exists()) {
			transDataToNext(file);
		}
	}

	private void transData(File file) {
		// FIXME: 测试使用端口号 + 1
		TransChannelClient data_client = new TransChannelClient(mContext,
				mConfig.getRemoteIp(), TransConfig.DATA_PORT);
		if (data_client != null) {
			try {
				InputStream is = new FileInputStream(file);
				byte[] buf = new byte[1024];
				int len = 0;

				while (true) {
					try {
						len = is.read(buf);
						if (len <= 0) {
							break;
						}

						data_client.transformData(buf, len);
					} catch (IOException e) {
						e.printStackTrace();
					}
				}
			} catch (Exception e) {
				e.printStackTrace();
			}
			data_client.stop();
		}
	}

	private void saveDeviceNumber(int deviceNum) {
		if (null == mSharedPreferences) {
			mSharedPreferences = mContext.getSharedPreferences(SPNAME,
					Context.MODE_WORLD_WRITEABLE);
		}
		
		SharedPreferences.Editor editor = mSharedPreferences.edit();
		editor.putInt("DeviceNumber", deviceNum);
		boolean rtn = editor.commit();
		
		if (!rtn) {
			Log.e(TAG, "saveDeviceNumber error !!!");
		}
	}

	private void sendUpdateIntent() {
		if (mContext != null) {
			Intent intent = new Intent();
			intent.setAction(DisplayOnePicActivity.UPDATE_INTENT);
			mContext.sendBroadcast(intent);
			Log.i(TAG, "sendUpdateIntent !!!");
		}
	}

	private boolean wakeupDevice() {
		File wakeupFile = new File(TransConfig.WAKEUP_INTERFACE);
		if (!wakeupFile.exists()) {
			Log.e(TAG, "Can not find /proc/chg_enable !!!");
			return false;
		}

		FileOutputStream os = null;
		String msg = String.format("%d", 1);
		try {
			os = new FileOutputStream(wakeupFile);
			os.write(msg.getBytes());
			os.flush();
			os.close();
		} catch (FileNotFoundException e) {
			e.printStackTrace();
		} catch (IOException e) {
			e.printStackTrace();
		}

		Log.i(TAG, "Transdata wakeupDevice");

		return true;
	}

	private void transDataToNext(final File file) {
		// TODO: 唤醒slave机台
		wakeupDevice();

		// FIXME:测试使用端口号 + 1
		final TransChannelClient command_client = new TransChannelClient(
				mContext, mConfig.getRemoteIp(), TransConfig.COMMAND_PORT);
		long size = file.length();
		String oldName = file.getName();
		String shortName = oldName.substring(0, oldName.lastIndexOf("."));
		Log.e(TAG, "shortName = " + shortName);
		String[] part2 = shortName.split("-"); // part[0]=bbs, part[1]=aa01,
												// part[2]=31, part[3]=0
		String newName = String.format(TRANS_FILE_FORMAT, file.getParent(),
				part2[0], part2[1], part2[2], Integer.valueOf(part2[3]) + 1);
		String cmdArg = String.format(COMMAND_ARG_FORMAT, newName, size);
		Log.e(TAG, "transDataToNext commandArg = " + cmdArg);

		TransProtocol command = new TransProtocol(mContext,
				TransProtocol.COMMAND_TRANS_DATA, cmdArg);
		command.setListener(new ITransCallback() {
			@Override
			public void action(String result) {
				Log.i(TAG, "transDataToNext action result = " + result);
				if (result.equals(TransProtocol.getEventFromType(
						TransProtocol.DATA_RESPONSE_FAILED, ""))
						&& (mRetryCount > 0)) {
					Thread thread = new Thread() {
						public void run() {
							// Message msg = Message.obtain();
							// msg.what = MSG_SEND_DATA;
							// msg.obj = file;
							// mHandler.sendMessageDelayed(msg, 1000);
							try {
								Thread.sleep(1000);
							} catch (InterruptedException e) {
								e.printStackTrace();
							}

							mRetryCount--;
							transDataToNext(file);
							if (mRetryCount <= 0) {
								mRetryCount = TransConfig.TRANS_RETRY_COUNT;
							}
						};
					};
					thread.start();
				} else if (result.equals(TransProtocol.getEventFromType(
						TransProtocol.COMMAND_RESPONSE_OK, ""))) {
					transData(file);
					command_client.waitRespond();
				}
			}
		});

		if (command_client != null) {
			command_client.transformCommand(command);
			command_client.waitRespond();

			Log.e(TAG, "transDataToNext command_client.stop()");
			command_client.stop();
		}
	}

	protected boolean onTransform(String msg) {
		boolean rtn = false;
		String buffer = "";
		String[] context = { "", "" };

		if (msg.startsWith(COMMAND_PREFIX)) {
			buffer = msg.substring(COMMAND_PREFIX.length());
			context = buffer.split(" ");
		}

		for (String str : context) {
			Log.e(TAG, "context = " + str);
		}

		if (context.length > 1) {
			mCommandArg = context[1];
		}

		if (context[0] != null) {
			if (context[0].equals(MSG_QUERY_STATUS)) {
				handleQueryStatus(context[1]);
				rtn = true;
			} else if (context[0].equals(MSG_QUERY_RESULT)) {
				handleQueryResult(context[1]);
				rtn = true;
			} else if (context[0].equals(MSG_TRANS_DATA)) {
				handleTransData(context[1]);
				rtn = true;
			} else if (context[0].equals(MSG_COMMAND_OK)) {
				Log.d(TAG, "onTransform MSG_RESPONSE_OK");
				if (mListener != null) {
					mListener.action(getEventFromType(COMMAND_RESPONSE_OK, ""));
				}
				rtn = true;
			} else if (context[0].equals(MSG_COMMAND_FAILED)) {
				Log.d(TAG, "onTransform MSG_COMMAND_FAILED");
				if (mListener != null) {
					mListener.action(getEventFromType(COMMAND_RESPONSE_FAILED,
							""));
				}
				rtn = true;
			} else if (context[0].equals(MSG_DATA_OK)) {
				Log.d(TAG, "onTransform MSG_RESPONSE_OK");
				if (mListener != null) {
					mListener.action(getEventFromType(DATA_RESPONSE_OK, ""));
				}
				rtn = true;
			} else if (context[0].equals(MSG_DATA_FAILED)) {
				Log.d(TAG, "onTransform MSG_COMMAND_FAILED");
				if (mListener != null) {
					mListener
							.action(getEventFromType(DATA_RESPONSE_FAILED, ""));
				}
				rtn = true;
			}
		}

		return rtn;
	}

	protected String getEvent() {
		return getEventFromType(mCommandType, mCommandArg);
	}
}
