#!/usr/bin/python
#!encoding=utf8
import glob
import sys, os
from subprocess import *
import config

#视频转换成图片格式
def video2pic(inputfile, outputfile, duration=20, fps=25):
	try:
		os.mkdir(config.DIR)
	except OSError:
		pass

	command = 'ffmpeg -r %d -i %s -t %d %s' % (fps, inputfile, duration, outputfile)
	print 'Convert %s to %s, duration %d sec...' % (inputfile, outputfile, duration)
	p = Popen(command.split(' '),  stderr=PIPE)
	os.waitpid(p.pid, 0)
	
#vcodec必须与图片格式对应
#输出文件后缀名必须一致
def pic2video(inputfile, outputfile, codec, extend, scale, fps=25, author='Zenki'):
	command = 'ffmpeg -i %s -y -s %s -author %s -r %d -vcodec %s %s%s' % (inputfile, scale, author, fps, codec, outputfile, extend)
	print 'Convert %s to %s%s...' % (inputfile, outputfile, extend)
	p = Popen(command.split(' '),  stderr=PIPE)
	os.waitpid(p.pid, 0)

def videodump(inputfile, outputfile, scale, duration):
    command = 'ffmpeg -i %s -y -s %s -t %d %s%s' % ( inputfile, scale, duration, outputfile, '.yuv')
    print 'Dumping YUV...'
    p = Popen( command.split(' '), stderr=PIPE )
    os.waitpid( p.pid, 0 )
