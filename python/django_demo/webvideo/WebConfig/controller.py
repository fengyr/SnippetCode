import socket

def init_socket(serverip, serverport):
	#import pdb;pdb.set_trace()
	try:
		s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
		addr = (serverip, int(serverport))
		s.settimeout(1.0)
		s.connect(addr)
	except socket.error:
		print 'Connect video server error'
	return s

def handle(s, msg):
	try:
		s.send(msg)
		s.close()
		#print msg
	except socket.error:
		print 'send to video server error'

