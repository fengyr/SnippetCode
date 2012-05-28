#!/usr/bin/python
import pdb

def popo_sort(array):
	l = len(array)

	for i in range(l):
		for j in range(i+1, l):
			if array[i] > array[j]:
				s = array[i]
				array[i] = array[j]
				array[j] = s
	
	return array
				

if __name__ == '__main__':
	a = [3, 2, 11, 6, 99, 21, 45, 67, 12, 999]
	print popo_sort(a)
	
