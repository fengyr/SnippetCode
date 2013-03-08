#!/usr/bin/env python
#!encoding=utf8

import socket
import struct

print socket.inet_ntoa(struct.pack('I', socket.htonl(251789322)))
print socket.ntohl(struct.unpack("I", socket.inet_aton('1.2.3.4'))[0])
print struct.unpack("I", socket.inet_aton('1.2.3.4'))
print socket.ntohl(67305985)
print socket.htonl(16909060)
print struct.unpack('i', socket.inet_aton('1.2.3.4'))
print struct.pack('i', 16909060)
print struct.pack('i', 67305985)
print socket.inet_ntoa(struct.pack('I', socket.ntohl(16909060)))
print socket.htonl(struct.unpack("I", socket.inet_aton('1.2.3.4'))[0])
