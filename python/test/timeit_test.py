import timeit

def sum():
	s = 0
	for i in range(100000):
		s = s + 2
	return s

print sum()
tm = timeit.Timer('sum')
print tm.timeit()
print tm.repeat()
