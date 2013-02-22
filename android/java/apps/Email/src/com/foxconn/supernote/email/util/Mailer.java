/**
 * @file       Mailer.java
 *
 * @revision:  none 
 *
 * @version    0.0.01
 * @author:    Zenki (zhajun), zenki2001cn@163.com
 * @date:      2012-3-2 上午11:04:44 
 */

package com.foxconn.supernote.email.util;

import java.io.File;
import java.util.ArrayList;
import java.util.Date;
import java.util.List;
import java.util.Properties;

import javax.activation.DataHandler;
import javax.activation.DataSource;
import javax.activation.FileDataSource;
import javax.mail.Address;
import javax.mail.BodyPart;
import javax.mail.Message;
import javax.mail.MessagingException;
import javax.mail.Session;
import javax.mail.Transport;
import javax.mail.internet.InternetAddress;
import javax.mail.internet.MimeBodyPart;
import javax.mail.internet.MimeMessage;
import javax.mail.internet.MimeMultipart;

import android.util.Log;

public class Mailer {

	static private final String TAG = "Mailer";
	private Properties mProperties;
	private String mHost = "";
	private String mUser = "";
	private String mPassword = "";
	private MyAuth mAuth;
	private String mFrom = "";
	private ArrayList<String> mTo = null;
	private String mSubject = "";
	private String mTextContent = "";
	private ArrayList<String> mAttachments;

	public Mailer() {
		mProperties = new Properties();
	}

	public void setProperties(String host, String port, boolean auth) {
		mHost = host;
		
		try {
			mProperties.put("mail.smtp.host", host);
			mProperties.put("mail.smtp.port", port);
			mProperties.put("mail.smtp.auth", auth ? "true" : "false");
			
			if (host.equals("smtp.gmail.com")) {
				mProperties.put("mail.smtp.starttls.enable", "true");
				mProperties.setProperty("mail.transprot.protocol", "smtps");
			}
		} catch (Exception e) {
			Log.d(TAG, "setProperties e = " + e);
		}
	}

	public void setUserPassword(String user, String password) {
		mUser = user;
		mPassword = password;
		mAuth = new MyAuth(user, password);
	}
	
	public void setForm(String from) {
		mFrom = from;
	}
	
	public void setTo(ArrayList<String> to) {
		mTo = to;
	}
	
	public void setSubject(String subject) {
		mSubject = subject;
	}
	
	public void setTextContent(String content) {
		mTextContent = content;
	}
	
	public void setAttachments(List<String> attachments) {
		mAttachments = (ArrayList<String>) attachments;
	}

	private String createRandomStr() {
		char[] randomChar = new char[8];
		for (int i = 0; i < 8; i++) {
			randomChar[i] = (char) (Math.random() * 26 + 'a');
		}
		String replaceStr = new String(randomChar);
		return replaceStr;
	}

	public boolean sendMail() {
		Session sendMailSession = Session
				.getDefaultInstance(mProperties, mAuth);
		try {
			Message mailMessage = new MimeMessage(sendMailSession);
			Address from = new InternetAddress(mFrom);
			mailMessage.setFrom(from);
			
			if (mTo != null) {
				Address[] tos = new InternetAddress[mTo.size()];
				
				for (int i = 0; i < tos.length; i++) {
					tos[i] = new InternetAddress(mTo.get(i));
				}
				
			mailMessage.setRecipients(Message.RecipientType.TO, tos);
			}
			
			mailMessage.setSubject(mSubject);
			mailMessage.setSentDate(new Date());
			// mailMessage.setText(mTextContent);

			MimeMultipart main = new MimeMultipart();
			BodyPart bp = new MimeBodyPart();
			String htmlContent = "<meta http-equiv=Content-Type content=text/html; charset=UTF-8>"
					+ mTextContent;

			if (mAttachments != null) {
				htmlContent += "<br><br>";
				
				ArrayList<String> randomList = new ArrayList<String>();
				for (int i = 0; i < mAttachments.size(); i++) {
					String random = createRandomStr();
					htmlContent += "<img src=" + "cid:" + random + " />";
					randomList.add(random);
				}
				
				bp.setContent(htmlContent, "text/html;charset=UTF-8");
				main.addBodyPart(bp);

				for (int i = 0; i < mAttachments.size(); i++) {
					File file = new File(mAttachments.get(i));
					BodyPart body = new MimeBodyPart();
					DataSource data = new FileDataSource(file);
					body.setDataHandler(new DataHandler(data));
					body.setFileName(file.getName());

					String contentId = "<" + randomList.get(i) + ">";
					body.setHeader("Content-ID", contentId);
					main.addBodyPart(body);
				}
			}
			
			mailMessage.setContent(main);
			mailMessage.saveChanges();

			if (mHost.equals("smtp.gmail.com")) {
				Transport transport;
				transport = sendMailSession.getTransport("smtps");
				transport.connect(mHost, mUser, mPassword);
				transport.sendMessage(mailMessage,
						mailMessage.getAllRecipients());
				transport.close();
			} else {
				Transport.send(mailMessage);
			}
			
			Log.d(TAG, "SendMail ok");
			return true;
		} catch (MessagingException ex) {
			Log.d(TAG, "SendMail" + ex);
		}
		return false;
	}
}
