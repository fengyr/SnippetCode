#!/usr/bin/python
#!encoding=utf8

import socket
import sys
import os
import time
import struct

HOST = '127.0.0.1'
PORT = 11014

def getData(ID, msg):
    # fm = 'i%is2s' % (len(msg))
    fm = 'i%is2s' % 1022
    # fm = 'i512s2s'
    data = struct.pack(fm, ID, msg, '\r\n')
    print data

    return data

def connect_server_ping():
    s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    s.connect((HOST, PORT))
    s.settimeout(3)
    s.send(getData(0, "type_ping"))

    return s

def connect_server_modbus():
    s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    s.connect((HOST, PORT))
    s.settimeout(3)
    s.send(getData(0, "type_modbus"))

    return s

def modbus():
    """docstring for ping"""
    s = None
    while True:
        try:
            s = connect_server_modbus()
        except Exception, e:
            print e
            time.sleep(1)
            continue

        while True:
            try:
                data = s.recv(4096)
                if len(data) == 0:
                    break
                else:
                    print data
            except socket.timeout:
                pass
                # os.system('killall ColorPickServer')
                # print 'killall ColorPickServer'
            except Exception, e:
                time.sleep(1)
                print e



def ping():
    """docstring for ping"""
    s = None
    while True:
        try:
            s = connect_server_ping()
        except Exception, e:
            print e
            time.sleep(1)
            continue

        while True:
            try:
                data = s.recv(4096)
                if len(data) == 0:
                    break
                else:
                    print data
            except socket.timeout:
                os.system('killall ColorPickServer')
                # print 'killall ColorPickServer'
            except Exception, e:
                time.sleep(1)
                print e

def tcp():
    """docstring for tcp"""
    s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    s.connect((HOST, PORT))
    s.settimeout(10)

    s.send(getData(0, "type_ui_control"))
    # s.sendto("hello world", (HOST, PORT));
    print s.recv(4096)
    time.sleep(1)

    # exit
    # s.send(getData(20, "exit"))
    # print s.recv(4096)

    while True:
        # modbus class
        s.send(getData(21, "1234567890 1"))
        print s.recv(4096)
        time.sleep(1)

        # modbus class
        s.send(getData(22, "1 4"))
        print s.recv(4096)
        time.sleep(1)

    return

    # 特征描述子
    s.send(getData(3, "2"))
    print s.recv(4096)

    # 子空间描述子
    s.send(getData(15, "1"))
    print s.recv(4096)

    # 分类器
    s.send(getData(4, "4"))
    print s.recv(4096)

    # 样本
    s.send(getData(2, "3"))
    print s.recv(4096)

    # 是否使用聚类
    s.send(getData(7, "1"))
    print s.recv(4096)

    # 最小可分类间距
    s.send(getData(13, "9"))
    print s.recv(4096)

    # 最小分类比率
    s.send(getData(14, "1.6"))
    print s.recv(4096)

    # 设置瓷砖边缘长度
    s.send(getData(18, "420"))
    print s.recv(4096)

    # 设置所有值
    s.send(getData(12, "100 101 103 104 105 106.8 410"))
    print s.recv(4096)

    # 获取版本信息
    s.send(getData(16, "get"))
    print s.recv(4096)

    # 进入高级选项模式
    s.send(getData(17, "leave"))
    print s.recv(4096)

    # 保存文件
    # s.send(getData(10, "1"))
    # print s.recv(4096)

    s.send(getData(5, "start"))
    print s.recv(4096)

    time.sleep(5)

    s.send(getData(6, "stop"))
    print s.recv(4096)

    time.sleep(3)

def tcp2():
    """docstring for tcp"""
    s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    s.connect((HOST, PORT))
    s.settimeout(10)

    s.send(getData(0, "type_img_data"))
    # s.sendto("hello world", (HOST, PORT));
    print s.recv(4096)

    while True:
        headLen = s.recv(4)
        l = struct.unpack('i', headLen)[0]
        print 'length = ', l

        dataLen = 0
        while dataLen < l:
            data = s.recv(l-dataLen)
            print data
            dataLen += len(data)

    time.sleep(10)

def tcp3():
    """docstring for tcp"""
    s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    s.connect((HOST, PORT))
    s.settimeout(10)

    s.send(getData(0, "type_ui_control"))
    # s.sendto("hello world", (HOST, PORT));
    print s.recv(4096)
    time.sleep(1)

    # add sample
    msg = "%s %s" % (sys.argv[3], sys.argv[4])
    s.send(getData(19, msg))
    print s.recv(4096)

def tcp4():
    """docstring for tcp"""
    s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    s.connect((HOST, PORT))
    s.settimeout(10)

    s.send(getData(0, "type_camera_control"))
    # s.sendto("hello world", (HOST, PORT));
    print s.recv(4096)
    time.sleep(1)

    s.send(getData(2, "type_camera_control 1"))
    print s.recv(4096)

    s.send(getData(3, "type_camera_control 2"))
    print s.recv(4096)

    time.sleep(10)

def udp():
    """docstring for udp"""
    s = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
    s.settimeout(2)

    while True:
        s.sendto("hello server", (HOST, PORT))
        print s.recv(4096)
        time.sleep(1)


if __name__ == '__main__':
    if len(sys.argv) < 2:
        print "socket_client tcp | udp"

    else:

        if sys.argv[1] == 'tcp':
            if sys.argv[2] == '1':
                tcp()
            elif sys.argv[2] == '2':
                tcp2()
            elif sys.argv[2] == '3':
                tcp3()
            elif sys.argv[2] == '4':
                tcp4()
        elif sys.argv[1] == 'udp':
            udp()
        elif sys.argv[1] == 'ping':
            ping()
        elif sys.argv[1] == 'modbus':
            modbus()
