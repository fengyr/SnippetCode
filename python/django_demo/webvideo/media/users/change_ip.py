#!/usr/bin/python
import cPickle
import sys, os

f = open('ip.conf', 'r+')
obj = cPickle.load(f)
print 'Before:', obj
f.close()

if len(sys.argv) > 1:
	obj['LOCAL'] = sys.argv[1:]

	f = open('ip.conf', 'r+')
	cPickle.dump(obj, f)
	print 'Modified:', obj
	f.close()
