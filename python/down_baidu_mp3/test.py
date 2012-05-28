#!/usr/bin/python
# -*- coding: utf-8 -*-

import httplib
import re
import urllib
import os
import locale
import pdb

topid = ['/topso/mp3topsong.html',     #Top500
		 '/list/newhits.html']         #New100

def get_http_url(host, header):
	conn = httplib.HTTPConnection(host)
	conn.request("GET",header)
	response = conn.getresponse()
	html = response.read()
	conn.close()
	return html

def get_url_url(url):
	data = ''
	try:
		urlopen = urllib.URLopener()
		fp = urlopen.open(url)
		data = fp.read()
		fp.close()
	except IOError, errmsg:
		print errmsg

	return data

def get_mp3_data(context, songname):
	regkey = 'href="(.*tn=baidumt.*)" onclick'
	url_list = re.findall(regkey, context)
	print url_list[0]
	tn = re.findall('&tn=baidumt,(.*)&word', url_list[0])
	print tn
	url_list[0] = url_list[0].replace(tn[0], songname+'%20%20')
	print url_list[0]
	data = get_url_url(url_list[0])
	f = open('m.html', 'w+')
	f.write(data)
	f.close()

def get_music_url(context):
	regkey = 'http://mp3.baidu.com/m\?tn=baidump3&ct=134217728&lm=-1&li=2&word=(.*\+.*)"'
	url_list = re.findall(regkey, context)
#	print url_list
	for i in url_list:
		#pdb.set_trace()
		url = 'http://mp3.baidu.com/' + 'm?tn=baidump3&ct=134217728&lm=-1&li=2&word=' + i
		#print url

		get_mp3_data(get_url_url(url), i.split('+')[0])
		return
			
		#h = get_http_url('mp3.baidu.com', header)
		#print h

if __name__ == "__main__":
	#pdb.set_trace()
	html = get_http_url('list.mp3.baidu.com', topid[1])
	get_music_url(html)
	#html=response.read().decode('gbk')
	#f = open('mp3.html', 'w+')
	#f.write(html)
	#f.close()
	#print html

	#urlopen = urllib.URLopener()
	#fp = urlopen.open('http://forum.ubuntu.org.cn/download.php?id=27015')
	#data = fp.read()
	#f = open('2.tar.gz', 'w+b')
	#f.write(data)
	#fp.close()
	#f.close()

