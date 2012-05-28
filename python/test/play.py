#!/usr/bin/python

import sys, os
from time import *

ftype = ("wmv", "rmvb", "avi", "mov")
action = "mplayer %s"

def play_movie(path_name):
	for path, dir, files in os.walk(path_name):
		a = [action % str(path+reg_name(name)) for name in files if name.split(".")[-1] in ftype]
		for m in a:
			os.system(m)

def reg_name(file):
	return file.replace(' ', '\ ').replace('(', '\(').replace(')', '\)')
				
if __name__ == '__main__':
	if len(sys.argv) < 2:
		print "need dir"
		exit(1)
	play_movie(sys.argv[1])



				

		


