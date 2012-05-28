#!/usr/bin/python
# -*- coding: utf-8 -*-
# Copyright (c) 2006 UbuntuChina <http://www.ubuntu.org.cn>
# License: GPLv2
# Author: oneleaf <oneleaf AT gmail.com>
# hack by ct <ctqucl AT gmail.com>

import httplib
import re
import urllib
import os
import locale
import pdb
global m,topid
global fsize
fsize=2     #文件大小下限(M)
m='0'       #'-1'=任意  '0'=mp3 '1'=rm '2'=wma '3'=asf '4'=ram '5'=mid '6'=flash
topid='1'

if topid=='0':
     topid='/list/newhits.html'         #新歌100
elif topid=='1':
     topid='/topso/mp3topsong.html'     #Top500
elif topid=='2':
     topid='/list/oldsong.html'         #老歌经典
elif topid=='3':
     topid='/list/movies.html'          #电影金曲
elif topid=='4':
     topid='/list/tvs.html'             #电视歌曲
elif topid=='5':
     topid='/minge/mp3topsong.html'     #民歌精选
elif topid=='6':
     topid='/xiaoyuan/mp3topsong.html'  #校园歌曲
elif topid=='7':
     topid='/list/liujinsuiyue.html'    #流金岁月(new)
elif topid=='8':
     topid='/list/yaogun.html'          #摇滚地带


def getdownfileurl(url):                #获取歌曲页的试听URL
    url = "http://220.181.27.54/m"+url
    tn = re.search('&tn=(.*)&word',url).group(0)
    url=url.replace(tn,'&tn=baidusg,mp3%20%20&word')
    try:
        urlopen = urllib.URLopener()
        fp=urlopen.open(url)
        data = fp.read()
        fp.close()
    except IOError, errmsg:
        print errmsg
    expression2='"_blank">(.*)</a></a></li>'
    url = re.search(expression2, data).group(0)[16:-13]
    try:
        url="http://"+urllib.quote(url)
    except:pass
    print u"发现 "+url
    return url

def getdownurl(url):                     #从歌曲页抓取URL列表
    urllist=[]
    urllist1=[]
    urllist2=[]
    conn = httplib.HTTPConnection('mp3.baidu.com')
    conn.request("GET",url)
    response = conn.getresponse()
    html=response.read()
    conn.close()
    expression2='http://220.181.27.54/m(.*)" target'
    listSentence2 = re.findall(expression2, html)     #抓取链接列表
    filesize=re.findall('<td>(.*)M</td>',html)        #抓取文件大小
    lineno=0
    while lineno<len(listSentence2):
        mp3url=getdownfileurl(listSentence2[lineno])  #转换链接为最终下载地址
        urllist1.append(mp3url)
        lineno+=1
    urllist=map(None,urllist1,filesize)
    return urllist

def downmp3(url,author,name,filelist):    #下载歌曲
    filename=author+"-"+name;
    for i in filelist:
        name=unicode(i,locale.getpreferredencoding())
        if name.find(filename) == 0:      #忽略
            print u"文件已经下载，忽略。"
            return 1
    urllists=getdownurl(url)              #获取文件url列表
    lineno=0
    while lineno<len(urllists):
        print u"尝试",urllists[lineno][0]
		#print urllists[lineno][0]
        ext=urllists[lineno][0][-4:]      #获取文件名后缀(最后4位)
        try:
            lineno+=1
            print urllists[lineno-1][1] +'M'
            if float(urllists[lineno-1][1])>float(fsize) :   #大小符合则下载
                  urlopen = urllib.URLopener()
                  fp=urlopen.open(urllists[lineno-1][0])
                  data = fp.read()
                  fp.close()
                  filename=filename+ext;
                  file=open(filename,'w+b')
                  file.write(data)
                  file.close()
                  print u"下载成功!"
                  return 1
            elif float(urllists[lineno][1])<float(fsize) :   #不符则略过
                  print u"文件太小,忽略!"
        except:
            continue
    return 0

if __name__ == "__main__":
	conn = httplib.HTTPConnection('list.mp3.baidu.com')
	conn.request("GET",topid )
	response = conn.getresponse()
	#print response.read()
	html=response.read().decode('gbk')
	#print html
	conn.close()
	expression1='border">(.*).</td>'
	expression2='><a href="http://mp3.baidu.com/m(.*)</a>'
	expression3='href="http://mp3.baidu.com/m(.*)</td>'
	listSentence1 = re.findall(expression1, html)   #编号特征
	listSentence2 = re.findall(expression2, html)   #歌曲名特征
	listSentence3 = re.findall(expression3, html)   #歌手名特征
	lineno=0
	while lineno<len(listSentence1):
		listSentence2[lineno]=listSentence2[lineno].replace('m=-1','m=' + m) #指定格式
		url=re.search('(.*)target',listSentence2[lineno])
		url='/m'+url.group(0)[:-8]
		idno=listSentence1[lineno]
		name=re.search('blank>(.*)',listSentence2[lineno])
		name=name.group(0)[6:]
		dirty=re.search('</A>/<A  href=(.*) target=_blank>',listSentence3[lineno])
		try:
			if dirty is not None :
				author1=re.search('>(.*)</A>/<A',listSentence3[lineno])
      			author1=author1.group(0)[1:-7]
      			author2=re.search('/<A  href=(.*)</A>',listSentence3[lineno])
      			author2=re.search('>(.*)<',author2.group(0))
      			author2=author2.group(0)[1:-1]
      			author=author1 + '+' + author2
		except:
			pass
		if dirty is None :
			author=re.search('blank>(.*)</',listSentence3[lineno])
			author=author.group(0)[6:-2]

		print u"开始下载",idno,name,author
		filelist=os.listdir('.');
		if downmp3(url,author,name,filelist)==0:  #判断失败
			print u"下载",author,name,u'失败！'
		lineno+=1
