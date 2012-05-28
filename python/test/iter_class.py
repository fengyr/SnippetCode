import os

class iter_test():
	def __init__(self, num):
		self.num = num

	def __iter__(self):
		return self
	
	def next(self):
		self.num = self.num - 1

		if self.num == 0:
			raise StopIteration
		return (self.num, self.num+1)

	def show(self):
		print self.num

test = iter_test(5)
for i in test:
	print i

for i in enumerate(['abc', 'cde', 'fff']):
	print i
