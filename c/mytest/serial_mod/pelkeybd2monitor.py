#!/usr/bin/python
#encoding=utf-8

from ctypes import *
from select import *
import os, sys
import cPickle
import time
import thread

#串口模块
LIB_SERIAL_PATH = './serial.so'
libserial = None
QUIT = False

#串口参数设置
TTYNAME  = '/dev/ttyUSB0'
BAUDRATE = 4800
DATABITS = 8
STOPBITS = 1
PARITY   = 'n'
PACKSIZE = 32

#球机控制协议
QUIT_CAMERA_CONTROL_ADDRESS = 2
Procotol = { "tile_n":[0, 8],        #向上 
			 "tile_s":[0, 16],       #向下
			 "tile_w":[0, 4],        #向左
			 "tile_e":[0, 2],        #向右
			 "tile_nw":[0, 12],      #左上
			 "tile_ne":[0, 10],      #右上
			 "tile_se":[0, 18],      #右下
			 "tile_sw":[0, 20],      #左下
			 "focus_near":[2, 0],    #焦距近
			 "focus_far":[1, 0],     #焦距远
			 "lunar_close":[8, 0],   #光圈关
			 "lunar_open":[4, 0],    #光圈开
			 "zoom_in":[0, 32],      #拉近
			 "zoom_out":[0, 64],     #拉远
			 "auto_navi":[0, 150],   #自动导航
			 "cancel_navi":[0, 153], #关闭自动导航
			 "jump_point":[0, 7],    #跳转预置位
			 "set_point":[0, 3],     #设置预置位
			 "del_point":[0, 5],     #删除预置位
			 "stop":[0, 0]           #停止指令
		   }

#此处为自定义的回调函数
#func表示当前球机的行为，如拉进，拉远，向左，向右
#arg1,arg2表示行为的参数，视行为而定
def camera_call_back(func, arg1, arg2):
	 print func, arg1, arg2

#打开并初始化串口
def dev_init():
	global libserial
	
	try:
		libserial = cdll.LoadLibrary(LIB_SERIAL_PATH)
	except:
		print "Load libserial error\n"

	f = c_int()
	f = libserial.OpenDev(TTYNAME)
	if f == -1:
		sys.exit(1)
	libserial.SetSpeed(f, c_int(BAUDRATE))
	libserial.SetParity(f, c_int(DATABITS), c_int(STOPBITS), c_char(PARITY))

	return f

#关闭串口
def dev_close(f):
	global libserial

	libserial.CloseDev(f)
	print 'Serial port close...'

#数据校验
def check_sum(data):
	fix = data[0] ^ data[1] ^ data[2] ^ data[3] ^ data[4] ^ data[5] ^ data[6]
	if fix == data[7]:
		return True
	else:
		return False

def proce_data(alldata):
	id = 0
	try:
		id = alldata.index(160)
	except:
		return alldata

	valid_data = alldata[id:id+8]
	if check_sum(valid_data):
		if valid_data[1] == QUIT_CAMERA_CONTROL_ADDRESS:
			camera_call_back("quit_control", 0, 0)
		else:
			for i in Procotol.keys():
				if Procotol[i] == valid_data[2:4]:
					camera_call_back(i, valid_data[4], valid_data[5])

	return alldata[id+8:]

#数据处理线程
def poll(*param):
	global libserial
	global QUIT

	seri_f = param[0]
	rlist = [seri_f]
	wlist = []
	xlist = []
	time = None
	
	rbuf = create_string_buffer(PACKSIZE)
	alldata = []
	while not QUIT:
		(rlist, wlist, xlist) = select(rlist, wlist, xlist, time)
		for i in rlist:
			#接收并处理串口数据
			if i == seri_f:
				rsize = libserial.ReadData(seri_f, byref(rbuf), c_int(PACKSIZE))
				if rsize <= 0:
					QUIT = True
				alldata.extend([ord(i) for i in rbuf.raw[:rsize]])
				#print "alldata: ", alldata
				if len(alldata) >= 8:
					alldata = proce_data(alldata)

def server_run():
	seri_f = dev_init()	
	thread_poll = thread.start_new_thread(poll, (seri_f, 0))
	while not QUIT:
		time.sleep(1)
	dev_close(seri_f)

if __name__ == '__main__':
	server_run()

		

