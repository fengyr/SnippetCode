#!/usr/bin/python
import socket
import os, sys
import time

HOST = '192.168.100.187'
PORT = 8802

s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
s.connect((HOST, PORT))

s.send('transform %s to %s\n' % (sys.argv[1], sys.argv[2]))

time.sleep(1000)
s.close()
