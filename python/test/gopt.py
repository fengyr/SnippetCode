#!/usr/bin/python2.5

import sys
import getopt, unittest

def pcomd():
	commd = getopt.getopt(sys.argv[1:], "h:g:d:")
	print commd

def phello():
	print "hello"

if __name__ == '__main__':
    pass
