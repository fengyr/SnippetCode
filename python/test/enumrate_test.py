#!/usr/bin/env python
#!encoding=utf8

def test(num):
	while num:
		num = num -1
		print num
		yield num

a = test(10)
for i in  enumerate(a):
	print i


