/**
 * @file       MyAuth.java
 *
 * @revision:  none 
 *
 * @version    0.0.01
 * @author:    Zenki (zhajun), zenki2001cn@163.com
 * @date:      2012-3-2 下午04:19:31 
 */

package com.foxconn.supernote.email.util;

import javax.mail.Authenticator;
import javax.mail.PasswordAuthentication;

/**
 *
 */
public class MyAuth extends Authenticator {
	String userName = "";
	String passWord = "";

	public MyAuth(String username, String password) {
		this.userName = username;
		this.passWord = password;
	}

	protected PasswordAuthentication getPasswordAuthentication() {
		return new PasswordAuthentication(userName, passWord);
	}
}
