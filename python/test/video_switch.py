#!encoding=utf8

import socket
import struct
import os, sys
import time

#客户端IP,端口
HOST_IP = '10.1.30.112'
HOST_PORT = 9001
#长天致远服务器IP,端口
REMOTE_IP = '10.1.30.68'
REMOTE_PORT = 7000

#登录,视频请求,视频退出
#视频请求分两种,一种是知道CMR号,直接请求视频源,第二种从服务器获取CMR号,再进行第二次请求.
COMMAND = { 
            'login':'VER=0;CMD=1;TKN=512;UID=0;OCMR=0;CMR=0;CDR=0;LVL=0;SIP=251789322;PRT=0;MOD=0;ERR=0;LEN=0;BUF=jl@jscd;.',
            'start':[
                'VER=0;CMD=11;TKN=512;UID=22;OCMR=0;CMR=0;CDR=%s;LVL=0;SIP=%d;PRT=%d;MOD=85;ERR=0;LEN=0;BUF=;.',
                'VER=0;CMD=21;TKN=512;UID=22;OCMR=0;CMR=%s;CDR=%s;LVL=0;SIP=%d;PRT=%d;MOD=85;ERR=0;LEN=0;BUF=;.'
            ],
            'stop':'VER=0;CMD=22;TKN=512;UID=22;OCMR=0;CMR=%s;CDR=%s;LVL=0;SIP=%d;PRT=%d;MOD=85;ERR=0;LEN=0;BUF=;.'
}

class Switcher():
    def __init__(self):
        self.datas = []
        self.IPNUM = struct.unpack("I",socket.inet_aton(HOST_IP))[0]
        self.last_cmr = ''
        self.last_cdr = ''
        self.last_camera = -1
        self.request_ok_num = 0
        self.request_failed_num_list = []
        self.sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        self.sock.connect((REMOTE_IP, REMOTE_PORT))
        self.sock.settimeout(5)

        self._load_file()
        #登入系统
        self.login()
        
    def _load_file(self):
        """
            读取配置文件
            """
        f = open('login_log', 'r')
        alldata = f.read().split('\n')[:-1]
        for data in alldata:
            try:
                info = data.split(';')
                cmr = info[5].split('=')[1]
                cdr = info[6].split('=')[1]
                buf = info[13].split('=')[1]
                self.datas.append([cmr, cdr, buf])
            except Exception, e:
                print 'Load file', e

    def login(self):
        """
            登录服务器
            """
        self.sock.send(COMMAND['login'])
        buf = self.sock.recv(1024)
        #print buf.split(';')[11]
        err = buf.split(';')[11].split('=')[1]
        if err == '0':
            print 'Login success'
            return True
        else:
            print 'Login error'
            return False

    def switchto(self, camera_num):
        """
            根据配置文件中的摄像机号,切换视频
            """
        if self.stop(self.last_camera):
            self.last_camera = -1
        else:
            return False

        command = ''
        cam_info = self.datas[camera_num]
        print 'Request:', cam_info[2]
        #CMR=0,首先请求通道号
        if cam_info[0] == '0':
            command = COMMAND['start'][0] % (cam_info[1], self.IPNUM, HOST_PORT)
            self.sock.send(command)
            while True:
                try:
                    buf = self.sock.recv(1024)
                except socket.timeout:
                    continue
                #print buf
                if buf == 'yes;.\x00':
                    continue
                infos = buf.split(';')
                #import pdb;pdb.set_trace()
                #print infos[5], infos[7], infos[11]
                cmr = infos[5].split('=')[1]
                lvl = infos[7].split('=')[1]
                err = infos[11].split('=')[1]
                if lvl != '0':
                    self.last_cmr = cmr
                    self.last_cdr = cmr[:4]+'10001'
                    self.last_camera = camera_num
                    print 'Switch success'
                    self.request_ok_num += 1
                    return True
                elif cmr != '0' and err == '0':
                    command = COMMAND['start'][1] % (cmr, cmr[:4]+'10001', self.IPNUM, HOST_PORT)
                    self.sock.send(command)
                    continue
                elif cmr == '0' and err != '0':
                    print 'Switch failed'
                    self.request_failed_num_list.append(str(camera_num))
                    return False
                    

        #CMR!=0,直接请求摄像机
        else:
            command = COMMAND['start'][1] % (cam_info[0], cam_info[1], self.IPNUM, HOST_PORT)
            self.sock.send(command)
            while True:
                try:
                    buf = self.sock.recv(1024)
                except socket.timeout:
                    continue
                #print buf
                if buf == 'yes;.\x00':
                    continue
                infos = buf.split(';')
                #import pdb;pdb.set_trace()
                #print infos[5], infos[7], infos[11]
                cmr = infos[5].split('=')[1]
                lvl = infos[7].split('=')[1]
                err = infos[11].split('=')[1]
                if lvl != '0':
                    self.last_cmr = cmr
                    self.last_cdr = cmr[:4]+'10001'
                    self.last_camera = camera_num
                    print 'Switch success'
                    self.request_ok_num += 1
                    return True
                else:
                    print 'Switch failed'
                    self.request_failed_num_list.append(str(camera_num))
                    return False

    def stop(self, camera_num):
        """
            根据配置文件中的摄像机号,停止视频
            """
        if self.last_camera == -1:
            return True
        command = ''
        cam_info = self.datas[camera_num]
        print 'Stop:', cam_info[2]
        command = COMMAND['stop'] % (self.last_cmr, self.last_cdr, self.IPNUM, HOST_PORT)
        #print command
        self.sock.send(command)
        while True:
            try:
                buf = self.sock.recv(1024)
            except socket.timeout:
                continue
            if buf == 'yes;.\x00':
                continue
            infos = buf.split(';')
            #print infos[11]
            err = infos[11].split('=')[1]
            if err == '0':
                print 'Stop success'
                return True
            else:
                print 'Stop failed'
                return False
                
    def show_info(self):
        """
            输出测试信息
            """
        f = open('switcher_log', 'w')
        for i in self.request_failed_num_list:
            print self.datas[int(i)][2]
            f.write(self.datas[int(i)][2])
            f.write('\n')

        print 'Total success:', self.request_ok_num
        f.write('Total success: %d' % self.request_ok_num)
        f.write('\n')
        f.write(str(self.request_failed_num_list))

if __name__ == '__main__':
    sw = Switcher()
    #sw.show_info()
    for i in range(1085):
        sw.switchto(i)
        time.sleep(0.1)

    sw.show_info()

    while True:
        time.sleep(71)
