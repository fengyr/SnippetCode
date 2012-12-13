/**
 * @file       IMailSender.java
 *
 * @revision:  none 
 *
 * @version    0.0.01
 * @author:    Zenki (zhajun), zenki2001cn@163.com
 * @date:      2012-3-2 上午08:30:24 
 */

package com.foxconn.supernote.email.util;

/**
 *
 */
public interface IMailSender {
	public void sendMail(SendMailInfo info);
}
