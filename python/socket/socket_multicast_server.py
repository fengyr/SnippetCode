#!/usr/bin/env python
#!encoding=utf8

import socket
import struct

MULTI_GROUP = '224.3.2.1'
MULTI_PORT = 9001

def main():
    """docstring for main"""
    sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM, socket.IPPROTO_UDP)
    sock.bind(('', MULTI_PORT))
    sock.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
    mreq = struct.pack('4sl', socket.inet_aton(MULTI_GROUP), socket.INADDR_ANY)
    sock.setsockopt(socket.IPPROTO_IP, socket.IP_ADD_MEMBERSHIP, mreq)

    while True:
        print "recvied from client" + sock.recv(1024)

if __name__ == '__main__':
    main()
