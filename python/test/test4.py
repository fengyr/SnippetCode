#!/usr/bin/python
import os

class X:
	def __init__(self):
		print "id %d new" % id(self)

	def show(self):
		print "id %d show" % id(self)

	def __del__(self):
		print "id %d del" % id(self)
	
A = X()
B = X()

for ins in (A.show, B.show):
	ins()
	print ins.__name__
