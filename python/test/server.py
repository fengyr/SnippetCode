#!/usr/bin/python
#!encoding=utf8

import socket
import os, sys
import time

HOST = '192.168.108.55'
PORT = 9001

s = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
s.settimeout(2)

while True:
    s.sendto("hello world", (HOST, PORT));
    time.sleep(1)
