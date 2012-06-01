#!/usr/bin/env python
#!encoding=utf8

from struct import unpack
import os
import glob
import sys

# 扫描当前目录中的odex文件，分析文件头信息
files = glob.glob('*.odex')
if files == []:
    print "No odex file found"
    sys.exit(1)

for fname in files:
    try:
        f = open(fname, 'rb')
        header = f.read(40)
        magic, magic_ver, dexoffset, dexlen, depsoffset, depslen, optoffset, optlen, flags, checksum = unpack('<4s4sIIIIIIII', header)

        # header_info = "filename=%s, magic=%s, magic_ver=%s, dexoffset=%d, dexlen=%d, depsoffset=%d, depslen=%d, optoffset=%d, optlen=%d, flags=%d, checksum=%d" % (fname, magic, magic_ver, dexoffset, dexlen, depsoffset, depslen, optoffset, optlen, flags, checksum)
        # print header_info

        f.seek(depsoffset, os.SEEK_SET)
        deps = f.read(16)
        # print "deps=", deps
        a, b, c, numdeps = unpack('IIII', deps)
        print "file=%s, numdeps=%d \n" % (fname, numdeps)
    except Exception, e:
        print e
    finally:
        f.close()
