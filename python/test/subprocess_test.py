#!/usr/bin/env python
#!encoding=utf8

import subprocess
import thread
import time

cmd = ['top', '-d', '0.5', '-n', '3']

#subprocess.Popen(cmd, shell=True)
proc = subprocess.Popen(cmd, stdout=subprocess.PIPE)

def filterMessage(*arg):
    while True:
        time.sleep(0.5)
        p = subprocess.Popen(['grep', 'Cpu'], stdin=arg[0])

thread.start_new_thread(filterMessage, (proc.stdout, 0))

while True:
    time.sleep(1)
