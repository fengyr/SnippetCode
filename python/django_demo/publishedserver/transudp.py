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
LOCAL_PORT  = 8802
LOCAL_IP    = '192.168.100.54'
#接收客户请求列表
RECV_LIST   = {}
MUTEX       = mutex.mutex()

class Transer(threading.Thread):
	def __init__(self, sock, msg, id, addr, video):
		self.count      = 0
		self.request_video = video
		self.c_sock_addr_port = {}
		self.c_sock_tcp = []
		#self.id_port    = id
		self.c_sock_tcp.append(sock)
		self.s_port_udp = int(id) + 1
		self.rlist = []
		self.wlist = []
		self.xlist = []
		#self.c_port_udp = int(id)
		self.c_sock_addr_port[sock]=[addr, int(id)]
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
			print '\nTranser socket init error'

		#pdb.set_trace()
		#self.add_one_trans()
		threading.Thread.__init__(self)
		self.running = True
		threading.Thread.start(self)
	
	def run(self):
		print '\n---Transer running---'
		#while self.running:
		#	time.sleep(1)
		self.rlist = [self.s_sock_tcp, self.s_sock_udp]
		self.rlist.extend(self.c_sock_tcp)
		self.wlist = [self.c_sock_udp]
		self.xlist = []
		timeout = 1.0

		quit = False
		while not quit:
			(rl, wl, xl) = select(self.rlist, self.wlist, self.xlist, timeout)
			for i in rl:
				try:
					#转码服务器断开连接
					if i == self.rlist[0]:
						buf = i.recv(64)
						if len(buf) == 0:
							print "\nRemote Server disconnect"
							quit = True
							break
					#接收转码包
					if i == self.rlist[1]:
						buf = i.recv(2048)
						for j in wl:
							if j == self.wlist[0]:
								if len(self.c_sock_addr_port) == 0:
									quit = True
									break
								for k in self.c_sock_addr_port.keys():
									try:
										#print self.c_sock_addr_port[k]
										j.sendto(buf, (self.c_sock_addr_port[k][0][0],self.c_sock_addr_port[k][1]))
									except:
										pass
										#print "\nsendto error"
					#监听客户端连接
					MUTEX.testandset()
					for n in range(len(self.rlist)-2):
						if i == self.rlist[n+2]:
							buf = i.recv(64)
							if len(buf) == 0:
								print self.c_sock_addr_port
								del self.c_sock_addr_port[i]
								self.rlist.remove(i)
								print self.c_sock_addr_port
								if (len(self.rlist)-2) == 0:
									quit = True
									break
					MUTEX.unlock()
				except:
					print "\nThread trans error"
			time.sleep(0.001)

		if RECV_LIST.has_key(self.request_video):
			MUTEX.testandset()
			del RECV_LIST[self.request_video]
			MUTEX.unlock()
			print RECV_LIST
				
		self.free()
		print '\n---Transer stopped---'

	def add_one_trans(self, sock, id, addr):
		#t = None
		#t = thread.start_new_thread(thread_trans, (self, 0))
		MUTEX.testandset()
		self.c_sock_tcp.append(sock)
		self.rlist.append(sock)
		self.c_sock_addr_port[sock] = [addr, int(id)]
		MUTEX.unlock()
		self.count = self.count + 1
		#print '\n', self.c_port_udp, 'request count:', self.count

	def decrease_one_trans(self):
		self.count = self.count - 1
		print '\n', self.c_port_udp, 'request count:', self.count

	def free(self):
		for i in self.c_sock_tcp:
			i.close()
		print '%s client tcp sock closed' 
		self.c_sock_udp.close()
		print '%s client udp sock closed'
		self.s_sock_tcp.close()
		print '%s server tcp sock closed'
		self.s_sock_udp.close()
		print '%s server udp sock closed'

class Server:
	def __init__(self):
		self.trans_list = {}
		#建立发布服务器TCP服务
		try:
			self.sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM, 0)
			self.sock.bind((LOCAL_IP, LOCAL_PORT))
		except:
			print '\nServer socket init error'

	def run(self):
		self.sock.listen(10)
		print "===Server running==="
		while True:
			(r_sock, r_addr) = self.sock.accept()
			msg = r_sock.recv(64)[:-1]
			print 'Recv MSG:', msg
			request_video = msg.split()[1]
			id_port = msg.split()[3]
			#加入客户请求列表
			if RECV_LIST.has_key(request_video):
				RECV_LIST[request_video].add_one_trans(r_sock, id_port, r_addr)
			else:
				MUTEX.testandset()	 
				transer = Transer(r_sock, msg, id_port, r_addr, request_video)
				RECV_LIST[request_video] = transer
				MUTEX.unlock()
			print RECV_LIST
				

	def __del__(self):
		self.sock.close()

if __name__ == '__main__':
	server = Server()
	server.run()
