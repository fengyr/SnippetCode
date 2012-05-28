#!/usr/bin/python
#!encoding=utf8
import socket
import sys, os, time
import threading, thread, mutex
from select import *
import pdb

#转码服务器的地址和端口
REMOTE_PORT = 8801
REMOTE_IP   = '192.168.100.187'
#本地地址和端口
LOCAL_PORT  = 8801
LOCAL_IP    = '192.168.100.54'
#接收客户请求列表
RECV_LIST   = {}
MUTEX       = mutex.mutex()

def thread_trans(*arg):
	self = arg[0]
	rlist = [self.s_sock_tcp, self.s_sock_udp, self.c_sock_tcp]
	wlist = [self.c_sock_udp]
	xlist = []
	timeout = 1

	quit = False
	while not quit:
		(rl, wl, xl) = select(rlist, wlist, xlist, timeout)
		#pdb.set_trace()
		for i in rl:
			try:
				#转码服务器断开连接
				if i == rlist[0]:
					buf = i.recv(64)
					if len(buf) == 0:
						print "Remote Server disconnect"
						quit = True
						break
				#接收转码包
				if i == rlist[1]:
					buf = i.recv(1400)
					for j in wl:
						if j == wlist[0]:
							j.sendto(buf, (self.c_addr, self.c_port_udp))
				#监听客户端连接
				if i == rlist[2]:
					buf = i.recv(64)
					if len(buf) == 0:
						quit = True
						break
			except:
				print 'Thread trans error'
		#sys.stdout.write('...')
		#sys.stdout.flush()
		time.sleep(0.00001)

	#pdb.set_trace()
	#删除客户请求列表
	self.decrease_one_trans()
	if self.count == 0:
		if RECV_LIST.has_key(self.id_port):
			MUTEX.testandset()	 
			del RECV_LIST[self.id_port]
			MUTEX.unlock()
		print RECV_LIST
		self.running = False

	return 0

class Transer(threading.Thread):
	def __init__(self, sock, msg, id, addr):
		self.count      = 0
		self.id_port    = id
		self.c_sock_tcp = sock
		self.s_port_udp = int(id) + 1
		self.c_port_udp = int(id)
		self.c_addr     = addr
		try:
			#pdb.set_trace()
			self.c_sock_udp = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
			self.s_sock_udp = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
			self.s_sock_udp.bind((LOCAL_IP, self.s_port_udp))
			self.s_sock_tcp = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
			self.s_sock_tcp.connect((REMOTE_IP, REMOTE_PORT))

			#向转码服务器建立请求
			res = msg.split()
			msg_new = res[0] + ' ' + res[1] + ' ' + res[2] + ' ' + str(self.s_port_udp) + '\n'
			print "Send MSG:", msg_new
			self.s_sock_tcp.send(msg_new)
		except:
			print 'Transer socket init error'

		#pdb.set_trace()
		self.add_one_trans()
		threading.Thread.__init__(self)
		threading.Thread.start(self)
		self.running = True
	
	def run(self):
		print '---Transer running---'
		while self.running:
			time.sleep(1)
		self.free()
		print '---Transer stopped---'

	def add_one_trans(self):
		t = None
		t = thread.start_new_thread(thread_trans, (self, 0))

		self.count = self.count + 1
		print self.c_port_udp, 'request count:', self.count

	def decrease_one_trans(self):
		self.count = self.count - 1
		print self.c_port_udp, 'request count:', self.count

	def free(self):
		self.c_sock_tcp.close()
		print '%s client tcp sock closed' % self.id_port
		self.c_sock_udp.close()
		print '%s client udp sock closed' % self.id_port
		self.s_sock_tcp.close()
		print '%s server tcp sock closed' % self.id_port
		self.s_sock_udp.close()
		print '%s server udp sock closed' % self.id_port

class Server:
	def __init__(self):
		self.trans_list = {}
		#建立发布服务器TCP服务
		try:
			self.sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM, 0)
			self.sock.bind((LOCAL_IP, LOCAL_PORT))
		except:
			print 'Server socket init error'

	def run(self):
		self.sock.listen(10)
		print "===Server running==="
		while True:
			(r_sock, r_addr) = self.sock.accept()
			msg = r_sock.recv(64)[:-1]
			print 'Recv MSG:', msg
			id_addr = msg.split()[3]
			id_port = msg.split()[4]
			#加入客户请求列表
			if RECV_LIST.has_key(id_port):
				RECV_LIST[id_port].add_one_trans()
			else:
				MUTEX.testandset()	 
				transer = Transer(r_sock, msg, id_port, id_addr)
				RECV_LIST[id_port] = transer
				MUTEX.unlock()
			print RECV_LIST
				

	def __del__(self):
		self.sock.close()

if __name__ == '__main__':
	server = Server()
	server.run()
