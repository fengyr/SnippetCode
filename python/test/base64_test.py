#!/usr/bin/env python
#!encoding=utf8

import base64
import gzip
import binascii

rf = gzip.open('gzip.gz', 'rb')
content = rf.read()
rf.close()

data = base64.encodestring(content)
print '---------data----------'
print data, len(data)
content = base64.decodestring(data)
print '---------content----------'
print content, len(content)
crc = '%X' % binascii.crc32('12345')
print '---------crc----------'
print crc
print int(crc, 16)
