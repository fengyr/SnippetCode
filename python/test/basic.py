#!/usr/bin/env python
#!encoding=utf8

import test1

string = "i am a student"
copy = string
string = "i am not a student"

print "copy:", copy, "string:", string
spitlist = string.split('a', 1)
print 'spitlist: ', spitlist

mylist = []


mylist.append('hello')
mylist.append('world')
mylist.append('abc')
copylist = mylist
del(mylist[0])
print "copylist:", copylist, "mylist:", mylist
s = '&'
sjoin = s.join(copylist)
print sjoin

mylist.sort()
for i in mylist:
    print i

mygrp1 = (2, 4)
mygrp2 = (1, mygrp1, 3)

print "mygrp1 num: %d, %d" % (mygrp1[0], mygrp1[1])

myhash = {'address': 'nanjing'}
myhash['name'] = 'zenki'
myhash['job'] = 'pro'
del(myhash['job'])
i = ()
for i in myhash.items():
    print i

#print dir(test1)
