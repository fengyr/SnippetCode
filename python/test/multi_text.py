#!/usr/bin/python

import socket
import sys

arg = sys.argv[1:]

s = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
addr = ('224.2.213.100', 20000)
s.connect(addr)
b = ' '.join(arg)
print b
s.send(b)
print s.recv(1024)

