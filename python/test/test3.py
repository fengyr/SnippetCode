#!/usr/bin/python

import sys, os
import thread
import time

def show(*info):
	while 1:
		print len(info)
		time.sleep(1)
	thread.exit()
try:
	i = thread.start_new_thread(show, (1, 2))
except:
	print "except"
	
print i, 'ok'

while 1:
	time.sleep(10)	