package com.foxconn.edu_bbs.service;

interface ITransService 
{
	boolean runServer();
	boolean stopServer();
	String queryStatus(String arg);
	boolean transData(String orgFile);
	boolean zipFile(String srcDir, String desFile);
	boolean unzipFile(String srcFile, String desDir);
}