#!/usr/bin/python

import time
import os
import cPickle as pickle
import sys

data = {'red':'apple', 'green':'leaf', 'yellow':'banana'}
filename = '/at/object.data'

print data
try:
	f = file(filename, 'w')
	pickle.dump(data, f)
	f.close()
except:
	print "file no found"
	sys.exit(1)
	
data.clear()
print data

f = file(filename, 'r')
data = pickle.load(f)
print data
f.close()