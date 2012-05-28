import cPickle

def test_func(arg):
	print arg

a = {'one':1, 'two':2}
f = open('pickl.pkl', 'w')
cPickle.dump(test_func, f, cPickle.HIGHEST_PROTOCOL)
f.close()

f = open('pickl.pkl', 'r')
func = cPickle.load(f)
func('hello world')

