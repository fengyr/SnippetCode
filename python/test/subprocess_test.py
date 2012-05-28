import subprocess
import thread
import time
import os, sys

cmd = ['top', '-d', '0.5']

#subprocess.Popen(cmd, shell=True)
proc = subprocess.Popen(cmd, stdout=subprocess.PIPE)

def message(*arg):
    while True:
        time.sleep(0.5)
	p = subprocess.Popen(['grep', 'Cpu'], stdin=arg[0])

thread.start_new_thread(message, (proc.stdout, 0))

while True:
    time.sleep(1)