#!/usr/bin/python
import socket
import sys

HOST = '127.0.0.1'
PORT = 9001

def tcp():
    """docstring for tcp"""
    server = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    server.bind((HOST, PORT))
    server.listen(10)

    while True:
        (conn, addr) = server.accept()
        conn.settimeout(10)

        while True:
            buf = conn.recv(4096)
            print buf
            if buf == '':
                break

def udp():
    """docstring for udp"""
    server = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
    server.bind((HOST, PORT))

    server.settimeout(10)

    while True:
        buf, address = server.recvfrom(4096)
        server.sendto('hello client', address)
        print buf, address


if __name__ == '__main__':
    if len(sys.argv) < 2:
        print "socket_client tcp | udp"

    else:
        if sys.argv[1] == 'tcp':
            tcp()
        elif sys.argv[1] == 'udp':
            udp()
