#!/usr/bin/python

import socket
import sys


def init_socket(serverip, serverport):
    # import pdb;pdb.set_trace()
    try:
        s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        addr = (serverip, int(serverport))
        s.settimeout(1.0)
        s.connect(addr)
    except socket.error:
        print 'Connect video server error'
    return s


def handle(s, msg):
    try:
        s.send(msg)
        s.close()
        print msg
    except socket.error:
        print 'send to video server error'


def use_age():
    print './con264 [start, stop] ipaddress [5001-5006]'
    print '5001---500K'
    print '5002---128K'
    print '5003---256K'
    print '5004---1000K'
    print '5005---2000K'
    print '5006---5000K'
    sys.exit(1)


if __name__ == '__main__':
    if len(sys.argv) < 4:
        use_age()
    cmd, ip, port = sys.argv[1:4]
    print cmd, ip, port
    s = init_socket(ip, 8800)
    # for i in [5001, 5002, 5003, 5004, 5005, 5006]:
    #   handle(s, 'stop:%d\r' % i)
    #   print 'stop:%d\r' % i
    if cmd == 'start':
        handle(s, 'start:%s\r' % port)
    else:
        handle(s, 'stop:%s\r' % port)
