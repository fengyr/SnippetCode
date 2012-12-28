/**
 * @file       SendMailInfo.java
 *
 * @revision:  none 
 *
 * @version    0.0.01
 * @author:    Zenki (zhajun), zenki2001cn@163.com
 * @date:      2012-3-2 上午08:32:02 
 */

package com.foxconn.supernote.email.util;

import java.util.ArrayList;

/**
 *
 */
public class SendMailInfo {
	// 发送邮箱地址: "zenki2001cn@163.com"
	public String strSendEmailAddr;
	// 接收邮箱地址: "zenki2001cn@163.com"
	public ArrayList<String> strRecvEmailAddr;
	// pop3服务器地址: "pop3.163.com"
	public String strPop3;
	// pop3服务器端口: 25
	public int nPop3Port;
	// smtp服务器地址: "smtp:163.com"
	public String strSmtp;
	// smtp服务器端口: 465
	public int nSmtpPort;
	// 发送用户名: "zenki2001cn"
	public String strSendUserName;
	// 发送密码: "xxxxx"
	public String strSendPassword;
	
	// 邮件主题
	public String strSubject;
	// 邮件正文内容
	public String strTextContent;
	// 邮件附件的文件名列表
	public ArrayList<String> listAttachments;
}
