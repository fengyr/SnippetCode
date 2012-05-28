#!/usr/bin/python
import os

info = os.statvfs("/")
rate = info[4]*100/info[2]

if  rate > 70:
	print rate
else:
	print "rate:", rate, "free"