/**
 * @file       JMailEngine.java
 *
 * @revision:  none 
 *
 * @version    0.0.01
 * @author:    Zenki (zhajun), zenki2001cn@163.com
 * @date:      2012-3-2 上午10:12:37 
 */

package com.foxconn.supernote.email.util;


import android.os.Handler;
import android.os.Message;
import android.util.Log;

/**
 * 邮件发送的实现类，该类实现了IMailSender接口，如果需要得到邮件通知，
 * 需要IMailNotify对象作为初始化的参数。
 * 
 * 示例：
 * 
 *  IMailSender sender = new JMailEngine(notify);
 *	sender.sendMail(info);
 */
public class JMailEngine implements IMailSender {

	static private final String TAG = "JmailEngine";
	public static final int SEND_MAIL_FINISH = 0;
	public static final int SEND_MAIL_CLEAN = 1;
	
	private IMailNotify mNotify;
	
	Handler mHandler = new Handler () {
		@Override
		public void dispatchMessage(Message msg) {
			switch (msg.what) {
			case SEND_MAIL_FINISH:
				if (mNotify != null) {
					mNotify.onFinished(msg.arg1);
					
					Message msg2 = Message.obtain();
					msg2.what = JMailEngine.SEND_MAIL_CLEAN;
					msg2.arg1 = msg.arg1;
					sendMessageDelayed(msg2, 3000);
				}
				break;
				
			case SEND_MAIL_CLEAN:
				if (mNotify != null) {
					mNotify.onClean(msg.arg1);
				}
				break;

			default:
				break;
			}
			super.dispatchMessage(msg);
		}
	};
	
	static class SendMailThread extends Thread {
		private SendMailInfo mMailInfo;
		private Handler mHd;
		
		public SendMailThread(Handler handler, SendMailInfo info) {
			mMailInfo = info;
			mHd = handler;
		}
		
		@Override
		public void run() {
			Mailer mail = new Mailer();
			mail.setProperties(mMailInfo.strSmtp, String.valueOf(mMailInfo.nSmtpPort), true);
			mail.setUserPassword(mMailInfo.strSendUserName, mMailInfo.strSendPassword);
			mail.setForm(mMailInfo.strSendEmailAddr);
			mail.setSubject(mMailInfo.strSubject);
			mail.setTextContent(mMailInfo.strTextContent);
			mail.setAttachments(mMailInfo.listAttachments);
			mail.setTo(mMailInfo.strRecvEmailAddr);
			dumpMailInfo();
			boolean rtn = mail.sendMail();
			
			Message msg = Message.obtain();
			msg.what = JMailEngine.SEND_MAIL_FINISH;
			msg.arg1 = rtn ? 0 : 1;
			mHd.sendMessage(msg);
			
			super.run();
		}
		
		private void dumpMailInfo() {
			Log.d(TAG, "MailInfo: mMailInfo.strSmtp = " + mMailInfo.strSmtp + "\n mMailInfo.nSmtpPort = " + mMailInfo.nSmtpPort
					+ "\n mMailInfo.strSendUserName = " + mMailInfo.strSendUserName + "\n mMailInfo.strSendPassword = " + mMailInfo.strSendPassword
					+ "\n mMailInfo.strSendEmailAddr = " + mMailInfo.strSendEmailAddr + "\n mMailInfo.strSubject = " + mMailInfo.strSubject
					+ "\n mMailInfo.strTextContent = " + mMailInfo.strTextContent);
		}
	}
	
	public JMailEngine() {
		
	}
	
	public JMailEngine(IMailNotify notify) {
		mNotify = notify;
	}
	
	@Override
	public void sendMail(SendMailInfo info) {
		
		SendMailThread thread = new SendMailThread(mHandler, info);
		if (thread != null) {
			thread.start();
		}
	}

}
