#!/usr/bin/python
# -*- coding: utf-8 -*-
"""
通过对os模块中的execvp进行包装,允许用户很简单地启动和中止一个子进程
"""
import os
import sys
import signal
import string
import thread
from time import sleep

def function(*arg):
	while True:
		try:
			os.wait()
		except OSError:
			pass
		sleep(1)
	
class EasyProcess:
	"""
	一个简单的子进程启停类
	"""
	def __init__(self):
		"""
		建立一个子进程EasyProcess类，并启动之。cmd参数可以采用shell下输入命令的等同方式。但目前不支持管道操作符。
		"""
		self.cmd = ''
		self.pid = -1
		#self.start(cmd)
		
	

	def start(self, _file, argv = []):
		"""
		启动用户子进程,可以配合使用参数列表,启动结果可以通过pid属性查看
		"""
		params = _file.split(' ') + argv
		a = ()
		thread.start_new_thread(function,a)
		pid = os.fork()
		if pid == 0:
			childpid = os.fork()
			if childpid > 0:
				os._exit(0)
			if childpid == 0:
				os.execvp(params[0], params)
				os._exit(0)
		sleep( 1 )
		s = os.popen('ps axf | grep vlc | cut -c1-6').read()
		s = s.split()
		print s
		self.pid = string.atoi(s[-1])
		self.cmd = _file
		return self.pid

	def stop(self, pid):
		"""
		停止用户子进程
		"""
		self.pid = pid
		try:
			if self.poll(pid) < 0:
				print '停止用户子进程', self.pid
				os.kill( self.pid, signal.SIGTERM )
				sleep( 0.1 )
				if self.poll() < 0:
					os.kill( self.pid, signal.SIGKILL )
		except:
			pass

	def poll(self, pid):
		"""
		检测子进程是否存在,如果子进程在运行返回-1,否则返回子进程状态
		"""
		stat = 0
		self.pid = pid
		try:
			x,stat = os.waitpid( self.pid, os.WNOHANG or os.WUNTRACED )
		except:
			x = 0
			pass
		if x == 0: return -1
		if os.WIFSTOPPED(stat) or os.WIFEXITED(stat) or os.WIFSIGNALED(stat):
			return self.pid
		return -1
		
	def wait(self):
		"""
		等待子进程结束,并返回子进程状态
		!!!该方法还需要调试
		"""
		stat = 0
		try:
			x,stat = os.wait( self.pid )
		except:
			x = 0
			pass
		if x == 0: return -1
		if os.WIFSTOPPED(stat) or os.WIFEXITED(stat) or os.WIFSIGNALED(stat):
			return stat
		return -1
