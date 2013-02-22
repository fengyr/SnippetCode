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
 * 邮件发送功能的接口，需要将SendMailInfo对象实例化实例后
 * 作为参数传递给sendMail方法。
 * 
 *  示例1：
 * 
 *  IMailSender sender = new JMailEngine();
 *  
 *  示例2：
 *  IMailSender sender = new JMailEngine(notify);
 */
public interface IMailSender {
	public void sendMail(SendMailInfo info);
}
