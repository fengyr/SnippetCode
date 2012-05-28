#!/usr/bin/python

import person

p = person.person("zenki", 25)

def show():
	global person_id
	person_id = 100
	print person_id

show()
print person_id

