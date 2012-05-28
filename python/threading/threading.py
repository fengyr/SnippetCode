#!/usr/bin/python
#!encoding=utf8

import threading, thread
import os, sys

test_args = (1, 2, 3)

def test(args):
	print args
	print 'threading running'

if __name__ == '__main__':
	test_thread = threading.Thread(target=test, args=(test_args))
	test_thread.start()
