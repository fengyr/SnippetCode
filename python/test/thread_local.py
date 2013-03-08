#!/usr/bin/env python
#!encoding=utf8

import threading
import time

info = 0

def test(*arg):
    global info

    while True:
        info = threading.local()
        info = 0
        if arg[0] == 'thread1':
            info += 1
        else:
            info -= 1

        print arg[0], info
        time.sleep(1)

thread1 = threading.Thread(target=test, args=('thread1', 0))
thread1.aemon = True
thread1.start()

thread2 = threading.Thread(target=test, args=('thread2', 0))
thread2.daemon = True
thread2.start()

while True:
    time.sleep(1)
