#!/usr/bin/python
import pdb

def quick_sort(array):
	if len(array) <= 1:
		return array

#	pdb.set_trace()
	key = array[0]
	left_array = []
	right_array = []
	
	for i in array[1:]:
		if i <= key:
			left_array.append(i)
		else:
			right_array.append(i)
	
	left_array = quick_sort(left_array)
	right_array = quick_sort(right_array)

	return left_array+[key]+right_array

def quick_sort2(array):
    if len(array) <= 1:return array

    return quick_sort2([ i for i in array[1:] if i < array[0]]) + array[0:1] + quick_sort2([ i for i in array[1:] if i > array[0]])

if __name__ == '__main__':
	a = [3, 2, 11, 6, 99, 21, 45, 67, 12, 999]
	#print quick_sort(a)
	print quick_sort2(a)
	
