#!/usr/bin/python
import socket
import sys
import threading
import struct
import time

HOST = '127.0.0.1'
# PORT = 11014
PORT = 9001


def recv_thread(*arg):
    conn = arg[0]

    print "client connect: ", conn

    count = 0
    while True:
        try:
            count = count + 1
            buf = conn.recv(256)
            print buf, " len=", len(buf), " count=", count

            # conn.send(buf);
            # continue

            if buf == '':
                break
            data = struct.unpack("<hc3d5cdhc9dh8d75c", buf)
            print data
            time.sleep(10)
            conn.send(buf);
        except Exception, e:
            break

def tcp():
    """docstring for tcp"""
    server = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    server.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
    server.bind((HOST, PORT))
    server.listen(10)

    while True:
        (conn, addr) = server.accept()

        thread_loop = threading.Thread(target=recv_thread, args=(conn, 0))
        thread_loop.daemon = True
        thread_loop.start()

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
