/**
 * @file       AccountInfo.java
 *
 * @revision:  none 
 *
 * @version    0.0.01
 * @author:    Zenki (zhajun), zenki2001cn@163.com
 * @date:      2012-3-2 上午08:19:23 
 */

package com.foxconn.supernote.email.database;

/**
 *
 */
public class AccountInfo {
	public String strMailAddress;
	public String strUserName;
	public String strPassword;
	public String strPop3;
	public String strSmtp;
	public int nPort1;
	public int nPort2;
	/* 
	 * @param i : point the type of website
	 *           1--gmail
	 *           2--163
	 *           3--sina
	 * 
	 * @return  the account from which website ;
	 */
	public int nType;
	public long nLastLogin;
	
}

