#!/usr/bin/python
#!encoding=utf8

import socket
import sys
import time

HOST = '127.0.0.1'
PORT = 9998

def tcp():
    """docstring for tcp"""
    s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    s.connect((HOST, PORT))
    s.settimeout(1)

    while True:
        s.send("hello server")
        # s.sendto("hello world", (HOST, PORT));
        print s.recv(4096)
        time.sleep(1)

def udp():
    """docstring for udp"""
    s = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
    s.settimeout(2)

    while True:
        s.sendto("hello server", (HOST, PORT));
        print s.recv(4096)
        time.sleep(1)


if __name__ == '__main__':
    if len(sys.argv) < 2:
        print "socket_client tcp | udp"
        
    else:
        if sys.argv[1] == 'tcp':
            tcp()
        elif sys.argv[1] == 'udp':
            udp()
