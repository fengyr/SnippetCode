#!/usr/bin/python
# -*- coding: utf-8 -*-
"""
	用于进行视频格式转换,需要读取视频图形配置列表
	配置表采用如下格式:
	站点:视频编号:组播地址:组播端口:图像格式(mpeg2|hik_m4|hik_cif|hik_d1)

	服务器创建一个TCP的服务，用于接入用户的图像使用请求。在TCP连接中，
	完成用户的有效性验证和用户视频的选择操作。同时返回一个有效的组播地址或者
	udp的单播地址。
"""
import config
import SocketServer
import md5
import os
from time import sleep,time
from select import select
from easyProcess import EasyProcess

VIDEO_RUNNING = True

class VideoSelect(SocketServer.StreamRequestHandler):
	"""
	用于处理用户具体的视频访问请求的类
	"""
	def __init__(self, request, client_address, server):
		"""
		构造函数,对用户的基本变量进行初始化
		"""
		SocketServer.StreamRequestHandler.__init__(self, request, client_address, server)
		self.rbufsize = 256
		self.wbufsize = 256
		self.client_port = 0
		self.client_video = ''

	def setup(self):
		print '远方计算机 %s:%d 建立连接' % (self.client_address[0], self.client_address[1])
		SocketServer.StreamRequestHandler.setup(self)

	def finish(self):
		print '远方计算机 %s:%d 断开连接' % (self.client_address[0], self.client_address[1])
		self.server.close_request( self.request )
		SocketServer.StreamRequestHandler.finish(self)

	def handle(self):
		
		localip = os.popen('/sbin/ifconfig | head -2 | tail -1 | cut -d: -f 2 | cut -d\  -f 1').read()
		#localip = localip[localip.find(':')+1:localip.find('\n')] 
		print localip
		
		print '远方计算机 %s:%d 处理数据' % (self.client_address[0], self.client_address[1])
		self.request.settimeout( config.SERVER_TIMEOUT )
		print 'self.request := ', self.request
		# 检查用户
		#cmds = []
		#try:
		#	cmd = ''
		#	while True:
		#		r_data = self.request.recv(256)
		#		if r_data == '': break
		#		cmd += r_data
		#		if cmd.find('\r') != -1 or cmd.find('\n') != -1: break
		#	#cmds = self.request.recv(256).strip().split(' ')
		#	cmds = cmd.strip().split(' ')
		#	for x in cmds: x.strip()
		#	if len(cmds) != 3 or cmds[0].lower() != 'login': raise Exception('非法命令')
		#	#print '%s:%s:md5$%s' % (cmds[1], cmds[2], md5.md5(cmds[2]).hexdigest())
		#	if not config.SERVER_AUTHORS.has_key( cmds[1] ) or 'md5$%s' % md5.md5(cmds[2]).hexdigest() != config.SERVER_AUTHORS[cmds[1]] :
		#		#print config.SERVER_AUTHORS[cmds[1]]
		#		raise Exception('用户%s不存在或密码错误' % cmds[1] )
		#	print '用户成功登录[%s]' % cmds[1]
		#	self.request.sendall('logined\n')
		#except Exception, e:
		#	print '远方计算机无法通过验证,',
		#	for x in e.args: print x,
		#	print ''
		#	return None
		# 选择视频源
		cmds = []
		try:
			cmd = ''
			while True:
				r_data = self.request.recv(256)
				if r_data == '': break
				cmd += r_data
				if cmd.find('\r') != -1 or cmd.find('\n') != -1: break
			#cmds = self.request.recv(256).strip().split(' ')
			cmds = cmd.strip().split(' ')
			for x in cmds: x.strip()
			if len(cmds) != 4 or cmds[0].lower() != 'select' or cmds[2].lower() != 'to': raise Exception('非法命令')
			if int(cmds[3]) < 2000 : raise Exception('非法的选择端口')
			if not config.SERVER_VIDEOS.has_key( cmds[1] ):
				raise Exception('指定的视频[%s]不存在' % cmds[1] )
			self.client_port = int(cmds[3])
			self.client_video = cmds[1]
		except Exception, e:
			print '远方计算机无法选择视频源,',
			for x in e.args: print x,
			print ''
			self.request.sendall('noexist\n')
			return None
		# 启动视频源的转码
		v = config.SERVER_VIDEOS[self.client_video]
		#cmd = './h264_ts %s %s:%s .libotest.%f.shm udp://%s %d' % (v[2], v[0], v[1], time(), self.client_address[0], self.client_port)
		cmd = 'vlc -vvv %s --daemon --quiet --sout=#transcode{vcodec=h264,vb=5000,scale=1}:duplicate{dst=std{access=http,mux=asf,dst=%s:%s}}' %(v[0],localip,str(self.client_port))
                print cmd
		x = EasyProcess(cmd)
		try:
			self.request.sendall('started\n')
			while VIDEO_RUNNING and x.poll() < 0:
				r,w,e = select([self.request],[],[],1)
				if len(r) != 0:
					cmd = ''
					while True:
						r_data = self.request.recv(256);
						if r_data == '': break
						cmd += r_data
						if cmd.find('\r') != -1 or cmd.find('\n') != -1: break
  					cmds = cmd.strip().split(' ')
					for y in cmds: y.strip()
					print y
							
					if len(r_data) == 0:
						print '远方计算机主动关闭了Socket'
						#x.stop()
						break;
                                        if cmds[0].lower() == 'stop':
                                         	#x.stop();
						print 'user stop vlc'
						break;   
		except:
			pass
		x.stop()
		self.request.sendall('stopped\n')
		#SocketServer.StreamRequestHandler.handle(self)

if __name__ == '__main__':
	#启动视频服务器
	video_svr = SocketServer.ThreadingTCPServer( ("", config.SERVER_CONTROL_PORT), VideoSelect )
	video_svr.allow_reuse_address = True
	print '视频服务启动于端口', config.SERVER_CONTROL_PORT
	try:
		video_svr.serve_forever()
	except:
		pass
	video_svr = None
	VIDEO_RUNNING = False
	print '视频服务已停止'
