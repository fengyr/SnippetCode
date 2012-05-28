#!/usr/bin/env python
#!encoding=utf8

def iter_func(n):
    """
        >>> iter_func(1)
        iter_func
        >>> iter_func(2)
        iter_func
        >>> iter_func(3)
        iter_func
    """
    for i in range(3):
		print i, 'hello'
		yield n
		print n
		n = n + 1
    print 'yield end'

a = iter_func(1)

#a.next()
#a.next()
#a.next()
#a.next()

for i in a:
    print i

## desc
# @param 
def sum_val(arg):
    """
    >>> sum_val(1)
    10
    >>> sum_val(2)
    20
    >>> sum_val(3)
    30
    """
    print arg * 10

if __name__ == '__main__':
    import doctest
    doctest.testmod()
