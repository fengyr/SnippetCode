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
 * UI层实现该接口，并注册给
 */
public interface IMailNotify {
	public static final int NOTIFY_CODE_SUCCESS = 0;
	public static final int NOTIFY_CODE_FAILED = 1;
	public static final int NOTIFY_CODE_TIMEOUT = 2;
	
	public void onFinished(int code);
}
