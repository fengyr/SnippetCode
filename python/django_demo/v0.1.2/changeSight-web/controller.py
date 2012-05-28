import socket
import config

def init_socket():
	try:
		s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
		a = config.VIDEO_SERVER_IP.split(':')
		addr = (a[0], int(a[1]))
		s.connect(addr)
	except socket.error:
		print 'Connect video server error'
	return s

def handle(s, msg):
	try:
		s.send(msg)
		s.close()
	except socket.error:
		print 'send to video server error'

