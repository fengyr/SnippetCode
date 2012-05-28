#!/usr/bin/python
import socket
import os
import time

HOST = '10.1.30.112'
PORT = 9001
client = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
client.bind((HOST, PORT))
#client.listen(10)

#(conn, addr) = client.accept()
#conn.settimeout(4)
#buf = conn.recv(1024)
#conn.send('VER=0;CMD=21;TKN=512;UID=22;OCMR=0;CMR=119101007;CDR=119110001;LVL=259;SIP=1881014538;PRT=9001;MOD=85;ERR=0;LEN=0;BUF=;.')
head = ''
f = open('dump.cif', 'wb')
buf = client.recv(4096)
#head = buf[12:52]
f.write(buf)
f.write('\n')
while True:
    buf = client.recv(4096)
    #id = buf.find(head)
    #if id != -1:
        #print id
        #f.write(buf[52:])
    #else:
        #f.write(buf)
        
    #print len(buf), len(buf[52:])
    f.write(buf)
    f.write('\n')

