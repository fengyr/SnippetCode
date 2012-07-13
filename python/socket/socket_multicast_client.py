#!/usr/bin/env python
#!encoding=utf8

import socket
import time

MCAST_GROUP = '224.3.2.1'
MCAST_PORT = 9001

def main():
  sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
  sock.setsockopt(socket.IPPROTO_IP, socket.IP_MULTICAST_TTL, 32)
  while True:
    sock.sendto(time.strftime('(%Y-%m-%d %H:%M:%S)'), (MCAST_GROUP, MCAST_PORT))
    time.sleep(2)

if __name__ == '__main__':
    main()
