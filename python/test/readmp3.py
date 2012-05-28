#!/usr/bin/python

import sys, os

content = None
f = None

def get_mp3_info(info):
	print info

if __name__ == '__main__':
	if len(sys.argv) < 2:
		exit(1);

	try:
		f = open(sys.argv[1], "rb")
		print os.path.split(f.name)[1]
		try:
			f.seek(-128, 2)
			content = f.read(128)
		except:
			print "seek file err"
	except:
		print "open file err"
		exit(1)
	if not f.closed:
		f.close()

	get_mp3_info(content)

	print "\n".join(sys.modules.keys())
