import socket

s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
s.bind(('192.168.100.54', 9989))
s.listen(5)

while True:
	(sa, c_addr) = s.accept()
	while True:
		data = sa.recv(100)
		print data

