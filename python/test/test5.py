#!/usr/bin/python

class sample:
	""" this is sample
		"""
	def __init__(self, x):
		self.x = x
	def show(self, n):
		print "x:", self.x[n]
	def __del__(self):
		None

if __name__ == '__main__':
	sam = sample({1:'110', 2:'220'})
	sam.show(1)

