import socket
import time

s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
s.connect(('192.168.100.54', 9989))

while True:
	s.send('hello')
	time.sleep(3)
