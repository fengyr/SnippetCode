/**
 * @file       IMailNotify.java
 *
 * @revision:  none 
 *
 * @version    0.0.01
 * @author:    Zenki (zhajun), zenki2001cn@163.com
 * @date:      2012-3-5 上午10:03:43 
 */

package com.foxconn.supernote.email.util;

/**
 * UI层实现该接口，并将该对象作为参数初始化JMailEngine。
 * 邮件发送完毕会自动调用该接口的onFinished方法。
 * 使用方法参见JMailEngine
 * 
 * 示例：
 * 
 * IMailNotify notify = new IMailNotify() {
 *
 *		@Override
 *		public void onFinished(int code) {
 *			Log.d(TAG, "sendFinished");
 *			
 *			switch (code) {
 *			case 0:
 *				Toast.makeText(EmailTestActivity.this, "success",
 *						Toast.LENGTH_SHORT).show();
 *				break;
 *			case 1:
 *				Toast.makeText(EmailTestActivity.this, "failed",
 *						Toast.LENGTH_SHORT).show();
 *				break;
 *			case 2:
 *				Toast.makeText(EmailTestActivity.this, "timeout",
 *						Toast.LENGTH_SHORT).show();
 *				break;
 *
 *			default:
 *				break;
 *			}
 *		}
 *	};
 */
public interface IMailNotify {
	// 邮件发送成功的标志
	public static final int NOTIFY_CODE_SUCCESS = 0;
	// 邮件发送失败的标志
	public static final int NOTIFY_CODE_FAILED = 1;
	// 邮件发送超时的标志
	public static final int NOTIFY_CODE_TIMEOUT = 2;
	
	public void onClean(int code);
	// 邮件发送完毕后调用该方法，code值为发送是否成功的标志
	public void onFinished(int code);
}
