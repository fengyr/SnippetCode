#!/usr/bin/python
#!encoding=utf8

import threading
import time


class TestThread(threading.Thread):
    def __init__(self):
        #初始化event变量
        self.event = threading.Event()
        threading.Thread.__init__(self)
        threading.Thread.start(self)

    def run(self):
        while not self.event.isSet():
            time.sleep(1)
            print 'sleepping '

        print 'thread end'

    def join(self, timeout=None):
        #设置event变量,结束线程
        self.event.set()
        threading.Thread.join(self, timeout)

if __name__ == '__main__':
    test = TestThread()
    time.sleep(1)

    test.join(2)
