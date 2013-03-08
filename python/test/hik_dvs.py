#!/usr/bin/python
#!encoding=utf8

import socket
import struct
import time

HOST = '192.168.108.161'
PORT = 8000
vport = 1  # 请求的编码器端口号

f = open('dump.cif', 'wb')
sock_request = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
sock_request.connect((HOST, PORT))
sock_get = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
sock_get.connect((HOST, PORT))
sock_end = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
sock_end.connect((HOST, PORT))

#使用用户名和密码登录，用户名和密码好像是加密过的，因此这个请求只适用于用户名是"admin"，密码是"12345"的情况
a = '\x00\x00\x00\x54\x5a\x00\x00\x00\x00\x00\x00\x00\x00\x01\x00\x10\x02\x02 \xce\x00\x00\x00\x00\x0f\x02\x00\x0a\x08\x00\x27\x19\x79\x04\x00\x00\x7a\xda\x66\x00\x8d\x16\xd2\x7e\x9d\x55\x05\xf1\x1f\x69\xbb\xa9\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\xc3\xd0\xa2\x7c\xa6\x76\xbd\xf3\x1e\x4f\xd1\xcb\xdc\xae\xcb\x64'
sock_request.send(a)

#发送数据请求
a = '\x00\x00\x00\x28\x5a\x00\x00\x00\x00\x00\x00\x00\x00\x03\x00\x00\x0f\x02\x00\x0a\x00\x01\x00\x00\x08\x00\x27\x19\x79\x04\x00\x00' + struct.pack('>I', vport) + '\x00\x00\x00\x00'
sock_get.send(a)

#前面20个字节好像无用,跳过
count = 0
sock_get.recv(20)
while count < 10:
    buf = sock_get.recv(1024)
    print buf
    f.write(buf)
    count += 1
print 'Get data end'

#这个好像是退出请求，目前还在调。如果没有退出请求，那么如果不登录，还可以接收数据一段时间.
a = '\x00\x00\x00\x20\x5a\x00\x00\x00\x00\x00\x00\x00\x00\x01\x01\x00\x0f\x02\x00\x0a\x00\x01\x00\x01\x08\x00\x27\x19\x79\x04\x00\x00'
sock_end.send(a)

sock_request.close()
sock_get.close()
sock_end.close()
